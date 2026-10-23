/*
 * WeAct Studio DAP Flasher
 *
 * Program entry point
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QApplication>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QIcon>
#include <QLocale>
#include <QStringList>
#include <QStyleHints>
#include <QTimer>
#include <QWidget>

#include "cli.h"
#include "languagemanager.h"
#include "mainwindow.h"
#include "thememanager.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName(QStringLiteral("WeAct Studio DAP Flasher"));
    QApplication::setApplicationVersion(QStringLiteral("1.0.0"));
    QApplication::setOrganizationName(QStringLiteral("WeActStudio"));

    QStringList arguments;
    for (int i = 1; i < argc; ++i)
        arguments << QString::fromLocal8Bit(argv[i]);

    /*
     * Any switch runs the tool without a window and returns a process exit
     * code. The work is handed over to the console companion when it is
     * installed next to this program: PowerShell does not wait for a Windows
     * GUI application, but it does wait for that one.
     */
    if (cliRequested(arguments)) {
        const int forwarded = cliForwardToConsoleApp(arguments);
        if (forwarded >= 0)
            return forwarded;

        return cliRun(arguments);
    }

    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/logo")));

    /* UI language: English is the source language, the Chinese text comes from
     * the compiled app_zh_CN.qm that is packed into the executable. */
    LanguageManager *langMgr = LanguageManager::instance();

    for (int i = 0; i < arguments.size(); ++i) {
        const QString argument = arguments.at(i);
        if (!argument.startsWith(QLatin1String("--lang")))
            continue;

        const QString value = argument.contains(QLatin1Char('='))
                                  ? argument.section(QLatin1Char('='), 1)
                                  : (i + 1 < arguments.size() ? arguments.at(++i) : QString());

        if (value.compare(QLatin1String("en"), Qt::CaseInsensitive) == 0)
            langMgr->setPreference(LanguageManager::Preference::English);
        else if (value.compare(QLatin1String("zh_CN"), Qt::CaseInsensitive) == 0
                 || value.compare(QLatin1String("zh"), Qt::CaseInsensitive) == 0)
            langMgr->setPreference(LanguageManager::Preference::ChineseSimplified);
        else if (value.compare(QLatin1String("ru"), Qt::CaseInsensitive) == 0)
            langMgr->setPreference(LanguageManager::Preference::Russian);
        else if (value.compare(QLatin1String("de"), Qt::CaseInsensitive) == 0)
            langMgr->setPreference(LanguageManager::Preference::German);
        else if (value.compare(QLatin1String("es"), Qt::CaseInsensitive) == 0)
            langMgr->setPreference(LanguageManager::Preference::Spanish);
        else
            langMgr->setPreference(LanguageManager::Preference::Auto);
        break;
    }
    langMgr->apply();

    /* UI theme: follows the system light/dark setting by default */
    ThemeManager *themeMgr = ThemeManager::instance();

    for (const QString &argument : arguments) {
        if (!argument.startsWith(QLatin1String("--theme")))
            continue;

        const QString value = argument.contains(QLatin1Char('='))
                                  ? argument.section(QLatin1Char('='), 1).toLower()
                                  : QString();

        if (value == QLatin1String("light"))
            themeMgr->setMode(ThemeManager::Mode::Light);
        else if (value == QLatin1String("dark"))
            themeMgr->setMode(ThemeManager::Mode::Dark);
        else
            themeMgr->setMode(ThemeManager::Mode::Auto);
        break;
    }

    themeMgr->apply();

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    QObject::connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
                     themeMgr, [themeMgr](Qt::ColorScheme) { themeMgr->apply(); });
#endif

    MainWindow window;
    window.show();

    /* Automation helper: --screenshot=<file> grabs the window once it is shown */
    for (const QString &argument : arguments) {
        if (!argument.startsWith(QLatin1String("--screenshot=")))
            continue;

        const QString path = argument.section(QLatin1Char('='), 1);
        QTimer::singleShot(1500, &window, [&window, path]() {
            QWidget *target = QApplication::activeWindow();
            if (!target)
                target = &window;
            target->grab().save(path);
        });
        break;
    }



    /* Automation helper: --about opens the About dialog */
    for (const QString &argument : arguments) {
        if (argument != QLatin1String("--about"))
            continue;
        QTimer::singleShot(800, &window, [&window]() { window.showAbout(); });
        break;
    }

    return app.exec();
}
