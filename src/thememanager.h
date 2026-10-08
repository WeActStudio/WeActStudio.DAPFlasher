/*
 * CH32H417 Logic Analyzer firmware upgrade tool
 *
 * Theme management (light / dark, follow the system setting)
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QPalette>

/**
 * UI theme management.
 *
 * - Qt 6.5 and newer: follows the system light/dark switch live through
 *   QStyleHints::colorSchemeChanged.
 * - Qt 5 (fallback): reads the Windows registry value AppsUseLightTheme once
 *   at start-up.
 *
 * Dark mode uses the Fusion style plus a custom dark QPalette so that it also
 * renders correctly in a static build (which normally only ships the
 * windowsvista style plugin, and the native style does not necessarily follow
 * the system dark setting).
 */
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    enum class Mode {
        Auto,   /* Follow the system */
        Light,  /* Force light */
        Dark    /* Force dark */
    };

    static ThemeManager *instance();

    Mode mode() const { return mode_; }

    /** Whether dark mode is actually in effect */
    bool isDark() const;

    /** Whether the system prefers dark, independent of the user setting */
    static bool systemPrefersDark();

    /** Name shown in the menu (follows the UI language) */
    static QString modeName(Mode mode);

    /** Apply the current theme (also called after a language / system change) */
    void apply();

public Q_SLOTS:
    void setMode(Mode mode);

Q_SIGNALS:
    void themeChanged();

private:
    explicit ThemeManager(QObject *parent = nullptr);

    bool effectiveDark() const;
    void loadFromSettings();
    void saveToSettings();

    Mode mode_;
    bool appliedDark_;
};

#endif // THEMEMANAGER_H
