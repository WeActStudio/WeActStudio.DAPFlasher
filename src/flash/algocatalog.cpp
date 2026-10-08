/*
 * WeAct Studio DAP Flasher
 *
 * Locating the Keil .FLM flash algorithms installed on this machine.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "algocatalog.h"

#include "flashalgo.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardPaths>
#include <QStringList>
#include <QVector>

#ifdef Q_OS_WIN
/* Only GetDriveTypeW() is used; the lean/nominmax defines keep <windows.h>
 * from dragging in the COM headers and the min/max macros. */
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#endif

namespace {

const char *kSettingsOrg = "WeActStudio";
const char *kSettingsApp = "WeAct Studio DAP Flasher";
const char *kRootsKey = "algorithmRoots";

/* Keil installation folders are looked for on every fixed drive */
const char *kKeilFolderNames[] = {
    "Keil_v5", "Keil_v4", "Keil", "MDK", "MDK-ARM"
};

/* A pack repository that was moved off the system drive, and the pack root a
 * Keil installation keeps beside itself */
const char *kDriveLevelPackDirs[] = {
    "Arm/Packs", "Arm/Pack"
};

/* Directories inside a pack that never hold flash algorithms. Skipping them
 * keeps the walk short even for a big installation. */
const char *kPackSkipDirs[] = {
    "Source", "Include", "SVD", "Descriptors", "Documentation", "Documents",
    "Examples", "Boards", "Utilities", "RTE", "Projects", "Middleware",
    "Components", "IDEs_patches", "Debug", "Release"
};

/* The Keil algorithm folder is flat; one extra level covers the odd
 * sub-directory without descending into the example projects it ships */
const int kPackDepth = 1;

/* How far below a pack version the "Flash" folder is looked for. Two levels
 * cover both <version>/Flash and <version>/CMSIS/Flash. */
const int kFlashSearchDepth = 2;

const int kFolderDepth = 3;
const int kMaxEntries = 4000;

/* How many rejected files are remembered by name. A few are worth reporting —
 * they are the only clue when an algorithm the user expects is missing. */
const int kMaxInvalidFiles = 30;

/* Bumped whenever the scan result would differ for the same roots, so that an
 * older cache is discarded instead of hiding algorithms. 2: the Flash folder is
 * now located below the pack version instead of at a fixed depth. */
const char *kCacheFormatVersion = "2";

/**
 * Every fixed drive of this machine, as "C:/".
 *
 * Drive letters differ from machine to machine, so they are queried from the
 * system rather than hard-coded. GetDriveTypeW() answers without touching the
 * medium, which matters: probing a drive that has no media would otherwise
 * raise the "insert a disk" prompt. Removable drives are skipped — an MDK
 * installation and a multi-gigabyte pack repository do not live on one in
 * practice, and a card reader slot with no card is indistinguishable from an
 * empty drive until it is touched.
 */
QStringList fixedDrives()
{
    QStringList drives;

#ifdef Q_OS_WIN
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        const QString root = QStringLiteral("%1:/").arg(QLatin1Char(letter));
        const UINT type = GetDriveTypeW(reinterpret_cast<const wchar_t *>(root.utf16()));
        if (type == DRIVE_FIXED)
            drives.append(root);
    }
#else
    const QFileInfoList list = QDir::drives();
    for (const QFileInfo &info : list)
        drives.append(info.absoluteFilePath());
#endif

    return drives;
}

/** The Keil/MDK installation folders found on this machine */
QStringList keilInstallBases()
{
    QStringList bases;

    for (const QString &drive : fixedDrives()) {
        for (const char *folder : kKeilFolderNames) {
            const QString base = drive + QLatin1String(folder);
            if (QFileInfo(base).isDir() && !bases.contains(base))
                bases.append(base);
        }
    }

    return bases;
}

/**
 * The pack repository a Keil installation uses, read from its TOOLS.INI.
 *
 * Keil stores it as
 *
 *     [UV2]
 *     RTEPATH="D:\Arm\Packs"
 *
 * which is authoritative and independent of the drive letter — unlike
 * guessing "C:\Keil_v5\ARM\Packs". The value may be quoted, may carry a
 * trailing comment and may list several paths separated by ';'.
 */
QStringList packRootsFromToolsIni(const QString &keilBase)
{
    QStringList roots;

    QFile file(QDir(keilBase).filePath(QStringLiteral("TOOLS.INI")));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return roots;

    while (!file.atEnd()) {
        QString line = QString::fromLatin1(file.readLine()).trimmed();
        if (line.isEmpty() || line.startsWith(QLatin1Char(';'))
            || line.startsWith(QLatin1Char('['))) {
            continue;
        }

        const int equals = line.indexOf(QLatin1Char('='));
        if (equals <= 0)
            continue;
        if (line.left(equals).trimmed().compare(QLatin1String("RTEPATH"),
                                                Qt::CaseInsensitive) != 0) {
            continue;
        }

        QString value = line.mid(equals + 1).trimmed();

        const int comment = value.indexOf(QLatin1Char('#'));
        if (comment >= 0)
            value = value.left(comment).trimmed();

        if (value.size() > 1 && value.startsWith(QLatin1Char('"'))
            && value.endsWith(QLatin1Char('"'))) {
            value = value.mid(1, value.size() - 2);
        }

        const QStringList parts = value.split(QLatin1Char(';'), Qt::SkipEmptyParts);
        for (const QString &part : parts) {
            const QString clean = QDir::cleanPath(part.trimmed());
            if (!clean.isEmpty() && !roots.contains(clean))
                roots.append(clean);
        }
    }

    return roots;
}

bool isSkippedPackDir(const QString &name)
{
    for (const char *skip : kPackSkipDirs) {
        if (name.compare(QLatin1String(skip), Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

/** Natural comparison of "1.2.10" against "1.2.9" */
int compareVersions(const QString &left, const QString &right)
{
    const QStringList a = left.split(QLatin1Char('.'));
    const QStringList b = right.split(QLatin1Char('.'));

    const int count = qMax(a.size(), b.size());
    for (int i = 0; i < count; ++i) {
        const int na = i < a.size() ? a.at(i).toInt() : 0;
        const int nb = i < b.size() ? b.at(i).toInt() : 0;
        if (na != nb)
            return na < nb ? -1 : 1;
    }
    return 0;
}

/**
 * Collect every algorithm below @p directory, at most @p depth levels deep.
 *
 * Every file that looks like an algorithm is opened and its FlashDevice
 * structure is parsed: the device name is what makes the list readable, and a
 * file the loader rejects would only fail later, during a flash.
 */
void collectAlgorithms(const QString &directory,
                       int depth,
                       AlgoGroup &group,
                       AlgoCatalog &catalog,
                       const std::function<bool()> &isCancelled,
                       bool skipHeavyDirs)
{
    if (isCancelled && isCancelled())
        return;

    QDir dir(directory);
    if (!dir.exists())
        return;

    const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::Readable,
                                                  QDir::Name | QDir::IgnoreCase);
    for (const QFileInfo &file : files) {
        if (group.entries.size() >= kMaxEntries)
            return;

        if (!algoIsAlgorithmFile(file.fileName()))
            continue;

        const QString path = file.absoluteFilePath();
        bool duplicate = false;
        for (const AlgoEntry &entry : group.entries) {
            if (entry.path == path) {
                duplicate = true;
                break;
            }
        }
        if (duplicate)
            continue;

        AlgoEntry entry;
        if (algoInspectFile(path, entry)) {
            group.entries.append(entry);
        } else {
            ++catalog.invalidCount;
            if (catalog.invalidFiles.size() < kMaxInvalidFiles)
                catalog.invalidFiles.append(path);
        }
    }

    if (depth <= 0)
        return;

    const QFileInfoList subdirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,
                                                    QDir::Name | QDir::IgnoreCase);
    for (const QFileInfo &sub : subdirs) {
        if (isCancelled && isCancelled())
            return;
        if (skipHeavyDirs && isSkippedPackDir(sub.fileName()))
            continue;

        collectAlgorithms(sub.absoluteFilePath(), depth - 1, group, catalog,
                          isCancelled, skipHeavyDirs);
    }
}

/**
 * Collect the algorithms of one pack version.
 *
 * The algorithms are not always in the same place. The classic layout keeps
 * them in
 *
 *     <version>/Flash/*.FLM
 *
 * while the newer CMSIS-Toolbox layout used by the Keil STM32 packs puts them
 * one level deeper:
 *
 *     <version>/CMSIS/Flash/*.FLM
 *
 * and a few packs drop them straight into the version directory. So rather than
 * walking a fixed number of levels — which silently loses whole device families
 * — the walk looks for a directory called "Flash" and stops there.
 */
void collectPackAlgorithms(const QString &directory,
                           int depth,
                           bool isVersionRoot,
                           AlgoGroup &group,
                           AlgoCatalog &catalog,
                           const std::function<bool()> &isCancelled)
{
    if (isCancelled && isCancelled())
        return;

    QDir dir(directory);
    if (!dir.exists())
        return;

    const bool isFlashDir =
        dir.dirName().compare(QLatin1String("Flash"), Qt::CaseInsensitive) == 0;

    /* Only the version directory itself and a "Flash" folder hold algorithms;
     * everything else below a pack is documentation, source or device headers. */
    if (isVersionRoot || isFlashDir) {
        collectAlgorithms(directory, isFlashDir ? 1 : 0, group, catalog,
                          isCancelled, isFlashDir);
    }

    if (depth <= 0)
        return;

    const QFileInfoList subdirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,
                                                    QDir::Name | QDir::IgnoreCase);
    for (const QFileInfo &sub : subdirs) {
        if (isCancelled && isCancelled())
            return;

        const QString name = sub.fileName();
        if (name.startsWith(QLatin1Char('.')))
            continue;

        const bool childIsFlashDir =
            name.compare(QLatin1String("Flash"), Qt::CaseInsensitive) == 0;
        if (!childIsFlashDir && isSkippedPackDir(name))
            continue;

        collectPackAlgorithms(sub.absoluteFilePath(), childIsFlashDir ? 0 : depth - 1,
                              false, group, catalog, isCancelled);
    }
}

/**
 * Walk one CMSIS pack root. The layout below a pack root is
 *
 *     <Vendor>/<Pack>/<Version>/
 *
 * so only three directory levels have to be looked at, and only the newest
 * version of a pack is used (an older one would list the same devices twice).
 */
void scanPackRoot(const QString &root,
                  QVector<AlgoGroup> &groups,
                  AlgoCatalog &catalog,
                  const std::function<bool()> &isCancelled)
{
    QDir rootDir(root);
    if (!rootDir.exists())
        return;

    const QFileInfoList vendors = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,
                                                        QDir::Name | QDir::IgnoreCase);
    for (const QFileInfo &vendorInfo : vendors) {
        if (isCancelled && isCancelled())
            return;

        const QString vendor = vendorInfo.fileName();
        if (vendor.startsWith(QLatin1Char('.')))    /* .Download, .Web, .Local */
            continue;

        const QFileInfoList packs = QDir(vendorInfo.absoluteFilePath())
                                        .entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,
                                                       QDir::Name | QDir::IgnoreCase);
        for (const QFileInfo &packInfo : packs) {
            if (isCancelled && isCancelled())
                return;

            const QFileInfoList versions = QDir(packInfo.absoluteFilePath())
                                               .entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot,
                                                              QDir::Name | QDir::IgnoreCase);
            if (versions.isEmpty())
                continue;

            QString newest = versions.first().fileName();
            for (const QFileInfo &version : versions) {
                if (compareVersions(version.fileName(), newest) > 0)
                    newest = version.fileName();
            }

            AlgoGroup group;
            group.kind = AlgoGroup::Pack;
            group.vendor = vendor;
            group.pack = packInfo.fileName();
            group.version = newest;
            group.root = QDir(packInfo.absoluteFilePath()).filePath(newest);

            collectPackAlgorithms(group.root, kFlashSearchDepth, true, group,
                                  catalog, isCancelled);

            if (!group.entries.isEmpty())
                groups.append(group);
        }
    }
}

QJsonObject entryToJson(const AlgoEntry &entry)
{
    QJsonObject object;
    object.insert(QStringLiteral("path"), entry.path);
    object.insert(QStringLiteral("device"), entry.deviceName);
    object.insert(QStringLiteral("type"), entry.deviceType);
    object.insert(QStringLiteral("size"), static_cast<double>(entry.deviceSize));
    return object;
}

AlgoEntry entryFromJson(const QJsonObject &object, const QString &path)
{
    AlgoEntry entry;
    entry.path = object.value(QStringLiteral("path")).toString(path);
    entry.fileName = QFileInfo(entry.path).fileName();
    entry.deviceName = object.value(QStringLiteral("device")).toString();
    entry.deviceType = object.value(QStringLiteral("type")).toString();
    entry.deviceSize = static_cast<quint32>(object.value(QStringLiteral("size")).toDouble());
    return entry;
}

} /* namespace */

/* ==========================================================================
 * AlgoGroup / AlgoCatalog
 * ========================================================================== */

QString AlgoGroup::key() const
{
    switch (kind) {
    case Builtin:
        return QStringLiteral("builtin:") + root;
    case Pack:
        return QStringLiteral("pack:") + vendor + QLatin1Char('.') + pack;
    case User:
        break;
    }
    return QStringLiteral("user:") + root;
}

/* The strings below are handed to QCoreApplication::translate() with both
 * arguments written out inline: lupdate only extracts literal arguments, a
 * helper function would hide them from the translation source. */

QString AlgoGroup::title() const
{
    switch (kind) {
    case Builtin:
        return QCoreApplication::translate("AlgoCatalog", "Keil built-in algorithms");
    case Pack:
        return QStringLiteral("%1.%2  %3").arg(vendor, pack, version);
    case User:
        break;
    }
    return QCoreApplication::translate("AlgoCatalog", "Additional folder");
}

int AlgoCatalog::entryCount() const
{
    int count = 0;
    for (const AlgoGroup &group : groups)
        count += group.entries.size();
    return count;
}

const AlgoEntry *AlgoCatalog::find(const QString &path) const
{
    const QString clean = QDir::cleanPath(path);
    for (const AlgoGroup &group : groups) {
        for (const AlgoEntry &entry : group.entries) {
            if (entry.path.compare(clean, Qt::CaseInsensitive) == 0
                || entry.path.compare(path, Qt::CaseInsensitive) == 0)
                return &entry;
        }
    }
    return nullptr;
}

/* ==========================================================================
 * Search roots
 * ========================================================================== */

QStringList algoUserRoots()
{
    QSettings settings(QString::fromLatin1(kSettingsOrg), QString::fromLatin1(kSettingsApp));

    QStringList roots;
    const QStringList stored = settings.value(QLatin1String(kRootsKey)).toStringList();
    for (const QString &root : stored) {
        if (QFileInfo(root).isDir() && !roots.contains(root))
            roots.append(root);
    }
    return roots;
}

QStringList algoBuiltinRoots()
{
    QStringList roots;

    for (const QString &base : keilInstallBases()) {
        /* The classic location of the algorithms shipped with Keil */
        const QString flash = base + QStringLiteral("/ARM/Flash");
        if (QFileInfo(flash).isDir() && !roots.contains(flash))
            roots.append(flash);
    }

    return roots;
}

QStringList algoPackRoots()
{
    QStringList roots;

    const auto add = [&roots](const QString &directory) {
        if (directory.isEmpty() || !QFileInfo(directory).isDir())
            return;
        const QString clean = QDir::cleanPath(directory);
        if (!roots.contains(clean))
            roots.append(clean);
    };

    /* 1. an explicit environment override wins over everything else */
    add(qEnvironmentVariable("CMSIS_PACK_ROOT"));

    /* 2. the pack root of the current user. */
    const QString localAppData =
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    if (!localAppData.isEmpty())
        add(QDir(localAppData).filePath(QStringLiteral("Arm/Packs")));

    const QStringList keilBases = keilInstallBases();

    /* 3. what Keil itself uses: RTEPATH in <Keil>/TOOLS.INI. This is the
     *    reliable one — people routinely move the pack repository to another
     *    drive ("D:\Arm\Packs"), so the location cannot be guessed. */
    for (const QString &base : keilBases) {
        const QStringList rteRoots = packRootsFromToolsIni(base);
        for (const QString &root : rteRoots)
            add(root);
    }

    /* 4. a pack root inside a Keil installation ("ARM/Packs"; older installs
     *    use the singular "ARM/Pack") */
    for (const QString &base : keilBases) {
        add(base + QStringLiteral("/ARM/Packs"));
        add(base + QStringLiteral("/ARM/Pack"));
    }

    /* 5. a pack repository at the root of a drive, which is where Keil's own
     *    "move the packs" suggestion puts it */
    for (const QString &drive : fixedDrives()) {
        for (const char *folder : kDriveLevelPackDirs)
            add(drive + QLatin1String(folder));
    }

    return roots;
}

void algoRememberRoot(const QString &directory)
{
    const QString clean = QDir::cleanPath(directory);
    if (clean.isEmpty() || !QFileInfo(clean).isDir())
        return;

    QStringList roots = algoUserRoots();
    if (roots.contains(clean))
        return;

    roots.append(clean);

    /* Keep the list short: the ten most recent folders are enough */
    while (roots.size() > 10)
        roots.removeFirst();

    QSettings settings(QString::fromLatin1(kSettingsOrg), QString::fromLatin1(kSettingsApp));
    settings.setValue(QLatin1String(kRootsKey), roots);
}

/* ==========================================================================
 * Scanning
 * ========================================================================== */

bool algoIsAlgorithmFile(const QString &path)
{
    const QString suffix = QFileInfo(path).suffix();
    return suffix.compare(QLatin1String("flm"), Qt::CaseInsensitive) == 0
           || suffix.compare(QLatin1String("flx"), Qt::CaseInsensitive) == 0
           || suffix.compare(QLatin1String("xldr"), Qt::CaseInsensitive) == 0;
}

QString algoFileFilter()
{
    return QCoreApplication::translate("AlgoCatalog",
                                       "Flash Algorithms (*.flm *.FLM *.flx *.xldr)"
                                       ";;All Files (*.*)");
}

bool algoInspectFile(const QString &path, AlgoEntry &entry)
{
    FlashAlgo algorithm;
    QString error;

    if (!algorithm.load(path, error))
        return false;

    const QFileInfo info(path);
    entry.path = info.absoluteFilePath();
    entry.fileName = info.fileName();
    entry.deviceName = algorithm.info().deviceName;
    entry.deviceType = algorithm.deviceTypeName();
    entry.deviceSize = algorithm.info().deviceSize;
    return true;
}

AlgoCatalog algoScanCatalog(const QStringList &builtinRoots,
                            const QStringList &packRoots,
                            const QStringList &userRoots,
                            const std::function<bool()> &isCancelled)
{
    AlgoCatalog catalog;
    catalog.builtinRoots = builtinRoots;
    catalog.packRoots = packRoots;
    catalog.userRoots = userRoots;

    /* 1. the algorithms shipped with Keil */
    for (const QString &root : builtinRoots) {
        if (isCancelled && isCancelled())
            return catalog;

        AlgoGroup group;
        group.kind = AlgoGroup::Builtin;
        group.root = root;

        collectAlgorithms(root, kPackDepth, group, catalog, isCancelled, true);

        if (!group.entries.isEmpty())
            catalog.groups.append(group);
    }

    /* 2. the device family packs. The same pack can sit in more than one root
     *    (Keil's own repository and the user one), and listing it twice would
     *    look like a bug: the first root that provides it wins, which is also
     *    the order of preference above. */
    for (const QString &root : packRoots) {
        if (isCancelled && isCancelled())
            return catalog;

        QVector<AlgoGroup> found;
        scanPackRoot(root, found, catalog, isCancelled);

        for (const AlgoGroup &group : found) {
            bool duplicate = false;
            for (const AlgoGroup &existing : catalog.groups) {
                if (existing.kind == AlgoGroup::Pack && existing.key() == group.key()) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate)
                catalog.groups.append(group);
        }
    }

    /* 3. the folders the user added */
    for (const QString &root : userRoots) {
        if (isCancelled && isCancelled())
            return catalog;

        AlgoGroup group;
        group.kind = AlgoGroup::User;
        group.root = QDir::cleanPath(root);

        collectAlgorithms(group.root, kFolderDepth, group, catalog,
                          isCancelled, false);

        if (!group.entries.isEmpty())
            catalog.groups.append(group);
    }

    catalog.scannedAt = QDateTime::currentDateTime();
    return catalog;
}

/* ==========================================================================
 * Cache
 * ========================================================================== */

QString algoCacheFilePath()
{
    QString base = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    if (base.isEmpty())
        base = QDir::tempPath();

    return QDir(base).filePath(QStringLiteral("WeActStudio/WeAct Studio DAP Flasher/"
                                              "algorithm-cache.json"));
}

bool algoLoadCache(const QString &path, AlgoCatalog &catalog)
{
    catalog = AlgoCatalog();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (!document.isObject())
        return false;

    const QJsonObject root = document.object();
    if (root.value(QStringLiteral("format")).toString()
        != QLatin1String(kCacheFormatVersion)) {
        return false;
    }

    catalog.scannedAt = QDateTime::fromString(
        root.value(QStringLiteral("scannedAt")).toString(), Qt::ISODate);
    catalog.invalidCount = root.value(QStringLiteral("invalid")).toInt();
    for (const QJsonValue &value : root.value(QStringLiteral("invalidFiles")).toArray())
        catalog.invalidFiles.append(value.toString());

    const QJsonObject roots = root.value(QStringLiteral("roots")).toObject();
    const auto readRoots = [&roots](const char *key) {
        QStringList list;
        const QJsonArray array = roots.value(QLatin1String(key)).toArray();
        for (const QJsonValue &value : array)
            list.append(value.toString());
        return list;
    };
    catalog.builtinRoots = readRoots("builtin");
    catalog.packRoots = readRoots("packs");
    catalog.userRoots = readRoots("user");

    const QJsonArray groups = root.value(QStringLiteral("groups")).toArray();
    for (const QJsonValue &value : groups) {
        const QJsonObject object = value.toObject();

        AlgoGroup group;
        const QString kind = object.value(QStringLiteral("kind")).toString();
        if (kind == QLatin1String("builtin"))
            group.kind = AlgoGroup::Builtin;
        else if (kind == QLatin1String("pack"))
            group.kind = AlgoGroup::Pack;
        else
            group.kind = AlgoGroup::User;

        group.vendor = object.value(QStringLiteral("vendor")).toString();
        group.pack = object.value(QStringLiteral("pack")).toString();
        group.version = object.value(QStringLiteral("version")).toString();
        group.root = object.value(QStringLiteral("root")).toString();

        const QJsonArray entries = object.value(QStringLiteral("entries")).toArray();
        for (const QJsonValue &item : entries)
            group.entries.append(entryFromJson(item.toObject(), group.root));

        if (!group.entries.isEmpty())
            catalog.groups.append(group);
    }

    return true;
}

bool algoSaveCache(const QString &path, const AlgoCatalog &catalog)
{
    QJsonObject root;
    root.insert(QStringLiteral("format"), QLatin1String(kCacheFormatVersion));
    root.insert(QStringLiteral("scannedAt"), catalog.scannedAt.toString(Qt::ISODate));
    root.insert(QStringLiteral("invalid"), catalog.invalidCount);

    QJsonArray invalidFiles;
    for (const QString &path : catalog.invalidFiles)
        invalidFiles.append(path);
    root.insert(QStringLiteral("invalidFiles"), invalidFiles);

    const auto writeRoots = [](const QStringList &list) {
        QJsonArray array;
        for (const QString &item : list)
            array.append(item);
        return array;
    };

    QJsonObject roots;
    roots.insert(QStringLiteral("builtin"), writeRoots(catalog.builtinRoots));
    roots.insert(QStringLiteral("packs"), writeRoots(catalog.packRoots));
    roots.insert(QStringLiteral("user"), writeRoots(catalog.userRoots));
    root.insert(QStringLiteral("roots"), roots);

    QJsonArray groups;
    for (const AlgoGroup &group : catalog.groups) {
        QJsonObject object;
        switch (group.kind) {
        case AlgoGroup::Builtin: object.insert(QStringLiteral("kind"), QStringLiteral("builtin")); break;
        case AlgoGroup::Pack:    object.insert(QStringLiteral("kind"), QStringLiteral("pack")); break;
        case AlgoGroup::User:    object.insert(QStringLiteral("kind"), QStringLiteral("user")); break;
        }
        object.insert(QStringLiteral("vendor"), group.vendor);
        object.insert(QStringLiteral("pack"), group.pack);
        object.insert(QStringLiteral("version"), group.version);
        object.insert(QStringLiteral("root"), group.root);

        QJsonArray entries;
        for (const AlgoEntry &entry : group.entries)
            entries.append(entryToJson(entry));
        object.insert(QStringLiteral("entries"), entries);

        groups.append(object);
    }
    root.insert(QStringLiteral("groups"), groups);

    const QFileInfo info(path);
    QDir().mkpath(info.absolutePath());

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool algoCacheRootsMatch(const AlgoCatalog &catalog)
{
    return catalog.builtinRoots == algoBuiltinRoots()
           && catalog.packRoots == algoPackRoots()
           && catalog.userRoots == algoUserRoots();
}
