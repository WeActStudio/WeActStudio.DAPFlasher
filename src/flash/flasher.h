/*
 * WeAct Studio DAP Flasher
 *
 * Flash programming: download a .FLM algorithm into the target RAM and drive
 * its Init / EraseSector / ProgramPage / Verify / UnInit routines.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FLASHER_H
#define FLASHER_H

#include <QByteArray>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include <functional>

#include "target/resetmode.h"

class CmsisDap;
class CortexM;
class FlashAlgo;

/** One contiguous block of data to program */
struct FlashRange
{
    quint32 address = 0;
    QByteArray data;
};

/** What the last run did, shown in the UI */
struct FlashStatistics
{
    int sectorsErased = 0;
    int pagesProgrammed = 0;
    quint32 bytesProgrammed = 0;
    quint32 ramBytesUsed = 0;
    quint32 ramBase = 0;
};

/** Callbacks of a running program operation */
struct FlasherCallbacks
{
    /** @return false to cancel the operation */
    std::function<bool(int percent, const QString &status)> progress;

    /** Non fatal notices and errors */
    std::function<void(const QString &message, bool isError)> log;

    /** @return true when the user asked to stop */
    std::function<bool()> cancelled;
};

/**
 * Runs a flash algorithm on the target.
 *
 * The target must be reachable; the caller keeps ownership of the link and of
 * the core object.
 */
class Flasher
{
public:
    struct Config
    {
        const FlashAlgo *algo = nullptr;    /* algorithm to run */
        quint32 ramBase = 0x20000000u;      /* where the algorithm is copied to */
        quint32 clockHz = 1000000u;         /* clock reported to Init() */
        bool eraseWholeChip = false;        /* chip erase instead of sector erase */
        bool verify = true;                 /* run the Verify routine afterwards */
        bool resetAfterProgram = false;     /* reset (and run) the target at the end */
        ResetMode resetMode = ResetMode::Software;  /* how that reset is done */
    };

    Flasher(CmsisDap *link, CortexM *core);

    bool program(const Config &config, const QVector<FlashRange> &ranges,
                 const FlasherCallbacks &callbacks, QString &error,
                 FlashStatistics *statistics = nullptr);

private:
    /* Layout of the RAM area the algorithm runs from */
    struct Layout
    {
        quint32 loadBase = 0;      /* address the image is copied to */
        quint32 imageSize = 0;     /* bytes of algorithm image */
        quint32 bufferAddress = 0; /* page buffer handed to ProgramPage() */
        quint32 bufferSize = 0;
        quint32 stackTop = 0;
        quint32 trapAddress = 0;   /* holds the BKPT that ends a call */
        quint32 totalSize = 0;
    };

    bool computeLayout(const FlashAlgo &algo, quint32 ramBase, Layout &layout, QString &error);
    bool downloadAlgorithm(const FlashAlgo &algo, const Layout &layout, QString &error);
    bool writeZeros(quint32 address, quint32 size, QString &error);

    bool callAlgorithm(const FlashAlgo &algo, const Layout &layout, const QString &function,
                       int timeoutMs, quint32 arg0, quint32 arg1, quint32 arg2,
                       quint32 &result, QString &error);

    bool eraseFlash(const FlashAlgo &algo, const Layout &layout,
                    const QVector<FlashRange> &ranges, const Config &config,
                    const FlasherCallbacks &callbacks, FlashStatistics &stats, QString &error);

    bool programFlash(const FlashAlgo &algo, const Layout &layout,
                      const QVector<FlashRange> &ranges,
                      const FlasherCallbacks &callbacks, FlashStatistics &stats, QString &error);

    bool verifyFlash(const FlashAlgo &algo, const Layout &layout,
                     const QVector<FlashRange> &ranges,
                     const FlasherCallbacks &callbacks, QString &error);

    bool reportProgress(const FlasherCallbacks &callbacks, int percent, const QString &status);
    bool isCancelled(const FlasherCallbacks &callbacks);
    void log(const FlasherCallbacks &callbacks, const QString &message, bool isError = false);

    CmsisDap *link_;
    CortexM *core_;
};

#endif /* FLASHER_H */
