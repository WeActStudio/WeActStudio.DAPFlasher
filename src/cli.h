/*
 * WeAct Studio DAP Flasher
 *
 * Command line interface
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CLI_H
#define CLI_H

#include <QStringList>

/** True when the command line asks for the console mode instead of the GUI */
bool cliRequested(const QStringList &arguments);

/** Run the console mode; returns the process exit code */
int cliRun(const QStringList &arguments);

/**
 * Hand the command line over to the console companion installed next to the
 * graphical program, so that a calling shell waits for the result.
 *
 * @return the exit code, or -1 when there is no companion
 */
int cliForwardToConsoleApp(const QStringList &arguments);

#endif /* CLI_H */
