/*
 * WeAct Studio DAP Flasher
 *
 * CMSIS-DAP transport abstraction.
 *
 * Two transports carry exactly the same CMSIS-DAP command packets, they only
 * differ in framing:
 *
 *   - v1 / USB HID    : one 64 byte report per packet, sent after a report id
 *                       byte, the probe answers with one input report
 *   - v2 / USB bulk   : two byte little endian length prefix, 1024 byte packets,
 *                       addressed through the bulk endpoints of a WinUSB
 *                       (or libusbK / libusb-win32) bound interface
 *
 * A few probes put the bare (v1 shaped) packet on a USB bulk endpoint instead
 * of the length prefixed (v2) one. The USB bulk transport measures which of
 * the two a probe answers, so both work without a VID/PID table.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DAPTRANSPORT_H
#define DAPTRANSPORT_H

#include <QByteArray>
#include <QString>

struct DapProbeInfo;

/**
 * One open connection to a probe.
 */
class DapTransport
{
public:
    /** Create the transport matching the probe kind; never fails */
    static DapTransport *create(const DapProbeInfo &probe);

    virtual ~DapTransport();

    virtual bool open(const DapProbeInfo &probe, QString &error) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    /** Maximum command packet size of this transport: 64 for v1 HID, at most
     *  1024 for v2 bulk, and whatever the probe reported for a bare packet on
     *  a bulk endpoint. One command never has to be split across two calls. */
    virtual int packetSize() const = 0;

    /** Adjust the packet size once the probe reported it (DAP_Info 0xFF).
     *  Both transports honour it: the answer tells how large the buffer of
     *  the probe firmware claims to have, which is neither the size of a USB
     *  packet nor always the truth - a probe that reports a 4 KB buffer may
     *  still refuse a command half that size, so the bulk transport keeps the
     *  endpoint packet size as a ceiling. The read buffer grows with whatever
     *  the value finally is, so an answer is never truncated. */
    virtual void setPacketSize(int size) { Q_UNUSED(size); }

    /** Human readable transport name, used in the log */
    virtual QString name() const = 0;

    /** Send one CMSIS-DAP command packet */
    bool sendCommand(const QByteArray &command, QString &error);

    /** Receive one CMSIS-DAP response packet */
    bool receiveResponse(QByteArray &response, int timeoutMs, QString &error);

protected:
    /* Framing primitives implemented by the concrete transports */
    virtual bool writeFrame(const QByteArray &command, QString &error) = 0;
    virtual bool readFrame(QByteArray &response, int timeoutMs, QString &error) = 0;
};

#endif /* DAPTRANSPORT_H */
