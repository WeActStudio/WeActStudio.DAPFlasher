/*
 * WeAct Studio DAP Flasher
 *
 * Minimal ELF32 little endian reader, enough to load a Keil .FLM file.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "elffile.h"

#include <QCoreApplication>
#include <QFile>

#include <cstring>

namespace {

const quint16 kEmArm = 40;
const quint32 kPtLoad = 1;
const quint32 kShtSymtab = 2;

/* ELF header offsets */
const int kOffMachine = 0x12;
const int kOffEntry = 0x18;
const int kOffPhOff = 0x1C;
const int kOffShOff = 0x20;
const int kOffPhEntSize = 0x2A;
const int kOffPhNum = 0x2C;
const int kOffShEntSize = 0x2E;
const int kOffShNum = 0x30;
const int kOffShStrNdx = 0x32;

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

} /* namespace */

bool ElfFile::load(const QString &path, QString &error)
{
    valid_ = false;
    segments_.clear();
    symbols_.clear();
    machine_ = 0;
    entryPoint_ = 0;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        error = QCoreApplication::translate("ElfFile", "The algorithm file could not be opened: %1").arg(file.errorString());
        return false;
    }

    data_ = file.readAll();
    file.close();

    if (data_.size() < 52) {
        error = QCoreApplication::translate("ElfFile", "The file is too small to be an ELF image.");
        return false;
    }

    if (std::memcmp(data_.constData(), "\x7f" "ELF", 4) != 0) {
        error = QCoreApplication::translate("ElfFile", "The file is not an ELF image (a Keil .FLM algorithm is expected).");
        return false;
    }

    if (static_cast<quint8>(data_.at(4)) != 1) {
        error = QCoreApplication::translate("ElfFile", "Only 32 bit ELF images are supported (this one is 64 bit).");
        return false;
    }

    if (static_cast<quint8>(data_.at(5)) != 1) {
        error = QCoreApplication::translate("ElfFile", "Only little endian ELF images are supported.");
        return false;
    }

    machine_ = readU16(data_, kOffMachine);
    if (machine_ != kEmArm) {
        error = QCoreApplication::translate("ElfFile", "The image is not an ARM image (e_machine = %1).").arg(machine_);
        return false;
    }

    entryPoint_ = readU32(data_, kOffEntry);

    /* ---------- program headers ---------- */
    const quint32 phOff = readU32(data_, kOffPhOff);
    const quint16 phEntSize = readU16(data_, kOffPhEntSize);
    const quint16 phNum = readU16(data_, kOffPhNum);

    for (quint16 i = 0; i < phNum; ++i) {
        const quint32 offset = phOff + static_cast<quint32>(i) * phEntSize;
        if (offset + 32 > static_cast<quint32>(data_.size()))
            break;

        const quint32 type = readU32(data_, static_cast<int>(offset));
        if (type != kPtLoad)
            continue;

        const quint32 fileOffset = readU32(data_, static_cast<int>(offset) + 4);
        const quint32 vaddr = readU32(data_, static_cast<int>(offset) + 8);
        const quint32 paddr = readU32(data_, static_cast<int>(offset) + 12);
        const quint32 fileSize = readU32(data_, static_cast<int>(offset) + 16);
        const quint32 memSize = readU32(data_, static_cast<int>(offset) + 20);

        ElfSegment segment;
        segment.address = paddr ? paddr : vaddr;
        segment.fileSize = fileSize;
        segment.memSize = memSize;

        if (fileSize > 0) {
            if (fileOffset + fileSize > static_cast<quint32>(data_.size())) {
                error = QCoreApplication::translate("ElfFile", "The image is truncated (segment %1 leaves the file).").arg(i);
                return false;
            }
            segment.data = data_.mid(static_cast<int>(fileOffset), static_cast<int>(fileSize));
        }

        segments_.append(segment);
    }

    if (segments_.isEmpty()) {
        error = QCoreApplication::translate("ElfFile", "The image carries no loadable segment.");
        return false;
    }

    /* ---------- symbol table ---------- */
    const quint32 shOff = readU32(data_, kOffShOff);
    const quint16 shEntSize = readU16(data_, kOffShEntSize);
    const quint16 shNum = readU16(data_, kOffShNum);

    for (quint16 i = 0; i < shNum; ++i) {
        const quint32 offset = shOff + static_cast<quint32>(i) * shEntSize;
        if (offset + 40 > static_cast<quint32>(data_.size()))
            break;

        const quint32 type = readU32(data_, static_cast<int>(offset) + 4);
        if (type != kShtSymtab)
            continue;

        const quint32 symOffset = readU32(data_, static_cast<int>(offset) + 16);
        const quint32 symSize = readU32(data_, static_cast<int>(offset) + 20);
        const quint32 link = readU32(data_, static_cast<int>(offset) + 24);

        const quint32 strOffset = shOff + link * shEntSize;
        if (strOffset + 40 > static_cast<quint32>(data_.size()))
            continue;

        const quint32 strTableOffset = readU32(data_, static_cast<int>(strOffset) + 16);
        const quint32 strTableSize = readU32(data_, static_cast<int>(strOffset) + 20);

        if (strTableOffset + strTableSize > static_cast<quint32>(data_.size()))
            continue;

        const char *strings = data_.constData() + strTableOffset;
        const int count = static_cast<int>(symSize / 16);

        for (int s = 0; s < count; ++s) {
            const quint32 entry = symOffset + static_cast<quint32>(s) * 16;
            if (entry + 16 > static_cast<quint32>(data_.size()))
                break;

            const quint32 nameOffset = readU32(data_, static_cast<int>(entry));
            if (nameOffset == 0 || nameOffset >= strTableSize)
                continue;

            ElfSymbol symbol;
            symbol.name = QString::fromLatin1(strings + nameOffset);
            symbol.value = readU32(data_, static_cast<int>(entry) + 4);
            symbol.size = readU32(data_, static_cast<int>(entry) + 8);
            symbol.type = static_cast<quint8>(data_.at(static_cast<int>(entry) + 12)) & 0x0F;
            symbol.bind = static_cast<quint8>(data_.at(static_cast<int>(entry) + 12)) >> 4;

            symbols_.append(symbol);
        }
    }

    valid_ = true;
    return true;
}

bool ElfFile::findSymbol(const QString &name, ElfSymbol &symbol) const
{
    for (const ElfSymbol &candidate : symbols_) {
        if (candidate.name == name) {
            symbol = candidate;
            return true;
        }
    }
    return false;
}

QByteArray ElfFile::readAt(quint32 address, int size) const
{
    if (size <= 0)
        return QByteArray();

    for (const ElfSegment &segment : segments_) {
        if (address < segment.address)
            continue;

        const quint32 delta = address - segment.address;
        if (delta + static_cast<quint32>(size) > static_cast<quint32>(segment.data.size()))
            continue;

        return segment.data.mid(static_cast<int>(delta), size);
    }

    return QByteArray();
}
