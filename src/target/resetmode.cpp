/*
 * WeAct Studio DAP Flasher
 *
 * How the target is reset.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "resetmode.h"

#include <QCoreApplication>

QString resetModeName(ResetMode mode)
{
    switch (mode) {
    case ResetMode::Hardware:
        return QCoreApplication::translate("ResetMode", "Hardware reset");
    case ResetMode::Auto:
        return QCoreApplication::translate("ResetMode", "Automatic");
    case ResetMode::Software:
        break;
    }

    return QCoreApplication::translate("ResetMode", "Software reset");
}

QString resetModeKey(ResetMode mode)
{
    switch (mode) {
    case ResetMode::Hardware:
        return QStringLiteral("hardware");
    case ResetMode::Auto:
        return QStringLiteral("auto");
    case ResetMode::Software:
        break;
    }

    return QStringLiteral("software");
}

bool resetModeFromKey(const QString &key, ResetMode &mode)
{
    const QString clean = key.trimmed().toLower();

    if (clean == QLatin1String("software") || clean == QLatin1String("sw")) {
        mode = ResetMode::Software;
        return true;
    }
    if (clean == QLatin1String("hardware") || clean == QLatin1String("hw")
        || clean == QLatin1String("pin")) {
        mode = ResetMode::Hardware;
        return true;
    }
    if (clean == QLatin1String("auto")) {
        mode = ResetMode::Auto;
        return true;
    }

    return false;
}
