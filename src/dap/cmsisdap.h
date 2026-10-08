/*
 * WeAct Studio DAP Flasher
 *
 * CMSIS-DAP link layer: probe commands, SWD transfers and memory access.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CMSISDAP_H
#define CMSISDAP_H

#include <QByteArray>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include <functional>

struct DapProbeInfo;
class DapTransport;

/** Log sink used while the SWD link is brought up; may be empty */
using DapAttachLog = std::function<void(const QString &message, bool isError)>;

/**
 * Hook run the instant the DP answers while nRESET is released.
 *
 * It is the only moment at which the target program of a hostile target can
 * still be stopped, so whatever it does has to happen right there. Returning
 * false puts the target back into reset and tries the window again.
 */
using DapAttachTouch = std::function<bool(QString &error)>;

/**
 * One open CMSIS-DAP probe.
 *
 * The class speaks the wire protocol (DAP_Info, DAP_Connect, DAP_Transfer, ...)
 * and adds the SWD / AP layer needed to touch target memory. Everything above
 * (core control, flash programming) is built on readBytes()/writeBytes().
 */
class CmsisDap
{
public:
    CmsisDap();
    ~CmsisDap();

    CmsisDap(const CmsisDap &) = delete;
    CmsisDap &operator=(const CmsisDap &) = delete;

    /* ---------- connection ---------- */

    bool open(const DapProbeInfo &probe, QString &error);
    void close();
    bool isOpen() const;
    QString transportName() const;

    /* ---------- probe information ---------- */

    bool queryInfo(quint8 id, QByteArray &value, QString &error);
    QString infoString(quint8 id);
    quint32 infoU32(quint8 id);
    bool swdSupported() const { return (capabilities_ & 0x01u) != 0; }
    bool jtagSupported() const { return (capabilities_ & 0x02u) != 0; }
    quint32 capabilities() const { return capabilities_; }

    /* ---------- DAP commands ---------- */

    bool connectSwd(QString &error);
    bool disconnect(QString &error);
    bool isConnected() const { return connected_; }

    /**
     * Connect over SWD and make the DP answer.
     *
     * A plain DP read is tried first, so a healthy link is left completely
     * untouched. Only when the DP stays silent is the attach sequence every
     * other debugger leads with walked through - line reset, JTAG to SWD
     * switch, a fresh DAP_Connect, a target reset and finally a much slower
     * clock - because that is what revives a DP whose target program has
     * switched the debug interface off or gone into a low power state.
     *
     * @param clockHz   requested SWD clock
     * @param dpIdCode  the DP IDCODE, valid when the call succeeds
     * @param log       optional sink for the progress notes
     */
    bool attachSwd(quint32 clockHz, quint32 &dpIdCode, const DapAttachLog &log,
                   QString &error);

    /**
     * Bring the SWD link up with nRESET held low: "connect under reset".
     *
     * The target program never runs between the release of the reset and the
     * first debug access, which is the only way into a target whose program
     * has taken the debug access over - by switching the SWD pins, by running
     * at a clock the debug logic cannot follow, or by going to sleep.
     *
     * @param touch  run as soon as the DP answers, still inside that window.
     *               Halting the core here is what keeps the program from
     *               starting again. May be empty.
     * @return false when the probe cannot drive nRESET at all, or when the
     *         DP (or the touch) did not answer inside the window. The reset
     *         is released before returning, whatever happened.
     */
    bool attachUnderReset(quint32 clockHz, quint32 &dpIdCode, const DapAttachLog &log,
                          const DapAttachTouch &touch, QString &error);

    bool setClock(quint32 hz, QString &error);
    bool configureTransfer(quint8 idleCycles, quint16 waitRetry, quint16 matchRetry, QString &error);

    /**
     * "10 MHz", "500 kHz", "250 Hz" - how a session names a clock.
     *
     * The window keeps the clock of the last run in its settings while the
     * command line always starts at its own default, so the two can run at
     * different speeds without anything saying so. Every session prints the
     * clock it uses through this, and a retry at a lower clock prints both.
     */
    static QString clockText(quint32 hz);

    /** Size of one DAP command packet this probe accepts */
    int packetSize() const;

    /** Largest number of 32 bit words that fits into one DAP_TransferBlock
     *  packet; a page write or a page read is split into as many blocks.
     *  Exposed so that the log can name the transfer size in use. */
    int maxBlockWords() const;

    /** Drive the SWJ pins; returns the pin state through pinState (optional) */
    bool setPins(quint8 value, quint8 select, quint32 waitUs, quint8 *pinState, QString &error);

    /** DAP_Reset_Target: nRESET low pulse, returns whether it was performed */
    bool resetTarget(bool *executedByProbe, QString &error);

    /**
     * Pull nRESET low (asserted) or let it go high, through DAP_SWJ_Pins.
     *
     * This is how every mainstream debugger holds a target in reset;
     * DAP_Reset_Target is a later, optional command that many firmwares never
     * implement beyond a stub. `followed` reports whether the pin state the
     * probe returned changed as asked, which is the only confirmation
     * obtainable without a scope - it proves the pin is driven by the probe,
     * not that the target's NRST is wired to it.
     */
    bool driveResetPin(bool asserted, quint32 waitUs, bool *followed, QString &error);

    /**
     * Assert nRESET for holdMs, then release it and wait settleMs.
     *
     * This is the canonical target reset (OpenOCD and pyOCD both do exactly
     * this) and the precondition for attaching a target whose program has
     * taken the debug pins over.
     */
    bool pulseResetPin(quint32 holdMs, quint32 settleMs, bool *followed, QString &error);

    /** Pulse nRESET in whatever way this probe supports: DAP_SWJ_Pins first,
     *  DAP_Reset_Target as the fallback. `performed` stays false when neither
     *  can actually drive a reset line. */
    bool pulseTargetReset(bool *performed, QString &error);

    /** SWJ sequence used for the SWD line reset and the JTAG-to-SWD switch */
    bool swjSequence(quint16 bitCount, const QByteArray &data, QString &error);

    /* ---------- SWD ---------- */

    /**
     * SWD line reset, JTAG-to-SWD switch and idle cycles.
     *
     * This is part of the SWD attach procedure, not an optional probe
     * feature: it is what brings a DP back that the target program has
     * switched off. Every probe speaks it.
     */
    bool swdLineReset(QString &error);

    bool swdReadDp(quint8 reg, quint32 &value, QString &error);
    bool swdWriteDp(quint8 reg, quint32 value, QString &error);
    bool swdReadAp(quint8 reg, quint32 &value, QString &error);
    bool swdWriteAp(quint8 reg, quint32 value, QString &error);

    /**
     * Clear the sticky error flags of the DP.
     *
     * This goes through the ABORT register, the way pyocd's
     * DAP.clear_sticky_err() does for SWD. CTRL/STAT also carries the two
     * power request bits, so a "clear the errors" write to it is one
     * careless mask away from switching a power domain off - which leaves
     * the target answering nothing until it is power cycled. ABORT has no
     * such bit.
     */
    bool clearStickyErrors(QString &error);

    /* ---------- memory access (through the AHB-AP) ---------- */

    bool readMem32(quint32 address, quint32 *values, int count, QString &error);
    bool writeMem32(quint32 address, const quint32 *values, int count, QString &error);

    /**
     * One word per address, several addresses, one AP setup.
     *
     * The addresses do not have to be consecutive or even ascending - each
     * element carries its own address, and every one is written with its own
     * TAR update. What is shared is everything around them: the AP bank
     * selection and the CSW write happen once, and the write-back barrier
     * that flushes the posted AHB-AP writes happens once at the end.
     *
     * Writing the same pairs through writeMem32() one call at a time costs
     * four commands per call on this probe - CSW write, TAR write, the block
     * transfer and a CSW read-back barrier. Measured on 1a86:5540, the
     * barrier alone is 0.28 ms of a 2.84 ms call, and the repeated CSW write
     * another 0.28 ms. Doing them once for a group of writes is worth about
     * 25% per group (2.843 -> 2.137 ms for two words).
     *
     * It is deliberately not "pack the writes into one DAP_Transfer": the
     * transfers stay separate commands with a full USB round trip between
     * them, which is what keeps the previous write settled before the next
     * TAR update (see the note above writeMem32). Only the redundant setup
     * and barriers are removed.
     *
     * @param address   one target address per element
     * @param values    the word to write at the matching address
     * @param count     number of pairs; 0 is a no-op and returns true
     */
    bool writeMem32Batch(const quint32 *address, const quint32 *values, int count,
                         QString &error);

    /**
     * Read one word with the AP access state kept warm across repeated calls.
     *
     * This exists for one specific shape of traffic: a caller that reads the
     * *same* address over and over while waiting for something to change -
     * waitForHalt() polling DHCSR is the one that matters, and it is what
     * sets the erase and program time.
     *
     * A plain readMem32() is deliberately stateless: it re-selects the AP
     * bank, re-writes CSW and re-writes TAR on every call, because it cannot
     * know what happened to the AP since the last one. That caution costs
     * three extra USB round trips per call, which is nothing for a bulk
     * transfer and everything for a poll loop.
     *
     * This call instead assumes the AP is already set up the way this object
     * left it, which holds precisely while nothing but this loop talks to the
     * probe. That is why it is a separate entry point rather than a shortcut
     * inside readMem32(): the caller has to know the loop is exclusive, and
     * the name is there to make that requirement visible.
     *
     * Any use of readMem32()/writeMem32()/the SWD helpers in between is fine -
     * they reset the cached state and the next poll here re-establishes it.
     */
    bool readMem32Polled(quint32 address, quint32 &value, QString &error);

    /** Byte granular access; address and length may be unaligned, the touched
     *  words are read-modify-written when needed. */
    bool readBytes(quint32 address, int length, QByteArray &data, QString &error);
    bool writeBytes(quint32 address, const QByteArray &data, QString &error);

    /** Last error text of a transfer, used for the log */
    QString lastTransferError() const { return lastTransferError_; }

private:
    /* One DAP_Transfer: requests in order, values for the writes, values out
     * for the reads */
    bool transfer(const QVector<quint8> &requests,
                  const QVector<quint32> &writeValues,
                  QVector<quint32> &readValues,
                  quint8 *transferResponse,
                  QString &error);

    bool transferBlock(quint8 request, quint16 count,
                       const QByteArray &writeData, QByteArray &readData,
                       quint8 *transferResponse, QString &error);

    /**
     * Request the debug and the system power domain of the DP.
     *
     * This follows pyocd's DAP.power_up_debug(): one write of
     * CSYSPWRUPREQ | CDBGPWRUPREQ | MASKLANE | TRNNORMAL, then a poll until
     * CDBGPWRUPACK and CSYSPWRUPACK are both set, with pyocd's five second
     * budget.
     *
     * DAP_Connect is supposed to power the debug domain up, but not every
     * probe firmware does, and a DP whose domains are down answers reads
     * happily while everything behind it - the whole AP, that is every byte
     * of target memory - comes back as WAIT or FAULT.
     *
     * The write always carries both request bits, so it can never perform
     * the one transition ADIv6 forbids: clearing CDBGPWRUPREQ and
     * CSYSPWRUPREQ together powers off the DP that would have to power the
     * domains back up, and the target then answers WAIT to every write until
     * something resets it. This method never lowers a request bit; pyocd's
     * power_down_debug() drops the system request first, waits for its
     * acknowledge to fall, and only then drops the debug request.
     */
    bool powerUpDebug(QString &error);

    /**
     * Select the AP and its register bank, through DP SELECT.
     *
     * SELECT sits in the *target's* SW-DP, not in the probe, so DAP_Connect
     * cannot clear it: a debugger that ran before this one leaves its own
     * APSEL and APBANKSEL behind. With a bank other than zero every AP
     * register is reached through a banked alias - the TAR write lands in
     * BD1, the CSW write in BD0 - so no address is ever written, every
     * memory read answers with the same stale word, and nothing reports an
     * error because the DP itself still answers. Asking for bank 0
     * explicitly is what makes the AP access independent of whatever ran
     * before. The value is cached, so a session pays for one DP write.
     */
    bool selectApBank(quint8 apsel, quint8 bank, QString &error);

    /** One DP IDCODE read; only a plausible value counts as an answer */
    bool readDpIdCode(quint32 &dpIdCode, QString &error);

    bool sendCommand(const QByteArray &command, QString &error);
    bool exchange(const QByteArray &command, QByteArray &response, QString &error);

    DapTransport *transport_;
    bool connected_;
    quint32 capabilities_;
    /* AP access state, as this object last left it on the probe.
     *
     * readMem32()/writeMem32() clear these before use - they are stateless
     * entry points and cannot assume anything about the AP. readMem32Polled()
     * is the opposite: it only exists because the caller guarantees nothing
     * else touched the probe, so it trusts these and skips the matching
     * writes. Cached CSW of 0 is a real value (the reset state), so it needs
     * no "known" flag; TAR of 0 is a valid address, so tarKnown_ is what
     * separates "cached address 0" from "no cached address". */
    quint32 cachedCsw_;
    quint32 cachedTar_;
    bool tarKnown_;
    quint8 cachedSelect_;
    bool selectKnown_;
    int retries_;
    QString lastTransferError_;
};

#endif /* CMSISDAP_H */
