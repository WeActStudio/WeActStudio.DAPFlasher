/*
 * WeAct Studio DAP Flasher
 *
 * Reading the vendor device id of the target.
 *
 * "Cortex-M7" says which core the target has, not which chip it is, and every
 * STM32H7 part answers with the same core. The device id register that the
 * vendor puts next to the debug unit is what separates an H743 from an H750,
 * and it is a single read through the AP that is already open.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MCUDB_H
#define MCUDB_H

#include <QString>
#include <QtGlobal>

class CmsisDap;

/** What the vendor device id register says about the target */
struct McuIdentity
{
    /** A table entry matched; @ref device is usable */
    bool known = false;
    /** Vendor name, empty when nothing matched */
    QString vendor;
    /** Device or family name, e.g. "STM32H743/753" */
    QString device;
    /** Address the id was read from, 0 when nothing was read */
    quint32 idAddress = 0;
    /** The value the matching table row carried. The bit layout differs per
     *  vendor - ST puts a 12 bit id in the low bits, Nordic a 20 bit part
     *  number, Artery a 32 bit code - so this is the table's own value, not a
     *  field of the register. */
    quint32 deviceId = 0;
    /** Raw register content */
    quint32 rawIdCode = 0;
    /** Revision, the upper half of the same register (ST convention) */
    quint16 revision = 0;
};

/**
 * Read the vendor device id through an open link.
 *
 * Every probe address in the table is tried; a read that faults or returns an
 * empty value simply moves on to the next one, so an unknown target costs a
 * handful of round trips and produces a result with @ref McuIdentity::known
 * cleared - the caller then keeps showing the core name alone.
 */
McuIdentity identifyMcu(CmsisDap *link);

/**
 * Human readable form of the DP IDCODE, e.g.
 * "SW-DP (v2), designer 0x23B (ARM), part 0x02".
 *
 * The register is ADIv5's DPIDR: the low 11 bits (shifted down by the always-1
 * bit 0) are the JEP106 designer code, and bits 19:12 are the part number -
 * SW-DP answers 0x01 or 0x02 depending on the ADIv5 revision it implements.
 * Everything else in the register is revision information that no two vendors
 * encode the same way, so it is left alone instead of guessed at.
 */
QString describeDpIdCode(quint32 idcode);

#endif /* MCUDB_H */
