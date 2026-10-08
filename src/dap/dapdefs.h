/*
 * WeAct Studio DAP Flasher
 *
 * CMSIS-DAP protocol constants
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DAPDEFS_H
#define DAPDEFS_H

#include <QtGlobal>

/** Command identifiers (host -> probe), CMSIS-DAP v1/v2 */
namespace DapCmd {

enum : quint8 {
    Info              = 0x00,
    HostStatus        = 0x01,
    Connect           = 0x02,
    Disconnect        = 0x03,
    TransferConfigure = 0x04,
    Transfer          = 0x05,
    TransferBlock     = 0x06,
    TransferAbort     = 0x07,
    WriteAbort        = 0x08,
    Delay             = 0x09,
    ResetTarget       = 0x0A,
    SwjPins           = 0x10,
    SwjClock          = 0x11,
    SwjSequence       = 0x12,
    JtagSequence      = 0x14,
    JtagConfigure     = 0x15,
    SwoTransport      = 0x17,
    SwoMode           = 0x18,
    SwoBaudrate       = 0x19,
    SwoControl        = 0x1A,
    SwoData           = 0x1B,
    QueueCommands     = 0x7E,
    ExecuteCommands   = 0x7F
};

} /* namespace DapCmd */

/** Status codes of one response block of the v1 atomic command protocol
 *  (DAP_ExecuteCommands / DAP_QueueCommands).
 *
 *  The plain commands used by this tool do not need them: they answer with
 *  DapStatus::Ok / DapStatus::Error, and DAP_Transfer with the
 *  DapTransferResult flags. The aliases that used to live here (WrongFormat,
 *  WrongArgument, ExecutionError, ResponseCount) each repeated one of the
 *  values below and only made the table look bigger than it is. */
namespace DapResp {

enum : quint8 {
    Ok          = 0x00,   /* DAP_OK; also the command id a response echoes */
    ErrorWait   = 0x01,   /* DAP_ERROR_WAIT: the target asked to be retried */
    ErrorFault  = 0x80,   /* DAP_ERROR_FAULT: no valid SWD/JTAG acknowledge */
    Unsupported = 0xF9,   /* DAP_ERROR_UNSUPPORTED */
    NoTarget    = 0xFB,   /* DAP_ERROR_NO_TARGET */
    ReadError   = 0xFC,   /* DAP_ERROR_READ */
    ListError   = 0xFD,   /* DAP_ERROR_LIST */
    WriteError  = 0xFE,   /* DAP_ERROR_WRITE */
    Error       = 0xFF    /* DAP_ERROR */
};

} /* namespace DapResp */

/** DAP_Connect port values */
namespace DapPort {

enum : quint8 {
    Default = 0x00,
    Swd     = 0x01,
    Jtag    = 0x02
};

} /* namespace DapPort */

/** DAP_Info identifiers */
namespace DapInfoId {

enum : quint8 {
    Vendor        = 0x01,
    Product       = 0x02,
    SerialNumber  = 0x03,
    FirmwareVer   = 0x04,
    TargetVendor  = 0x05,
    TargetName    = 0x06,
    BoardVendor   = 0x07,
    BoardName     = 0x08,
    ProductFwVer  = 0x09,
    Capabilities  = 0xF0,
    TestDomainTimer = 0xF1,
    UartCount     = 0xF2,
    UartConfigure = 0xF3,
    SwoBufferSize = 0xFD,
    PacketCount   = 0xFE,
    PacketSize    = 0xFF
};

} /* namespace DapInfoId */

/** DAP_Info capabilities: the first byte of the DAP_Info(0xF0) answer.
 *
 * The bit assignment is fixed by the specification (reference implementation:
 * ARM-software/CMSIS-DAP, Firmware/Source/DAP.c, case DAP_ID_CAPABILITIES).
 * It is easy to drift by one from bit 2 onwards: bit 2 is SWO over UART, not
 * "SWJ sequences" - no probe has a capability bit for those, because the SWJ
 * sequences are part of every SWD attach. */
namespace DapCapability {

enum : quint32 {
    Swd            = (1u << 0),   /* DAP_SWD */
    Jtag           = (1u << 1),   /* DAP_JTAG */
    SwoUart        = (1u << 2),   /* SWO_UART: SWO captured through a UART */
    SwoManchester  = (1u << 3),   /* SWO_MANCHESTER */
    AtomicCommands = (1u << 4),   /* always set by the reference firmware */
    TimestampClock = (1u << 5),   /* TIMESTAMP_CLOCK, the test domain timer */
    SwoStream      = (1u << 6),   /* SWO_STREAM: continuous SWO streaming */
    Uart           = (1u << 7)    /* DAP_UART: the UART communication port */
};

} /* namespace DapCapability */

/** DAP_SWJ_Pins bit masks */
namespace DapPin {

enum : quint8 {
    Swclk  = (1u << 0),
    Swdio  = (1u << 1),
    Tdi    = (1u << 2),
    Tdo    = (1u << 3),
    NTreset = (1u << 5),
    NReset = (1u << 7)
};

} /* namespace DapPin */

/**
 * DAP_Transfer / DAP_TransferBlock request byte.
 *
 *   bit 0     : APnDP          (0 = DP, 1 = AP)
 *   bit 1     : RnW            (1 = read)
 *   bits 2..3 : register address A[3:2]
 *   bit 4     : Match Value    (v1)
 *   bit 5     : Match Mask     (v1)
 */
namespace DapTransferRequest {

enum : quint8 {
    ApnDp     = 0x01,
    Read      = 0x02,
    MatchValue = 0x10,
    MatchMask = 0x20,

    /* DP registers */
    DpIdCode  = 0x00 | 0x02,            /* 0x02 */
    DpCtrlStat = 0x04,
    DpSelect  = 0x08,
    DpRdBuff  = 0x0C,

    /* AP registers */
    ApCsw     = 0x00 | 0x01,            /* 0x01 */
    ApTar     = 0x04 | 0x01,            /* 0x05 */
    ApDrw     = 0x0C | 0x01             /* 0x0D */
};

} /* namespace DapTransferRequest */

/** Transfer response value of a single DAP_Transfer operation */
namespace DapTransferResult {

enum : quint8 {
    Ok        = 0x01,
    Wait      = 0x02,
    Fault     = 0x04,
    Error     = 0x08,
    Mismatch  = 0x10
};

} /* namespace DapTransferResult */

/** Status byte of the plain commands (DAP_Disconnect, DAP_TransferConfigure,
 *  DAP_SWJ_Clock, DAP_SWJ_Sequence, DAP_Reset_Target): DAP_OK or DAP_ERROR.
 *  Identical in CMSIS-DAP v1 and v2 - this is not a v1 only table. The finer
 *  grained codes below belong to the v1 atomic command protocol. */
namespace DapStatus {

enum : quint8 {
    Ok        = 0x00,
    Error     = 0xFF,
    Probe     = 0x01,
    SwdFault  = 0x30,
    NoAck     = 0x07,
    StickyErr = 0x08
};

} /* namespace DapStatus */

/** Well known CMSIS-DAP USB identifiers, used to recognise probes whose HID
 *  report descriptor does not use the standard vendor defined usage page. */
struct DapUsbId
{
    quint16 vid;
    quint16 pid;
    const char *name;
};

/** The vendor defined usage page every CMSIS-DAP v1 probe should use */
static const quint16 kDapUsagePage = 0xFF00;

#endif /* DAPDEFS_H */
