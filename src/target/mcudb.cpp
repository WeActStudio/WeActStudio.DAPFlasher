/*
 * WeAct Studio DAP Flasher
 *
 * Device id table and the read that uses it.
 *
 * The table itself lives outside the program, in mcu-ids.txt next to the
 * executable (see docs/mcu-identification.md). It is a plain text file so a
 * new part can be added without a rebuild; when it is missing the program
 * writes the built-in copy of it out on first use, so a fresh install still
 * identifies the common parts.
 *
 * The values are the DBGMCU_IDCODE DEV_ID fields from the ST reference
 * manuals (RM0433 for the H7, the per-family manuals for the rest). They
 * identify a sub-family, not a package or a pin count - a STM32H743VIT6 and a
 * STM32H743ZIT6 both answer 0x450, and nothing in the chip tells them apart.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mcudb.h"

#include "dap/cmsisdap.h"

#include <QCoreApplication>
#include <QFile>
#include <QVector>

namespace {

struct Row
{
    quint32 address = 0;
    quint32 mask = 0xFFFu;
    quint32 value = 0;
    QString vendor;
    QString device;
};

/* Two addresses, because ST moved the register: every STM32 up to the H7 has
 * DBGMCU_IDCODE at 0xE0042000, the later H7 parts put it in the D3 domain at
 * 0x5C001000. A part that does not own the address faults the read, which
 * readMem32() clears, so probing both costs one round trip. */
/** Fallback used when there is no mcu-ids.txt next to the program.
 *
 * Only the parts that turn up most often: the full table ships as that file
 * (and in docs/mcu-identification.md), so a missing file is a degraded mode,
 * not an empty one. */
const char *const kDefaultTable = R"(# Built-in fallback table (no mcu-ids.txt next to the program)
0x5C001000 | 0x450 | STMicroelectronics | STM32H742/743/750/753
0x5C001000 | 0x480 | STMicroelectronics | STM32H7A3/7B3
0x5C001000 | 0x483 | STMicroelectronics | STM32H723/725/730/733/735
0xE0042000 | 0x410 | STMicroelectronics | STM32F10x medium density
0xE0042000 | 0x414 | STMicroelectronics | STM32F10x high density
0xE0042000 | 0x418 | STMicroelectronics | STM32F105/107
0xE0042000 | 0x411 | STMicroelectronics | STM32F2xx
0xE0042000 | 0x413 | STMicroelectronics | STM32F405/407/415/417
0xE0042000 | 0x419 | STMicroelectronics | STM32F42x/43x
0xE0042000 | 0x423 | STMicroelectronics | STM32F401xB/C
0xE0042000 | 0x433 | STMicroelectronics | STM32F401xD/E
0xE0042000 | 0x431 | STMicroelectronics | STM32F411
0xE0042000 | 0x421 | STMicroelectronics | STM32F446
0xE0042000 | 0x440 | STMicroelectronics | STM32F030x8/F05x
0xE0042000 | 0x442 | STMicroelectronics | STM32F030xC/F09x
0xE0042000 | 0x448 | STMicroelectronics | STM32F070xB/F071/F072
0xE0042000 | 0x449 | STMicroelectronics | STM32F74x/75x
0xE0042000 | 0x451 | STMicroelectronics | STM32F76x/77x
0xE0042000 | 0x415 | STMicroelectronics | STM32L47x/48x
0xE0042000 | 0x435 | STMicroelectronics | STM32L43x/44x
0xE0042000 | 0x461 | STMicroelectronics | STM32L496/4A6
0xE0042000 | 0x460 | STMicroelectronics | STM32G0x1
0xE0042000 | 0x468 | STMicroelectronics | STM32G431/441
0xE0042000 | 0x469 | STMicroelectronics | STM32G47x/48x
#
# --- Nordic: FICR.INFO.PART, 20 bit part number --------------------------
0x10000100 | 0x52840 | Nordic Semiconductor | nRF52840 | 0x1FFFF
0x10000100 | 0x52832 | Nordic Semiconductor | nRF52832 | 0x1FFFF
0x10000100 | 0x52833 | Nordic Semiconductor | nRF52833 | 0x1FFFF
0x10000100 | 0x52820 | Nordic Semiconductor | nRF52820 | 0x1FFFF
#
# --- Artery: DEBUG_IDCODE, 0x7005xxxx = AT32F403A/407 --------------------
0xE0042000 | 0x70050000 | Artery | AT32F403A/407 | 0xFFFF0000
)";

QString databasePath()
{
    /* For trying a table without touching the installed one */
    const QByteArray override = qgetenv("DAPFLASHER_MCU_DB");
    if (!override.isEmpty())
        return QString::fromLocal8Bit(override);

    return QCoreApplication::applicationDirPath() + QStringLiteral("/mcu-ids.txt");
}

QVector<Row> parse(const QString &text)
{
    QVector<Row> rows;

    const QStringList lines = text.split(QLatin1Char('\n'));
    for (const QString &raw : lines) {
        const QString line = raw.trimmed();
        if (line.isEmpty() || line.startsWith(QLatin1Char('#')))
            continue;

        const QStringList fields = line.split(QLatin1Char('|'));
        if (fields.size() < 4)
            continue;

        bool addressOk = false;
        bool valueOk = false;
        const quint32 address = fields.at(0).trimmed().toUInt(&addressOk, 0);
        const quint32 value = fields.at(1).trimmed().toUInt(&valueOk, 0);

        if (!addressOk || !valueOk || value == 0)
            continue;

        /* Optional fifth field: the mask the value has to be compared with.
         * ST's device id is 12 bits wide, Nordic's part number 20, Artery's
         * code fills the whole register - so the width belongs in the table
         * rather than in the code. */
        quint32 mask = 0xFFFu;
        if (fields.size() > 4 && !fields.at(4).trimmed().isEmpty()) {
            bool maskOk = false;
            const quint32 parsed = fields.at(4).trimmed().toUInt(&maskOk, 0);
            if (maskOk && parsed != 0)
                mask = parsed;
        }

        Row row;
        row.address = address;
        row.mask = mask;
        row.value = value & mask;
        row.vendor = fields.at(2).trimmed();
        row.device = fields.at(3).trimmed();
        if (row.device.isEmpty())
            continue;

        rows.append(row);
    }

    return rows;
}

/** The table of the moment: the file when there is one, the built-in few
 *  otherwise. Read on every identify so an edit needs no restart, and nothing
 *  is written back - a missing file is simply the fallback case. */
QVector<Row> table()
{
    QFile file(databasePath());
    if (file.open(QIODevice::ReadOnly)) {
        const QVector<Row> rows = parse(QString::fromUtf8(file.readAll()));
        if (!rows.isEmpty())
            return rows;
    }

    return parse(QString::fromUtf8(kDefaultTable));
}

bool readIdCode(CmsisDap *link, quint32 address, quint32 &idcode)
{
    QString ignored;
    if (!link->readMem32(address, &idcode, 1, ignored))
        return false;

    /* An address that nothing answers comes back as zero or all ones; both are
     * "this part has no register here", not a device id. */
    return idcode != 0x00000000u && idcode != 0xFFFFFFFFu;
}

} /* namespace */

McuIdentity identifyMcu(CmsisDap *link)
{
    McuIdentity identity;

    if (!link || !link->isOpen())
        return identity;

    const QVector<Row> rows = table();
    if (rows.isEmpty())
        return identity;

    /* Every distinct address once, in the order the table lists them */
    QVector<quint32> addresses;
    for (const Row &row : rows) {
        if (!addresses.contains(row.address))
            addresses.append(row.address);
    }

    for (quint32 address : addresses) {
        quint32 idcode = 0;
        if (!readIdCode(link, address, idcode))
            continue;

        const quint16 devId = static_cast<quint16>(idcode & 0xFFFu);

        for (const Row &row : rows) {
            if (row.address != address)
                continue;
            if ((idcode & row.mask) != row.value)
                continue;

            identity.known = true;
            identity.vendor = row.vendor;
            identity.device = row.device;
            identity.idAddress = address;
            identity.deviceId = row.value;
            identity.rawIdCode = idcode;
            identity.revision = static_cast<quint16>(idcode >> 16);
            return identity;
        }

        /* The register answered but the id is not in the table. Keep what was
         * read - it is the one number that makes the part searchable - and try
         * the remaining addresses. */
        if (identity.idAddress == 0) {
            identity.idAddress = address;
            identity.deviceId = devId;
            identity.rawIdCode = idcode;
            identity.revision = static_cast<quint16>(idcode >> 16);
        }
    }

    return identity;
}

QString describeDpIdCode(quint32 idcode)
{
    if (idcode == 0)
        return QString();

    const quint16 designer = static_cast<quint16>((idcode >> 1) & 0x7FFu);
    const quint8 part = static_cast<quint8>((idcode >> 12) & 0xFFu);

    QString kind = QStringLiteral("SW-DP");
    if (part == 0x01)
        kind = QStringLiteral("SW-DP v1");
    else if (part == 0x02)
        kind = QStringLiteral("SW-DP v2");

    QString text = kind;
    if (designer == 0x23B)
        text += QStringLiteral(", ARM");
    else
        text += QStringLiteral(", designer 0x%1").arg(
            QString::number(designer, 16).toUpper().rightJustified(3, QLatin1Char('0')));

    text += QStringLiteral(", part 0x%1").arg(
        QString::number(part, 16).toUpper().rightJustified(2, QLatin1Char('0')));

    return text;
}
