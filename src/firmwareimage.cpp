/*
 * WeAct Studio DAP Flasher
 *
 * Firmware image parsing: Intel HEX (.hex) and raw binary (.bin)
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "firmwareimage.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QFile>
#include <QList>
#include <QStringList>

#include <algorithm>

namespace {

/* One HEX data record */
struct HexChunk
{
    quint32 address;
    QByteArray bytes;
};

/* Maximum accepted image size, guards against a malformed HEX exhausting memory */
const qint64 kMaxImageSize = 4 * 1024 * 1024;

/* Translate helper: every message below is user visible */
QString trText(const char *text)
{
    return QCoreApplication::translate("FirmwareImage", text);
}

/* Convert one HEX text line into a byte sequence; false means malformed */
bool hexLineToBytes(const QString &line, QByteArray &bytes)
{
    const QString body = line.trimmed();

    if (body.length() < 11)          /* ':' plus at least 5 bytes = 11 characters */
        return false;
    if (!body.startsWith(QLatin1Char(':')))
        return false;
    if (body.length() % 2 == 0)      /* an even number of characters must follow ':' */
        return false;

    bytes.clear();
    bytes.reserve((body.length() - 1) / 2);

    bool ok = false;
    for (int i = 1; i < body.length(); i += 2) {
        const unsigned value = body.mid(i, 2).toUInt(&ok, 16);
        if (!ok)
            return false;
        bytes.append(static_cast<char>(value));
    }

    return true;
}

/* Parse Intel HEX text */
bool parseHex(const QByteArray &raw, FirmwareImage &image, QString &errorMessage)
{
    QList<HexChunk> chunks;
    quint32 upperAddress = 0;      /* Extended linear address (type 04) */
    quint32 segmentAddress = 0;    /* Extended segment address (type 02) */
    quint32 minAddress = 0xFFFFFFFFu;
    quint32 maxAddress = 0;
    bool endRecordSeen = false;
    int lineNumber = 0;

    const QString text = QString::fromLatin1(raw);
    const QStringList lines = text.split(QLatin1Char('\n'));

    for (const QString &rawLine : lines) {
        ++lineNumber;

        const QString line = rawLine.trimmed();
        if (line.isEmpty())
            continue;
        if (!line.startsWith(QLatin1Char(':'))) {
            /* Non HEX content (blank lines, comments) around the records is tolerated */
            continue;
        }

        QByteArray record;
        if (!hexLineToBytes(line, record)) {
            errorMessage = trText("Invalid HEX file: cannot parse line %1").arg(lineNumber);
            return false;
        }

        if (record.size() < 5) {
            errorMessage = trText("Invalid HEX file: line %1 is too short").arg(lineNumber);
            return false;
        }

        /* Checksum: the sum of the whole record, including the checksum byte, is 0x00 */
        quint8 sum = 0;
        for (int i = 0; i < record.size(); ++i)
            sum = static_cast<quint8>(sum + static_cast<quint8>(record.at(i)));
        if (sum != 0) {
            errorMessage = trText("HEX checksum error on line %1 (0x%2)")
                               .arg(lineNumber)
                               .arg(static_cast<unsigned>(sum), 2, 16, QLatin1Char('0'));
            return false;
        }

        const int byteCount = static_cast<quint8>(record.at(0));
        if (record.size() < byteCount + 5) {
            errorMessage = trText("Invalid HEX file: data length mismatch on line %1").arg(lineNumber);
            return false;
        }

        const quint32 offset = (static_cast<quint8>(record.at(1)) << 8)
                               | static_cast<quint8>(record.at(2));
        const int type = static_cast<quint8>(record.at(3));
        const char *payload = record.constData() + 4;

        switch (type) {
        case 0x00: {   /* Data record */
            const quint32 address = upperAddress + segmentAddress + offset;
            HexChunk chunk;
            chunk.address = address;
            chunk.bytes = QByteArray(payload, byteCount);
            chunks.append(chunk);

            if (address < minAddress)
                minAddress = address;
            const quint32 last = address + static_cast<quint32>(byteCount) - 1;
            if (last > maxAddress)
                maxAddress = last;
            break;
        }
        case 0x01:     /* End of file */
            endRecordSeen = true;
            break;
        case 0x02:     /* Extended segment address */
            segmentAddress = ((static_cast<quint8>(record.at(4)) << 8)
                              | static_cast<quint8>(record.at(5))) << 4;
            break;
        case 0x03:     /* Start segment address (CS:IP) */
            image.entryAddress = (static_cast<quint8>(record.at(4)) << 8)
                                 | static_cast<quint8>(record.at(5));
            break;
        case 0x04:     /* Extended linear address */
            upperAddress = (static_cast<quint8>(record.at(4)) << 24)
                           | (static_cast<quint8>(record.at(5)) << 16);
            break;
        case 0x05:     /* Start linear address (EIP) */
            image.entryAddress = (static_cast<quint8>(record.at(4)) << 24)
                                 | (static_cast<quint8>(record.at(5)) << 16)
                                 | (static_cast<quint8>(record.at(6)) << 8)
                                 | static_cast<quint8>(record.at(7));
            break;
        default:       /* Unknown record type, ignored */
            break;
        }

        if (endRecordSeen)
            break;
    }

    if (chunks.isEmpty()) {
        errorMessage = trText("HEX file contains no valid data record");
        return false;
    }

    const qint64 size = static_cast<qint64>(maxAddress) - static_cast<qint64>(minAddress) + 1;
    if (size <= 0 || size > kMaxImageSize) {
        errorMessage = trText("HEX address range out of bounds: 0x%1 - 0x%2")
                           .arg(minAddress, 8, 16, QLatin1Char('0'))
                           .arg(maxAddress, 8, 16, QLatin1Char('0'));
        return false;
    }

    /* Fill the gaps with 0xFF to build one contiguous image. This buffer is
     * what the checksum and the address range in the UI are computed from;
     * the blocks below are what is actually erased and programmed. */
    QByteArray buffer(static_cast<int>(size), static_cast<char>(0xFF));
    for (const HexChunk &chunk : chunks) {
        const int offset = static_cast<int>(chunk.address - minAddress);
        for (int i = 0; i < chunk.bytes.size(); ++i)
            buffer[offset + i] = chunk.bytes.at(i);
    }

    /* The blocks that carry data: sorted, then joined wherever they touch, so
     * that a record split over several lines stays one block while a real gap
     * becomes a block boundary. Writing the gaps as well is what made this
     * tool program a bootloader-plus-application image as one 448 KB run
     * instead of the 325 KB it actually holds. */
    std::sort(chunks.begin(), chunks.end(),
              [](const HexChunk &a, const HexChunk &b) { return a.address < b.address; });

    for (const HexChunk &chunk : chunks) {
        if (!image.segments.isEmpty()) {
            const quint32 lastEnd = image.segments.last().address
                                    + static_cast<quint32>(image.segments.last().data.size());

            if (chunk.address <= lastEnd) {
                /* Touching or overlapping: extend the current block by the
                 * part of this record that is not covered yet. */
                const quint32 covered = lastEnd - chunk.address;
                if (covered < static_cast<quint32>(chunk.bytes.size()))
                    image.segments.last().data.append(chunk.bytes.mid(static_cast<int>(covered)));
                continue;
            }
        }

        FirmwareSegment segment;
        segment.address = chunk.address;
        segment.data = chunk.bytes;
        image.segments.append(segment);
    }

    image.data = buffer;
    image.startAddress = minAddress;
    image.endAddress = maxAddress;
    image.fromHex = true;
    return true;
}

/* 32-bit additive checksum */
quint32 sumBytes(const QByteArray &data)
{
    quint32 sum = 0;
    for (int i = 0; i < data.size(); ++i)
        sum += static_cast<quint8>(data.at(i));
    return sum;
}

} /* namespace */

bool firmware_load(const QString &path, quint32 binBaseAddress,
                   FirmwareImage &image, QString &errorMessage)
{
    errorMessage.clear();
    image = FirmwareImage();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        errorMessage = trText("Cannot open firmware file: %1").arg(path);
        return false;
    }

    const QByteArray raw = file.readAll();
    file.close();

    if (raw.isEmpty()) {
        errorMessage = trText("Firmware file is empty");
        return false;
    }

    const QString lower = path.toLower();

    if (lower.endsWith(QLatin1String(".hex")) || lower.endsWith(QLatin1String(".ihex"))
        || lower.endsWith(QLatin1String(".h86"))) {
        if (!parseHex(raw, image, errorMessage))
            return false;
    } else {
        /* Sniffing: a leading ':' plus a second ':' on another line means HEX */
        const QString head = QString::fromLatin1(raw.left(200));
        if (head.startsWith(QLatin1Char(':')) && head.contains(QLatin1Char('\n'))) {
            if (!parseHex(raw, image, errorMessage))
                return false;
        } else {
            image.data = raw;
            image.startAddress = binBaseAddress;
            image.endAddress = binBaseAddress
                               + static_cast<quint32>(qMax<qint64>(raw.size() - 1, 0));
            image.fromHex = false;
        }
    }

    /* A .bin file is one block by definition; the HEX path fills the list in
     * itself. The fallback also covers a HEX file whose records all touch,
     * where the block list is built but ends up holding a single entry. */
    if (image.segments.isEmpty() && !image.data.isEmpty()) {
        FirmwareSegment segment;
        segment.address = image.startAddress;
        segment.data = image.data;
        image.segments.append(segment);
    }

    image.checksum = sumBytes(image.data);
    return true;
}

quint32 FirmwareImage::payloadSize() const
{
    quint32 total = 0;
    for (const FirmwareSegment &segment : segments)
        total += static_cast<quint32>(segment.data.size());
    return total;
}

QString firmware_format_size(qint64 bytes)
{
    if (bytes < 1024)
        return trText("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) {
        return trText("%1 KB (%2 bytes)")
            .arg(static_cast<double>(bytes) / 1024.0, 0, 'f', 2)
            .arg(bytes);
    }
    return trText("%1 MB (%2 bytes)")
        .arg(static_cast<double>(bytes) / (1024.0 * 1024.0), 0, 'f', 2)
        .arg(bytes);
}
