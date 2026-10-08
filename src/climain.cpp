/*
 * WeAct Studio DAP Flasher
 *
 * Console companion: the same command line interface, built as a Windows
 * console application so that a calling shell waits for it and can redirect
 * its output.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QCoreApplication>
#include <QLocale>
#include <QSettings>
#include <QStringList>
#include <QTranslator>

#ifdef Q_OS_WIN
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#endif

#include "cli.h"

namespace {

/** Language the operating system is set to, e.g. "zh-cn".
 *
 * Same rule as LanguageManager::systemLanguageName(): QLocale::system() follows
 * the LANG / LC_ALL environment variables on Windows, so a program started from
 * an MSYS2 shell would report English on a Chinese Windows. The console
 * companion cannot use LanguageManager itself, it links QtCore only.
 */
QString systemLanguageName()
{
#ifdef Q_OS_WIN
    wchar_t buffer[LOCALE_NAME_MAX_LENGTH] = { 0 };
    const int length = GetUserDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
    if (length > 1)
        return QString::fromWCharArray(buffer).toLower();
#endif

    return QLocale::system().name().toLower();
}

bool wantsChinese(const QStringList &arguments)
{
    /* --lang=<x> applies to this run only. A command line switch must not
     * rewrite the language the graphical program was set to, and it must not
     * save anything: the preference is read, never written, here. */
    for (int i = 0; i < arguments.size(); ++i) {
        const QString argument = arguments.at(i);
        if (!argument.startsWith(QLatin1String("--lang")))
            continue;

        const QString value = argument.contains(QLatin1Char('='))
                                  ? argument.section(QLatin1Char('='), 1)
                                  : (i + 1 < arguments.size() ? arguments.at(++i) : QString());

        if (value.compare(QLatin1String("en"), Qt::CaseInsensitive) == 0)
            return false;
        if (value.compare(QLatin1String("zh"), Qt::CaseInsensitive) == 0
            || value.compare(QLatin1String("zh_CN"), Qt::CaseInsensitive) == 0)
            return true;
        break;   /* auto or unknown: fall through to the saved preference */
    }

    /* The preference the graphical program remembers */
    const QSettings settings(QStringLiteral("WeActStudio"),
                             QStringLiteral("WeAct Studio DAP Flasher"));
    const QString saved = settings.value(QStringLiteral("language"),
                                         QStringLiteral("auto")).toString();

    if (saved.compare(QLatin1String("zh_CN"), Qt::CaseInsensitive) == 0
        || saved.compare(QLatin1String("zh"), Qt::CaseInsensitive) == 0)
        return true;
    if (saved.compare(QLatin1String("en"), Qt::CaseInsensitive) == 0
        || saved.compare(QLatin1String("en_US"), Qt::CaseInsensitive) == 0)
        return false;

    return systemLanguageName().startsWith(QLatin1String("zh"));
}

/** Install the compiled Chinese translation; English is the source language */
void installTranslator(const QStringList &arguments)
{
    if (!wantsChinese(arguments))
        return;

    QTranslator *translator = new QTranslator(qApp);
    if (translator->load(QStringLiteral(":/i18n/app_zh_CN.qm")))
        qApp->installTranslator(translator);
    else
        delete translator;
}

} /* namespace */

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName(QStringLiteral("WeAct Studio DAP Flasher CLI"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.0.0"));
    QCoreApplication::setOrganizationName(QStringLiteral("WeActStudio"));

    QStringList arguments;
    for (int i = 1; i < argc; ++i)
        arguments << QString::fromLocal8Bit(argv[i]);

    if (arguments.isEmpty()) {
        arguments << QStringLiteral("--help");
    }

    installTranslator(arguments);

    return cliRun(arguments);
}
