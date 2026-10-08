/*
 * WeAct Studio DAP Flasher
 *
 * Command line interface
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "cli.h"

#include "dap/cmsisdap.h"
#include "dap/dapdefs.h"
#include "dap/probes.h"
#include "flash/algocatalog.h"
#include "flash/flashalgo.h"
#include "flashworker.h"
#include "targetsession.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QTextStream>

namespace {

QTextStream &out()
{
    static QTextStream stream(stdout);
    return stream;
}

QTextStream &err()
{
    static QTextStream stream(stderr);
    return stream;
}

QString hex32(quint32 value)
{
    return QStringLiteral("0x%1").arg(value, 8, 16, QLatin1Char('0'));
}

void printUsage()
{
    out() << "WeAct Studio DAP Flasher " << QCoreApplication::applicationVersion() << "\n\n";
    out() << "Usage:\n";
    out() << "  dapflasher --list\n";
    out() << "  dapflasher --identify [options]\n";
    out() << "  dapflasher --firmware <file> --algo <file.flm> [options]\n";
    out() << "  dapflasher --erase-chip --algo <file.flm>\n";
    out() << "  dapflasher --halt | --run | --reset [options]\n\n";
    out() << "Options:\n";
    out() << "  --probe <n>          Probe number as printed by --list (default 1)\n";
    out() << "  --algo <file>        Keil .FLM flash algorithm\n";
    out() << "  --firmware <file>    Firmware image (.bin or Intel .hex)\n";
    out() << "  --bin-base <hex>     Load address of .bin files (default 08000000)\n";
    out() << "  --ram-base <hex>     RAM area the algorithm runs in (default 20000000)\n";
    out() << "  --clock <hz>         SWD clock in Hz (default 1000000)\n";
    out() << "  --erase-chip         Erase the whole chip instead of single sectors.\n";
    out() << "                       Without --firmware this erases only, which is how a\n";
    out() << "                       target that holds the debug access is brought back\n";
    out() << "  --no-verify          Skip the verification\n";
    out() << "  --no-reset           Leave the target halted after programming\n";
    out() << "  --reset-mode <mode>  Reset mode: software, hardware or auto (default software)\n";
    out() << "  --all-hid            Also list USB HID devices that are not CMSIS-DAP\n";
    out() << "  --lang <code>        Output language: auto, en or zh_CN (default auto)\n";
    out() << "  --help               This text\n";
    out() << "  --version            Print the version\n";
    out() << "\nExit codes: 0 success, 1 failure, 2 usage error\n";
}

/** Split "--name=value" or "--name value" */
bool optionValue(const QStringList &arguments, int &index, const QString &name,
                 const QString &argument, QString &value)
{
    if (argument.startsWith(name + QLatin1Char('='))) {
        value = argument.section(QLatin1Char('='), 1);
        return true;
    }

    if (argument == name) {
        if (index + 1 >= arguments.size())
            return false;
        value = arguments.at(++index);
        return true;
    }

    return false;
}

bool parseHex(const QString &text, quint32 &value)
{
    QString clean = text.trimmed();
    if (clean.startsWith(QLatin1String("0x"), Qt::CaseInsensitive))
        clean.remove(0, 2);

    bool ok = false;
    value = clean.toUInt(&ok, 16);
    return ok;
}

int listProbes(bool allHid)
{
    QString error;
    const QList<DapProbeInfo> probes = dapEnumerateProbes(allHid, &error);

    if (probes.isEmpty()) {
        err() << "No CMSIS-DAP probe found.\n";
        if (!error.isEmpty())
            err() << error << "\n";
        return 1;
    }

    out() << "Found " << probes.size() << " probe(s):\n";
    for (int i = 0; i < probes.size(); ++i) {
        const DapProbeInfo &probe = probes.at(i);
        out() << QStringLiteral("  %1. %2\n").arg(i + 1).arg(probe.displayName());
        out() << QStringLiteral("     VID:PID %1:%2, %3\n")
                     .arg(probe.vid, 4, 16, QLatin1Char('0'))
                     .arg(probe.pid, 4, 16, QLatin1Char('0'))
                     .arg(probe.isBulk() ? QStringLiteral("USB bulk (WinUSB)")
                                         : QStringLiteral("USB HID"));
        if (!probe.serial.isEmpty())
            out() << "     serial: " << probe.serial << "\n";
        if (probe.busy)
            out() << "     (in use by another program)\n";
    }

    return 0;
}

} /* namespace */

bool cliRequested(const QStringList &arguments)
{
    /* Options the window itself handles (see main.cpp): they must not send the
     * process down the console path. */
    static const char *kWindowOptions[] = {
        "--lang", "--theme", "--screenshot", "--about"
    };

    for (const QString &argument : arguments) {
        if (!argument.startsWith(QLatin1Char('-')))
            continue;

        bool windowOption = false;
        for (const char *option : kWindowOptions) {
            const QLatin1String prefix(option);
            if (argument == prefix
                || argument.startsWith(prefix + QLatin1Char('='))) {
                windowOption = true;
                break;
            }
        }

        if (!windowOption)
            return true;
    }
    return false;
}

int cliRun(const QStringList &arguments)
{
    FlashRequest request;
    bool listRequested = false;
    bool identifyRequested = false;
    bool haltRequested = false;
    bool runRequested = false;
    bool resetRequested = false;
    bool allHid = false;
    int probeIndex = 0;                 /* as printed by --list, 1 based */

    for (int i = 0; i < arguments.size(); ++i) {
        const QString argument = arguments.at(i);
        QString value;

        if (argument == QLatin1String("--help") || argument == QLatin1String("-h")) {
            printUsage();
            return 0;
        }
        if (argument == QLatin1String("--version")) {
            out() << QCoreApplication::applicationVersion() << "\n";
            return 0;
        }
        if (argument == QLatin1String("--list")) {
            listRequested = true;
            continue;
        }
        if (argument == QLatin1String("--identify")) {
            identifyRequested = true;
            continue;
        }
        if (argument == QLatin1String("--halt")) {
            haltRequested = true;
            continue;
        }
        if (argument == QLatin1String("--run")) {
            runRequested = true;
            continue;
        }
        if (argument == QLatin1String("--reset")) {
            resetRequested = true;
            continue;
        }
        if (argument == QLatin1String("--erase-chip")) {
            request.eraseWholeChip = true;
            continue;
        }
        if (argument == QLatin1String("--no-verify")) {
            request.verify = false;
            continue;
        }
        if (argument == QLatin1String("--no-reset")) {
            request.resetAfterProgram = false;
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--reset-mode"), argument, value)) {
            if (!resetModeFromKey(value, request.resetMode)) {
                err() << "Unknown reset mode: " << value
                      << " (software, hardware or auto)\n";
                return 2;
            }
            continue;
        }
        if (argument == QLatin1String("--all-hid")) {
            allHid = true;
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--lang"), argument, value)) {
            /* Already honoured by main() before the translator was installed,
             * which has to happen before the first line of output. Nothing is
             * left to do here but to consume the option, and it is not saved:
             * a command line switch must not change the language the graphical
             * program was set to. */
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--probe"), argument, value)) {
            bool ok = false;
            probeIndex = value.toInt(&ok);
            if (!ok || probeIndex < 1) {
                err() << "Invalid probe number: " << value << "\n";
                return 2;
            }
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--algo"), argument, value)) {
            request.algoPath = value;
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--firmware"), argument, value)) {
            request.firmwarePath = value;
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--bin-base"), argument, value)) {
            if (!parseHex(value, request.firmwareBase)) {
                err() << "Invalid --bin-base value: " << value << "\n";
                return 2;
            }
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--ram-base"), argument, value)) {
            if (!parseHex(value, request.ramBase)) {
                err() << "Invalid --ram-base value: " << value << "\n";
                return 2;
            }
            continue;
        }
        if (optionValue(arguments, i, QStringLiteral("--clock"), argument, value)) {
            bool ok = false;
            const uint hz = value.toUInt(&ok);
            if (!ok || hz == 0) {
                err() << "Invalid --clock value: " << value << "\n";
                return 2;
            }
            request.clockHz = hz;
            continue;
        }

        err() << "Unknown option: " << argument << "\n";
        printUsage();
        return 2;
    }

    if (listRequested)
        return listProbes(allHid);

    /* ---------- pick the probe ---------- */
    QString error;
    const QList<DapProbeInfo> probes = dapEnumerateProbes(allHid, &error);

    if (probes.isEmpty()) {
        err() << (error.isEmpty() ? QStringLiteral("No CMSIS-DAP probe found.")
                                  : error)
              << "\n";
        return 1;
    }

    /* probeIndex is 1 based as printed by --list, 0 means "pick the first
     * one". The last valid number equals the number of probes, so the test
     * is "greater than", not "greater or equal" - with a single probe
     * "--probe 1" is legal and used to be rejected. */
    if (probeIndex > probes.size()) {
        err() << "Probe " << probeIndex << " does not exist (" << probes.size()
              << " found). Run --list.\n";
        return 1;
    }

    request.probe = probes.at(probeIndex > 0 ? probeIndex - 1 : 0);

    /* ---------- target only operations ---------- */
    if (haltRequested || runRequested || resetRequested || identifyRequested) {
        const SessionLog log = [](const QString &message, bool isError) {
            QTextStream &stream = isError ? err() : out();
            stream << message << "\n";
        };

        if (identifyRequested) {
            const TargetInfo info = targetIdentify(request.probe, request.clockHz, log);
            if (!info.ok) {
                err() << info.error << "\n";
                return 1;
            }
            out() << "Target: " << info.coreName << "\n"
                  << "CPUID: " << hex32(info.cpuid) << "\n"
                  << "DP IDCODE: " << hex32(info.dpIdCode) << "\n"
                  << "State: " << (info.halted ? "halted" : "running") << "\n";
        }

        if (haltRequested) {
            if (!targetHalt(request.probe, request.clockHz, log, nullptr, error)) {
                err() << error << "\n";
                return 1;
            }
            out() << "Core halted.\n";
        }

        if (runRequested) {
            if (!targetRun(request.probe, request.clockHz, log, nullptr, error)) {
                err() << error << "\n";
                return 1;
            }
            out() << "Core running.\n";
        }

        if (resetRequested) {
            if (!targetReset(request.probe, request.clockHz, request.resetMode, false,
                             log, nullptr, error)) {
                err() << error << "\n";
                return 1;
            }
            out() << "Target reset.\n";
        }

        return 0;
    }

    /* ---------- programming ---------- */
    if (request.algoPath.isEmpty()) {
        err() << "No flash algorithm given (--algo).\n";
        return 2;
    }

    /* Without a firmware the only thing left to do is to erase, and that is
     * the one operation that brings a target back whose own program holds the
     * debug access - so the combination is an action, not a mistake. */
    const bool eraseOnly = request.eraseWholeChip && request.firmwarePath.isEmpty();

    if (!eraseOnly && request.firmwarePath.isEmpty()) {
        err() << "No firmware given (--firmware).\n";
        return 2;
    }
    if (!QFileInfo(request.algoPath).isFile()) {
        err() << "Algorithm not found: " << request.algoPath << "\n";
        return 1;
    }
    if (!eraseOnly && !QFileInfo(request.firmwarePath).isFile()) {
        err() << "Firmware not found: " << request.firmwarePath << "\n";
        return 1;
    }

    int lastPercent = -1;
    QString lastStatus;

    const ProgramResult result = runProgramSession(
        request,
        [&lastPercent, &lastStatus](int percent, const QString &status) {
            /* Two steps can land on the same percentage - the first sector to
             * be erased shares its number with the initialisation that came
             * before it - so looking at the number alone swallowed the status
             * of whichever one came second. Both have to match to be a
             * repeat. */
            if (percent == lastPercent && status == lastStatus)
                return true;

            lastPercent = percent;
            lastStatus = status;
            out() << QStringLiteral("[%1%] %2\n").arg(percent, 3).arg(status);
            out().flush();
            return true;
        },
        [](const QString &message, bool isError) {
            QTextStream &stream = isError ? err() : out();
            stream << message << "\n";
            stream.flush();
        },
        []() { return false; });

    if (!result.ok) {
        err() << "\nFailed: " << result.message << "\n";
        return 1;
    }

    /* No summary line here: the worker already logged it through its note()
     * before it reported success, so printing result.message again made every
     * run end with that one line twice. A failure is the other way round -
     * that path has no note() of its own - which is why the branch above does
     * print. This mirrors what the graphical program does. */
    return 0;
}

int cliForwardToConsoleApp(const QStringList &arguments)
{
    const QDir dir(QCoreApplication::applicationDirPath());
    QString name = QFileInfo(QCoreApplication::applicationFilePath()).completeBaseName()
                   + QStringLiteral(" CLI");

#ifdef Q_OS_WIN
    name += QStringLiteral(".exe");
#endif

    const QString companion = dir.filePath(name);
    if (!QFileInfo(companion).isFile())
        return -1;

    QStringList forwarded = arguments;

    QProcess process;
    process.setProgram(companion);
    process.setArguments(forwarded);
    process.setProcessChannelMode(QProcess::ForwardedChannels);

    /* The companion is a console application, so a calling shell waits for it */
    process.start();
    if (!process.waitForStarted(5000))
        return -1;

    process.waitForFinished(-1);

    return process.exitStatus() == QProcess::NormalExit ? process.exitCode() : -1;
}
