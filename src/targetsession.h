/*
 * WeAct Studio DAP Flasher
 *
 * Short lived target sessions: identify the probe and the core, halt, run and
 * reset it.
 *
 * Every operation opens the probe, does its work and closes it again. That
 * keeps the tool stateless (unplugging the probe can never leave a stale
 * handle behind) and matches the three buttons of the user interface.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TARGETSESSION_H
#define TARGETSESSION_H

#include <QString>
#include <QtGlobal>

#include <functional>

#include "target/resetmode.h"

struct DapProbeInfo;

class CmsisDap;
class CortexM;

/** What was found on the other end of the probe */
struct TargetInfo
{
    bool ok = false;
    QString error;
    QString probeName;      /* transport description */
    QString probeFirmware;  /* CMSIS-DAP firmware version string */
    quint16 probeVendorId = 0;      /* CMSIS-DAP DAP ID (vendor) */
    quint16 probeProductId = 0;
    quint32 cpuid = 0;
    QString coreName;
    bool halted = false;
    quint32 dpIdCode = 0;
};

/** Log sink used by the session helpers */
using SessionLog = std::function<void(const QString &message, bool isError)>;

/**
 * Force a link to a target whose own program is holding the debug access.
 *
 * A plain attach is tried first by the caller, because a healthy target must
 * not be disturbed. When that fails - the DP stays silent, or it answers
 * while the AP behind it does not - the program has already run: it can have
 * switched the SWD pins over, stopped the bus clock or gone to sleep, and no
 * amount of retrying gets past that. What does get past it is the reset the
 * program cannot survive: hold nRESET low, build the link, and halt the core
 * the instant the reset is released, so the startup code never runs.
 *
 * The caller owns @p link and @p core and has to close the link afterwards.
 *
 * @param dpIdCode  the DP IDCODE read back under reset
 */
bool recoverUnderReset(CmsisDap &link, CortexM &core, quint32 clockHz,
                       const SessionLog &log, quint32 &dpIdCode, QString &error);

/**
 * Open the probe, connect over SWD and read the identification registers.
 */
TargetInfo targetIdentify(const DapProbeInfo &probe, quint32 clockHz, const SessionLog &log);

/**
 * The same, without the running commentary.
 *
 * This exists for the one call site that is not a user action: the status
 * refresh the window runs after a programming run, when the core state it
 * cached before the run is stale because the run reset the target.
 *
 * A refresh is not a session the user asked for, so it must not print the
 * identify sequence - "Identifying the target over ...", the probe line, the
 * clock line and so on - into a log that has just finished saying "programming
 * complete". Those lines read as if a new connection had started on its own.
 *
 * Failures stay silent for the same reason: the refresh runs right behind a
 * modal result box, and a probe that has not finished releasing its interface
 * yet would produce an error nobody asked about. A refresh that cannot read
 * the state leaves the caller's previous status untouched; @p error carries
 * the reason for a caller that does want it.
 */
TargetInfo targetIdentifyQuiet(const DapProbeInfo &probe, quint32 clockHz, QString &error);

/** Halt the core.
 *
 * @param info  optional: the identification the session read on its way in.
 *              The session is open anyway, so the window can take its status
 *              from here instead of opening a second one to ask. */
bool targetHalt(const DapProbeInfo &probe, quint32 clockHz, const SessionLog &log,
                TargetInfo *info, QString &error);

/** Let the core run */
bool targetRun(const DapProbeInfo &probe, quint32 clockHz, const SessionLog &log,
               TargetInfo *info, QString &error);

/**
 * Reset the core.
 *
 * @param mode       the reset to use; "Automatic" falls back to the software
 *                   reset when the probe has no nRESET line
 * @param haltAfter  keep it halted on the reset vector
 * @param info       optional identification of the same session (see targetHalt)
 */
bool targetReset(const DapProbeInfo &probe, quint32 clockHz, ResetMode mode,
                 bool haltAfter, const SessionLog &log, TargetInfo *info, QString &error);

#endif /* TARGETSESSION_H */
