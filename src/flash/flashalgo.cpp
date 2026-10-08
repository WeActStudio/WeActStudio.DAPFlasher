/*
 * WeAct Studio DAP Flasher
 *
 * Keil .FLM flash programming algorithm.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "flashalgo.h"

#include <QCoreApplication>
#include <QFileInfo>

#include <cmath>

namespace {

/* Offsets inside the FlashDevice structure (Keil FlashOS.h, 32 bit ARM):
 *
 *   0   unsigned short Vers
 *   2   char           DevName[128]
 *   130 unsigned short DevType
 *   132 unsigned long  DevAdr
 *   136 unsigned long  szDev
 *   140 unsigned long  szPage
 *   144 unsigned long  Res
 *   148 unsigned char  valEmpty
 *   152 unsigned long  toProg
 *   156 unsigned long  toErase
 *   160 FlashSectors   sectors[512]      (8 bytes each)
 */
const int kOffVers = 0;
const int kOffName = 2;
const int kNameLength = 128;
const int kOffDevType = 130;
const int kOffDevAdr = 132;
const int kOffSzDev = 136;
const int kOffSzPage = 140;
const int kOffRes = 144;
const int kOffValEmpty = 148;
const int kOffToProg = 152;
const int kOffToErase = 156;
const int kOffSectors = 160;
const int kSectorCount = 512;
const int kStructSize = kOffSectors + kSectorCount * 8;    /* 4256 bytes */

const quint32 kSectorEndMarker = 0xFFFFFFFFu;

quint16 readU16(const QByteArray &data, int offset)
{
    const uchar *p = reinterpret_cast<const uchar *>(data.constData()) + offset;
    return static_cast<quint16>(p[0] | (p[1] << 8));
}

quint32 readU32(const QByteArray &data, int offset)
{
    const uchar *p = reinterpret_cast<const uchar *>(data.constData()) + offset;
    return static_cast<quint32>(p[0])
           | (static_cast<quint32>(p[1]) << 8)
           | (static_cast<quint32>(p[2]) << 16)
           | (static_cast<quint32>(p[3]) << 24);
}

bool isPowerOfTwo(quint32 value)
{
    return value != 0 && (value & (value - 1)) == 0;
}

/**
 * Sanity check of a FlashDevice structure.
 *
 * @p strict is used by the search fallback, which scans raw image bytes and
 * therefore has to demand a shape that only a real FlashDevice has, so that a
 * random byte pattern is not mistaken for one.
 *
 * The symbol based lookup passes @c false: there the structure is known to be
 * the right one, and the extra demands would only throw away perfectly good
 * algorithms:
 *
 *   - szDev is not always a power of two — 384 KB, 640 KB, 1.5 MB and 3 MB
 *     flash parts exist (STM32F4xx_384, STM32WB5x_M4_640, STM32F4xx_1536,
 *     GD32F30x_XD, RA4M2_384K, ...),
 *   - neither is szPage — STM32H7xx_MT25TL01G declares 0x5000,
 *   - DevAdr is 0 for the off-chip algorithms (AM29F…, S25FL…), whose address
 *     is configured by the user rather than fixed by the device,
 *   - the option byte / OTP / configuration algorithms (STM32F4xx_OPT,
 *     RA4M2_CONF, AT32F403A_USD, …) describe a region of a few dozen bytes,
 *     and some of them declare a program page larger than that region.
 */
bool looksLikeFlashDevice(const QByteArray &blob, int offset, bool strict)
{
    if (offset + kStructSize > blob.size())
        return false;

    const quint16 vers = readU16(blob, offset + kOffVers);
    if (vers != 0x0100 && vers != 0x0101 && vers != 0x0102 && vers != 0x0200)
        return false;

    /* A printable, NUL terminated device name */
    int printable = 0;
    bool terminated = false;
    for (int i = 0; i < kNameLength; ++i) {
        const uchar c = static_cast<uchar>(blob.at(offset + kOffName + i));
        if (c == 0) {
            terminated = true;
            break;
        }
        if (c >= 0x20 && c < 0x7F)
            ++printable;
        else
            return false;
    }
    if (!terminated || printable < 4)
        return false;

    const quint16 devType = readU16(blob, offset + kOffDevType);
    if (devType > 5)
        return false;

    const quint32 szDev = readU32(blob, offset + kOffSzDev);
    const quint32 szPage = readU32(blob, offset + kOffSzPage);

    if (!strict)
        return szDev > 0 && szPage > 0;

    if (!isPowerOfTwo(szDev) || szDev < 256)
        return false;
    if (!isPowerOfTwo(szPage) || szPage > szDev)
        return false;

    /* Only accept an address range that a flash device can actually use */
    const quint32 devAdr = readU32(blob, offset + kOffDevAdr);
    if (devAdr == 0 || devAdr == 0xFFFFFFFFu)
        return false;

    return true;
}

} /* namespace */

QString FlashAlgo::fileName() const
{
    return QFileInfo(path_).fileName();
}

bool FlashAlgo::load(const QString &path, QString &error)
{
    valid_ = false;
    info_ = FlashAlgoInfo();
    flashDeviceAddress_ = 0;
    path_ = path;

    if (!elf_.load(path, error))
        return false;

    if (!parseFlashDevice(error))
        return false;

    /* The routines a flash algorithm must provide */
    if (!hasFunction(QStringLiteral("Init"))) {
        error = QCoreApplication::translate("FlashAlgo", "The algorithm does not export an Init() function.");
        return false;
    }

    if (!hasFunction(QStringLiteral("ProgramPage"))
        && !hasFunction(QStringLiteral("ProgramPageBuffer"))) {
        error = QCoreApplication::translate("FlashAlgo", "The algorithm does not export a ProgramPage() function.");
        return false;
    }

    if (!hasFunction(QStringLiteral("EraseChip"))
        && !hasFunction(QStringLiteral("EraseSector"))) {
        error = QCoreApplication::translate("FlashAlgo", "The algorithm does not export EraseSector() nor EraseChip().");
        return false;
    }

    valid_ = true;
    return true;
}

bool FlashAlgo::parseFlashDevice(QString &error)
{
    quint32 deviceAddress = 0;
    bool found = false;

    /* Preferred path: the symbol table names the structure */
    ElfSymbol symbol;
    if (elf_.findSymbol(QStringLiteral("FlashDevice"), symbol)) {
        deviceAddress = symbol.value;
        found = true;
    } else {
        /* Some algorithms are stripped: search the image for the signature of
         * the structure instead (Vers, printable name, valid geometry). */
        for (const ElfSegment &segment : elf_.segments()) {
            for (int offset = 0; offset + kStructSize <= segment.data.size(); offset += 2) {
                if (looksLikeFlashDevice(segment.data, offset, true)) {
                    deviceAddress = segment.address + static_cast<quint32>(offset);
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
    }

    if (!found) {
        error = QCoreApplication::translate("FlashAlgo", "The FlashDevice structure could not be located in the algorithm.");
        return false;
    }

    /* Read it out of the image */
    const QByteArray blob = elf_.readAt(deviceAddress, kStructSize);
    if (blob.size() < kStructSize
        || !looksLikeFlashDevice(blob, 0, false)) {
        error = QCoreApplication::translate("FlashAlgo", "The FlashDevice structure at offset 0x%1 is not valid.")
                    .arg(deviceAddress, 1, 16);
        return false;
    }

    flashDeviceAddress_ = deviceAddress;

    info_.interfaceVersion = readU16(blob, kOffVers);

    const QByteArray rawName = blob.mid(kOffName, kNameLength);
    info_.deviceName = QString::fromLatin1(rawName.constData(),
                                           static_cast<int>(qstrlen(rawName.constData()))).trimmed();

    info_.deviceType = readU16(blob, kOffDevType);
    info_.deviceAddress = readU32(blob, kOffDevAdr);
    info_.deviceSize = readU32(blob, kOffSzDev);
    info_.pageSize = readU32(blob, kOffSzPage);
    info_.emptyValue = static_cast<quint8>(blob.at(kOffValEmpty));
    info_.programTimeout = readU32(blob, kOffToProg);
    info_.eraseTimeout = readU32(blob, kOffToErase);

    /* The sector table is RUN LENGTH ENCODED, not one entry per sector.
     *
     * An entry gives one sector size and the address that size starts at, and
     * the size repeats from there up to the address of the next entry - or up
     * to the end of the device for the last entry. Keil's own STM32F4xx_512
     * declares exactly three entries:
     *
     *     0x00004000 @ 0x00000000     16 kB sectors from 0
     *     0x00010000 @ 0x00010000     64 kB sector at 0x10000
     *     0x00020000 @ 0x00020000     128 kB sectors from 0x20000
     *
     * which describe the whole 512 kB: 4 x 16 kB + 1 x 64 kB + 3 x 128 kB.
     * Reading one entry as one sector - the obvious reading, and what this
     * code used to do - yields 3 sectors covering 208 kB of 512 kB, so the
     * sectors the table does not name are never erased. Erasing only some of
     * them is worse than not erasing at all: writing a firmware image leaves
     * whatever the previous image had wherever the new one has a hole, and
     * the mismatch only shows up in the verification of those bytes.
     *
     * Verified against every algorithm of the STM32F4xx device family pack
     * (20 files): each one expands to a cover of [0, szDev) with no gap and
     * no overlap, while the one-entry-one-sector reading is short for every
     * single one of them. A table that really does list every sector on its
     * own still parses correctly, because then each entry's size already
     * reaches the next entry's address. */
    QVector<FlashSector> runs;
    for (int i = 0; i < kSectorCount; ++i) {
        const int offset = kOffSectors + i * 8;
        const quint32 size = readU32(blob, offset);
        const quint32 address = readU32(blob, offset + 4);

        if (size == kSectorEndMarker && address == kSectorEndMarker)
            break;
        if (size == 0)
            break;

        FlashSector run;
        run.address = address;
        run.size = size;
        runs.append(run);
    }

    for (int i = 0; i < runs.size() && info_.sectors.size() < kSectorCount; ++i) {
        const quint32 size = runs.at(i).size;
        const quint32 start = runs.at(i).address;
        /* Where this run of equal sized sectors ends: the next entry names the
         * sector size that follows, the device size ends the last run. */
        const quint32 stop = (i + 1 < runs.size()) ? runs.at(i + 1).address
                                                   : info_.deviceSize;

        if (size == 0 || stop <= start)
            continue;

        for (quint32 a = start; a < stop && info_.sectors.size() < kSectorCount; a += size) {
            FlashSector sector;
            sector.address = a;
            sector.size = qMin(size, stop - a);
            info_.sectors.append(sector);
        }
    }

    if (info_.sectors.isEmpty()) {
        error = QCoreApplication::translate("FlashAlgo", "The algorithm carries an empty sector table.");
        return false;
    }

    return true;
}

quint32 FlashAlgo::imageSize() const
{
    quint32 highest = 0;

    for (const ElfSegment &segment : elf_.segments()) {
        const quint32 end = segment.address + qMax(segment.fileSize, segment.memSize);
        if (end > highest)
            highest = end;
    }

    return highest;
}

bool FlashAlgo::hasFunction(const QString &name) const
{
    ElfSymbol symbol;
    return elf_.findSymbol(name, symbol);
}

quint32 FlashAlgo::functionOffset(const QString &name) const
{
    ElfSymbol symbol;
    if (!elf_.findSymbol(name, symbol))
        return 0;

    /* Thumb entry points carry bit 0 set; the offset inside the image must not */
    return symbol.value & ~1u;
}

QString FlashAlgo::deviceTypeName() const
{
    switch (info_.deviceType) {
    case 0: return QCoreApplication::translate("FlashAlgo", "Unknown");
    case 1: return QCoreApplication::translate("FlashAlgo", "On-chip flash");
    case 2: return QCoreApplication::translate("FlashAlgo", "External flash, 8 bit bus");
    case 3: return QCoreApplication::translate("FlashAlgo", "External flash, 16 bit bus");
    case 4: return QCoreApplication::translate("FlashAlgo", "External flash, 32 bit bus");
    case 5: return QCoreApplication::translate("FlashAlgo", "External SPI flash");
    default: break;
    }
    return QCoreApplication::translate("FlashAlgo", "Unknown");
}

int FlashAlgo::sectorIndexFor(quint32 address) const
{
    if (address < info_.deviceAddress)
        return -1;

    const quint32 offset = address - info_.deviceAddress;

    /* The table is searched from the end: the last entry that starts at or
     * before the address is the one covering it. */
    for (int i = info_.sectors.size() - 1; i >= 0; --i) {
        const FlashSector &sector = info_.sectors.at(i);
        if (offset >= sector.address && offset < sector.address + sector.size)
            return i;
    }

    return -1;
}

bool FlashAlgo::sectorFor(quint32 address, FlashSector &sector) const
{
    const int index = sectorIndexFor(address);
    if (index < 0)
        return false;

    sector = info_.sectors.at(index);
    return true;
}
