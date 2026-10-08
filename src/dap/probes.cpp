/*
 * WeAct Studio DAP Flasher
 *
 * CMSIS-DAP probe enumeration (USB HID and USB bulk transports)
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "probes.h"
#include "dapdefs.h"

#include <QCoreApplication>
#include <QStringList>

#include <algorithm>

#include <hidapi/hidapi.h>
#include <libusb-1.0/libusb.h>

namespace {

/* Probes that are known to implement CMSIS-DAP but do not always advertise
 * the vendor defined usage page in their HID report descriptor. */
const DapUsbId kKnownDapIds[] = {
    { 0x0D28, 0x0204, "mbed DAPLink" },
    { 0x0D28, 0x0205, "mbed DAPLink (CDC)" },
    { 0x2E8A, 0x000C, "Raspberry Pi Debug Probe" },
    { 0x2E8A, 0x0004, "Raspberry Pi Pico (DAP)" },
    { 0xC251, 0x2722, "Keil ULINKplus" },
    { 0xC251, 0x2712, "Keil ULINK2" },
    { 0x1209, 0xBDF0, "Black Magic Probe" },
    { 0x1A86, 0x8012, "WCH-Link CMSIS-DAP" },
    { 0x1A86, 0x8011, "WCH-Link CMSIS-DAP" },
    { 0x2886, 0x0001, "SEGGER J-Link (DAP)" }
};

QString fromWide(const wchar_t *text)
{
    if (!text)
        return QString();
    return QString::fromWCharArray(text).trimmed();
}

/** First line of a possibly multi line USB product string */
QString firstLine(const QString &text)
{
    const int newline = text.indexOf(QLatin1Char('\n'));
    return newline < 0 ? text : text.left(newline).trimmed();
}

const char *knownName(quint16 vid, quint16 pid)
{
    for (const DapUsbId &id : kKnownDapIds) {
        if (id.vid == vid && id.pid == pid)
            return id.name;
    }
    return nullptr;
}

/* ------------------------------------------------------------------ */
/*  USB HID transport                                                 */
/* ------------------------------------------------------------------ */

bool ensureHidReady(QString *errorMessage)
{
    static int state = 0;    /* 0 = unknown, 1 = ready, -1 = failed */

    if (state == 1)
        return true;
    if (state == -1) {
        if (errorMessage)
            *errorMessage = QCoreApplication::translate("Probes", "The HID interface could not be initialised.");
        return false;
    }

    if (hid_init() != 0) {
        state = -1;
        const wchar_t *err = hid_error(nullptr);
        if (errorMessage) {
            *errorMessage = err
                                ? QCoreApplication::translate("Probes", "HID init failed: %1").arg(fromWide(err))
                                : QCoreApplication::translate("Probes", "HID init failed.");
        }
        return false;
    }

    state = 1;
    return true;
}

void enumerateHid(QList<DapProbeInfo> &out, bool includeAllHid)
{
    struct hid_device_info *list = hid_enumerate(0x0000, 0x0000);
    if (!list)
        return;

    for (struct hid_device_info *dev = list; dev; dev = dev->next) {
        DapProbeInfo info;
        info.transport = DapTransportKind::Hid;
        info.path = QString::fromLocal8Bit(dev->path ? dev->path : "");
        info.vid = dev->vendor_id;
        info.pid = dev->product_id;
        info.manufacturer = fromWide(dev->manufacturer_string);
        info.product = fromWide(dev->product_string);
        info.serial = fromWide(dev->serial_number);
        info.hidInterface = dev->interface_number;
        info.usagePage = dev->usage_page;
        info.usage = dev->usage;

        const bool vendorPage = (info.usagePage == kDapUsagePage);
        const bool known = (knownName(info.vid, info.pid) != nullptr);
        info.dapCandidate = vendorPage || known;

        if (!info.dapCandidate && !includeAllHid)
            continue;

        /* Only the candidates are probed for availability: opening every HID
         * device of the system on each refresh would be needlessly noisy. */
        if (info.dapCandidate) {
            hid_device *handle = hid_open_path(dev->path);
            if (handle)
                hid_close(handle);
            else
                info.busy = true;
        }

        out.append(info);
    }

    hid_free_enumeration(list);
}

/* ------------------------------------------------------------------ */
/*  USB bulk transport                                                */
/* ------------------------------------------------------------------ */

libusb_context *g_usbContext = nullptr;
bool g_usbInitTried = false;

libusb_context *usbContext()
{
    if (g_usbInitTried)
        return g_usbContext;

    g_usbInitTried = true;
    if (libusb_init(&g_usbContext) != 0) {
        g_usbContext = nullptr;
        return nullptr;
    }
    return g_usbContext;
}

/** Build the stable USB port path, e.g. "usb:1-3.2" */
QString usbPortPath(libusb_device *dev)
{
    uint8_t ports[8] = { 0 };
    const int count = libusb_get_port_numbers(dev, ports, sizeof(ports));

    QString path = QString::number(libusb_get_bus_number(dev));
    for (int i = 0; i < count; ++i)
        path += QStringLiteral(".%1").arg(ports[i]);

    return QStringLiteral("usb:") + path;
}

QString usbString(libusb_device_handle *handle, uint8_t index)
{
    if (!handle || index == 0)
        return QString();

    unsigned char buffer[255] = { 0 };
    if (libusb_get_string_descriptor_ascii(handle, index, buffer, sizeof(buffer)) <= 0)
        return QString();

    return QString::fromLatin1(reinterpret_cast<const char *>(buffer)).trimmed();
}

void enumerateUsb(QList<DapProbeInfo> &out)
{
    libusb_context *ctx = usbContext();
    if (!ctx)
        return;

    libusb_device **list = nullptr;
    const ssize_t count = libusb_get_device_list(ctx, &list);
    if (count < 0)
        return;

    for (ssize_t i = 0; i < count; ++i) {
        libusb_device *dev = list[i];

        libusb_device_descriptor desc = {};
        if (libusb_get_device_descriptor(dev, &desc) != 0)
            continue;

        libusb_config_descriptor *config = nullptr;
        if (libusb_get_active_config_descriptor(dev, &config) != 0) {
            if (libusb_get_config_descriptor(dev, 0, &config) != 0)
                continue;
        }

        for (int n = 0; n < config->bNumInterfaces; ++n) {
            const libusb_interface &iface = config->interface[n];

            for (int a = 0; a < iface.num_altsetting; ++a) {
                const libusb_interface_descriptor &id = iface.altsetting[a];

                /* The USB bulk transport of a CMSIS-DAP probe: a vendor
                 * specific interface without a subclass or protocol,
                 * carrying exactly one bulk IN and one bulk OUT endpoint.
                 * The framing the probe speaks on it is a separate question -
                 * see the file header of probes.h. */
                if (id.bInterfaceClass != 0xFF || id.bInterfaceSubClass != 0x00
                    || id.bInterfaceProtocol != 0x00)
                    continue;

                int epIn = -1;
                int epOut = -1;
                for (int e = 0; e < id.bNumEndpoints; ++e) {
                    const libusb_endpoint_descriptor &ep = id.endpoint[e];
                    if ((ep.bmAttributes & 0x03) != LIBUSB_TRANSFER_TYPE_BULK)
                        continue;
                    if (ep.bEndpointAddress & 0x80)
                        epIn = ep.bEndpointAddress;
                    else
                        epOut = ep.bEndpointAddress;
                }

                if (epIn < 0 || epOut < 0)
                    continue;

                DapProbeInfo info;
                info.transport = DapTransportKind::WinUsb;
                info.vid = desc.idVendor;
                info.pid = desc.idProduct;
                info.busNumber = libusb_get_bus_number(dev);
                info.deviceAddress = libusb_get_device_address(dev);
                info.usbInterface = id.bInterfaceNumber;
                info.epIn = epIn;
                info.epOut = epOut;
                info.path = usbPortPath(dev) + QStringLiteral("#%1").arg(id.bInterfaceNumber);
                info.dapCandidate = true;

                /* Reading the strings needs a handle; failing to open is not
                 * fatal here, it only means the probe is not usable (no
                 * WinUSB driver, or another program holds it). */
                libusb_device_handle *handle = nullptr;
                if (libusb_open(dev, &handle) == 0) {
                    info.manufacturer = usbString(handle, desc.iManufacturer);
                    info.product = usbString(handle, desc.iProduct);
                    info.serial = usbString(handle, desc.iSerialNumber);
                    libusb_close(handle);
                } else {
                    info.busy = true;
                }

                out.append(info);
            }
        }

        libusb_free_config_descriptor(config);
    }

    libusb_free_device_list(list, 1);
}

} /* namespace */

libusb_context *dapLibusbContext()
{
    return usbContext();
}

/* ------------------------------------------------------------------ */
/*  DapProbeInfo                                                      */
/* ------------------------------------------------------------------ */

QString DapProbeInfo::displayName() const
{
    QString name = firstLine(product);
    if (name.isEmpty())
        name = firstLine(manufacturer);
    if (name.isEmpty()) {
        const char *known = knownName(vid, pid);
        name = known ? QString::fromLatin1(known)
                     : QCoreApplication::translate("Probes", "HID device");
    }

    if (name.size() > 42)
        name = name.left(39) + QStringLiteral("...");

    const QString id = QStringLiteral(" [%1:%2]")
                           .arg(vid, 4, 16, QLatin1Char('0'))
                           .arg(pid, 4, 16, QLatin1Char('0'));

    /* The transport kind, spelled out instead of the bare "v2"/"v1" it used to
     * print: a bulk transport does not by itself say whether the two byte
     * length prefix some tools assume is on the wire (see probes.h for the
     * WeAct probe, which is spec-conformant and sends none), and printing both
     * words next to each other made the list read as if one probe were two
     * different things. */
    const QString transport = isBulk() ? QStringLiteral(" USB bulk")
                                       : QStringLiteral(" USB HID");

    if (busy)
        return name + id + transport + QCoreApplication::translate("Probes", " (in use)");

    return name + id + transport;
}

QString DapProbeInfo::description() const
{
    QStringList parts;
    parts << QStringLiteral("VID:PID = %1:%2")
                 .arg(vid, 4, 16, QLatin1Char('0'))
                 .arg(pid, 4, 16, QLatin1Char('0'));
    parts << QCoreApplication::translate("Probes", "Transport: %1")
                 .arg(isBulk()
                          ? QCoreApplication::translate("DapTransport", "USB bulk (WinUSB, CMSIS-DAP v2)")
                          : QCoreApplication::translate("DapTransport", "USB HID (CMSIS-DAP v1)"));

    if (!product.isEmpty())
        parts << QCoreApplication::translate("Probes", "Product: %1").arg(firstLine(product));
    if (!manufacturer.isEmpty())
        parts << QCoreApplication::translate("Probes", "Manufacturer: %1").arg(firstLine(manufacturer));
    if (!serial.isEmpty())
        parts << QCoreApplication::translate("Probes", "Serial: %1").arg(serial);

    if (isBulk()) {
        parts << QCoreApplication::translate("Probes", "USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5")
                     .arg(busNumber)
                     .arg(deviceAddress)
                     .arg(usbInterface)
                     .arg(epIn, 2, 16, QLatin1Char('0'))
                     .arg(epOut, 2, 16, QLatin1Char('0'));
    } else {
        if (hidInterface >= 0)
            parts << QCoreApplication::translate("Probes", "Interface: %1").arg(hidInterface);
        parts << QCoreApplication::translate("Probes", "Usage page: 0x%1  Usage: 0x%2")
                     .arg(usagePage, 4, 16, QLatin1Char('0'))
                     .arg(usage, 4, 16, QLatin1Char('0'));
    }

    parts << path;
    return parts.join(QStringLiteral("\n"));
}

QString DapProbeInfo::key() const
{
    /* Stable across refreshes: HID path or USB port path. Both change when the
     * probe is moved to another USB port, which is what the UI wants to see. */
    return path;
}

/* ------------------------------------------------------------------ */
/*  Enumeration entry point                                           */
/* ------------------------------------------------------------------ */

QList<DapProbeInfo> dapEnumerateProbes(bool includeAllHid, QString *errorMessage)
{
    QList<DapProbeInfo> result;

    if (ensureHidReady(errorMessage))
        enumerateHid(result, includeAllHid);

    enumerateUsb(result);

    /* Sort: USB bulk (the fast transport) first, then by VID/PID and path
     * so that the list does not jump around between refreshes. */
    std::sort(result.begin(), result.end(),
              [](const DapProbeInfo &a, const DapProbeInfo &b) {
                  if (a.isBulk() != b.isBulk())
                      return a.isBulk();
                  if (a.vid != b.vid)
                      return a.vid < b.vid;
                  if (a.pid != b.pid)
                      return a.pid < b.pid;
                  return a.path < b.path;
              });

    return result;
}

void dapProbesShutdown()
{
    if (g_usbContext) {
        libusb_exit(g_usbContext);
        g_usbContext = nullptr;
    }
    hid_exit();
}
