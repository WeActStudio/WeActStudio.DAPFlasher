/*
 * WeAct Studio DAP Flasher
 *
 * Minimal ELF32 little endian reader, enough to load a Keil .FLM file.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef ELFFILE_H
#define ELFFILE_H

#include <QByteArray>
#include <QString>
#include <QVector>
#include <QtGlobal>

/** One PT_LOAD segment of the file */
struct ElfSegment
{
    quint32 address = 0;    /* p_paddr: load address, 0 based in a .FLM */
    quint32 fileSize = 0;   /* bytes present in the file */
    quint32 memSize = 0;    /* bytes to reserve, fileSize < memSize = zero fill */
    QByteArray data;        /* fileSize bytes */
};

/** One entry of the symbol table */
struct ElfSymbol
{
    QString name;
    quint32 value = 0;
    quint32 size = 0;
    quint8 type = 0;        /* STT_* */
    quint8 bind = 0;        /* STB_* */
};

class ElfFile
{
public:
    bool load(const QString &path, QString &error);

    bool isValid() const { return valid_; }

    quint16 machine() const { return machine_; }
    quint32 entryPoint() const { return entryPoint_; }

    const QVector<ElfSegment> &segments() const { return segments_; }
    const QVector<ElfSymbol> &symbols() const { return symbols_; }

    /** Look a symbol up by name; returns false when it is not present */
    bool findSymbol(const QString &name, ElfSymbol &symbol) const;

    /** True when the file carries a symbol table */
    bool hasSymbols() const { return !symbols_.isEmpty(); }

    /** Read @p size bytes at load address @p address (0 when out of range) */
    QByteArray readAt(quint32 address, int size) const;

private:
    QByteArray data_;
    bool valid_ = false;
    quint16 machine_ = 0;
    quint32 entryPoint_ = 0;
    QVector<ElfSegment> segments_;
    QVector<ElfSymbol> symbols_;
};

#endif /* ELFFILE_H */
