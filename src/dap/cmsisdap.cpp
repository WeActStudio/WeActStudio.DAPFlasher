/*
 * WeAct Studio DAP Flasher
 *
 * CMSIS-DAP link layer: probe commands, SWD transfers and memory access.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "cmsisdap.h"

#include "dapdefs.h"
#include "daptransport.h"
#include "probes.h"

#include <QCoreApplication>
#include <QThread>

#include <cstring>

namespace {

/* Response timeout of one command, in milliseconds */
const int kCommandTimeoutMs = 2000;

/* AHB-AP control/status word: 32 bit access, single address increment,
 * privileged and non secure access - the combination every CMSIS-DAP host
 * uses for the AHB-AP of a Cortex-M. */
const quint32 kApCsw = 0x23000052u;

/* The same word with AddrInc set to "auto-increment off" (bits[5:4] = 0b00).
 *
 * The AP advances TAR itself after every data register access when AddrInc is
 * set, which is what makes bulk transfers possible and what makes a poll loop
 * impossible to optimise: a loop that always reads the same register would
 * find the AP pointing one word further on after each read and never match its
 * own cache. Turning the increment off for the duration of the poll pins TAR
 * to the register under test, so the TAR write happens once and every later
 * poll is a single transfer. The block paths write kApCsw back, so the switch
 * only ever lasts as long as a poll loop. */
const quint32 kApCswFixed = 0x23000042u;

/* DP registers. Address 0x00 is IDCODE when read and the ABORT register when
 * written. */
const quint8 kDpAbort = 0x00;
const quint8 kDpCtrlStat = 0x04;
const quint8 kDpSelect = 0x08;
const quint8 kDpRdBuff = 0x0C;

/* AP registers (bank 0 of the MEM-AP) */
const quint8 kApCswReg = 0x00;
const quint8 kApTarReg = 0x04;
const quint8 kApDrwReg = 0x0C;

/* DP ABORT bits: write a one to clear the matching sticky flag.
 * Spelled as pyocd spells them in coresight/dap.py. */
const quint32 kAbortStkCmpClr = 0x00000002u;
const quint32 kAbortStkErrClr = 0x00000004u;
const quint32 kAbortWDataErrClr = 0x00000008u;
const quint32 kAbortOrunErrClr = 0x00000010u;
const quint32 kAbortClearAll = kAbortOrunErrClr | kAbortWDataErrClr
                               | kAbortStkErrClr | kAbortStkCmpClr;

/* DP CTRL/STAT. The same four constants exist in pyocd's coresight/dap.py,
 * and the power handling below is a direct transcription of its
 * DAP.power_up_debug() and DAP.clear_sticky_err(). */
const quint32 kDpCDbgPowerUpReq = 0x10000000u;   /* bit 28 */
const quint32 kDpCDbgPowerUpAck = 0x20000000u;   /* bit 29 */
const quint32 kDpCSysPowerUpReq = 0x40000000u;   /* bit 30 */
const quint32 kDpCSysPowerUpAck = 0x80000000u;   /* bit 31 */
const quint32 kDpMaskLane = 0x00000F00u;
const quint32 kDpTrnNormal = 0x00000000u;

const quint32 kPowerUpRequest = kDpCSysPowerUpReq | kDpCDbgPowerUpReq;
const quint32 kPowerUpAck = kDpCSysPowerUpAck | kDpCDbgPowerUpAck;

/* The exact word pyocd writes when it powers the two domains up. It always
 * carries both request bits, which is what keeps a power up write from ever
 * turning into a power down one. */
const quint32 kPowerUpWrite = kPowerUpRequest | kDpMaskLane | kDpTrnNormal;

/* pyocd's DP_POWER_REQUEST_TIMEOUT. The domains come up in microseconds, so
 * the budget only comes into play on a target that never answers. */
const int kPowerUpTimeoutMs = 5000;
const int kPowerUpPollIntervalMs = 2;

quint32 readLe32(const QByteArray &data, int offset)
{
    const uchar *p = reinterpret_cast<const uchar *>(data.constData()) + offset;
    return static_cast<quint32>(p[0])
           | (static_cast<quint32>(p[1]) << 8)
           | (static_cast<quint32>(p[2]) << 16)
           | (static_cast<quint32>(p[3]) << 24);
}

void appendLe32(QByteArray &data, quint32 value)
{
    data.append(static_cast<char>(value & 0xFF));
    data.append(static_cast<char>((value >> 8) & 0xFF));
    data.append(static_cast<char>((value >> 16) & 0xFF));
    data.append(static_cast<char>((value >> 24) & 0xFF));
}

} /* namespace */

CmsisDap::CmsisDap()
    : transport_(nullptr)
    , connected_(false)
    , capabilities_(0)
    , cachedCsw_(0)
    , cachedTar_(0)
    , tarKnown_(false)
    , cachedSelect_(0)
    , selectKnown_(false)
    , retries_(0)
{
}

CmsisDap::~CmsisDap()
{
    close();
}

/* ==========================================================================
 * Connection
 * ========================================================================== */

bool CmsisDap::open(const DapProbeInfo &probe, QString &error)
{
    close();

    transport_ = DapTransport::create(probe);
    if (!transport_->open(probe, error)) {
        delete transport_;
        transport_ = nullptr;
        return false;
    }

    /* The probe reports the packet size it accepts (64 for v1 HID, 1024 for
     * v2 bulk, 4096 on the measured WeAct probe). DAP_Info(0xFF) is a 16 bit
     * little endian value; probes that send only its low byte are handled
     * too. Each transport clamps the value to what it can actually carry. */
    QByteArray packetSize;
    QString sizeError;
    if (queryInfo(DapInfoId::PacketSize, packetSize, sizeError) && !packetSize.isEmpty()) {
        int size = static_cast<quint8>(packetSize.at(0));
        if (packetSize.size() >= 2)
            size |= static_cast<int>(static_cast<quint8>(packetSize.at(1))) << 8;

        transport_->setPacketSize(size);
    }

    capabilities_ = infoU32(DapInfoId::Capabilities);
    return true;
}

void CmsisDap::close()
{
    if (transport_) {
        if (connected_) {
            QString ignored;
            disconnect(ignored);
        }
        transport_->close();
        delete transport_;
        transport_ = nullptr;
    }

    connected_ = false;
    capabilities_ = 0;
    cachedCsw_ = 0;
    cachedTar_ = 0;
    tarKnown_ = false;
    cachedSelect_ = 0;
    selectKnown_ = false;
}

bool CmsisDap::isOpen() const
{
    return transport_ && transport_->isOpen();
}

QString CmsisDap::transportName() const
{
    return transport_ ? transport_->name() : QString();
}

/* ==========================================================================
 * Low level command exchange
 * ========================================================================== */

bool CmsisDap::exchange(const QByteArray &command, QByteArray &response, QString &error)
{
    if (!isOpen()) {
        error = QCoreApplication::translate("CmsisDap", "No probe is open.");
        return false;
    }

    if (!transport_->sendCommand(command, error))
        return false;

    if (!transport_->receiveResponse(response, kCommandTimeoutMs, error))
        return false;

    if (response.isEmpty()) {
        error = QCoreApplication::translate("CmsisDap", "The probe sent an empty answer.");
        return false;
    }

    return true;
}

bool CmsisDap::sendCommand(const QByteArray &command, QString &error)
{
    QByteArray response;
    return exchange(command, response, error);
}

/* ==========================================================================
 * Probe information
 * ========================================================================== */

bool CmsisDap::queryInfo(quint8 id, QByteArray &value, QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::Info));
    command.append(static_cast<char>(id));

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 2 || static_cast<quint8>(response.at(0)) != DapCmd::Info) {
        error = QCoreApplication::translate("CmsisDap", "The probe answered with a malformed DAP_Info response.");
        return false;
    }

    const int length = static_cast<quint8>(response.at(1));
    if (length == 0) {
        value.clear();
        return true;
    }

    if (response.size() < 2 + length) {
        error = QCoreApplication::translate("CmsisDap", "The probe sent a truncated DAP_Info response.");
        return false;
    }

    value = response.mid(2, length);
    return true;
}

QString CmsisDap::infoString(quint8 id)
{
    QByteArray value;
    QString error;
    if (!queryInfo(id, value, error) || value.isEmpty())
        return QString();

    /* Trim trailing NUL / whitespace the probes like to append */
    while (!value.isEmpty() && (value.endsWith('\0') || value.endsWith(' ')))
        value.chop(1);

    return QString::fromLatin1(value);
}

quint32 CmsisDap::infoU32(quint8 id)
{
    QByteArray value;
    QString error;
    if (!queryInfo(id, value, error) || value.isEmpty() || value.size() > 4)
        return 0;

    /* A probe may answer with fewer than the four bytes the specification
     * asks for: the WeAct probe reports its capabilities with two. Zero
     * extend instead of giving up, a short answer still carries the low
     * bits. */
    while (value.size() < 4)
        value.append('\0');

    return readLe32(value, 0);
}

/* ==========================================================================
 * DAP commands
 * ========================================================================== */

bool CmsisDap::connectSwd(QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::Connect));
    command.append(static_cast<char>(DapPort::Swd));

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 2 || static_cast<quint8>(response.at(0)) != DapCmd::Connect) {
        error = QCoreApplication::translate("CmsisDap", "The probe answered with a malformed DAP_Connect response.");
        return false;
    }

    const quint8 port = static_cast<quint8>(response.at(1));
    if (port == 0) {
        error = QCoreApplication::translate("CmsisDap", "The probe refused the SWD connection.");
        return false;
    }

    connected_ = true;
    cachedCsw_ = 0;
    cachedTar_ = 0;
    tarKnown_ = false;
    cachedSelect_ = 0;
    /* DAP_Connect restarts the SWD protocol, but it cannot clear DP SELECT:
     * that register is part of the target, and only a host write to the DP
     * reaches it. Whatever the previous session selected therefore survives,
     * so the selection has to be treated as unknown until it is written. */
    selectKnown_ = false;

    /* The two steps pyocd's connect sequence runs after the probe is
     * connected, in the same order: clear the DP's sticky error flags, then
     * request the power domains. The clear has to come first because a
     * leftover flag - WDATAERR from an aborted write of the previous session,
     * say - otherwise sits next to the power up write and is reported as a
     * failure of it.
     *
     * Both are best effort: neither result is fatal on its own, because a DP
     * that is not answering yet is reported by the IDCODE read that follows
     * and the attach ladder still gets its chance. */
    QString clearError;
    clearStickyErrors(clearError);

    QString powerError;
    powerUpDebug(powerError);

    /* Put the AP back on bank 0 while the link is fresh. A leftover APBANKSEL
     * would send every TAR write into a banked alias, and the failures that
     * causes are invisible: the DP keeps answering, so only a stale CPUID
     * gives the problem away. */
    QString selectError;
    selectApBank(0, 0, selectError);

    return true;
}

bool CmsisDap::powerUpDebug(QString &error)
{
    /* The write pyocd's DAP.power_up_debug() sends, unchanged: both request
     * bits plus MASKLANE and TRNNORMAL as one constant.
     *
     * Because the constant always carries CDBGPWRUPREQ and CSYSPWRUPREQ,
     * this call can never perform the one transition ADIv6 forbids - clearing
     * both of them at once. That transition switches off the DP that would
     * have to switch the domains back on, and the target then answers WAIT to
     * every write until something resets it. Measured on this board: with the
     * two bits cleared in a single write, CTRL/STAT still read back but every
     * write to the DP - CTRL/STAT and SELECT included - came back WAIT, and
     * only a full line reset afterwards brought it back. */
    if (!swdWriteDp(kDpCtrlStat, kPowerUpWrite, error))
        return false;

    /* Wait for both acknowledges, with pyocd's five second budget. The
     * domains come up in microseconds, so the loop normally ends on its
     * first pass; the budget is there for a target that never answers at
     * all. */
    const int polls = kPowerUpTimeoutMs / kPowerUpPollIntervalMs;
    for (int poll = 0; poll < polls; ++poll) {
        quint32 ctrlStat = 0;
        if (!swdReadDp(kDpCtrlStat, ctrlStat, error))
            return false;

        if ((ctrlStat & kPowerUpAck) == kPowerUpAck)
            return true;

        QThread::msleep(kPowerUpPollIntervalMs);
    }

    error = QCoreApplication::translate("CmsisDap",
                "The DP did not acknowledge the power up request within %1 ms.")
                .arg(kPowerUpTimeoutMs);
    return false;
}

bool CmsisDap::selectApBank(quint8 apsel, quint8 bank, QString &error)
{
    /* SELECT = APSEL[7:0] | APBANKSEL[11:8], the register the DP uses to route
     * every following AP access. */
    const quint8 select = static_cast<quint8>((apsel & 0xFFu) | ((bank & 0xFu) << 4));

    if (selectKnown_ && cachedSelect_ == select)
        return true;

    if (!swdWriteDp(kDpSelect, select, error))
        return false;

    cachedSelect_ = select;
    selectKnown_ = true;
    return true;
}

bool CmsisDap::disconnect(QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::Disconnect));

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    connected_ = false;
    return true;
}

bool CmsisDap::setClock(quint32 hz, QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::SwjClock));
    appendLe32(command, hz);

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 2 || static_cast<quint8>(response.at(1)) != DapStatus::Ok) {
        error = QCoreApplication::translate("CmsisDap", "Setting the SWD clock failed.");
        return false;
    }

    return true;
}

bool CmsisDap::configureTransfer(quint8 idleCycles, quint16 waitRetry, quint16 matchRetry, QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::TransferConfigure));
    command.append(static_cast<char>(0));            /* DAP index */
    command.append(static_cast<char>(idleCycles));
    command.append(static_cast<char>(waitRetry & 0xFF));
    command.append(static_cast<char>((waitRetry >> 8) & 0xFF));
    command.append(static_cast<char>(matchRetry & 0xFF));
    command.append(static_cast<char>((matchRetry >> 8) & 0xFF));

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 2 || static_cast<quint8>(response.at(1)) != DapStatus::Ok) {
        error = QCoreApplication::translate("CmsisDap", "Configuring the DAP transfer failed.");
        return false;
    }

    return true;
}

bool CmsisDap::setPins(quint8 value, quint8 select, quint32 waitUs, quint8 *pinState, QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::SwjPins));
    command.append(static_cast<char>(value));
    command.append(static_cast<char>(select));
    appendLe32(command, waitUs);

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 2 || static_cast<quint8>(response.at(0)) != DapCmd::SwjPins) {
        error = QCoreApplication::translate("CmsisDap", "The probe answered with a malformed DAP_SWJ_Pins response.");
        return false;
    }

    if (pinState)
        *pinState = static_cast<quint8>(response.at(1));

    return true;
}

bool CmsisDap::resetTarget(bool *executedByProbe, QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::ResetTarget));

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 3 || static_cast<quint8>(response.at(0)) != DapCmd::ResetTarget) {
        error = QCoreApplication::translate("CmsisDap", "The probe answered with a malformed DAP_Reset_Target response.");
        return false;
    }

    if (executedByProbe)
        *executedByProbe = (static_cast<quint8>(response.at(2)) != 0);

    return true;
}

bool CmsisDap::driveResetPin(bool asserted, quint32 waitUs, bool *followed, QString &error)
{
    /* Only the nRESET bit is selected: the pins that are not selected keep
     * whatever the probe had them at, so an SWCLK/SWDIO state that was set up
     * earlier is not disturbed by a reset pulse. */
    const quint8 select = DapPin::NReset;
    const quint8 value = asserted ? 0x00 : DapPin::NReset;

    quint8 pinState = 0;
    if (!setPins(value, select, waitUs, &pinState, error))
        return false;

    if (followed)
        *followed = asserted ? ((pinState & DapPin::NReset) == 0)
                             : ((pinState & DapPin::NReset) != 0);

    return true;
}

bool CmsisDap::pulseResetPin(quint32 holdMs, quint32 settleMs, bool *followed, QString &error)
{
    bool lowFollowed = false;
    if (!driveResetPin(true, holdMs * 1000u, &lowFollowed, error))
        return false;

    bool highFollowed = false;
    if (!driveResetPin(false, settleMs * 1000u, &highFollowed, error))
        return false;

    if (followed)
        *followed = lowFollowed && highFollowed;

    return true;
}

bool CmsisDap::pulseTargetReset(bool *performed, QString &error)
{
    if (performed)
        *performed = false;

    /* DAP_SWJ_Pins first. It is the older and far more widely implemented of
     * the two commands, and it is what every mainstream debugger uses: this
     * probe answers DAP_Reset_Target with "executed = 0" while its nRESET bit
     * follows DAP_SWJ_Pins perfectly. */
    bool followed = false;
    QString pinError;
    if (pulseResetPin(20, 50, &followed, pinError)) {
        if (performed)
            *performed = followed;
        return true;
    }

    /* No DAP_SWJ_Pins on this probe: fall back to its own reset command. */
    error.clear();
    bool executed = false;
    if (!resetTarget(&executed, error))
        return false;

    if (performed)
        *performed = executed;
    return true;
}

bool CmsisDap::swjSequence(quint16 bitCount, const QByteArray &data, QString &error)
{
    QByteArray command;
    command.append(static_cast<char>(DapCmd::SwjSequence));

    /* DAP_SWJ_Sequence carries the sequence length in ONE byte, and a zero
     * means 256. Writing a second length byte - the natural thing to do for a
     * 16 bit count - does not add a byte, it shifts the whole data stream by
     * one: a 56 clock line reset then goes out as 8 low clocks followed by
     * only 48 high ones, which is two clocks short of the 50 that a SWD line
     * reset needs, and the leftover byte is parsed by the probe as one more
     * command. Links that the probe itself builds through DAP_Connect do not
     * notice, because the line reset happens inside the probe; the fault only
     * bites where the host has to rebuild the link by hand, which is exactly
     * what a recovery after a target reset does. */
    command.append(static_cast<char>(bitCount == 256u ? 0u : (bitCount & 0xFFu)));
    command.append(data);

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 2 || static_cast<quint8>(response.at(1)) != DapStatus::Ok) {
        error = QCoreApplication::translate("CmsisDap", "The SWJ sequence failed.");
        return false;
    }

    return true;
}

/* ==========================================================================
 * SWD
 * ========================================================================== */

QString CmsisDap::clockText(quint32 hz)
{
    /* The unit is the same in every language, so only the message around it
     * is translated. */
    if (hz >= 1000000u && hz % 1000000u == 0)
        return QStringLiteral("%1 MHz").arg(hz / 1000000u);
    if (hz >= 1000u && hz % 1000u == 0)
        return QStringLiteral("%1 kHz").arg(hz / 1000u);

    return QStringLiteral("%1 Hz").arg(hz);
}

bool CmsisDap::attachSwd(quint32 clockHz, quint32 &dpIdCode, const DapAttachLog &log,
                         QString &error)
{
    dpIdCode = 0;

    if (!connectSwd(error))
        return false;

    /* Which speed the session runs at is the one thing the log never said,
     * and it is what tells a window that fails apart from a command line that
     * works: both use the same code, only the clock differs. */
    if (log)
        log(QCoreApplication::translate("CmsisDap", "SWD clock: %1").arg(clockText(clockHz)), false);

    if (!setClock(clockHz, error)) {
        /* A probe that does not take a clock simply keeps its own */
        if (log)
            log(QCoreApplication::translate("CmsisDap", "The SWD clock could not be set: %1").arg(error), true);
        error.clear();
    }

    QString ignored;
    /* wait_retry is how many times the probe itself retries a SWD access that
     * came back WAIT before it gives the host that answer. The usual 100 is
     * tuned for a target that is already stopped; a target that is still
     * running its own program answers WAIT for as long as it holds the bus,
     * and 100 retries at 1 MHz is well under a millisecond of target time.
     * The failure that reaches the user then is "the SWD transfer was retried
     * too often", which says nothing and retries nothing useful. 5000 retries
     * is still only tens of milliseconds, and it is the difference between a
     * sporadic failure and no failure at all. */
    configureTransfer(0, 5000, 0, ignored);

    /* First attempt: plain reads, without touching the interface at all. A
     * target that answers is read with no side effect whatsoever. */
    QString lastError;
    for (int attempt = 0; attempt < 3; ++attempt) {
        if (readDpIdCode(dpIdCode, lastError))
            return true;
    }

    if (log)
        log(QCoreApplication::translate("CmsisDap", "The SWD DP did not answer, trying to revive the link."), true);

    /* The steps every debugger runs before its first read, most gentle one
     * first. A line reset alone is usually enough; some targets only wake up
     * on a reset; and a link that was trained at a high clock sometimes only
     * comes up slowly. */
    struct Step
    {
        bool lineReset;
        bool targetReset;
        bool reconnect;
        quint32 clockHz;
    };

    const Step ladder[] = {
        { true,  false, false, clockHz },
        { true,  false, true,  clockHz },
        { true,  true,  true,  clockHz },
        { true,  true,  true,  100000u },
    };

    for (const Step &step : ladder) {
        QString stepError;

        if (step.clockHz != clockHz)
            setClock(step.clockHz, stepError);

        if (step.reconnect) {
            /* DAP_Connect is what makes the probe drive the SWD interface
             * again after the pins were used for a raw sequence */
            disconnect(stepError);
            stepError.clear();
            if (!connectSwd(stepError))
                continue;
        }

        if (step.lineReset) {
            if (!swdLineReset(stepError)) {
                if (log)
                    log(stepError, true);
                stepError.clear();
                continue;
            }
        }

        if (step.targetReset) {
            bool executed = false;
            if (pulseTargetReset(&executed, stepError) && executed) {
                /* Give the core the time to come out of reset */
                QThread::msleep(50);
            }
            stepError.clear();
        }

        for (int attempt = 0; attempt < 3; ++attempt) {
            if (readDpIdCode(dpIdCode, lastError)) {
                if (log) {
                    log(QCoreApplication::translate("CmsisDap", "The SWD link came back at %1 Hz.")
                            .arg(step.clockHz), false);
                }
                return true;
            }
        }
    }

    /* Last resort, and the only way in once the target program has really
     * taken the debug pins over: build the link with nRESET held low, so the
     * program never gets to run, then let the reset go and read the DP at
     * once - a Cortex-M answers within a few clocks of the release, long
     * before its startup code can touch the pins. */
    {
        QString underResetError;
        if (attachUnderReset(clockHz, dpIdCode, log, DapAttachTouch(), underResetError))
            return true;
    }

    error = QCoreApplication::translate("CmsisDap",
                "The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), "
                "the target power supply and the SWD clock. A target program that "
                "switched the debug interface off, or that put the core into a low "
                "power state, only comes back on a power cycle. If it comes back "
                "with BOOT0 held high, that program is what takes the debug pins "
                "over: start the ROM bootloader that way, which leaves the debug "
                "pins alone, and erase the chip from there.");

    /* Keep what the probe actually reported: "protocol error" means the SWD
     * line gave no valid ACK (nothing is driving it, or the target program
     * took the debug pins over), which is a very different fault from a
     * target that answers with a fault. */
    if (!lastError.isEmpty())
        error += QLatin1String(" [") + lastError + QLatin1Char(']');

    return false;
}

bool CmsisDap::attachUnderReset(quint32 clockHz, quint32 &dpIdCode,
                                const DapAttachLog &log, const DapAttachTouch &touch,
                                QString &error)
{
    dpIdCode = 0;

    /* The window is only opened by a reset this object drives itself, so a
     * probe without an nRESET line cannot do this at all. */
    bool followed = false;
    QString pinError;

    if (!driveResetPin(true, 1000, &followed, pinError) || !followed) {
        error = pinError.isEmpty()
                    ? QCoreApplication::translate("CmsisDap",
                              "This probe does not drive an nRESET line.")
                    : pinError;
        return false;
    }

    if (log) {
        log(QCoreApplication::translate("CmsisDap",
                "Holding nRESET low and connecting under reset."), true);
    }

    /* Everything that is not time critical happens while the core is still
     * held: only the release and the first access afterwards have to fit into
     * the window before the target's startup code runs. */
    const quint32 clocks[] = { clockHz, 100000u };
    QString lastError;

    for (quint32 hz : clocks) {
        QString stepError;

        if (!connectSwd(stepError))
            continue;

        setClock(hz, stepError);

        for (int attempt = 0; attempt < 3; ++attempt) {
            /* Back into reset, then out of it and straight for the DP. The
             * line reset in between is deliberate: a DP that was reset wants
             * a fresh start of the SWD protocol state. */
            driveResetPin(true, 1000, &followed, stepError);
            swdLineReset(stepError);
            driveResetPin(false, 0, &followed, stepError);

            if (!readDpIdCode(dpIdCode, lastError))
                continue;

            /* The DP is up and the core has not started yet. This is the one
             * moment at which the caller can still stop it. */
            if (touch && !touch(stepError))
                continue;

            if (log) {
                log(QCoreApplication::translate("CmsisDap",
                        "The SWD link came back under reset at %1 Hz.")
                        .arg(clockText(hz)), false);
            }

            return true;
        }
    }

    /* Whatever happened, never leave the target held in reset. */
    QString ignored;
    driveResetPin(false, 0, &followed, ignored);

    if (log) {
        log(QCoreApplication::translate("CmsisDap",
                "Connecting under reset did not bring the DP back either."), true);
    }

    error = lastError.isEmpty()
                ? QCoreApplication::translate("CmsisDap",
                          "The SWD DP did not answer while nRESET was being driven.")
                : lastError;
    return false;
}

bool CmsisDap::readDpIdCode(quint32 &dpIdCode, QString &error)
{
    quint32 value = 0;
    if (!swdReadDp(0x00, value, error))
        return false;

    /* All ones or all zeros is what a floating read line produces */
    if (value == 0 || value == 0xFFFFFFFFu) {
        error = QCoreApplication::translate("CmsisDap", "The SWD DP reported an invalid IDCODE.");
        return false;
    }

    dpIdCode = value;
    return true;
}

bool CmsisDap::swdLineReset(QString &error)
{
    /* Line reset: at least 50 clock cycles with SWDIO high */
    const QByteArray high(7, static_cast<char>(0xFF));

    if (!swjSequence(56, high, error))
        return false;

    /* JTAG to SWD switch sequence (MSB first, 0xE79E) */
    QByteArray switchSeq;
    switchSeq.append(static_cast<char>(0x9E));
    switchSeq.append(static_cast<char>(0xE7));
    if (!swjSequence(16, switchSeq, error))
        return false;

    /* Line reset again, then 8 idle cycles with SWDIO low */
    if (!swjSequence(56, high, error))
        return false;

    const QByteArray idle(1, static_cast<char>(0x00));
    return swjSequence(8, idle, error);
}

bool CmsisDap::swdReadDp(quint8 reg, quint32 &value, QString &error)
{
    const QVector<quint8> requests(1, static_cast<quint8>((reg & 0x0C) | DapTransferRequest::Read));
    QVector<quint32> readValues;
    quint8 response = 0;

    if (!transfer(requests, QVector<quint32>(), readValues, &response, error))
        return false;

    if (readValues.isEmpty()) {
        error = QCoreApplication::translate("CmsisDap", "The DP read returned no data.");
        return false;
    }

    value = readValues.first();
    return true;
}

bool CmsisDap::swdWriteDp(quint8 reg, quint32 value, QString &error)
{
    const QVector<quint8> requests(1, static_cast<quint8>(reg & 0x0C));
    QVector<quint32> writeValues(1, value);
    QVector<quint32> readValues;
    quint8 response = 0;

    return transfer(requests, writeValues, readValues, &response, error);
}

bool CmsisDap::swdReadAp(quint8 reg, quint32 &value, QString &error)
{
    const QVector<quint8> requests(
        1, static_cast<quint8>((reg & 0x0C) | DapTransferRequest::ApnDp | DapTransferRequest::Read));
    QVector<quint32> readValues;
    quint8 response = 0;

    if (!transfer(requests, QVector<quint32>(), readValues, &response, error))
        return false;

    if (readValues.isEmpty()) {
        error = QCoreApplication::translate("CmsisDap", "The AP read returned no data.");
        return false;
    }

    value = readValues.first();
    return true;
}

bool CmsisDap::swdWriteAp(quint8 reg, quint32 value, QString &error)
{
    const QVector<quint8> requests(
        1, static_cast<quint8>((reg & 0x0C) | DapTransferRequest::ApnDp));
    QVector<quint32> writeValues(1, value);
    QVector<quint32> readValues;
    quint8 response = 0;

    return transfer(requests, writeValues, readValues, &response, error);
}

bool CmsisDap::clearStickyErrors(QString &error)
{
    /* SWD clears the sticky flags through ABORT, and pyocd's
     * DAP.clear_sticky_err() does the same - it only reaches for CTRL/STAT
     * on JTAG. The reason is the power bits: CTRL/STAT also carries
     * CDBGPWRUPREQ and CSYSPWRUPREQ, so writing a computed "clear the
     * errors" value into it is one wrong mask away from switching a domain
     * off. ABORT has no such bit, so this cannot go wrong. */
    return swdWriteDp(kDpAbort, kAbortClearAll, error);
}

/* ==========================================================================
 * Transfers
 * ========================================================================== */

bool CmsisDap::transfer(const QVector<quint8> &requests,
                        const QVector<quint32> &writeValues,
                        QVector<quint32> &readValues,
                        quint8 *transferResponse,
                        QString &error)
{
    readValues.clear();

    if (requests.isEmpty()) {
        error = QCoreApplication::translate("CmsisDap", "Empty transfer.");
        return false;
    }

    const int readCount = [&requests]() {
        int n = 0;
        for (quint8 request : requests) {
            if (request & DapTransferRequest::Read)
                ++n;
        }
        return n;
    }();

    if (writeValues.size() != requests.size() - readCount) {
        error = QCoreApplication::translate("CmsisDap", "Internal error: the write value count does not match the request count.");
        return false;
    }

    for (int attempt = 0; attempt < 3; ++attempt) {
        QByteArray command;
        command.append(static_cast<char>(DapCmd::Transfer));
        command.append(static_cast<char>(0));                       /* DAP index */
        command.append(static_cast<char>(requests.size()));

        for (quint8 request : requests)
            command.append(static_cast<char>(request));

        for (quint32 value : writeValues)
            appendLe32(command, value);

        QByteArray response;
        if (!exchange(command, response, error))
            return false;

        if (response.size() < 3) {
            error = QCoreApplication::translate("CmsisDap", "The probe answered with a truncated transfer response.");
            return false;
        }

        /* The response starts with the command id, then the transfer count and
         * the transfer status:
         *     [0] DAP_Transfer  [1] count  [2] status  [3..] read values
         * Reading the count from byte 0 instead of byte 1 turns an idle probe
         * into "the probe performed 5 of 1 transfers", so check the id. */
        if (static_cast<quint8>(response.at(0)) != DapCmd::Transfer) {
            error = QCoreApplication::translate("CmsisDap", "The probe answered with a malformed DAP_Transfer response.");
            return false;
        }

        const int count = static_cast<quint8>(response.at(1));
        const quint8 status = static_cast<quint8>(response.at(2));

        if (transferResponse)
            *transferResponse = status;

        /* WAIT: the target asked to be retried */
        if ((status & DapTransferResult::Wait) && !(status & (DapTransferResult::Fault
                                                              | DapTransferResult::Error))) {
            continue;
        }

        if (status & DapTransferResult::Fault) {
            QString clearError;
            clearStickyErrors(clearError);
            lastTransferError_ = QCoreApplication::translate("CmsisDap", "SWD fault (the target did not acknowledge the access).");
            error = lastTransferError_;
            return false;
        }

        if (status & DapTransferResult::Error) {
            lastTransferError_ = QCoreApplication::translate("CmsisDap", "Protocol error during the SWD transfer.");
            error = lastTransferError_;
            return false;
        }

        if (count != requests.size()) {
            error = QCoreApplication::translate("CmsisDap", "The probe performed %1 of %2 transfers.")
                        .arg(count)
                        .arg(requests.size());
            return false;
        }

        int offset = 3;
        for (int i = 0; i < readCount; ++i) {
            if (response.size() < offset + 4) {
                error = QCoreApplication::translate("CmsisDap", "The probe sent a truncated transfer payload.");
                return false;
            }
            readValues.append(readLe32(response, offset));
            offset += 4;
        }

        return true;
    }

    error = QCoreApplication::translate("CmsisDap", "The SWD transfer was retried too often (WAIT response).");
    return false;
}

bool CmsisDap::transferBlock(quint8 request, quint16 count,
                             const QByteArray &writeData, QByteArray &readData,
                             quint8 *transferResponse, QString &error)
{
    readData.clear();

    if (count == 0) {
        error = QCoreApplication::translate("CmsisDap", "Empty block transfer.");
        return false;
    }

    QByteArray command;
    command.append(static_cast<char>(DapCmd::TransferBlock));
    command.append(static_cast<char>(0));                       /* DAP index */
    /* The order is fixed by the specification and it is easy to get wrong:
     *     DAP_TransferBlock, DAP Index, Transfer Count (16 bit, LE), Transfer
     *     Request, Transfer Data
     * The count comes *before* the request, unlike in DAP_Transfer where a
     * single byte count is followed by the requests. Swapping the two makes
     * the probe read the request byte as the low half of the count: a one
     * word block then asks for 0x010F transfers and is answered with a count
     * the caller never requested. */
    command.append(static_cast<char>(count & 0xFF));
    command.append(static_cast<char>((count >> 8) & 0xFF));
    command.append(static_cast<char>(request));

    if (!(request & DapTransferRequest::Read))
        command.append(writeData);

    QByteArray response;
    if (!exchange(command, response, error))
        return false;

    if (response.size() < 4) {
        error = QCoreApplication::translate("CmsisDap", "The probe answered with a truncated block transfer response.");
        return false;
    }

    /* [0] DAP_TransferBlock  [1..2] count (LE)  [3] status  [4..] read data */
    if (static_cast<quint8>(response.at(0)) != DapCmd::TransferBlock) {
        error = QCoreApplication::translate("CmsisDap", "The probe answered with a malformed DAP_TransferBlock response.");
        return false;
    }

    const int reported = static_cast<quint8>(response.at(1))
                         | (static_cast<quint8>(response.at(2)) << 8);
    const quint8 status = static_cast<quint8>(response.at(3));

    if (transferResponse)
        *transferResponse = status;

    if (status & DapTransferResult::Fault) {
        QString clearError;
        clearStickyErrors(clearError);
        lastTransferError_ = QCoreApplication::translate("CmsisDap", "SWD fault during the block transfer.");
        error = lastTransferError_;
        return false;
    }

    if (status & DapTransferResult::Error) {
        lastTransferError_ = QCoreApplication::translate("CmsisDap", "Protocol error during the block transfer.");
        error = lastTransferError_;
        return false;
    }

    if (reported != count) {
        error = QCoreApplication::translate("CmsisDap", "The probe performed %1 of %2 block transfers.")
                    .arg(reported)
                    .arg(count);
        return false;
    }

    if (request & DapTransferRequest::Read) {
        const int bytes = count * 4;
        if (response.size() < 4 + bytes) {
            error = QCoreApplication::translate("CmsisDap", "The probe sent a truncated block transfer payload.");
            return false;
        }
        readData = response.mid(4, bytes);
    }

    return true;
}

/* ==========================================================================
 * Memory access
 * ========================================================================== */

int CmsisDap::packetSize() const
{
    return transport_ ? transport_->packetSize() : 64;
}

int CmsisDap::maxBlockWords() const
{
    const int size = packetSize();

    /* On the way out a DAP_TransferBlock is 5 bytes of header plus four bytes
     * per word, on the way back 4 bytes of header plus four bytes per word.
     * Whichever limits first decides the block size, and both have to fit
     * into the one packet the probe accepts - the transport sizes its read
     * buffer from the same value, so a block that fits here is never
     * truncated on the way back. */
    const int byCommand = (size - 5) / 4;
    const int byResponse = (size - 4) / 4;

    return qBound(1, qMin(byCommand, byResponse), 16384);
}

bool CmsisDap::readMem32(quint32 address, quint32 *values, int count, QString &error)
{
    if (count <= 0)
        return true;

    /* The AP bank has to be known before any AP register is touched, and the
     * only way to know it is to have written it in this session. */
    if (!selectApBank(0, 0, error))
        return false;

    if (!swdWriteAp(kApCswReg, kApCsw, error))
        return false;
    cachedCsw_ = kApCsw;

    const int blockWords = maxBlockWords();

    int done = 0;
    while (done < count) {
        /* A DAP_TransferBlock advances the address itself, but the AHB-AP
         * wraps that auto-increment at a 4 KB page boundary: the word that
         * belongs at 0x...1000 comes back from 0x...0000 instead (measured on
         * this target with a 1064 word block four kilobytes into RAM, which
         * landed on top of the first 272 bytes). A block must never cross a
         * page. */
        const quint32 blockAddress = address + static_cast<quint32>(done) * 4u;
        const int pageWords = static_cast<int>((0x1000u - (blockAddress & 0xFFFu)) / 4u);
        const int chunk = qMin(qMin(count - done, blockWords), pageWords);

        /* Every block starts with its own TAR write, so a firmware that does
         * not keep the address across two commands still starts at the right
         * place. One extra command per block is cheap next to that. */
        if (!swdWriteAp(kApTarReg, blockAddress, error))
            return false;

        QByteArray readData;
        quint8 response = 0;

        if (!transferBlock(static_cast<quint8>(DapTransferRequest::ApnDp
                                               | DapTransferRequest::Read
                                               | (kApDrwReg & 0x0C)),
                           static_cast<quint16>(chunk), QByteArray(), readData, &response, error)) {
            return false;
        }

        const uchar *p = reinterpret_cast<const uchar *>(readData.constData());
        for (int i = 0; i < chunk; ++i) {
            values[done + i] = static_cast<quint32>(p[i * 4])
                               | (static_cast<quint32>(p[i * 4 + 1]) << 8)
                               | (static_cast<quint32>(p[i * 4 + 2]) << 16)
                               | (static_cast<quint32>(p[i * 4 + 3]) << 24);
        }

        done += chunk;
    }

    cachedTar_ = 0;
    tarKnown_ = false;
    return true;
}

bool CmsisDap::readMem32Polled(quint32 address, quint32 &value, QString &error)
{
    /* See the header for why this is separate from readMem32(). The short
     * version: this is the poll-loop fast path, and it may only skip the AP
     * setup when the AP is still set up exactly the way this object left it.
     *
     * The saving is three USB round trips per call - SELECT, CSW and TAR -
     * which on this probe is the difference between ~15 ms and ~4 ms per
     * poll. waitForHalt() does nothing but call this, so the whole erase and
     * program wait shortens by that factor. */

    if (!selectApBank(0, 0, error)) {
        cachedCsw_ = 0;
        cachedTar_ = 0;
        return false;
    }

    /* CSW decides the transfer size and, crucially, whether the AP advances
     * TAR by itself after each access.
     *
     * This is the whole trick. With the usual single-increment CSW every read
     * of the data register leaves the AP pointing one word further on, so a
     * loop that always wants the same address can never reuse its own TAR:
     * asking for it again always misses, and the TAR write this fast path
     * exists to skip goes on the wire every time. Measured that way: no
     * speedup at all.
     *
     * Setting AddrInc to "auto-increment off" pins TAR. The AP then reads the
     * same address for as long as we ask, one TAR write covers the entire
     * poll loop, and every poll after the first is a single transfer on the
     * wire.
     *
     * The block paths need the opposite, so the switch is not global: they
     * write kApCsw themselves and then restore this (see writeMem32 and
     * readMem32), which is why cachedCsw_ has to be compared rather than
     * assumed. */
    if (cachedCsw_ != kApCswFixed) {
        if (!swdWriteAp(kApCswReg, kApCswFixed, error)) {
            cachedCsw_ = 0;
            cachedTar_ = 0;
            return false;
        }
        cachedCsw_ = kApCswFixed;
    }

    /* TAR is written once. Nothing else in the loop moves it: AddrInc is off
     * (see above), so the read below leaves it exactly where it is. */
    if (!tarKnown_ || cachedTar_ != address) {
        if (!swdWriteAp(kApTarReg, address, error)) {
            cachedCsw_ = 0;
            cachedTar_ = 0;
            tarKnown_ = false;
            return false;
        }
        cachedTar_ = address;
        tarKnown_ = true;
    }

    QVector<quint32> readValues;
    quint8 status = 0;

    /* The data register is AP index 3, which lives in the top nibble of the
     * request byte. One DAP_Transfer of one AP register, with AddrInc off so
     * the read is idempotent.
     *
     * Getting the AP state wrong here is not a subtle performance issue - it
     * silently reads the wrong register. Measured: with a one-word read and
     * TAR left to advance on its own, the second poll read DCRSR
     * (0xE000EDF4) instead of DHCSR (0xE000EDF0) and never saw S_HALT, so
     * every flash died with "the core did not halt within 10000 ms" while
     * DFSR showed the algorithm had in fact reached its BKPT. */
    if (!transfer(QVector<quint8>{ static_cast<quint8>(DapTransferRequest::ApnDp
                                                       | DapTransferRequest::Read
                                                       | (kApDrwReg & 0x0C)) },
                  QVector<quint32>(), readValues, &status, error)) {
        /* A failed transfer leaves the AP state unknown; make the next call
         * pay for a full setup again rather than trust a stale cache. */
        cachedCsw_ = 0;
        cachedTar_ = 0;
        tarKnown_ = false;
        return false;
    }

    if (readValues.size() < 1) {
        error = QCoreApplication::translate("CmsisDap", "The probe answered a poll with no data.");
        cachedCsw_ = 0;
        cachedTar_ = 0;
        tarKnown_ = false;
        return false;
    }

    value = readValues.at(0);
    return true;
}

/* Deliberately absent: a "write two words in one DAP_Transfer" helper, and
 * the read-after-write variant of it.
 *
 * The idea is attractive - an AHB-AP access costs four USB round trips of
 * setup, so a caller writing DCRDR and then DCRSR (every register write in
 * callFunction() does exactly that) looks like it should pay for the setup
 * once. Measured, on this probe: packing the two writes into one DAP_Transfer
 * makes the very next algorithm call fail with "SWD fault (the target did not
 * acknowledge the access)".
 *
 * The reason is that an AHB-AP write is posted. writeMem32() ends with a
 * read-back of CSW for precisely this reason (see the comment there): it is
 * what guarantees the write reached the target before the caller moves on.
 * Two dependent data register writes inside one packet have no such barrier
 * between them, and the AP answers the next access with a fault.
 *
 * Roughly 100 KB/s of programming throughput is a small price for not having
 * that failure mode back, and the measured gain from removing the setup was
 * zero anyway: at 1 MHz the SWD wire, not the USB round trips, is the
 * bottleneck.
 *
 * What is *not* absent is writeMem32Batch(): it shares the setup and the
 * barrier across several addresses, but keeps every data transfer a separate
 * command with its own USB round trip in between. Measured on 1a86:5540 over
 * 2000 iterations of the two-word DCRDR/DCRSR pattern, that is 2.843 ms per
 * group down to 2.137 ms - and, unlike the packed variant, it survives 60
 * rounds of the full callFunction shape with zero faults. The distinction
 * that matters is not "one packet versus two", it is whether the previous
 * write has had a bus round trip to settle before the next TAR update. */

/* Several addresses, one AP setup, one barrier.
 *
 * This is not the contiguous case - writeMem32() below keeps that, because a
 * page write wants one DAP_TransferBlock for the whole page and this wants one
 * command per address. Two different shapes, two functions; what they share is
 * the rule that every data transfer gets its own bus round trip. */
bool CmsisDap::writeMem32Batch(const quint32 *address, const quint32 *values, int count,
                               QString &error)
{
    if (count <= 0)
        return true;

    /* The AP bank has to be known before any AP register is touched, and the
     * only way to know it is to have written it in this session. */
    if (!selectApBank(0, 0, error))
        return false;

    /* CSW is written once for the whole group. Its "auto-increment on" setting
     * is what lets one word follow the previous one inside a block, and the
     * AddrInc reset by the previous writeMem32() or poll is undone here. */
    if (!swdWriteAp(kApCswReg, kApCsw, error))
        return false;
    cachedCsw_ = kApCsw;

    for (int i = 0; i < count; ++i) {
        /* One word per address. Every element gets its own TAR, because the
         * addresses need not be consecutive (DCRDR and DCRSR are not) nor
         * ascending (DCRDR sits *above* DCRSR). */
        if (!swdWriteAp(kApTarReg, address[i], error)) {
            cachedTar_ = 0;
            tarKnown_ = false;
            return false;
        }

        QByteArray writeData;
        appendLe32(writeData, values[i]);

        QByteArray ignored;
        quint8 response = 0;

        if (!transferBlock(static_cast<quint8>(DapTransferRequest::ApnDp | (kApDrwReg & 0x0C)),
                           1, writeData, ignored, &response, error)) {
            cachedTar_ = 0;
            tarKnown_ = false;
            return false;
        }
    }

    /* AHB-AP writes are posted: read the control register back so that the
     * last write is guaranteed to have reached the target before the caller
     * continues (for example before starting the flash algorithm).
     *
     * One barrier for the whole group is enough - each transfer above had a
     * full USB round trip after the one before it, and this last bus access
     * orders the final one. That is measured, not assumed: the same pattern
     * run 60 times over (Sp, R0-R2, LR, PC of the callFunction shape) wrote
     * back correctly every time with no AP fault. */
    quint32 dummy = 0;
    QString ignored;
    swdReadAp(kApCswReg, dummy, ignored);

    cachedTar_ = 0;
    tarKnown_ = false;
    return true;
}

bool CmsisDap::writeMem32(quint32 address, const quint32 *values, int count, QString &error)
{
    if (count <= 0)
        return true;

    /* The contiguous case is a batch of count words, each one 4 bytes after
     * the last, and a single DAP_TransferBlock per 4 KB page - not one
     * command per word. That is why this is not a call to writeMem32Batch()
     * with count groups: the loop here is what keeps a page write one
     * transfer. */
    if (!selectApBank(0, 0, error))
        return false;

    if (!swdWriteAp(kApCswReg, kApCsw, error))
        return false;
    cachedCsw_ = kApCsw;

    const int blockWords = maxBlockWords();

    int done = 0;
    while (done < count) {
        /* See readMem32: a block must not cross a 4 KB page. The AHB-AP wraps
         * the auto-increment at the boundary and the tail of the block then
         * lands at the start of the page. */
        const quint32 blockAddress = address + static_cast<quint32>(done) * 4u;
        const int pageWords = static_cast<int>((0x1000u - (blockAddress & 0xFFFu)) / 4u);
        const int chunk = qMin(qMin(count - done, blockWords), pageWords);

        /* Each block sets its own start address */
        if (!swdWriteAp(kApTarReg, blockAddress, error)) {
            cachedTar_ = 0;
            tarKnown_ = false;
            return false;
        }

        QByteArray writeData;
        writeData.reserve(chunk * 4);
        for (int i = 0; i < chunk; ++i)
            appendLe32(writeData, values[done + i]);

        QByteArray ignored;
        quint8 response = 0;

        if (!transferBlock(static_cast<quint8>(DapTransferRequest::ApnDp | (kApDrwReg & 0x0C)),
                           static_cast<quint16>(chunk), writeData, ignored, &response, error)) {
            cachedTar_ = 0;
            tarKnown_ = false;
            return false;
        }

        done += chunk;
    }

    /* AHB-AP writes are posted: read the control register back so that the
     * last write is guaranteed to have reached the target before the caller
     * continues (for example before starting the flash algorithm). */
    quint32 dummy = 0;
    QString ignored;
    swdReadAp(kApCswReg, dummy, ignored);

    cachedTar_ = 0;
    tarKnown_ = false;
    return true;
}

bool CmsisDap::readBytes(quint32 address, int length, QByteArray &data, QString &error)
{
    data.clear();

    if (length <= 0)
        return true;

    const quint32 aligned = address & ~0x3u;
    const int lead = static_cast<int>(address - aligned);
    const int words = (lead + length + 3) / 4;

    QVector<quint32> buffer(words, 0);

    if (!readMem32(aligned, buffer.data(), words, error))
        return false;

    data.resize(length);
    for (int i = 0; i < length; ++i) {
        const int index = lead + i;
        const quint32 word = buffer.at(index / 4);
        data[i] = static_cast<char>((word >> ((index % 4) * 8)) & 0xFF);
    }

    return true;
}

bool CmsisDap::writeBytes(quint32 address, const QByteArray &data, QString &error)
{
    if (data.isEmpty())
        return true;

    const quint32 aligned = address & ~0x3u;
    const int lead = static_cast<int>(address - aligned);
    const int total = lead + data.size();
    const int words = (total + 3) / 4;

    QVector<quint32> buffer(words, 0xFFFFFFFFu);

    /* Read the touched words first so that the bytes in front of and behind
     * the payload keep their value */
    if (lead != 0 || (data.size() % 4) != 0) {
        if (!readMem32(aligned, buffer.data(), words, error))
            return false;
    }

    for (int i = 0; i < data.size(); ++i) {
        const int word = (lead + i) / 4;
        const int shift = ((lead + i) % 4) * 8;
        buffer[word] = (buffer[word] & ~(0xFFu << shift))
                       | (static_cast<quint32>(static_cast<quint8>(data.at(i))) << shift);
    }

    return writeMem32(aligned, buffer.constData(), words, error);
}
