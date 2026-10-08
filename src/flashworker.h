/*
 * WeAct Studio DAP Flasher
 *
 * The program operation: one request structure, one synchronous runner shared
 * by the command line program and by the background thread of the GUI.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FLASHWORKER_H
#define FLASHWORKER_H

#include <QObject>
#include <QString>
#include <QtGlobal>

#include <functional>

#include "dap/probes.h"
#include "flash/flasher.h"
#include "targetsession.h"

/** Everything a programming run needs */
struct FlashRequest
{
    DapProbeInfo probe;
    QString algoPath;
    QString firmwarePath;
    quint32 firmwareBase = 0x08000000u;   /* used for .bin files */
    quint32 ramBase = 0x20000000u;        /* where the algorithm runs */
    quint32 clockHz = 1000000u;
    bool eraseWholeChip = false;
    bool verify = true;
    bool resetAfterProgram = true;
    ResetMode resetMode = ResetMode::Software;
};

/** Outcome of one run */
struct ProgramResult
{
    bool ok = false;
    QString message;            /* failure description, or the success summary */
    TargetInfo target;
    FlashStatistics statistics;
    QString algoDevice;
    QString firmwareSummary;
};

/**
 * Run one complete programming session: open the probe, identify the core,
 * download the algorithm, erase, program and verify.
 *
 * The function blocks until it is done. All callbacks are optional.
 */
ProgramResult runProgramSession(const FlashRequest &request,
                                const std::function<bool(int percent, const QString &status)> &progress,
                                const std::function<void(const QString &message, bool isError)> &log,
                                const std::function<bool()> &cancelled);

/**
 * Background worker used by the graphical user interface.
 */
class FlashWorker : public QObject
{
    Q_OBJECT

public:
    explicit FlashWorker(const FlashRequest &request, QObject *parent = nullptr);

    void cancel();

public Q_SLOTS:
    void run();

Q_SIGNALS:
    void progress(int percent, const QString &status);
    void logMessage(const QString &message, bool isError);
    void finished(bool success, const QString &message);

private:
    FlashRequest request_;
    bool cancelled_;
};

#endif /* FLASHWORKER_H */
