/*
 * WeAct Studio DAP Flasher
 *
 * The program operation.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "flashworker.h"

#include "dap/cmsisdap.h"
#include "dap/dapdefs.h"
#include "flash/flashalgo.h"
#include "firmwareimage.h"
#include "target/cortexm.h"

#include <QCoreApplication>
#include <QElapsedTimer>

namespace {

QString hex32(quint32 value)
{
    return QStringLiteral("0x%1").arg(value, 8, 16, QLatin1Char('0'));
}

/**
 * The clock the command line always starts at.
 *
 * FlashRequest defaults to it and the command line never stores anything, so
 * every command line run uses exactly this. The window instead restores the
 * clock of the previous run, and a clock that is fast for the wiring fails
 * somewhere in the middle - which is how "it works from the command line but
 * not from the window" happens although both run the same code.
 */
const quint32 kCommandLineClock = 1000000u;

/**
 * One complete attempt.
 *
 * @param onAir   set when the probe was opened, i.e. when the failure came
 *                from the target link and a slower clock can still help.
 *                Left alone for the failures that a retry cannot fix, such
 *                as an algorithm that does not match the firmware.
 */
ProgramResult runSession(const FlashRequest &request,
                         const std::function<bool(int, const QString &)> &progress,
                         const std::function<void(const QString &, bool)> &log,
                         const std::function<bool()> &cancelled,
                         bool *onAir)
{
    ProgramResult result;

    const auto report = [&progress](int percent, const QString &status) {
        return progress ? progress(percent, status) : true;
    };

    const auto note = [&log](const QString &message, bool isError = false) {
        if (log)
            log(message, isError);
    };

    const auto stopped = [&cancelled]() {
        return cancelled ? cancelled() : false;
    };

    /* ---------- 1. the algorithm ---------- */
    FlashAlgo algo;
    QString error;

    if (!report(1, QCoreApplication::translate("FlashWorker", "Loading the flash algorithm ...")))
        return result;

    if (!algo.load(request.algoPath, error)) {
        result.message = error;
        return result;
    }

    result.algoDevice = algo.info().deviceName;
    note(QCoreApplication::translate("FlashWorker", "Algorithm %1: %2, %3, page %4 bytes, %5 sectors")
             .arg(algo.fileName(), algo.info().deviceName, algo.deviceTypeName())
             .arg(algo.info().pageSize)
             .arg(algo.info().sectors.size()));

    /* ---------- 2. the firmware ---------- */
    /* An erase only run is how a target whose own program holds the debug
     * access is brought back: the image in the way has to go before a new one
     * can be written, and the erase does not need a replacement image. */
    const bool eraseOnly = request.eraseWholeChip && request.firmwarePath.isEmpty();

    FirmwareImage image;

    if (!eraseOnly) {
        if (!firmware_load(request.firmwarePath, request.firmwareBase, image, error)) {
            result.message = error;
            return result;
        }

        /* The size is the payload, not the address span: a HEX file with a
         * bootloader at 0x08000000 and an application at 0x08020000 covers
         * 448 KB of address space but only carries 325 KB of data, and the
         * payload is what is programmed. */
        result.firmwareSummary = QCoreApplication::translate("FlashWorker", "%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6")
                                     .arg(image.fromHex ? QCoreApplication::translate("FlashWorker", "Intel HEX") : QCoreApplication::translate("FlashWorker", "BIN"),
                                          firmware_format_size(image.payloadSize()))
                                     .arg(image.segments.size())
                                     .arg(image.startAddress, 8, 16, QLatin1Char('0'))
                                     .arg(image.endAddress, 8, 16, QLatin1Char('0'))
                                     .arg(image.checksum, 8, 16, QLatin1Char('0'));

        note(QCoreApplication::translate("FlashWorker", "Firmware %1").arg(result.firmwareSummary));

        /* The image has to live inside the flash the algorithm describes */
        const quint32 deviceEnd = algo.info().deviceAddress + algo.info().deviceSize;
        if (image.startAddress < algo.info().deviceAddress || image.endAddress >= deviceEnd) {
            result.message = QCoreApplication::translate("FlashWorker", "The firmware (0x%1 - 0x%2) does not fit into the flash of the "
                                 "algorithm (0x%3 - 0x%4). Check the loading address.")
                                 .arg(image.startAddress, 8, 16, QLatin1Char('0'))
                                 .arg(image.endAddress, 8, 16, QLatin1Char('0'))
                                 .arg(algo.info().deviceAddress, 8, 16, QLatin1Char('0'))
                                 .arg(deviceEnd - 1, 8, 16, QLatin1Char('0'));
            return result;
        }

        if (algo.sectorIndexFor(image.startAddress) < 0) {
            result.message = QCoreApplication::translate("FlashWorker", "The algorithm has no sector at 0x%1.")
                                 .arg(image.startAddress, 8, 16, QLatin1Char('0'));
            return result;
        }
    } else {
        note(QCoreApplication::translate("FlashWorker",
                 "No firmware given: the whole chip is erased and the target is left halted."));
    }

    if (stopped()) {
        result.message = QCoreApplication::translate("FlashWorker", "Cancelled.");
        return result;
    }

    /* ---------- 3. the probe ---------- */
    CmsisDap link;
    CortexM core(&link);

    if (!link.open(request.probe, error)) {
        result.message = error;
        return result;
    }

    /* From here on the probe is in hand, so a failure is a failure of the
     * target link and not of the input. */
    if (onAir)
        *onAir = true;

    note(QCoreApplication::translate("FlashWorker", "Probe: %1").arg(link.transportName()));

    const QString firmwareVersion = link.infoString(DapInfoId::FirmwareVer);
    if (!firmwareVersion.isEmpty())
        note(QCoreApplication::translate("FlashWorker", "CMSIS-DAP firmware: %1").arg(firmwareVersion));

    /* ---------- 4. the core ---------- */
    quint32 dpIdCode = 0;
    QString plainError;

    if (!link.attachSwd(request.clockHz, dpIdCode, note, plainError) || !core.probe(plainError)) {
        /* The target program is holding the debug access, which is exactly
         * the board the user is trying to program again. The reset it cannot
         * survive is the way in - and erasing the chip is the way out. */
        if (!recoverUnderReset(link, core, request.clockHz, note, dpIdCode, error)) {
            link.close();
            result.message = plainError;
            return result;
        }
    }

    result.target.ok = true;
    result.target.dpIdCode = dpIdCode;
    result.target.cpuid = core.cpuid();
    result.target.coreName = core.coreName();
    result.target.probeName = link.transportName();
    result.target.probeFirmware = firmwareVersion;

    note(QCoreApplication::translate("FlashWorker", "Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)")
             .arg(core.coreName())
             .arg(core.cpuid(), 8, 16, QLatin1Char('0'))
             .arg(dpIdCode, 8, 16, QLatin1Char('0')));

    /* ---------- 5. program ---------- */
    Flasher::Config config;
    config.algo = &algo;
    config.ramBase = request.ramBase;
    config.clockHz = request.clockHz;
    config.eraseWholeChip = request.eraseWholeChip;
    config.verify = request.verify;
    /* An erased chip has no vector table left: letting the core run would
     * only send it into a lockup, so it stays halted instead. */
    config.resetAfterProgram = eraseOnly ? false : request.resetAfterProgram;
    config.resetMode = request.resetMode;

    QVector<FlashRange> ranges;

    if (!eraseOnly) {
        /* Only the blocks that carry data are programmed. The gaps a HEX file
         * leaves between them are not part of the image: writing them would
         * erase and program flash the file says nothing about, and on the
         * usual bootloader-plus-application image that is the difference
         * between programming 325 KB and programming 448 KB. */
        for (const FirmwareSegment &segment : image.segments) {
            if (segment.data.isEmpty())
                continue;

            FlashRange range;
            range.address = segment.address;
            range.data = segment.data;
            ranges.append(range);
        }

        /* Never program nothing: a HEX file whose records all touch still
         * goes through the block list, but an image built some other way
         * must not end up with an empty list. */
        if (ranges.isEmpty()) {
            FlashRange range;
            range.address = image.startAddress;
            range.data = image.data;
            ranges.append(range);
        }
    }

    FlasherCallbacks callbacks;
    callbacks.progress = progress;
    callbacks.log = log;
    callbacks.cancelled = cancelled;

    QElapsedTimer timer;
    timer.start();

    const bool ok = Flasher(&link, &core).program(config, ranges, callbacks, error,
                                                  &result.statistics);

    if (!ok) {
        /* A cancelled run is not an error the user has to worry about */
        result.ok = false;
        result.message = error;
        if (stopped() && error == QCoreApplication::translate("FlashWorker", "Cancelled."))
            result.message = QCoreApplication::translate("FlashWorker", "Cancelled.");
        link.close();
        return result;
    }

    const double seconds = timer.elapsed() / 1000.0;

    result.ok = true;
    result.message = eraseOnly
                         ? QCoreApplication::translate("FlashWorker",
                                   "The whole chip was erased in %1 s; the target is left halted.")
                               .arg(seconds, 0, 'f', 2)
                         : QCoreApplication::translate("FlashWorker", "Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.")
                               .arg(seconds, 0, 'f', 2)
                               .arg(result.statistics.pagesProgrammed)
                               .arg(result.statistics.bytesProgrammed)
                               .arg(result.statistics.sectorsErased);

    note(result.message);
    note(QCoreApplication::translate("FlashWorker", "The algorithm used 0x%1 of RAM at %2.")
             .arg(result.statistics.ramBytesUsed)
             .arg(hex32(result.statistics.ramBase)));

    link.close();
    return result;
}

} /* namespace */

ProgramResult runProgramSession(const FlashRequest &request,
                                const std::function<bool(int, const QString &)> &progress,
                                const std::function<void(const QString &, bool)> &log,
                                const std::function<bool()> &cancelled)
{
    bool onAir = false;
    const ProgramResult result = runSession(request, progress, log, cancelled, &onAir);

    /* Only a run that started faster than the command line's own clock is
     * repeated, and then at exactly that clock. A run the command line would
     * fail at as well is reported straight away: repeating it would only make
     * the window slower than the command line, not better. */
    if (result.ok || !onAir || request.clockHz <= kCommandLineClock)
        return result;

    /* Giving up because the user asked for it is not a link problem */
    if (cancelled && cancelled())
        return result;

    const FlashRequest retry = [&request]() {
        FlashRequest copy = request;
        copy.clockHz = kCommandLineClock;
        return copy;
    }();

    if (log) {
        log(QCoreApplication::translate("FlashWorker", "The SWD link failed at %1; retrying at %2.")
                .arg(CmsisDap::clockText(request.clockHz), CmsisDap::clockText(retry.clockHz)),
            true);
    }

    if (progress)
        progress(1, QCoreApplication::translate("FlashWorker", "Retrying at a slower clock ..."));

    return runSession(retry, progress, log, cancelled, nullptr);
}

/* ==========================================================================
 * FlashWorker
 * ========================================================================== */

FlashWorker::FlashWorker(const FlashRequest &request, QObject *parent)
    : QObject(parent)
    , request_(request)
    , cancelled_(false)
{
}

void FlashWorker::cancel()
{
    cancelled_ = true;
}

void FlashWorker::run()
{
    const ProgramResult result = runProgramSession(
        request_,
        [this](int percent, const QString &status) {
            Q_EMIT progress(percent, status);
            return !cancelled_;
        },
        [this](const QString &message, bool isError) {
            Q_EMIT logMessage(message, isError);
        },
        [this]() { return cancelled_; });

    Q_EMIT finished(result.ok, result.message);
}
