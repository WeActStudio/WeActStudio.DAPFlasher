/*
 * WeAct Studio DAP Flasher
 *
 * Short lived target sessions: identify the probe and the core, halt, run and
 * reset it.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "targetsession.h"

#include "dap/cmsisdap.h"
#include "dap/dapdefs.h"
#include "dap/probes.h"
#include "target/cortexm.h"

#include <QCoreApplication>

namespace {

/** Common part of every session: open the probe, connect SWD, find the core */
bool openSession(const DapProbeInfo &probe, quint32 clockHz, const SessionLog &log,
                 CmsisDap &link, CortexM &core, TargetInfo *info, QString &error)
{
    if (!link.open(probe, error))
        return false;

    if (log)
        log(QCoreApplication::translate("TargetSession", "Probe: %1").arg(link.transportName()), false);

    /* Connect, make the DP answer and only then look for the core. The DP is
     * the gate: nothing behind it can be reached while it stays silent. */
    quint32 dpIdCode = 0;
    QString plainError;

    if (!link.attachSwd(clockHz, dpIdCode, log, plainError) || !core.probe(plainError)) {
        /* Either the DP stayed silent, or it answered while the AP behind it
         * did not - and a target that runs its own program is the same thing
         * seen from two sides. Both mean the plain attach cannot get in, so
         * the reset the program cannot survive is used instead. */
        if (!recoverUnderReset(link, core, clockHz, log, dpIdCode, error)) {
            /* The plain failure is the one that describes the target the user
             * has in front of them; the recovery only says how it was tried. */
            error = plainError;
            return false;
        }
    }

    if (info) {
        info->ok = true;
        info->dpIdCode = dpIdCode;
        info->cpuid = core.cpuid();
        info->coreName = core.coreName();

        quint32 dhcsr = 0;
        QString ignored;
        if (link.readMem32(0xE000EDF0u, &dhcsr, 1, ignored))
            info->halted = (dhcsr & 0x00020000u) != 0;
    }

    return true;
}

} /* namespace */

bool recoverUnderReset(CmsisDap &link, CortexM &core, quint32 clockHz,
                       const SessionLog &log, quint32 &dpIdCode, QString &error)
{
    /* Halting is the whole point: the reset alone only opens a window, and a
     * window the program is free to close again is worth nothing. */
    const DapAttachTouch holdCore = [&core](QString &touchError) {
        return core.halt(touchError);
    };

    if (!link.attachUnderReset(clockHz, dpIdCode, log, holdCore, error))
        return false;

    if (!core.probe(error))
        return false;

    if (log) {
        log(QCoreApplication::translate("TargetSession",
                "The target program was holding the debug access; "
                "the core is now halted under reset."), false);
    }

    return true;
}

TargetInfo targetIdentify(const DapProbeInfo &probe, quint32 clockHz, const SessionLog &log)
{
    TargetInfo info;
    CmsisDap link;
    CortexM core(&link);

    if (!openSession(probe, clockHz, log, link, core, &info, info.error)) {
        info.ok = false;
        link.close();
        return info;
    }

    info.probeName = link.transportName();
    info.probeFirmware = link.infoString(DapInfoId::FirmwareVer);
    info.probeVendorId = probe.vid;
    info.probeProductId = probe.pid;

    link.close();
    return info;
}

TargetInfo targetIdentifyQuiet(const DapProbeInfo &probe, quint32 clockHz, QString &error)
{
    /* No log sink at all: every message openSession() and attachSwd() would
     * emit is dropped, which is the whole point (see the header). The session
     * itself is exactly the same one - this is a real read of the target, not
     * an assumption about it. */
    TargetInfo info = targetIdentify(probe, clockHz, SessionLog());
    error = info.error;
    return info;
}

bool targetHalt(const DapProbeInfo &probe, quint32 clockHz, const SessionLog &log,
                TargetInfo *info, QString &error)
{
    CmsisDap link;
    CortexM core(&link);
    TargetInfo session;

    if (!openSession(probe, clockHz, log, link, core, &session, error)) {
        link.close();
        return false;
    }

    if (info) {
        /* The values openSession() read, plus whatever the halt changed. The
         * core is halted from here on, and saying so is the whole reason the
         * caller wants this structure - it is the difference between the
         * status showing "halted" and the caller opening a second session to
         * find it out. */
        session.halted = true;
        *info = session;
    }

    const bool ok = core.halt(error);
    link.close();
    return ok;
}

bool targetRun(const DapProbeInfo &probe, quint32 clockHz, const SessionLog &log,
               TargetInfo *info, QString &error)
{
    CmsisDap link;
    CortexM core(&link);
    TargetInfo session;

    if (!openSession(probe, clockHz, log, link, core, &session, error)) {
        link.close();
        return false;
    }

    if (info) {
        /* A resumed core is running, whatever it was doing when the session
         * was opened. */
        session.halted = false;
        *info = session;
    }

    const bool ok = core.resume(error);
    link.close();
    return ok;
}

bool targetReset(const DapProbeInfo &probe, quint32 clockHz, ResetMode mode,
                 bool haltAfter, const SessionLog &log, TargetInfo *info, QString &error)
{
    CmsisDap link;
    CortexM core(&link);
    TargetInfo session;

    if (openSession(probe, clockHz, log, link, core, &session, error)) {
        ResetMode used = mode;
        if (!core.resetWithMode(mode, haltAfter, &used, error)) {
            link.close();
            return false;
        }

        /* "Automatic" can end up using the other way, so the log always says
         * what actually reset the target. */
        if (log)
            log(QCoreApplication::translate("TargetSession", "Reset: %1").arg(resetModeName(used)), false);

        if (info) {
            /* The reset decides the state, not what the core was doing when
             * the session was opened. */
            session.halted = haltAfter;
            *info = session;
        }

        link.close();
        return true;
    }

    /* No session could be opened, which means the DP did not answer. A reset
     * through the probe's nRESET line does not need the DP at all - and this
     * is precisely the situation a reset is asked for, so it is still tried
     * instead of giving up on the attach failure. The software reset does
     * need the DP, so it cannot help here and its own error is kept. */
    if (mode == ResetMode::Software) {
        link.close();
        return false;
    }

    const QString attachError = error;

    bool performed = false;
    QString pinError;
    if (!link.pulseTargetReset(&performed, pinError) || !performed) {
        link.close();
        error = pinError.isEmpty() ? attachError : pinError;
        return false;
    }

    if (log) {
        log(QCoreApplication::translate("TargetSession",
                "Reset: %1 (the DP did not answer, so this was a pin reset only).")
                .arg(resetModeName(ResetMode::Hardware)), false);
    }

    link.close();
    return true;
}
