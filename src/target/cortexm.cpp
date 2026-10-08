/*
 * WeAct Studio DAP Flasher
 *
 * Cortex-M core control: halt, run, reset and core register access.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "cortexm.h"

#include "dap/cmsisdap.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QThread>

#include <cstdio>

namespace {

/* System control block */
const quint32 kCpuid = 0xE000ED00u;
const quint32 kDfsr  = 0xE000ED30u;
const quint32 kAircr = 0xE000ED0Cu;
const quint32 kDhcsr = 0xE000EDF0u;
const quint32 kDcrsr = 0xE000EDF4u;
const quint32 kDcrdr = 0xE000EDF8u;
const quint32 kDemcr = 0xE000EDFCu;

/* DHCSR bits */
const quint32 kDbgKey    = 0xA05F0000u;
const quint32 kCDebugEn  = 0x00000001u;
const quint32 kCHalt     = 0x00000002u;
const quint32 kCMaskInts = 0x00000008u;
const quint32 kSRegRdy   = 0x00010000u;
const quint32 kSHalt     = 0x00020000u;
const quint32 kSLockup   = 0x00080000u;
const quint32 kSResetSt  = 0x02000000u;

/* DCRSR / DEMCR / AIRCR bits */
const quint32 kRegWnR       = 0x00010000u;
const quint32 kVecCoreReset = 0x00000001u;
const quint32 kAircrKey     = 0x05FA0000u;
const quint32 kSysResetReq  = 0x00000004u;
const quint32 kVecKey       = 0x0000FFFFu;

} /* namespace */

CortexM::CortexM(CmsisDap *link)
    : link_(link)
    , valid_(false)
    , cpuid_(0)
{
}

QString CortexM::coreName() const
{
    if (!valid_)
        return QString();

    const quint32 part = (cpuid_ >> 4) & 0xFFFu;

    switch (part) {
    case 0xC20: return QStringLiteral("Cortex-M0");
    case 0xC21: return QStringLiteral("Cortex-M1");
    case 0xC23: return QStringLiteral("Cortex-M3");
    case 0xC24: return QStringLiteral("Cortex-M4");
    case 0xC27: return QStringLiteral("Cortex-M7");
    case 0xC60: return QStringLiteral("Cortex-M0+");
    case 0xD20: return QStringLiteral("Cortex-M23");
    case 0xD21: return QStringLiteral("Cortex-M33");
    case 0xD22: return QStringLiteral("Cortex-M55");
    case 0xD23: return QStringLiteral("Cortex-M85");
    default:    break;
    }

    return QCoreApplication::translate("CortexM", "Unknown Cortex-M core");
}

QString CortexM::cpuidText() const
{
    return QStringLiteral("0x%1").arg(cpuid_, 8, 16, QLatin1Char('0')).toUpper().replace(
        QStringLiteral("0X"), QStringLiteral("0x"));
}

/* ==========================================================================
 * Probing
 * ========================================================================== */

bool CortexM::probe(QString &error)
{
    valid_ = false;
    cpuid_ = 0;

    if (!link_->readMem32(kCpuid, &cpuid_, 1, error))
        return false;

    /* A missing core answers all ones or all zeros */
    if (cpuid_ == 0x00000000u || cpuid_ == 0xFFFFFFFFu) {
        error = QCoreApplication::translate("CortexM", "No Cortex-M core answered at CPUID (0x%1).")
                    .arg(cpuid_, 8, 16, QLatin1Char('0'));
        return false;
    }

    valid_ = true;
    return true;
}

/* ==========================================================================
 * Run state
 * ========================================================================== */

bool CortexM::writeDhcsr(quint32 value, QString &error)
{
    return link_->writeMem32(kDhcsr, &value, 1, error);
}

bool CortexM::readDhcsr(quint32 &value, QString &error)
{
    return link_->readMem32(kDhcsr, &value, 1, error);
}

bool CortexM::enableDebug(QString &error)
{
    return writeDhcsr(kDbgKey | kCDebugEn, error);
}

bool CortexM::halt(QString &error)
{
    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
        return false;

    if (!waitForHalt(2000, error))
        return false;

    /* Clear a leftover C_MASKINTS while the core is halted.
     *
     * The core only takes C_HALT from the write above when it was running, so
     * C_MASKINTS survives it; a second write now changes C_MASKINTS alone,
     * which a halted core does take. Leaving it set would make the next
     * resumeMasked() useless: a write that clears C_HALT while C_MASKINTS is
     * set is dropped, the core stays halted and the flash algorithm never
     * runs. */
    QString ignored;
    writeDhcsr(kDbgKey | kCDebugEn | kCHalt, ignored);
    return true;
}

bool CortexM::resume(QString &error)
{
    /* C_MASKINTS may only be changed while the core is halted, and a target
     * left with it set would run with every interrupt masked - SysTick stops
     * and a WFI never returns. Halt the core, clear the flag, release it.
     *
     * Two writes go to the halting value: the first is taken as C_HALT by a
     * running core (its C_MASKINTS change is ignored), the second - with the
     * core halted - is what clears C_MASKINTS. */
    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
        return false;

    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
        return false;

    return writeDhcsr(kDbgKey | kCDebugEn, error);
}

bool CortexM::resumeMasked(QString &error)
{
    /* Run with interrupts masked, which is what a flash algorithm needs.
     * Every write changes one group of DHCSR bits, because a write that
     * changes C_MASKINTS together with C_HALT is ignored by the core:
     *  1+2. halt the core, then clear a leftover C_MASKINTS (the running core
     *       only takes C_HALT from the first of the two)
     *  3.   assert C_MASKINTS while the core is halted
     *  4.   release C_HALT - C_MASKINTS is already stable, so only C_HALT
     *       changes and the write is taken
     */
    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
        return false;

    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
        return false;

    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt | kCMaskInts, error))
        return false;

    return writeDhcsr(kDbgKey | kCDebugEn | kCMaskInts, error);
}

bool CortexM::isHalted(bool &halted, QString &error)
{
    quint32 dhcsr = 0;
    if (!readDhcsr(dhcsr, error))
        return false;

    halted = (dhcsr & kSHalt) != 0;
    return true;
}

bool CortexM::resetStatus(bool &resetDetected, QString &error)
{
    quint32 dhcsr = 0;
    if (!readDhcsr(dhcsr, error))
        return false;

    resetDetected = (dhcsr & kSResetSt) != 0;
    return true;
}

bool CortexM::waitForHalt(int timeoutMs, QString &error)
{
    QElapsedTimer timer;
    timer.start();

    /* Timing probe, off unless DAPFLASHER_POLL_STATS is set in the
     * environment. It exists to answer one question with numbers instead of
     * guesses: how much of a flash is actually spent in this loop, and how
     * many polls one wait takes. Printed once per call to stderr so the CLI
     * can be run straight into a file. */
    static const bool statsOn = qEnvironmentVariableIsSet("DAPFLASHER_POLL_STATS");
    int polls = 0;
    qint64 firstPollUs = -1;
    qint64 lastPollUs = -1;

    /* This loop is what sets the erase and program time: the caller starts the
     * algorithm and then sits here until the core returns to the trap.
     *
     * Two things were wrong with the old shape (poll, then always sleep 2 ms).
     * Measured with the logic analyzer on a 1 MHz flash: each poll cost four
     * USB round trips (~13 ms), and the fixed 2 ms sleep on top of that made
     * 14.95 ms per round - so 97% of the 26 s flash was spent in this loop,
     * 96% of that inside the round trip, and the SWD wire was busy 2% of the
     * time. Raising the SWD clock to 10 MHz then bought only 1.5x, because
     * the round trips do not care what the wire clock is.
     *
     * So: the poll itself is now a single round trip (readMem32Polled skips
     * the SELECT/CSW/TAR writes the AP already has), and the sleep backs off
     * instead of being fixed. A flash page programs in well under a
     * millisecond, so most waits end inside the first few polls; the backoff
     * only grows once we are clearly waiting on a real erase. */
    /* The backoff, in milliseconds, applied before poll N+1 once poll N came
     * back "still running".
     *
     * There is none by default, and that is a measured result, not a
     * preference. A poll is already a full USB round trip - measured 3.3 ms
     * per poll on this probe - which leaves the target far more slack than
     * any sleep could add. Sleeping on top of it buys nothing and costs
     * exactly the time it sleeps.
     *
     * Measured on the 332 KB 1 MHz flash (logic analyzer on SWD, three runs
     * each, identical page count):
     *
     *     backoff { 0,0,1,1,2,3,5,5 }   26.50 s   <- the old default
     *     backoff { 0,0,0,0,0,0,0,0 }   22.66 s   <- this one
     *     backoff { 5,5,5,5,5,5,5,5 }   26.51 s
     *
     * The longer list is the control: it leaves the total exactly where the
     * old default sat, so the ~13 ms a sector erase really takes belongs to
     * the chip and no amount of sleeping shortens it. The gain comes from the
     * *first* few entries - the 1 ms and 2 ms sleeps that landed inside a
     * ProgramPage wait which the 3.3 ms poll already covers.
     *
     * On the wire the change is structural: the default list broke the flash
     * into 495 bursts separated by 15 ms of idle; with no backoff the same
     * flash runs as one 7.6 s unbroken transfer with the SWD wire busy 57% of
     * the time instead of 27%. Erase polls climb from 147 to ~15000 per
     * sector and the total still drops, which is the point: a poll that is
     * answered "still busy" immediately costs one round trip and nothing
     * else.
     *
     * DAPFLASHER_POLL_BACKOFF still overrides it with a comma separated list,
     * so the old behaviour is one environment variable away. */
    int backoffMs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int backoffCount = 8;

    if (const QByteArray spec = qgetenv("DAPFLASHER_POLL_BACKOFF"); !spec.isEmpty()) {
        const QList<QByteArray> parts = spec.split(',');
        backoffCount = qMin(parts.size(), 8);
        for (int i = 0; i < backoffCount; ++i) {
            bool ok = false;
            const int value = parts.at(i).trimmed().toInt(&ok);
            if (ok && value >= 0)
                backoffMs[i] = value;
        }
    }

    int step = 0;

    while (timer.elapsed() < timeoutMs) {
        quint32 dhcsr = 0;
        if (!link_->readMem32Polled(kDhcsr, dhcsr, error))
            return false;

        ++polls;
        if (firstPollUs < 0)
            firstPollUs = timer.nsecsElapsed() / 1000;
        lastPollUs = timer.nsecsElapsed() / 1000;

        if (dhcsr & kSHalt) {
            if (dhcsr & kSLockup) {
                error = QCoreApplication::translate("CortexM", "The core is locked up (the flash algorithm crashed).");
                return false;
            }
            if (statsOn) {
                fprintf(stderr, "[poll] ok   polls=%d wait=%lldus first=%lldus total=%lldus\n",
                        polls, static_cast<long long>(lastPollUs - firstPollUs),
                        static_cast<long long>(firstPollUs),
                        static_cast<long long>(timer.nsecsElapsed() / 1000));
            }
            return true;
        }

        const int delay = backoffMs[qMin(step, backoffCount - 1)];
        if (delay > 0)
            QThread::msleep(static_cast<unsigned long>(delay));
        ++step;
    }

    error = QCoreApplication::translate("CortexM", "The core did not halt within %1 ms.").arg(timeoutMs);
    return false;
}

bool CortexM::resetSoftware(bool haltAfterReset, QString &error)
{
    /* The order of the DHCSR writes below is the whole point of this function,
     * and it is not the obvious one.
     *
     * Releasing the core before asking for the reset - the sequence this used
     * to do, one write with C_HALT clear and only then AIRCR - races the
     * target: that first write takes C_HALT away from a core that is halted
     * (which is exactly where programming leaves it) and it starts running the
     * old code immediately, so the reset request lands on a core that is
     * already executing and the C_MASKINTS left over from resumeMasked() can
     * survive into the reset. On an STM32F411 the run then reported a clean
     * "Reset: software" and the target sat there with the program not
     * started.
     *
     * So the core is kept halted across the reset, the interrupt mask is
     * cleared while it is halted (a halted core takes C_MASKINTS, a running
     * one does not - the same rule halt() documents), the reset is requested,
     * and the core is released only once the reset has come and gone. It then
     * fetches from the vector table with no leftover state.
     *
     * The debug logic has to stay enabled across the reset: with C_DEBUGEN
     * clear the core would not stop on VC_CORERESET, and either way a target
     * that runs off into a lockup is not something this can recover. */
    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
        return false;

    /* Clear a leftover C_MASKINTS. The write above cannot do it: C_HALT and
     * C_MASKINTS may not change in the same write, so a second one with the
     * core now halted is what takes. Leaving it set would send the target off
     * with every interrupt masked - it runs, SysTick never ticks and a WFI
     * never returns - which looks just like the program not having started. */
    if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
        return false;

    quint32 demcr = 0;
    if (!link_->readMem32(kDemcr, &demcr, 1, error))
        return false;

    const quint32 newDemcr = haltAfterReset ? (demcr | kVecCoreReset)
                                            : (demcr & ~kVecCoreReset);
    if (newDemcr != demcr) {
        if (!link_->writeMem32(kDemcr, &newDemcr, 1, error))
            return false;
    }

    const quint32 aircr = kAircrKey | kSysResetReq;
    if (!link_->writeMem32(kAircr, &aircr, 1, error))
        return false;

    /* Wait for the reset to happen: S_RESET_ST rises and then falls again.
     * Some cores do not report it at all, so a failure here is tolerated as
     * long as the core ends up in the expected state. */
    QElapsedTimer timer;
    timer.start();
    bool sawReset = false;
    bool settled = false;

    while (timer.elapsed() < 1500) {
        quint32 dhcsr = 0;
        if (!readDhcsr(dhcsr, error))
            return false;

        const bool resetSt = (dhcsr & kSResetSt) != 0;

        if (resetSt)
            sawReset = true;
        else if (sawReset) {
            settled = true;
            break;
        }

        QThread::msleep(5);
    }

    if (!sawReset)
        settled = true;              /* core does not implement S_RESET_ST */

    Q_UNUSED(settled);

    /* Put the vector catch back to its previous value */
    if (haltAfterReset) {
        demcr &= ~kVecCoreReset;
        QString ignored;
        link_->writeMem32(kDemcr, &demcr, 1, ignored);
    }

    if (haltAfterReset)
        return waitForHalt(2000, error);

    /* Release the core last, with the reset behind it. Only C_HALT changes in
     * this write - a halted core takes that - so the target starts from the
     * vector table rather than from wherever it was left. */
    return writeDhcsr(kDbgKey | kCDebugEn, error);
}

bool CortexM::resetHardware(bool haltAfterReset, bool *performed, QString &error)
{
    if (performed)
        *performed = false;

    /* Halting on the reset vector needs VC_CORERESET, and the vector catch
     * has to be armed before the pulse: the reset never reaches the debug
     * port again once nRESET is released. */
    quint32 demcr = 0;
    bool demcrArmed = false;

    if (haltAfterReset) {
        if (!link_->readMem32(kDemcr, &demcr, 1, error))
            return false;

        const quint32 armed = demcr | kVecCoreReset;
        if (armed != demcr && !link_->writeMem32(kDemcr, &armed, 1, error))
            return false;

        demcrArmed = true;

        if (!writeDhcsr(kDbgKey | kCDebugEn | kCHalt, error))
            return false;
    }

    bool executed = false;
    const bool answered = link_->pulseTargetReset(&executed, error);

    /* Put the vector catch back whatever the pulse did */
    if (demcrArmed) {
        QString ignored;
        link_->writeMem32(kDemcr, &demcr, 1, ignored);
    }

    if (!answered)
        return false;

    if (!executed) {
        /* Neither DAP_SWJ_Pins nor DAP_Reset_Target can drive a reset line on
         * this probe. Say so instead of pretending the target was reset. */
        error = QCoreApplication::translate("CortexM", "The probe has no nRESET line; use the software reset instead.");
        return false;
    }

    if (performed)
        *performed = true;

    if (!haltAfterReset)
        return true;

    return waitForHalt(2000, error);
}

bool CortexM::resetWithMode(ResetMode mode, bool haltAfterReset,
                            ResetMode *usedMode, QString &error)
{
    const auto done = [usedMode](ResetMode used) {
        if (usedMode)
            *usedMode = used;
        return true;
    };

    if (mode == ResetMode::Software)
        return resetSoftware(haltAfterReset, error) ? done(ResetMode::Software) : false;

    /* "Automatic" asks the probe for its reset line by simply trying it: a
     * probe without one answers "not performed" and nothing was pulsed, so
     * the software reset takes over without any extra detection step. */
    bool performed = false;
    QString pinError;
    const bool pinOk = resetHardware(haltAfterReset, &performed, pinError);

    if (performed) {
        /* The line was pulsed, so this was a hardware reset - also when the
         * core could not be found halted afterwards. */
        if (pinOk)
            return done(ResetMode::Hardware);
        error = pinError;
        return false;
    }

    if (mode == ResetMode::Hardware) {
        error = pinError;
        return false;
    }

    return resetSoftware(haltAfterReset, error) ? done(ResetMode::Software) : false;
}

/* ==========================================================================
 * Registers
 * ========================================================================== */

bool CortexM::readRegister(quint8 reg, quint32 &value, QString &error)
{
    const quint32 select = reg & 0x1Fu;
    if (!link_->writeMem32(kDcrsr, &select, 1, error))
        return false;

    /* Wait for the transfer to complete. The poll uses the cached-AP variant:
     * this loop reads the same register over and over, which is exactly the
     * traffic it exists for, and nothing else touches the probe in between. */
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < 1000) {
        quint32 dhcsr = 0;
        if (!link_->readMem32Polled(kDhcsr, dhcsr, error))
            return false;
        if (dhcsr & kSRegRdy)
            break;
        /* No sleep between polls: one poll is already a 3.3 ms USB round
         * trip, which is well past the target's own turnaround. The old 1 ms
         * only ever added to it (see waitForHalt for the measurement). */
    }

    return link_->readMem32(kDcrdr, &value, 1, error);
}

bool CortexM::writeRegister(quint8 reg, quint32 value, QString &error)
{
    /* The two words go out as one batch: DCRDR takes the value, DCRSR takes
     * the request. See writeMem32Batch() for what "batch" means here and what
     * it deliberately is not.
     *
     * This used to be two writeMem32() calls, each with its own CSW write and
     * its own CSW read-back barrier - eight USB commands where six carry the
     * same information. The batch keeps the two transfers as separate
     * commands with a bus round trip between them (which is what the packed
     * one-packets variant that used to be here lacked, and why it faulted),
     * and shares the setup and the single trailing barrier instead.
     *
     * Measured on this probe over the full callFunction shape - Sp then
     * R0,R1,R2,LR,PC, sixty times over - the batch writes back correctly every
     * time and never faults the AP, and it is 2.137 ms against 2.843 ms per
     * call. Since callAlgorithm() issues six of these per page, that is worth
     * about 4 ms of every 32 ms page at 1 MHz. */
    const quint32 addresses[2] = { kDcrdr, kDcrsr };
    const quint32 values[2] = { value, (reg & 0x1Fu) | kRegWnR };

    if (!link_->writeMem32Batch(addresses, values, 2, error))
        return false;

    /* Wait for S_REGRDY. readMem32Polled keeps the AP state it just built up,
     * so the first poll pays for it and any repeats do not. */
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < 1000) {
        quint32 dhcsr = 0;
        if (!link_->readMem32Polled(kDhcsr, dhcsr, error))
            return false;
        if (dhcsr & kSRegRdy)
            return true;
        /* No sleep here either, for the same reason as readRegister(): the
         * poll is the pacing, and it is 3.3 ms long by itself. */
    }

    error = QCoreApplication::translate("CortexM", "The core register transfer timed out.");
    return false;
}

bool CortexM::readAllRegisters(quint32 *values, QString &error)
{
    if (!values) {
        error = QCoreApplication::translate("CortexM", "Internal error: no register buffer.");
        return false;
    }

    for (quint8 reg = 0; reg <= CoreReg::Xpsr; ++reg) {
        if (!readRegister(reg, values[reg], error))
            return false;
    }

    return true;
}

bool CortexM::readPc(quint32 &pc, QString &error)
{
    return readRegister(CoreReg::Pc, pc, error);
}

/* ==========================================================================
 * Algorithm call
 * ========================================================================== */

bool CortexM::callFunction(quint32 entry, quint32 trapAddress,
                           quint32 arg0, quint32 arg1, quint32 arg2,
                           QString &error)
{
    if (!writeRegister(CoreReg::R0, arg0, error))
        return false;
    if (!writeRegister(CoreReg::R1, arg1, error))
        return false;
    if (!writeRegister(CoreReg::R2, arg2, error))
        return false;

    /* LR points at the BKPT instruction that stops the algorithm when it
     * returns; bit 0 marks the Thumb state. */
    if (!writeRegister(CoreReg::Lr, trapAddress | 1u, error))
        return false;

    /* The entry point of a Thumb routine carries bit 0 as well */
    if (!writeRegister(CoreReg::Pc, entry | 1u, error))
        return false;

    /* Run with interrupts masked: a flash algorithm must not be preempted */
    return resumeMasked(error);
}

bool CortexM::readReturnValue(quint32 &value, QString &error)
{
    return readRegister(CoreReg::R0, value, error);
}

bool CortexM::readFaultStatus(quint32 &dfsr, QString &error)
{
    return link_->readMem32(kDfsr, &dfsr, 1, error);
}
