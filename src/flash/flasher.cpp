/*
 * WeAct Studio DAP Flasher
 *
 * Flash programming: download a .FLM algorithm into the target RAM and drive
 * its Init / EraseSector / ProgramPage / Verify / UnInit routines.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "flasher.h"

#include "target/cortexm.h"
#include "dap/cmsisdap.h"
#include "flashalgo.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QSet>

#include <algorithm>
#include <cstdio>
#include <cstring>

namespace {

/* Stack reserved for the algorithm (a small routine, 2 KB is ample) */
const quint32 kStackSize = 0x800;

/* How often the algorithm image is written to the RAM before the operation is
 * given up. The write is idempotent and the image is read back and compared,
 * so a probe that loses part of a block transfer once does not have to cost
 * the user the whole flashing session. */
const int kDownloadAttempts = 3;

/* Zero fill is written in chunks so that one DAP command stays reasonable */
const int kZeroChunk = 1024;

/* Timeouts, in milliseconds */
const int kInitTimeout = 5000;
const int kInitAttempts = 2;
const int kUnInitTimeout = 5000;
const int kProgramTimeoutFloor = 5000;
const int kEraseTimeoutFloor = 10000;
const int kChipEraseTimeoutCeiling = 120000;

/* Progress percentages of the phases */
const int kProgressPrepare = 2;
const int kProgressLoaded = 12;
const int kProgressInit = 15;
const int kProgressEraseStart = 15;
const int kProgressEraseEnd = 30;
const int kProgressProgramStart = 30;
const int kProgressProgramEnd = 88;
const int kProgressVerifyStart = 88;
const int kProgressVerifyEnd = 99;

quint32 alignUp(quint32 value, quint32 alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}

QString hex32(quint32 value)
{
    return QStringLiteral("0x%1").arg(value, 8, 16, QLatin1Char('0'));
}

} /* namespace */

Flasher::Flasher(CmsisDap *link, CortexM *core)
    : link_(link)
    , core_(core)
{
}

/* ==========================================================================
 * Helpers
 * ========================================================================== */

bool Flasher::reportProgress(const FlasherCallbacks &callbacks, int percent, const QString &status)
{
    if (callbacks.progress)
        return callbacks.progress(percent, status);

    return true;
}

bool Flasher::isCancelled(const FlasherCallbacks &callbacks)
{
    return callbacks.cancelled && callbacks.cancelled();
}

void Flasher::log(const FlasherCallbacks &callbacks, const QString &message, bool isError)
{
    if (callbacks.log)
        callbacks.log(message, isError);
}

/* ==========================================================================
 * RAM layout
 * ========================================================================== */

bool Flasher::computeLayout(const FlashAlgo &algo, quint32 ramBase, Layout &layout, QString &error)
{
    layout = Layout();
    layout.loadBase = ramBase;
    layout.imageSize = alignUp(algo.imageSize(), 8);

    if (layout.imageSize == 0) {
        error = QCoreApplication::translate("Flasher", "The algorithm image is empty.");
        return false;
    }

    const quint32 pageSize = algo.info().pageSize;
    if (pageSize == 0 || pageSize > 1024u * 1024u) {
        error = QCoreApplication::translate("Flasher", "The algorithm reports an unusable page size (%1 bytes).").arg(pageSize);
        return false;
    }

    /* The page buffer is where ProgramPage() reads its data from */
    layout.bufferSize = alignUp(pageSize, 4);
    layout.bufferAddress = ramBase + layout.imageSize;
    layout.stackTop = alignUp(layout.bufferAddress + layout.bufferSize + kStackSize, 8);
    layout.trapAddress = layout.stackTop;
    layout.totalSize = layout.trapAddress - ramBase + 4;

    if (layout.trapAddress < ramBase) {
        error = QCoreApplication::translate("Flasher", "The RAM base address is too high for the algorithm.");
        return false;
    }

    return true;
}

bool Flasher::writeZeros(quint32 address, quint32 size, QString &error)
{
    QVector<quint32> words(kZeroChunk / 4, 0);

    quint32 done = 0;
    while (done < size) {
        const quint32 remaining = size - done;
        const int wordsNow = static_cast<int>(qMin<quint32>(remaining / 4,
                                                            static_cast<quint32>(words.size())));
        if (wordsNow == 0) {
            /* Tail smaller than one word: written byte by byte */
            QByteArray tail(static_cast<int>(remaining), '\0');
            if (!link_->writeBytes(address + done, tail, error))
                return false;
            break;
        }

        if (!link_->writeMem32(address + done, words.constData(), wordsNow, error))
            return false;

        done += static_cast<quint32>(wordsNow) * 4;
    }

    return true;
}

bool Flasher::downloadAlgorithm(const FlashAlgo &algo, const Layout &layout, QString &error)
{
    /* The segments are copied in ascending address order: the zero filled tail
     * of a lower segment must not overwrite the initialised data of a higher
     * one (Keil places the FlashDevice structure right behind the code). */
    QVector<ElfSegment> segments = algo.segments();

    std::sort(segments.begin(), segments.end(),
              [](const ElfSegment &a, const ElfSegment &b) {
                  return a.address < b.address;
              });

    for (const ElfSegment &segment : segments) {
        const quint32 target = layout.loadBase + segment.address;

        if (segment.memSize > segment.fileSize) {
            const quint32 zeroSize = segment.memSize - segment.fileSize;
            if (!writeZeros(target + segment.fileSize, zeroSize, error))
                return false;
        }

        if (!segment.data.isEmpty()) {
            if (!link_->writeBytes(target, segment.data, error))
                return false;
        }
    }

    /* The BKPT that ends every algorithm call */
    const quint32 trap = 0x0000BE00u;
    if (!link_->writeMem32(layout.trapAddress, &trap, 1, error))
        return false;

    /* Read the image back: a RAM area that does not exist answers with
     * garbage, which would otherwise show up later as a core lockup. */
    for (const ElfSegment &segment : segments) {
        if (segment.data.isEmpty())
            continue;

        const quint32 target = layout.loadBase + segment.address;
        QByteArray readBack;
        if (!link_->readBytes(target, segment.data.size(), readBack, error))
            return false;

        if (readBack != segment.data) {
            /* Report what actually differs. "The base address is probably not
             * RAM" was the only hint this used to give, which sends the user
             * looking for a hardware fault even when the probe simply lost
             * part of the block transfer. */
            int first = -1;
            int differing = 0;
            const int common = qMin(readBack.size(), segment.data.size());

            for (int i = 0; i < common; ++i) {
                if (readBack.at(i) != segment.data.at(i)) {
                    if (first < 0)
                        first = i;
                    ++differing;
                }
            }

            differing += readBack.size() - common;   /* a short read counts as well */
            if (first < 0)
                first = common;

            const quint8 wrote = first < segment.data.size()
                                     ? static_cast<quint8>(segment.data.at(first)) : 0;
            const quint8 got = first < readBack.size()
                                   ? static_cast<quint8>(readBack.at(first)) : 0;

            error = QCoreApplication::translate("Flasher",
                        "The algorithm could not be written to the target RAM at %1: "
                        "%2 of %3 bytes came back different, the first one at %4 "
                        "(wrote 0x%5, read 0x%6). The base address has to be RAM the "
                        "debug unit can write to; if it is, the probe did not move the "
                        "data reliably.")
                        .arg(hex32(target))
                        .arg(differing)
                        .arg(segment.data.size())
                        .arg(hex32(target + static_cast<quint32>(first)))
                        .arg(QStringLiteral("%1").arg(wrote, 2, 16, QLatin1Char('0')))
                        .arg(QStringLiteral("%1").arg(got, 2, 16, QLatin1Char('0')));
            return false;
        }
    }

    return true;
}

/* ==========================================================================
 * Algorithm call
 * ========================================================================== */

bool Flasher::callAlgorithm(const FlashAlgo &algo, const Layout &layout, const QString &function,
                            int timeoutMs, quint32 arg0, quint32 arg1, quint32 arg2,
                            quint32 &result, QString &error)
{
    result = 0;

    if (!algo.hasFunction(function)) {
        error = QCoreApplication::translate("Flasher", "The algorithm does not provide %1().").arg(function);
        return false;
    }

    const quint32 entry = layout.loadBase + algo.functionOffset(function);

    if (!core_->writeRegister(CoreReg::Sp, layout.stackTop, error))
        return false;

    if (!core_->callFunction(entry, layout.trapAddress, arg0, arg1, arg2, error))
        return false;

    if (!core_->waitForHalt(timeoutMs, error)) {
        /* A timeout on its own says nothing about what went wrong. Halt the
         * core and report where it actually is: a core parked in a fault
         * handler and a core that never left the entry point look identical
         * from the outside, and they are fixed in completely different
         * ways. */
        const QString reason = error;
        QString ignored;
        core_->halt(ignored);

        quint32 pc = 0;
        quint32 dfsr = 0;
        quint32 cfsr = 0;
        quint32 hfsr = 0;
        core_->readPc(pc, ignored);
        core_->readFaultStatus(dfsr, ignored);
        if (link_) {
            link_->readMem32(0xE000ED28u, &cfsr, 1, ignored);
            link_->readMem32(0xE000ED2Cu, &hfsr, 1, ignored);
        }

        error = QCoreApplication::translate("Flasher",
                    "%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)")
                    .arg(function, reason)
                    .arg(pc, 8, 16, QLatin1Char('0'))
                    .arg(dfsr, 8, 16, QLatin1Char('0'))
                    .arg(cfsr, 8, 16, QLatin1Char('0'))
                    .arg(hfsr, 8, 16, QLatin1Char('0'));
        return false;
    }

    /* Where did the core stop? It has to sit on the trap instruction. */
    quint32 pc = 0;
    if (!core_->readPc(pc, error))
        return false;

    if ((pc & ~1u) != layout.trapAddress) {
        quint32 dfsr = 0;
        QString ignored;
        core_->readFaultStatus(dfsr, ignored);

        error = QCoreApplication::translate("Flasher", "%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).")
                    .arg(function)
                    .arg(pc, 8, 16, QLatin1Char('0'))
                    .arg(layout.trapAddress, 8, 16, QLatin1Char('0'))
                    .arg(dfsr, 8, 16, QLatin1Char('0'));
        return false;
    }

    if (!core_->readReturnValue(result, error))
        return false;

    return true;
}

/* ==========================================================================
 * Erase
 * ========================================================================== */

bool Flasher::eraseFlash(const FlashAlgo &algo, const Layout &layout,
                         const QVector<FlashRange> &ranges, const Config &config,
                         const FlasherCallbacks &callbacks, FlashStatistics &stats, QString &error)
{
    const quint32 eraseTimeout = qMax(algo.info().eraseTimeout + 1000u,
                                      static_cast<quint32>(kEraseTimeoutFloor));

    /* Whole chip erase when asked for and supported */
    if (config.eraseWholeChip && algo.hasFunction(QStringLiteral("EraseChip"))) {
        if (!reportProgress(callbacks, kProgressEraseStart, QCoreApplication::translate("Flasher", "Erasing the whole chip ...")))
            return false;

        quint32 result = 0;
        const int timeout = qMin(static_cast<int>(eraseTimeout) * 8, kChipEraseTimeoutCeiling);

        if (!callAlgorithm(algo, layout, QStringLiteral("EraseChip"), timeout,
                           0, 0, 0, result, error)) {
            return false;
        }
        if (result != 0) {
            error = QCoreApplication::translate("Flasher", "EraseChip() failed with code %1.").arg(result);
            return false;
        }

        stats.sectorsErased = algo.info().sectors.size();
        log(callbacks, QCoreApplication::translate("Flasher", "Chip erase done."));
        return true;
    }

    if (!algo.hasFunction(QStringLiteral("EraseSector"))) {
        error = QCoreApplication::translate("Flasher", "The algorithm provides neither EraseChip() nor EraseSector().");
        return false;
    }

    /* The sectors covering every range, without duplicates */
    QVector<quint32> sectorAddresses;
    for (const FlashRange &range : ranges) {
        if (range.data.isEmpty())
            continue;

        const quint32 first = range.address;
        const quint32 last = range.address + static_cast<quint32>(range.data.size()) - 1;

        int index = algo.sectorIndexFor(first);
        if (index < 0) {
            error = QCoreApplication::translate("Flasher", "Address 0x%1 is not covered by the sector table of the algorithm.")
                        .arg(first, 8, 16, QLatin1Char('0'));
            return false;
        }

        while (index >= 0 && index < algo.info().sectors.size()) {
            const FlashSector &sector = algo.info().sectors.at(index);
            const quint32 absolute = algo.info().deviceAddress + sector.address;

            if (!sectorAddresses.contains(absolute))
                sectorAddresses.append(absolute);

            if (algo.info().deviceAddress + sector.address + sector.size > last)
                break;

            ++index;
        }
    }

    if (sectorAddresses.isEmpty()) {
        error = QCoreApplication::translate("Flasher", "Nothing to erase.");
        return false;
    }

    for (int i = 0; i < sectorAddresses.size(); ++i) {
        if (isCancelled(callbacks)) {
            error = QCoreApplication::translate("Flasher", "Cancelled.");
            return false;
        }

        const int percent = kProgressEraseStart
                            + (kProgressEraseEnd - kProgressEraseStart) * i
                                  / qMax(1, sectorAddresses.size());

        if (!reportProgress(callbacks, percent,
                            QCoreApplication::translate("Flasher", "Erasing sector %1 of %2 (0x%3) ...")
                                .arg(i + 1)
                                .arg(sectorAddresses.size())
                                .arg(sectorAddresses.at(i), 8, 16, QLatin1Char('0')))) {
            return false;
        }

        quint32 result = 0;
        if (!callAlgorithm(algo, layout, QStringLiteral("EraseSector"), static_cast<int>(eraseTimeout),
                           sectorAddresses.at(i), 0, 0, result, error)) {
            return false;
        }

        if (result != 0) {
            error = QCoreApplication::translate("Flasher", "Erasing the sector at 0x%1 failed with code %2.")
                        .arg(sectorAddresses.at(i), 8, 16, QLatin1Char('0'))
                        .arg(result);
            return false;
        }

        ++stats.sectorsErased;
    }

    return true;
}

/* ==========================================================================
 * Program
 * ========================================================================== */

bool Flasher::programFlash(const FlashAlgo &algo, const Layout &layout,
                           const QVector<FlashRange> &ranges,
                           const FlasherCallbacks &callbacks, FlashStatistics &stats, QString &error)
{
    const quint32 pageSize = algo.info().pageSize;
    const quint8 emptyValue = algo.info().emptyValue;
    const int programTimeout = static_cast<int>(qMax(algo.info().programTimeout + 1000u,
                                                     static_cast<quint32>(kProgramTimeoutFloor)));

    quint32 totalBytes = 0;
    for (const FlashRange &range : ranges)
        totalBytes += static_cast<quint32>(range.data.size());

    if (totalBytes == 0) {
        error = QCoreApplication::translate("Flasher", "The firmware file carries no data.");
        return false;
    }

    quint32 written = 0;

    for (const FlashRange &range : ranges) {
        if (range.data.isEmpty())
            continue;

        const quint32 start = range.address;
        const quint32 end = start + static_cast<quint32>(range.data.size());
        quint32 address = start;

        while (address < end) {
            if (isCancelled(callbacks)) {
                error = QCoreApplication::translate("Flasher", "Cancelled.");
                return false;
            }

            /* The algorithm programs whole pages, so the buffer is built for
             * the page that contains the current address */
            const quint32 pageBase = address & ~(pageSize - 1);
            const quint32 pageOffset = address - pageBase;
            const quint32 room = pageSize - pageOffset;
            const quint32 copy = qMin(room, end - address);

            QByteArray page(static_cast<int>(pageSize), static_cast<char>(emptyValue));
            std::memcpy(page.data() + pageOffset,
                        range.data.constData() + (address - start),
                        static_cast<size_t>(copy));

            /* Per-page timing probe, off unless DAPFLASHER_POLL_STATS is set.
             * A page costs far more than the wait for the algorithm to return,
             * and guessing which part dominates has been wrong twice, so the
             * four stages are measured separately. */
            static const bool statsOn = qEnvironmentVariableIsSet("DAPFLASHER_POLL_STATS");
            QElapsedTimer stageTimer;
            if (statsOn)
                stageTimer.start();

            if (!link_->writeBytes(layout.bufferAddress, page, error))
                return false;

            const qint64 tWrite = statsOn ? stageTimer.nsecsElapsed() / 1000 : 0;

            const int percent = kProgressProgramStart
                                + static_cast<int>((kProgressProgramEnd - kProgressProgramStart)
                                                       * static_cast<qint64>(written)
                                                   / static_cast<qint64>(totalBytes));

            if (!reportProgress(callbacks, percent,
                                QCoreApplication::translate("Flasher", "Programming 0x%1 (%2 / %3 bytes) ...")
                                    .arg(pageBase, 8, 16, QLatin1Char('0'))
                                    .arg(written)
                                    .arg(totalBytes))) {
                return false;
            }

            const qint64 tProgress = statsOn ? stageTimer.nsecsElapsed() / 1000 : 0;

            quint32 result = 0;
            if (!callAlgorithm(algo, layout, QStringLiteral("ProgramPage"), programTimeout,
                               pageBase, static_cast<quint32>(page.size()),
                               layout.bufferAddress, result, error)) {
                return false;
            }

            if (statsOn) {
                const qint64 tTotal = stageTimer.nsecsElapsed() / 1000;
                fprintf(stderr, "[page] write=%lldus progress=%lldus algo=%lldus total=%lldus\n",
                        static_cast<long long>(tWrite),
                        static_cast<long long>(tProgress - tWrite),
                        static_cast<long long>(tTotal - tProgress),
                        static_cast<long long>(tTotal));
            }

            if (result != 0) {
                error = QCoreApplication::translate("Flasher", "Programming the page at 0x%1 failed with code %2.")
                            .arg(pageBase, 8, 16, QLatin1Char('0'))
                            .arg(result);
                return false;
            }

            ++stats.pagesProgrammed;
            written += copy;
            address += copy;
        }
    }

    stats.bytesProgrammed = totalBytes;
    return true;
}

/* ==========================================================================
 * Verify
 * ========================================================================== */

bool Flasher::verifyFlash(const FlashAlgo &algo, const Layout &layout,
                          const QVector<FlashRange> &ranges,
                          const FlasherCallbacks &callbacks, QString &error)
{
    const quint32 pageSize = algo.info().pageSize;
    const quint8 emptyValue = algo.info().emptyValue;

    quint32 totalBytes = 0;
    for (const FlashRange &range : ranges)
        totalBytes += static_cast<quint32>(range.data.size());

    const bool useAlgorithm = algo.hasFunction(QStringLiteral("Verify"));
    const int timeout = static_cast<int>(qMax(algo.info().programTimeout + 1000u,
                                              static_cast<quint32>(kProgramTimeoutFloor)));

    if (!useAlgorithm) {
        log(callbacks, QCoreApplication::translate("Flasher", "The algorithm has no Verify() routine; the flash is read back "
                           "through the debug port instead."));
    }

    quint32 verified = 0;

    for (const FlashRange &range : ranges) {
        if (range.data.isEmpty())
            continue;

        const quint32 start = range.address;
        const quint32 end = start + static_cast<quint32>(range.data.size());
        quint32 address = start;

        while (address < end) {
            if (isCancelled(callbacks)) {
                error = QCoreApplication::translate("Flasher", "Cancelled.");
                return false;
            }

            const quint32 pageBase = address & ~(pageSize - 1);
            const quint32 pageOffset = address - pageBase;
            const quint32 room = pageSize - pageOffset;
            const quint32 copy = qMin(room, end - address);

            QByteArray expected(static_cast<int>(pageSize), static_cast<char>(emptyValue));
            std::memcpy(expected.data() + pageOffset,
                        range.data.constData() + (address - start),
                        static_cast<size_t>(copy));

            const int percent = kProgressVerifyStart
                                + static_cast<int>((kProgressVerifyEnd - kProgressVerifyStart)
                                                       * static_cast<qint64>(verified)
                                                   / qMax<qint64>(1, totalBytes));

            if (!reportProgress(callbacks, percent,
                                QCoreApplication::translate("Flasher", "Verifying 0x%1 ...")
                                    .arg(pageBase, 8, 16, QLatin1Char('0')))) {
                return false;
            }

            if (useAlgorithm) {
                if (!link_->writeBytes(layout.bufferAddress, expected, error))
                    return false;

                quint32 result = 0;
                if (!callAlgorithm(algo, layout, QStringLiteral("Verify"), timeout,
                                   pageBase, static_cast<quint32>(expected.size()),
                                   layout.bufferAddress, result, error)) {
                    return false;
                }

                if (result != 0) {
                    error = QCoreApplication::translate("Flasher", "Verification failed at 0x%1 (code %2).")
                                .arg(pageBase, 8, 16, QLatin1Char('0'))
                                .arg(result);
                    return false;
                }
            } else {
                QByteArray readBack;
                if (!link_->readBytes(pageBase, expected.size(), readBack, error)) {
                    log(callbacks, QCoreApplication::translate("Flasher", "Reading back 0x%1 failed; the verification was skipped.")
                                       .arg(pageBase, 8, 16, QLatin1Char('0')), true);
                    return true;
                }

                if (readBack != expected) {
                    int offset = 0;
                    for (int i = 0; i < expected.size(); ++i) {
                        if (readBack.at(i) != expected.at(i)) {
                            offset = i;
                            break;
                        }
                    }

                    error = QCoreApplication::translate("Flasher", "Verification failed at 0x%1: expected 0x%2, read 0x%3.")
                                .arg(pageBase + static_cast<quint32>(offset), 8, 16, QLatin1Char('0'))
                                .arg(static_cast<quint8>(expected.at(offset)), 2, 16, QLatin1Char('0'))
                                .arg(static_cast<quint8>(readBack.at(offset)), 2, 16, QLatin1Char('0'));
                    return false;
                }
            }

            verified += copy;
            address += copy;
        }
    }

    return true;
}

/* ==========================================================================
 * Entry point
 * ========================================================================== */

bool Flasher::program(const Config &config, const QVector<FlashRange> &ranges,
                      const FlasherCallbacks &callbacks, QString &error,
                      FlashStatistics *statistics)
{
    FlashStatistics stats;

    if (!config.algo || !config.algo->isValid()) {
        error = QCoreApplication::translate("Flasher", "No flash algorithm is selected.");
        return false;
    }

    const FlashAlgo &algo = *config.algo;

    /* An erase only run has nothing to program: the whole chip goes and the
     * target is left blank, which is how a target whose own program holds the
     * debug access is brought back. */
    if (ranges.isEmpty() && !config.eraseWholeChip) {
        error = QCoreApplication::translate("Flasher", "No firmware data to program.");
        return false;
    }

    /* ---------- 1. layout and download ---------- */
    Layout layout;
    if (!computeLayout(algo, config.ramBase, layout, error))
        return false;

    stats.ramBytesUsed = layout.totalSize;
    stats.ramBase = layout.loadBase;

    if (!reportProgress(callbacks, kProgressPrepare, QCoreApplication::translate("Flasher", "Preparing ...")))
        return false;

    log(callbacks, QCoreApplication::translate("Flasher", "Algorithm: %1 (%2, %3 bytes)")
                       .arg(algo.info().deviceName, algo.deviceTypeName())
                       .arg(algo.imageSize()));

    /* ---------- 0. start from a known state ---------- */
    /* The target is reset and halted *before* anything touches the flash, and
     * it is not only the reset-the-program-cannot-survive that this buys.
     *
     * A run that ends without going through the normal path - Ctrl+C, a closed
     * window, a killed process, a USB hiccup - leaves the target mid-operation:
     * the flash controller keeps the error flags of the erase it was in the
     * middle of, and the core can be left locked up. Neither a freshly
     * downloaded RAM image nor Init() clears that, and the next run then fails
     * in a way that looks like a firmware problem rather than a leftover state.
     * Measured on an STM32F411: a kill during the erase made the next three
     * runs fail with "Programming the page at 0x08000000 failed with code 1",
     * and one single reset made the run right after it pass again.
     *
     * Resetting costs one reset, which is also what the other programmers do
     * before a flash, and the target is about to be reprogrammed anyway. The
     * reset is asked for the way the user configured it; a target that cannot
     * be reset falls back to the plain halt this used to do. */
    {
        QString resetError;
        ResetMode used = config.resetMode;

        if (!core_->resetWithMode(config.resetMode, true, &used, resetError)) {
            /* The reset is the rescue, and when it does not work the failure
             * that is worth reporting is the halt the user asked for. */
            error = resetError;
            if (!core_->halt(error))
                return false;
        } else {
            log(callbacks, QCoreApplication::translate("Flasher", "Reset: %1").arg(resetModeName(used)));
        }
    }

    /* Name the transfer size in use: it is the one number that tells whether
     * the probe is driven the way it wants to be, and it is the first thing
     * to look at when a RAM transfer comes back wrong. */
    if (link_) {
        const int blockWords = link_->maxBlockWords();
        log(callbacks, QCoreApplication::translate("Flasher",
                           "RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, "
                           "command packet %3 bytes.")
                           .arg(blockWords)
                           .arg(blockWords * 4)
                           .arg(link_->packetSize()));
    }

    bool downloaded = false;
    for (int attempt = 1; attempt <= kDownloadAttempts && !downloaded; ++attempt) {
        downloaded = downloadAlgorithm(algo, layout, error);

        if (!downloaded && attempt < kDownloadAttempts) {
            log(callbacks, QCoreApplication::translate("Flasher",
                               "Writing the algorithm to the RAM failed, attempt %1 of %2: %3")
                               .arg(attempt)
                               .arg(kDownloadAttempts)
                               .arg(error), true);
        }
    }

    if (!downloaded)
        return false;

    if (!reportProgress(callbacks, kProgressLoaded,
                        QCoreApplication::translate("Flasher", "Algorithm loaded at 0x%1 (%2 bytes of RAM).")
                            .arg(layout.loadBase, 8, 16, QLatin1Char('0'))
                            .arg(layout.totalSize))) {
        return false;
    }

    /* ---------- 2. Init ---------- */
    /* A target that is running its own program can take the debug access back
     * between the halt and the call, and the core then never reaches the
     * algorithm: Init() times out with the core sitting in whatever the target
     * firmware was doing. Nothing about that state is broken, it is only
     * stale, so the whole start-up sequence is rebuilt and the call is made
     * once more before the session is given up. */
    {
        quint32 result = 0;
        bool ok = false;

        for (int attempt = 1; attempt <= kInitAttempts && !ok; ++attempt) {
            if (attempt > 1) {
                log(callbacks, QCoreApplication::translate("Flasher",
                                   "Init() did not complete, restarting the algorithm (%1 of %2): %3")
                                   .arg(attempt)
                                   .arg(kInitAttempts)
                                   .arg(error), true);

                /* Halt first: it is the state the retry starts from, and the
                 * halt is also what clears a MASKINTS that a previous release
                 * left behind on a running core. */
                QString haltError;
                if (!core_->halt(haltError)) {
                    error = haltError;
                    break;
                }

                /* The RAM image is re-downloaded as well. A core that ran free
                 * for a moment may have written over it, and a stale copy
                 * fails in a way that looks random rather than diagnosable. */
                if (!downloadAlgorithm(algo, layout, error))
                    break;
            }

            ok = callAlgorithm(algo, layout, QStringLiteral("Init"), kInitTimeout,
                               algo.info().deviceAddress, config.clockHz, 1, result, error);
            if (ok && result != 0) {
                error = QCoreApplication::translate("Flasher", "Init() failed with code %1.").arg(result);
                ok = false;
            }
        }

        if (!ok)
            return false;
    }

    if (!reportProgress(callbacks, kProgressInit, QCoreApplication::translate("Flasher", "Initialised.")))
        return false;

    /* ---------- 3. Erase ---------- */
    if (!eraseFlash(algo, layout, ranges, config, callbacks, stats, error)) {
        QString ignored;
        quint32 result = 0;
        callAlgorithm(algo, layout, QStringLiteral("UnInit"), kUnInitTimeout, 1, 0, 0, result, ignored);
        return false;
    }

    /* ---------- 4. Program ---------- */
    /* An erase only run has nothing to write, and the algorithm is right to
     * refuse an empty image: it is skipped instead of being called. */
    if (!ranges.isEmpty() && !programFlash(algo, layout, ranges, callbacks, stats, error)) {
        QString ignored;
        quint32 result = 0;
        callAlgorithm(algo, layout, QStringLiteral("UnInit"), kUnInitTimeout, 1, 0, 0, result, ignored);
        return false;
    }

    /* ---------- 5. Verify ---------- */
    if (!ranges.isEmpty() && config.verify
        && !verifyFlash(algo, layout, ranges, callbacks, error)) {
        QString ignored;
        quint32 result = 0;
        callAlgorithm(algo, layout, QStringLiteral("UnInit"), kUnInitTimeout, 1, 0, 0, result, ignored);
        return false;
    }

    /* ---------- 6. UnInit ---------- */
    if (algo.hasFunction(QStringLiteral("UnInit"))) {
        quint32 result = 0;
        QString uninitError;
        if (!callAlgorithm(algo, layout, QStringLiteral("UnInit"), kUnInitTimeout,
                           1, 0, 0, result, uninitError)) {
            /* The flash is already programmed: a failed UnInit is reported but
             * does not turn the operation into a failure. */
            log(callbacks, QCoreApplication::translate("Flasher", "UnInit() failed: %1").arg(uninitError), true);
        } else if (result != 0) {
            log(callbacks, QCoreApplication::translate("Flasher", "UnInit() returned code %1.").arg(result), true);
        }
    }

    /* ---------- 7. Finish ---------- */
    if (config.resetAfterProgram) {
        ResetMode used = config.resetMode;
        QString resetError;
        if (!core_->resetWithMode(config.resetMode, false, &used, resetError)) {
            log(callbacks, QCoreApplication::translate("Flasher", "Resetting the target failed: %1").arg(resetError), true);
        } else {
            log(callbacks, QCoreApplication::translate("Flasher", "Reset: %1").arg(resetModeName(used)));
        }
    } else {
        QString ignored;
        core_->halt(ignored);
    }

    if (!reportProgress(callbacks, 100, QCoreApplication::translate("Flasher", "Done.")))
        return false;

    if (statistics)
        *statistics = stats;

    return true;
}
