/*
 * WeAct Studio DAP Flasher
 *
 * UI language management (English / Simplified Chinese / Russian / German /
 * Spanish, plus a follow-system option)
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>

class QTranslator;

/**
 * UI language management.
 *
 * - Five UI languages are available, plus "Follow System" (any unsupported
 *   system language falls back to English).
 * - The selection is stored in QSettings and restored on the next start.
 * - Switching the language posts a LanguageChange event to every widget, so
 *   the UI refreshes immediately without restarting.
 *
 * English is the source language of every string in the source code; the other
 * languages live in translations/app_<lang>.ts. Switching to English therefore
 * only requires removing the installed translator.
 */
class LanguageManager : public QObject
{
    Q_OBJECT

public:
    /** Language options shown in the menu */
    enum class Preference {
        Auto,               /**< Follow the system language */
        English,            /**< English */
        ChineseSimplified,  /**< Simplified Chinese */
        Russian,            /**< Russian */
        German,             /**< German */
        Spanish             /**< Spanish */
    };

    /** Effective language */
    enum class Language {
        English,            /**< en */
        ChineseSimplified,  /**< zh_CN */
        Russian,            /**< ru */
        German,             /**< de */
        Spanish             /**< es */
    };

    static LanguageManager *instance();

    /** Current preference (may be Auto) */
    Preference preference() const { return preference_; }

    /** Language currently in effect (Auto already resolved) */
    Language resolvedLanguage() const;

    /** Qt name of the effective language (en / zh_CN / ru / de / es) */
    QString languageCode() const;

    /** Localised name of the effective language, used in the menu */
    QString languageDisplayName() const;

    /** Available options, in the same order as the Preference enum */
    static QStringList preferenceNames();

    /** Store the preference and apply it */
    void setPreference(Preference pref);

    /** Reload the translation for the current setting and refresh the UI */
    void apply();

    /** Load the given translation; false on failure */
    bool loadLanguage(Language lang);

Q_SIGNALS:
    /** Emitted when the language changed, before the UI is refreshed */
    void languageChanged(Language language);

private:
    explicit LanguageManager(QObject *parent = nullptr);
    ~LanguageManager();

    void loadFromSettings();
    void saveToSettings();
    void uninstallTranslator();

    Preference preference_;
    QTranslator *translator_;      /**< Application translation */
    Language current_;
};

#endif /* LANGUAGEMANAGER_H */
