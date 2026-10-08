/*
 * WeAct Studio DAP Flasher
 *
 * How the target is reset: through the debug port or through nRESET.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef RESETMODE_H
#define RESETMODE_H

#include <QString>

/**
 * The reset used by the Reset button, by "Reset and run" after programming and
 * by --reset on the command line.
 *
 * Both ways end with a running (or halted, on request) core; they differ in
 * what they reset. SYSRESETREQ only restarts the core and the peripherals the
 * core knows about, while nRESET is the reset pin of the chip and restarts
 * everything - including a target whose program has switched the debug
 * interface off.
 */
enum class ResetMode
{
    /**
     * SYSRESETREQ written to AIRCR through the debug port.
     *
     * Works on every Cortex-M and needs no extra wire, which is why it is the
     * default: a board with only SWCLK, SWDIO and GND wired can still be
     * reset.
     */
    Software,

    /** A pulse on the nRESET line of the probe */
    Hardware,

    /** nRESET when the probe drives one, SYSRESETREQ otherwise */
    Auto
};

/** Name of the mode as shown in the user interface */
QString resetModeName(ResetMode mode);

/** Stable identifier used in the settings file and on the command line */
QString resetModeKey(ResetMode mode);

/**
 * Parse the identifier written by resetModeKey().
 *
 * @return false when the text is not a known mode, leaving @p mode untouched
 */
bool resetModeFromKey(const QString &key, ResetMode &mode);

#endif /* RESETMODE_H */
