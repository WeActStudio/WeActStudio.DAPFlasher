/*
 * WeAct Studio DAP Flasher
 *
 * Keil .FLM flash programming algorithm.
 *
 * A .FLM is an ELF32 image that is linked to address 0 and therefore position
 * independent: the host copies it into a RAM area of the target, points the
 * core at one of its entry points and lets it run. The flash geometry lives in
 * a FlashDevice structure (see Keil's FlashOS.h) that the image carries along.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef FLASHALGO_H
#define FLASHALGO_H

#include <QString>
#include <QVector>
#include <QtGlobal>

#include "elffile.h"

/** One entry of the sector table of a FlashDevice structure */
struct FlashSector
{
    quint32 address = 0;    /* offset from the device base address */
    quint32 size = 0;
};

/** What the FlashDevice structure of a .FLM says */
struct FlashAlgoInfo
{
    QString deviceName;
    quint16 interfaceVersion = 0;
    quint16 deviceType = 0;
    quint32 deviceAddress = 0;      /* DevAdr: default flash base address */
    quint32 deviceSize = 0;         /* szDev */
    quint32 pageSize = 0;           /* szPage: programming granularity */
    quint8 emptyValue = 0xFF;       /* valEmpty: content of erased memory */
    quint32 programTimeout = 0;     /* toProg, milliseconds */
    quint32 eraseTimeout = 0;       /* toErase, milliseconds */
    QVector<FlashSector> sectors;
};

/**
 * One loaded .FLM file.
 */
class FlashAlgo
{
public:
    bool load(const QString &path, QString &error);

    bool isValid() const { return valid_; }

    QString path() const { return path_; }
    QString fileName() const;

    const FlashAlgoInfo &info() const { return info_; }
    const QVector<ElfSegment> &segments() const { return elf_.segments(); }

    /** Highest address the image occupies, including zero filled areas */
    quint32 imageSize() const;

    /** Offset of the FlashDevice structure inside the image */
    quint32 flashDeviceOffset() const { return flashDeviceAddress_; }

    bool hasSymbols() const { return elf_.hasSymbols(); }

    /** Whether a flash routine is present */
    bool hasFunction(const QString &name) const;

    /** Entry point of a flash routine, relative to the load address */
    quint32 functionOffset(const QString &name) const;

    /** Localised name of the device type (ONCHIP, EXTSPI, ...) */
    QString deviceTypeName() const;

    /** Sector containing @p address (absolute flash address) */
    bool sectorFor(quint32 address, FlashSector &sector) const;

    /** Sector index of @p address, -1 when the address is not covered */
    int sectorIndexFor(quint32 address) const;

private:
    bool parseFlashDevice(QString &error);

    ElfFile elf_;
    QString path_;
    bool valid_ = false;
    FlashAlgoInfo info_;
    quint32 flashDeviceAddress_ = 0;
};

#endif /* FLASHALGO_H */
