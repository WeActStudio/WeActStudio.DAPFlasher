/*
 * WeAct Studio DAP Flasher
 *
 * CMSIS-DAP transport implementations (USB HID and USB bulk)
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "daptransport.h"
#include "dapdefs.h"
#include "probes.h"

#include <QCoreApplication>

#include <cstring>
#include <cstdlib>

#include <hidapi/hidapi.h>
#include <libusb-1.0/libusb.h>

namespace {

/* USB bulk transfers of the v2 protocol are sent in chunks that are a
 * multiple of four bytes; the two byte length prefix tells the probe how much
 * of the payload is meaningful. */
const int kUsbWriteAlignment = 4;

/* Largest DAP packet size the CMSIS-DAP v2 specification allows. A probe that
 * reports more than this through DAP_Info(0xFF) is capped here. */
const int kUsbV2PacketSize = 1024;

/* Command size the measured WeAct probe really accepts on the bare (v1 style)
 * bulk framing, before its firmware buffer overflows.
 *
 * The probe's own DAP_Info(0xFF) answer is useless for this: it reports 1024
 * bytes, and the firmware behind it happily takes far more. A direct sweep of
 * DAP_TransferBlock commands against the bare interface - no SWD traffic, the
 * answer alone carries the verdict - gives an exact edge:
 *
 *   command 15829 bytes (3956 words) -> DAP_OK, word count echoed back
 *   command 15833 bytes (3957 words) -> status 0x08, count pinned at 3957
 *
 * so the firmware buffer holds 15832 bytes and a command of that size is the
 * largest that can be relied on.
 *
 * Measured, a larger command buys nothing: a 1 KB page transfer goes from
 * 14.29 ms to 13.71 ms at 1 MHz and from 3.27 ms to 2.89 ms at 10 MHz, while
 * the algorithm call that follows absorbs the difference - the page total
 * stays at 46.22/30.81 ms. The reason is that a RAM write is limited by the
 * SWD line rate and the target, not by the command count. So the default
 * stays at a modest value and the probe is never driven to the very edge of
 * its buffer, where a firmware change of one byte would start truncating. */
const int kPlainBulkPacketSize = 512;

/* The buffer edge measured above, as the hard ceiling for an override. */
const int kPlainBulkPacketSizeMax = 15832;

/* Read the experimental override, if any. */
int plainPacketSizeOverride()
{
    static const int value = [] {
        const QByteArray spec = qgetenv("DAPFLASHER_PACKET_SIZE");
        if (spec.isEmpty())
            return 0;
        bool ok = false;
        const int parsed = spec.toInt(&ok);
        return ok && parsed > 0 ? parsed : 0;
    }();
    return value;
}

/* Largest DAP packet size accepted from a probe's own DAP_Info answer. A
 * larger buffer than that is a firmware misreport, not a usable packet. */
const int kMaxReportedPacketSize = 4096;

/* Slack on top of the packet size when the read buffer is sized: a length
 * prefixed answer carries two bytes in front of the packet, and either
 * framing may pad the transfer. */
const int kReadBufferSlack = 8;

/* Timeouts */
const int kUsbWriteTimeoutMs = 2000;

/* One DAP_Info round trip while the framing is being detected. The probe is
 * local hardware, it answers in microseconds or not at all. */
const int kFramingProbeTimeoutMs = 500;

/** wMaxPacketSize of the bulk endpoints behind one interface, 0 when the
 *  descriptor cannot be read. */
int bulkPacketSize(libusb_device *dev, int interfaceNumber)
{
    libusb_config_descriptor *config = nullptr;
    if (libusb_get_active_config_descriptor(dev, &config) != 0)
        return 0;

    int size = 0;
    for (int n = 0; n < config->bNumInterfaces && size == 0; ++n) {
        const libusb_interface &iface = config->interface[n];
        for (int a = 0; a < iface.num_altsetting && size == 0; ++a) {
            const libusb_interface_descriptor &id = iface.altsetting[a];
            if (id.bInterfaceNumber != interfaceNumber)
                continue;

            for (int e = 0; e < id.bNumEndpoints; ++e) {
                const libusb_endpoint_descriptor &ep = id.endpoint[e];
                if ((ep.bmAttributes & 0x03) == LIBUSB_TRANSFER_TYPE_BULK) {
                    size = ep.wMaxPacketSize;
                    break;
                }
            }
        }
    }

    libusb_free_config_descriptor(config);
    return size;
}

QString hidErrorText(hid_device *handle)
{
    const wchar_t *err = hid_error(handle);
    if (!err)
        return QCoreApplication::translate("DapTransport", "unknown HID error");
    return QString::fromWCharArray(err);
}

QString libusbErrorText(int code)
{
    return QString::fromLatin1(libusb_error_name(code));
}

/** Stable USB port path of a device, e.g. "usb:1.3.2" */
QString usbPortPath(libusb_device *dev)
{
    uint8_t ports[8] = { 0 };
    const int count = libusb_get_port_numbers(dev, ports, sizeof(ports));

    QString path = QString::number(libusb_get_bus_number(dev));
    for (int i = 0; i < count; ++i)
        path += QStringLiteral(".%1").arg(ports[i]);

    return QStringLiteral("usb:") + path;
}

/* ------------------------------------------------------------------ */
/*  USB HID transport                                                 */
/* ------------------------------------------------------------------ */

class HidTransport : public DapTransport
{
public:
    HidTransport()
        : handle_(nullptr)
        , packetSize_(64)
    {
    }

    ~HidTransport() override
    {
        close();
    }

    bool open(const DapProbeInfo &probe, QString &error) override
    {
        close();

        handle_ = hid_open_path(probe.path.toLocal8Bit().constData());
        if (!handle_) {
            error = QCoreApplication::translate(
                        "DapTransport",
                        "The HID probe could not be opened. It is probably used by "
                        "another program (Keil, OpenOCD, a debug session).");
            return false;
        }

        hid_set_nonblocking(handle_, 0);
        packetSize_ = 64;
        return true;
    }

    void close() override
    {
        if (handle_) {
            hid_close(handle_);
            handle_ = nullptr;
        }
    }

    bool isOpen() const override { return handle_ != nullptr; }

    int packetSize() const override { return packetSize_; }

    void setPacketSize(int size) override
    {
        /* Only accepted within what a HID report can carry */
        if (size >= 64 && size <= 512)
            packetSize_ = size;
    }

    /* The transport name, matching the wording probes.cpp uses for the same
     * thing. The prefix question does not arise here: HID frames already carry
     * exactly one packet, so the bytes are the bare CMSIS-DAP packet. This line
     * describes the transport. */
    QString name() const override
    {
        return QCoreApplication::translate("DapTransport", "USB HID (CMSIS-DAP v1)");
    }

protected:
    bool writeFrame(const QByteArray &command, QString &error) override
    {
        if (!handle_) {
            error = QStringLiteral("transport is closed");
            return false;
        }

        /* hidapi expects the report id as the first byte of the buffer */
        QByteArray frame(packetSize_ + 1, '\0');
        frame[0] = 0;

        const int payload = qMin(command.size(), packetSize_);
        std::memcpy(frame.data() + 1, command.constData(), static_cast<size_t>(payload));

        const int written = hid_write(handle_,
                                      reinterpret_cast<const unsigned char *>(frame.constData()),
                                      static_cast<size_t>(frame.size()));
        if (written < 0) {
            error = QCoreApplication::translate("DapTransport", "HID write failed: %1")
                        .arg(hidErrorText(handle_));
            return false;
        }

        return true;
    }

    bool readFrame(QByteArray &response, int timeoutMs, QString &error) override
    {
        if (!handle_) {
            error = QStringLiteral("transport is closed");
            return false;
        }

        QByteArray buffer(packetSize_, '\0');
        const int read = hid_read_timeout(handle_,
                                          reinterpret_cast<unsigned char *>(buffer.data()),
                                          static_cast<size_t>(packetSize_),
                                          timeoutMs);
        if (read < 0) {
            error = QCoreApplication::translate("DapTransport", "HID read failed: %1")
                        .arg(hidErrorText(handle_));
            return false;
        }
        if (read == 0) {
            error = QCoreApplication::translate("DapTransport", "The probe did not answer (timeout).");
            return false;
        }

        buffer.resize(read);
        response = buffer;
        return true;
    }

private:
    hid_device *handle_;
    int packetSize_;
};

/* ------------------------------------------------------------------ */
/*  USB bulk transport (WinUSB)                                       */
/* ------------------------------------------------------------------ */

/**
 * A CMSIS-DAP probe whose command interface is USB bulk.
 *
 * The CMSIS-DAP v2 specification (ARM, "Communication via USB bulk endpoints")
 * defines the command interface as the *same* command/response byte stream that
 * v1 uses over HID - `[Command-ID][data...]` - moved onto USB bulk endpoints.
 * It does not define any length prefix between packets: a packet boundary is
 * found the way DAPLink's own usbd_bulk.c does it, by "short packet (< MPS)
 * terminates the transfer". The official reference (DAPLink), OpenOCD and
 * pyOCD all speak exactly that, with the command ID in byte 0.
 *
 * Some third-party tools nevertheless *assume* a two byte little endian length
 * prefix on bulk, which makes a spec-conformant probe look dead: the probe
 * reads byte 0 as the command ID, while the tool reads the same byte as a
 * length and mis-frames the reply. The WeAct Studio USB UART DAP is
 * spec-conformant (no prefix), so the framing is measured instead of assumed
 * (see detectFraming()) and both forms are supported on the wire.
 */
class WinUsbTransport : public DapTransport
{
public:
    /** How the CMSIS-DAP packet is wrapped on the bulk endpoint */
    enum class Framing {
        LengthPrefixed,   /* length-prefixed bulk: two byte little endian length first */
        Plain             /* spec-conformant CMSIS-DAP v2 bulk: the packet as-is */
    };

    WinUsbTransport()
        : handle_(nullptr)
        , interfaceNumber_(0)
        , epIn_(0)
        , epOut_(0)
        , framing_(Framing::LengthPrefixed)
        , maxPacketSize_(0)
        , reportPacketSize_(0)
    {
    }

    ~WinUsbTransport() override
    {
        close();
    }

    bool open(const DapProbeInfo &probe, QString &error) override
    {
        close();

        libusb_context *ctx = dapLibusbContext();
        if (!ctx) {
            error = QCoreApplication::translate("DapTransport", "The USB (libusb) layer could not be initialised.");
            return false;
        }

        libusb_device **list = nullptr;
        const ssize_t count = libusb_get_device_list(ctx, &list);
        if (count < 0) {
            error = QCoreApplication::translate("DapTransport", "USB enumeration failed.");
            return false;
        }

        libusb_device *found = nullptr;
        for (ssize_t i = 0; i < count && !found; ++i) {
            libusb_device *dev = list[i];
            const QString candidate = usbPortPath(dev)
                                      + QStringLiteral("#%1").arg(probe.usbInterface);
            if (candidate == probe.path)
                found = dev;
        }

        if (!found) {
            libusb_free_device_list(list, 1);
            error = QCoreApplication::translate(
                        "DapTransport",
                        "The USB probe was not found any more. Replug it and refresh the list.");
            return false;
        }

        /* Packet size of the endpoint that carries the commands. A bare packet
         * is assembled by the firmware from as many USB packets as it takes,
         * so this is not a hard limit on its own - but the firmware buffer
         * behind it is, and that one is over-reported (see packetSize()).
         * Read the descriptor while the device list is still alive, it goes
         * away with it. */
        const int descriptorPacketSize =
            probe.usbInterface >= 0 ? bulkPacketSize(found, probe.usbInterface) : 0;

        const int rc = libusb_open(found, &handle_);
        libusb_free_device_list(list, 1);

        if (rc != 0) {
            handle_ = nullptr;
            if (rc == LIBUSB_ERROR_ACCESS) {
                error = QCoreApplication::translate(
                            "DapTransport",
                            "Access to the USB probe was denied. It is either used by "
                            "another program, or no WinUSB driver is bound to it.");
            } else if (rc == LIBUSB_ERROR_NOT_SUPPORTED) {
                error = QCoreApplication::translate(
                            "DapTransport",
                            "The probe is not bound to a WinUSB (or libusbK) driver. "
                            "Install the driver, or select the same probe over its "
                            "USB HID (CMSIS-DAP v1) interface.");
            } else {
                error = QCoreApplication::translate("DapTransport", "Opening the USB probe failed: %1")
                            .arg(libusbErrorText(rc));
            }
            return false;
        }

        interfaceNumber_ = static_cast<uint8_t>(probe.usbInterface);
        epIn_ = static_cast<uint8_t>(probe.epIn);
        epOut_ = static_cast<uint8_t>(probe.epOut);

        const int claim = libusb_claim_interface(handle_, interfaceNumber_);
        if (claim != 0) {
            error = QCoreApplication::translate("DapTransport", "Claiming the USB interface failed: %1")
                        .arg(libusbErrorText(claim));
            close();
            return false;
        }

        /* A probe left over from an interrupted transfer can still have its
         * endpoints halted; clear them so the first command goes through. */
        libusb_clear_halt(handle_, epIn_);
        libusb_clear_halt(handle_, epOut_);

        if (descriptorPacketSize > 0)
            maxPacketSize_ = descriptorPacketSize;

        /* Ask the probe a question whose answer only makes sense in one of the
         * two framings; everything after this can assume the right one. */
        if (!detectFraming(error)) {
            close();
            return false;
        }

        return true;
    }

    void close() override
    {
        if (handle_) {
            libusb_release_interface(handle_, interfaceNumber_);
            libusb_close(handle_);
            handle_ = nullptr;
        }

        framing_ = Framing::LengthPrefixed;
        maxPacketSize_ = 0;
        reportPacketSize_ = 0;
    }

    bool isOpen() const override { return handle_ != nullptr; }

    int packetSize() const override
    {
        if (framing_ == Framing::Plain) {
            /* A bare packet is assembled by the firmware from as many USB
             * packets as it takes, so neither the framing nor the endpoint
             * packet size is a hard limit by itself - the firmware buffer
             * behind them is, and that buffer is 15832 bytes on the measured
             * WeAct probe (see kPlainBulkPacketSize for the sweep that
             * measured it).
             *
             * The probe's own answer is not usable as the limit: DAP_Info(0xFF)
             * reports 1024 bytes while the firmware takes 15832. The endpoint
             * packet size (512 here) is a USB detail, not a command limit, and
             * the firmware reassembles the command across as many USB packets
             * as it spans. So the buffer is what is clamped to - walking past
             * it makes the firmware truncate the transfer count (status 0x08),
             * which is how the edge was found.
             *
             * DAPFLASHER_PACKET_SIZE lowers or raises this for experiments;
             * the override never goes past the measured buffer edge. */
            int size = kPlainBulkPacketSize;

            if (const int override = plainPacketSizeOverride())
                size = qMin(override, kPlainBulkPacketSizeMax);

            /* A descriptor that announces less than the buffer really is still
             * a statement about the endpoint, not about the command, so it is
             * deliberately not applied here. */
            (void)maxPacketSize_;

            return size;
        }

        /* With a length prefix the packet may span USB packets, so the only
         * limit is the one the probe reported - never more than the v2
         * specification allows. */
        if (reportPacketSize_ > 0)
            return qMin(reportPacketSize_, kUsbV2PacketSize);

        return kUsbV2PacketSize;
    }

    void setPacketSize(int size) override
    {
        /* Anything below a DAP_Transfer header makes no sense, anything above
         * what a firmware buffer can hold is a misreport. */
        if (size >= 8 && size <= kMaxReportedPacketSize)
            reportPacketSize_ = size;
    }

    /* Reports the *framing* only, never the transport kind. This whole class
     * is the USB bulk transport, and which framing it ends up speaking on it
     * is decided per probe by detectFraming() - the two are not the same thing
     * and must not be conflated. Both branches therefore describe framing over
     * the one transport this class owns.
     *
     * Neither branch is a version of the protocol: the spec-conformant bulk
     * framing is byte-identical to what a v1 HID report carries, and the
     * length-prefixed form is a tool-side convention, not part of CMSIS-DAP.
     * The wording below says exactly what is on the wire and nothing more. */    QString name() const override
    {
        if (framing_ == Framing::Plain) {
            return QCoreApplication::translate(
                        "DapTransport",
                        "CMSIS-DAP bulk transport, spec framing (no length prefix)");
        }

        return QCoreApplication::translate(
                    "DapTransport",
                    "CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)");
    }

protected:
    bool writeFrame(const QByteArray &command, QString &error) override
    {
        if (!handle_) {
            error = QStringLiteral("transport is closed");
            return false;
        }

        QByteArray frame;

        if (framing_ == Framing::Plain) {
            /* The bare CMSIS-DAP packet. This is the byte stream the CMSIS-DAP
             * v2 bulk transport is defined to carry - byte identical to what a
             * v1 HID report would carry, with no length prefix. */
            frame = command;
        } else {
            frame.reserve(command.size() + 8);

            const quint16 length = static_cast<quint16>(command.size());
            frame.append(static_cast<char>(length & 0xFF));
            frame.append(static_cast<char>((length >> 8) & 0xFF));
            frame.append(command);

            while (frame.size() % kUsbWriteAlignment != 0)
                frame.append('\0');
        }

        int transferred = 0;
        const int rc = libusb_bulk_transfer(
            handle_,
            epOut_,
            reinterpret_cast<unsigned char *>(frame.data()),
            frame.size(),
            &transferred,
            kUsbWriteTimeoutMs);

        if (rc != 0) {
            error = QCoreApplication::translate("DapTransport", "USB bulk write failed: %1")
                        .arg(libusbErrorText(rc));
            return false;
        }

        return true;
    }

    bool readFrame(QByteArray &response, int timeoutMs, QString &error) override
    {
        if (!handle_) {
            error = QStringLiteral("transport is closed");
            return false;
        }

        /* The answer is at most one DAP packet long, so the buffer follows the
         * packet size instead of being a fixed array. A fixed one truncates
         * the answer of a probe that reported a larger buffer, and the bytes
         * left behind on the endpoint then desynchronise every command that
         * follows - one silent truncation turns into a whole session of
         * garbage. */
        QByteArray buffer(packetSize() + kReadBufferSlack, '\0');
        int transferred = 0;

        const int rc = libusb_bulk_transfer(handle_, epIn_,
                                            reinterpret_cast<unsigned char *>(buffer.data()),
                                            static_cast<int>(buffer.size()),
                                            &transferred, timeoutMs);
        if (rc == LIBUSB_ERROR_TIMEOUT) {
            error = QCoreApplication::translate("DapTransport", "The probe did not answer (timeout).");
            return false;
        }
        if (rc != 0) {
            error = QCoreApplication::translate("DapTransport", "USB bulk read failed: %1")
                        .arg(libusbErrorText(rc));
            return false;
        }

        if (framing_ == Framing::Plain) {
            /* The whole transfer is the answer: there is no prefix to skip and
             * a one byte answer (DAP_Disconnect) is legal. */
            if (transferred <= 0) {
                error = QCoreApplication::translate("DapTransport", "The probe sent a malformed packet.");
                return false;
            }

            buffer.truncate(transferred);
            response = buffer;
            return true;
        }

        if (transferred < 2) {
            error = QCoreApplication::translate("DapTransport", "The probe sent a malformed packet.");
            return false;
        }

        const int length = static_cast<quint8>(buffer.at(0))
                           | (static_cast<quint8>(buffer.at(1)) << 8);
        const int available = transferred - 2;
        const int payload = qBound(0, length, available);

        response = buffer.mid(2, payload);
        return true;
    }

private:
    /** Ask the probe one DAP_Info question in both framings and keep the one
     *  that produced a real answer.
     *
     *  DAP_Info(Vendor) is two bytes long, so the length prefixed form goes on
     *  the wire as "02 00 00 01". A probe that expects the bare packet reads
     *  that as DAP_Connect(port 0) and answers "02 01", which the length
     *  prefixed reader parses as a header claiming 258 bytes and therefore
     *  hands back an empty response - the probe looks dead although it is
     *  answering perfectly. Framing is the only difference, so it is measured
     *  once here rather than guessed from the VID/PID.
     */
    bool detectFraming(QString &error)
    {
        QByteArray command;
        command.append(static_cast<char>(DapCmd::Info));
        command.append(static_cast<char>(DapInfoId::Vendor));

        static const Framing candidates[] = {
            Framing::LengthPrefixed,
            Framing::Plain
        };

        /* The very first packet after a probe has been opened is sometimes
         * swallowed (measured on the WeAct probe: two out of three opens), so a
         * silent attempt is retried once before that framing is written off. */
        const int attempts = 2;

        for (Framing candidate : candidates) {
            framing_ = candidate;

            for (int attempt = 0; attempt < attempts; ++attempt) {
                QByteArray response;
                QString ignored;

                if (!sendCommand(command, ignored)
                    || !receiveResponse(response, kFramingProbeTimeoutMs, ignored)) {
                    continue;    /* nothing came back, try once more */
                }

                if (!response.isEmpty()
                    && static_cast<quint8>(response.at(0)) == DapCmd::Info) {
                    return true;
                }

                /* The probe answered, but not in this framing; retrying it
                 * would only make the same probe answer the same way again */
                break;
            }

            /* The command may have been answered after the timeout; drop the
             * stale packets so the next framing starts from a clean endpoint */
            QByteArray stale;
            QString ignored;
            for (int i = 0; i < 4 && receiveResponse(stale, 50, ignored); ++i)
                ;
        }

        framing_ = Framing::LengthPrefixed;
        error = QCoreApplication::translate(
                    "DapTransport",
                    "The probe did not answer DAP_Info. Check that the right "
                    "interface of the device was selected and that the firmware "
                    "runs a CMSIS-DAP implementation.");
        return false;
    }

    libusb_device_handle *handle_;
    uint8_t interfaceNumber_;
    uint8_t epIn_;
    uint8_t epOut_;
    Framing framing_;
    int maxPacketSize_;      /* wMaxPacketSize of the bulk endpoints, 0 = unknown */
    int reportPacketSize_;   /* DAP packet size the probe reported, 0 = unknown */
};

} /* namespace */

/* ------------------------------------------------------------------ */
/*  DapTransport                                                      */
/* ------------------------------------------------------------------ */

DapTransport *DapTransport::create(const DapProbeInfo &probe)
{
    if (probe.transport == DapTransportKind::WinUsb)
        return new WinUsbTransport();

    return new HidTransport();
}

DapTransport::~DapTransport() = default;

bool DapTransport::sendCommand(const QByteArray &command, QString &error)
{
    if (command.isEmpty()) {
        error = QStringLiteral("empty command");
        return false;
    }
    if (command.size() > packetSize()) {
        error = QCoreApplication::translate("DapTransport",
                                            "The command does not fit into one packet (%1 > %2 bytes).")
                    .arg(command.size())
                    .arg(packetSize());
        return false;
    }

    return writeFrame(command, error);
}

bool DapTransport::receiveResponse(QByteArray &response, int timeoutMs, QString &error)
{
    return readFrame(response, timeoutMs, error);
}
