/*
 * WeAct Studio DAP Flasher
 *
 * Locating the Keil .FLM flash algorithms installed on this machine.
 *
 * Two kinds of locations are searched:
 *
 *   - Keil's own algorithm folder (<Keil>/ARM/Flash), a flat directory that
 *     holds the classic algorithms shipped with the tool chain,
 *   - the CMSIS device family packs. Their layout is
 *
 *         <pack root>/<Vendor>/<Pack>/<Version>/
 *             <Vendor>.<Pack>.pdsc
 *             Flash        <- the algorithms of that pack (.FLM / .xldr)
 *
 *     so every pack is reported as one group, which is what the user picks
 *     from: a device family pack, not an anonymous list of files.
 *
 * The pack roots are not guessed: drive letters differ between machines and
 * the repository is routinely moved to another drive. The sources, in order of
 * preference, are %CMSIS_PACK_ROOT%, %LOCALAPPDATA%/Arm/Packs, the RTEPATH
 * entry of <Keil>/TOOLS.INI (what Keil itself uses, e.g. "D:\Arm\Packs"),
 * <Keil>/ARM/Packs, and finally Arm/Packs at the root of every fixed drive.
 *
 * Scanning a full pack installation touches hundreds of directories, so the
 * result is cached on disk (see algoCacheFilePath()) and the GUI refreshes it
 * in the background instead of blocking the start-up.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef ALGOCATALOG_H
#define ALGOCATALOG_H

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QVector>

#include <functional>

/**
 * One flash algorithm file.
 *
 * The Keil flash algorithms are ELF images that carry a FlashDevice structure.
 * @c deviceName comes from that structure and is what makes a list readable
 * ("STM32C542xx 256K Flash" instead of "STM32C5[34]x.xldr").
 */
struct AlgoEntry
{
    QString path;            /* absolute file name */
    QString fileName;        /* base name, exactly as Keil shows it */
    QString deviceName;      /* FlashDevice.DevName (may be empty) */
    QString deviceType;      /* localised device type, e.g. "On-chip Flash" */
    quint32 deviceSize = 0;  /* FlashDevice.szDev */
};

/**
 * A set of algorithms that belong together: one CMSIS device family pack, one
 * plain folder, or the algorithms shipped with Keil.
 */
struct AlgoGroup
{
    enum Kind { Builtin, Pack, User };

    Kind kind = User;
    QString vendor;                       /* Pack: "STMicroelectronics" */
    QString pack;                         /* Pack: "stm32c5xx_dfp" */
    QString version;                      /* Pack: "2.0.0" */
    QString root;                         /* directory the group was found in */
    QVector<AlgoEntry> entries;

    /** Stable identifier, e.g. "pack:STMicroelectronics.stm32c5xx_dfp" */
    QString key() const;

    /** Caption of the group header, localised ("STM32C5xx DFP 2.0.0") */
    QString title() const;
};

/** Everything that was found, plus the roots it was found under */
struct AlgoCatalog
{
    QStringList builtinRoots;
    QStringList packRoots;
    QStringList userRoots;

    QVector<AlgoGroup> groups;
    QDateTime scannedAt;

    /* Algorithm files that were found but rejected: no FlashDevice structure,
     * or the routines a flash algorithm must provide are missing. */
    int invalidCount = 0;

    /* The rejected files by name (capped), so that a missing algorithm can be
     * explained instead of silently disappearing. */
    QStringList invalidFiles;

    bool isEmpty() const { return groups.isEmpty(); }
    int entryCount() const;

    /** The entry of @p path, or nullptr */
    const AlgoEntry *find(const QString &path) const;
};

/* ==========================================================================
 * Search roots
 * ========================================================================== */

/** Keil's own algorithm folder (<Keil>/ARM/Flash) */
QStringList algoBuiltinRoots();

/** CMSIS pack roots (Arm/Packs) that exist on this machine */
QStringList algoPackRoots();

/** Folders the user added by hand */
QStringList algoUserRoots();

/** Remember a folder the user picked, so that it is searched from now on */
void algoRememberRoot(const QString &directory);

/* ==========================================================================
 * Scanning
 * ========================================================================== */

/**
 * Walk @p builtinRoots, @p packRoots and @p userRoots and build the catalog.
 *
 * Pure QtCore code without any GUI dependency, so it can run in a worker
 * thread. @p isCancelled is polled between directories and makes the scan
 * return early.
 */
AlgoCatalog algoScanCatalog(const QStringList &builtinRoots,
                            const QStringList &packRoots,
                            const QStringList &userRoots,
                            const std::function<bool()> &isCancelled = std::function<bool()>());

/**
 * Read the FlashDevice structure of a single file. Used for a file the user
 * picked by hand, which is not part of the catalog.
 */
bool algoInspectFile(const QString &path, AlgoEntry &entry);

/** Whether @p path is a file this program can use as a flash algorithm */
bool algoIsAlgorithmFile(const QString &path);

/** File name filter for the browse dialog */
QString algoFileFilter();

/* ==========================================================================
 * Cache
 * ========================================================================== */

/** Where the scan result is remembered between runs (%APPDATA%/.../algorithm-cache.json) */
QString algoCacheFilePath();

bool algoLoadCache(const QString &path, AlgoCatalog &catalog);
bool algoSaveCache(const QString &path, const AlgoCatalog &catalog);

/** Whether the roots of @p catalog are still the ones of this machine */
bool algoCacheRootsMatch(const AlgoCatalog &catalog);

#endif /* ALGOCATALOG_H */
