/*
 * WeAct Studio DAP Flasher
 *
 * CMSIS-DAP probe enumeration.
 *
 * A probe is reachable over one of two transports:
 *
 *   - USB HID:  vendor defined usage page 0xFF00, 64 byte report packets
 *   - USB bulk: a vendor specific interface carrying one bulk IN and one bulk
 *               OUT endpoint (WinUSB or libusbK on Windows)
 *
 * Modern probes (DAPLink, WCH-Link, the WeAct probe, ...) usually expose both
 * interfaces at the same time, so the same physical probe can show up twice.
 * USB bulk is preferred: it is dramatically faster.
 *
 * The transport kind and the CMSIS-DAP *framing* are two separate things, even
 * though the specification pairs them up ("v1 over HID", "v2 over bulk"):
 *
 *   - the spec-conformant form is the bare command, starting at byte 0 of the
 *     packet (this is what CMSIS-DAP v2 over USB bulk actually specifies, and
 *     what DAPLink, OpenOCD and pyOCD all put on the wire)
 *   - a length-prefixed form prepends a two byte little endian length, padded
 *     to 4 bytes; some host tools assume this on bulk, but it is not part of
 *     the CMSIS-DAP specification
 *
 * This file no longer calls a transport "v1" or "v2" because that pairing does
 * not hold for every probe. The WeAct probe is reached over USB bulk and is
 * spec-conformant: it reads the first byte of the packet as the command ID and
 * never looks for a length prefix (measured, see H:/tmp/dapfram3.c) - which is
 * exactly what the CMSIS-DAP v2 bulk transport is defined to do, not a
 * shortcoming. So what is enumerated here is the transport kind, and the
 * framing is decided at run time, by asking the probe itself (see
 * WinUsbTransport::detectFraming()). Keep the two apart in user visible text
 * as well: a transport is "USB HID" or "USB bulk", never "v1" or "v2"; the
 * prefix question is a framing detail, not a protocol version.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PROBES_H
#define PROBES_H

#include <QList>
#include <QString>
#include <QtGlobal>

/** How the probe is reached - not how the packets it carries are framed */
enum class DapTransportKind {
    Hid,        /* USB HID interface, vendor defined usage page */
    WinUsb      /* USB bulk endpoints (WinUSB / libusbK) */
};

/** One CMSIS-DAP probe found on the USB bus */
struct DapProbeInfo
{
    DapTransportKind transport = DapTransportKind::Hid;

    QString path;               /* unique key: HID path or USB port path */
    quint16 vid = 0;
    quint16 pid = 0;
    QString manufacturer;
    QString product;
    QString serial;

    /* USB HID interface */
    int hidInterface = -1;
    int usagePage = 0;
    int usage = 0;

    /* USB bulk interface */
    int busNumber = 0;
    int deviceAddress = 0;
    int usbInterface = -1;
    int epIn = -1;
    int epOut = -1;

    bool dapCandidate = false;  /* looks like a CMSIS-DAP probe */
    bool busy = false;          /* present but owned by another program */

    /** True when the probe is reached over USB bulk rather than USB HID.
     *  Says nothing about the framing the probe speaks - see the file header. */
    bool isBulk() const { return transport == DapTransportKind::WinUsb; }

    /** Short single line label for the probe list */
    QString displayName() const;

    /** Complete description, used as the tooltip */
    QString description() const;

    /** Identity across bus refreshes */
    QString key() const;
};

/**
 * Enumerate the CMSIS-DAP probes of the system (HID and WinUSB).
 *
 * @param includeAllHid  When false only devices that really look like a
 *                       CMSIS-DAP probe are returned (vendor defined usage page
 *                       0xFF00, a known VID/PID, or a vendor specific USB bulk
 *                       interface with exactly one IN and one OUT endpoint).
 * @param errorMessage   Set when the enumeration itself failed
 */
QList<DapProbeInfo> dapEnumerateProbes(bool includeAllHid, QString *errorMessage = nullptr);

/** Release the HID and libusb contexts; call once at shutdown. */
void dapProbesShutdown();

/** The libusb context shared by the enumeration and the v2 transport */
struct libusb_context;
libusb_context *dapLibusbContext();

#endif /* PROBES_H */
