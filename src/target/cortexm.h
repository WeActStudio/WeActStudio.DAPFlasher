/*
 * WeAct Studio DAP Flasher
 *
 * Cortex-M core control: halt, run, reset and core register access.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CORTEXM_H
#define CORTEXM_H

#include <QString>
#include <QtGlobal>

#include "resetmode.h"

class CmsisDap;

/**
 * Core register numbers of the debug register interface (DCRSR REGSEL).
 */
namespace CoreReg {

enum : quint8 {
    R0 = 0, R1, R2, R3, R4, R5, R6, R7,
    R8, R9, R10, R11, R12,
    Sp = 13,
    Lr = 14,
    Pc = 15,
    Xpsr = 16,
    Msp = 17,
    Psp = 18,
    Cfbp = 20
};

} /* namespace CoreReg */

/**
 * The debug registers of a Cortex-M core, reached through the AHB-AP of a
 * CMSIS-DAP probe.
 */
class CortexM
{
public:
    explicit CortexM(CmsisDap *link);

    /** Read the CPUID register and remember the core type */
    bool probe(QString &error);

    bool isValid() const { return valid_; }

    quint32 cpuid() const { return cpuid_; }

    /** e.g. "Cortex-M4" */
    QString coreName() const;

    /** The 32 bit CPUID as an eight digit hexadecimal string */
    QString cpuidText() const;

    /* ---------- run state ---------- */

    /** Enable the debug logic without halting the core */
    bool enableDebug(QString &error);

    /** Halt the core */
    bool halt(QString &error);

    /** Let the core run freely (the debug logic stays enabled) */
    bool resume(QString &error);

    /**
     * Let the core run with interrupts masked.
     *
     * A flash algorithm must not be interrupted, otherwise the target could
     * service an interrupt from flash while the flash controller is busy.
     * The ARM architecture requires C_MASKINTS to be asserted while the core
     * is halted and then to release C_HALT.
     */
    bool resumeMasked(QString &error);

    /** Is the core halted right now */
    bool isHalted(bool &halted, QString &error);

    /** S_RESET_ST (a reset happened since it was last read) */
    bool resetStatus(bool &resetDetected, QString &error);

    /**
     * Reset the core through the debug port: SYSRESETREQ in AIRCR.
     *
     * @param haltAfterReset  keep the core halted on the reset vector
     */
    bool resetSoftware(bool haltAfterReset, QString &error);

    /**
     * Reset the core through the nRESET line of the probe.
     *
     * The line is pulsed through DAP_SWJ_Pins, the way every mainstream
     * debugger does it, with DAP_Reset_Target as the fallback for probes that
     * only implement that one.
     *
     * @param performed  reports whether the probe actually drove the line; a
     *                   probe with neither command answers "not performed",
     *                   which is a failure here. A probe whose pin read-back
     *                   follows the command counts as driving it, which says
     *                   nothing about whether the target's NRST is wired to
     *                   that pin.
     */
    bool resetHardware(bool haltAfterReset, bool *performed, QString &error);

    /**
     * Reset the core the way the user selected.
     *
     * @param haltAfterReset  keep the core halted on the reset vector
     * @param usedMode        the way that actually reset the core, which
     *                        differs from @p mode when "Automatic" falls back
     *                        to the software reset
     */
    bool resetWithMode(ResetMode mode, bool haltAfterReset,
                       ResetMode *usedMode, QString &error);

    /**
     * Wait until the core is halted.
     *
     * @param timeoutMs  how long to wait
     */
    bool waitForHalt(int timeoutMs, QString &error);

    /* ---------- registers ---------- */

    bool readRegister(quint8 reg, quint32 &value, QString &error);
    bool writeRegister(quint8 reg, quint32 value, QString &error);

    /** Read R0..R12, SP, LR, PC and xPSR */
    bool readAllRegisters(quint32 *values, QString &error);

    /** Program counter of the halted core */
    bool readPc(quint32 &pc, QString &error);

    /* ---------- helper used by the flash algorithm ---------- */

    /**
     * Run the function at @p entry with the three arguments in R0..R2 and
     * stop it with a BKPT instruction placed at @p trapAddress.
     *
     * The caller prepares stack and trap beforehand; this only loads the
     * registers and resumes the core.
     */
    bool callFunction(quint32 entry, quint32 trapAddress,
                      quint32 arg0, quint32 arg1, quint32 arg2,
                      QString &error);

    /** Read back R0, the return value of a callFunction() call */
    bool readReturnValue(quint32 &value, QString &error);

    /** Fault status register DFSR, used to explain a failed algorithm */
    bool readFaultStatus(quint32 &dfsr, QString &error);

private:
    bool writeDhcsr(quint32 value, QString &error);
    bool readDhcsr(quint32 &value, QString &error);

    CmsisDap *link_;
    bool valid_;
    quint32 cpuid_;
};

#endif /* CORTEXM_H */
