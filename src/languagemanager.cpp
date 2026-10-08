/*
 * CH32H417 Logic Analyzer firmware upgrade tool
 *
 * UI language management (English / Simplified Chinese, follow-system option)
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "languagemanager.h"

#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include <QSettings>
#include <QWidget>
#include <QEvent>
#include <QDebug>

#ifdef Q_OS_WIN
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#endif

namespace {

const char *kSettingsOrg = "WeActStudio";
const char *kSettingsApp = "WeAct Studio DAP Flasher";
const char *kSettingsKey = "language";

/* The translations are compiled into the executable, no external file needed */
const char *kQmPrefix = ":/i18n/";

QString qmFileName(LanguageManager::Language lang)
{
    switch (lang) {
    case LanguageManager::Language::ChineseSimplified:
        return QStringLiteral("app_zh_CN.qm");
    case LanguageManager::Language::English:
    default:
        /* English is the source language, there is no app_en.qm */
        return QString();
    }
}

/** The language the operating system is set to, e.g. "zh-cn".
 *
 * QLocale::system() is not good enough for this on Windows: it honours the LANG
 * and LC_ALL environment variables, so a program started from a shell that
 * exports LANG=en_US.UTF-8 reports English even though Windows itself is set to
 * Chinese. GetUserDefaultLocaleName() always reports the Windows setting, which
 * is what "follow the system language" has to mean.
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

/** Map the system language onto a supported language; fall back to English */
LanguageManager::Language detectSystemLanguage()
{
    const QString name = systemLanguageName();

    /* Any Chinese variant (zh-CN, zh-TW, zh-Hans, ...) gets the Chinese UI:
       there is one Chinese translation covering both scripts. */
    if (name.startsWith(QLatin1String("zh")))
        return LanguageManager::Language::ChineseSimplified;

    return LanguageManager::Language::English;
}

} // namespace

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent)
    /* Follow the system language on the first run: a Chinese Windows gets the
       Chinese UI straight away. The choice made in the menu is remembered. */
    , preference_(Preference::Auto)
    , translator_(nullptr)
    , current_(Language::English)
{
}

LanguageManager::~LanguageManager()
{
    delete translator_;
}

LanguageManager *LanguageManager::instance()
{
    static LanguageManager *self = nullptr;
    if (!self) {
        self = new LanguageManager(qApp);
        self->loadFromSettings();
    }
    return self;
}

QStringList LanguageManager::preferenceNames()
{
    /* Same order as the Preference enum: Auto / English / ChineseSimplified */
    return QStringList()
           << QCoreApplication::translate("LanguageManager", "Follow System(&A)")
           << QCoreApplication::translate("LanguageManager", "English(&E)")
           << QCoreApplication::translate("LanguageManager", "Simplified Chinese(&C)");
}

LanguageManager::Language LanguageManager::resolvedLanguage() const
{
    if (preference_ == Preference::Auto)
        return detectSystemLanguage();

    return preference_ == Preference::ChineseSimplified
               ? Language::ChineseSimplified
               : Language::English;
}

QString LanguageManager::languageCode() const
{
    switch (resolvedLanguage()) {
    case Language::ChineseSimplified:
        return QStringLiteral("zh_CN");
    case Language::English:
    default:
        return QStringLiteral("en");
    }
}

QString LanguageManager::languageDisplayName() const
{
    switch (resolvedLanguage()) {
    case Language::ChineseSimplified:
        return QCoreApplication::translate("LanguageManager", "Simplified Chinese");
    case Language::English:
    default:
        return QCoreApplication::translate("LanguageManager", "English");
    }
}

void LanguageManager::loadFromSettings()
{
    QSettings settings(kSettingsOrg, kSettingsApp);
    /* No saved preference yet: follow the system language */
    const QString saved = settings.value(kSettingsKey, QStringLiteral("auto")).toString();

    if (saved.compare(QLatin1String("zh_CN"), Qt::CaseInsensitive) == 0
        || saved.compare(QLatin1String("zh"), Qt::CaseInsensitive) == 0)
        preference_ = Preference::ChineseSimplified;
    else if (saved.compare(QLatin1String("en"), Qt::CaseInsensitive) == 0
             || saved.compare(QLatin1String("en_US"), Qt::CaseInsensitive) == 0)
        preference_ = Preference::English;
    else
        preference_ = Preference::Auto;
}

void LanguageManager::saveToSettings()
{
    QSettings settings(kSettingsOrg, kSettingsApp);

    QString value;
    switch (preference_) {
    case Preference::ChineseSimplified:
        value = QStringLiteral("zh_CN");
        break;
    case Preference::English:
        value = QStringLiteral("en");
        break;
    case Preference::Auto:
    default:
        value = QStringLiteral("auto");
        break;
    }

    settings.setValue(kSettingsKey, value);
}

void LanguageManager::uninstallTranslator()
{
    if (translator_) {
        qApp->removeTranslator(translator_);
        delete translator_;
        translator_ = nullptr;
    }
}

bool LanguageManager::loadLanguage(Language lang)
{
    /* English is the source language of every UI string: nothing to load,
       just drop the Chinese translator if one is installed. */
    if (lang == Language::English) {
        uninstallTranslator();
        current_ = Language::English;
        return true;
    }

    QTranslator *translator = new QTranslator(qApp);
    const QString path = QString::fromLatin1(kQmPrefix) + qmFileName(lang);

    if (!translator->load(path)) {
        qWarning() << "Failed to load translation:" << path;
        delete translator;
        return false;
    }

    if (translator_)
        qApp->removeTranslator(translator_);

    qApp->installTranslator(translator);

    delete translator_;
    translator_ = translator;
    current_ = lang;

    return true;
}

void LanguageManager::apply()
{
    const Language target = resolvedLanguage();

    if (current_ != target || !translator_)
        loadLanguage(target);

    Q_EMIT languageChanged(current_);

    /* Tell every window that its texts must be retranslated */
    QWidgetList widgets = QApplication::topLevelWidgets();
    for (QWidget *widget : widgets)
        QApplication::postEvent(widget, new QEvent(QEvent::LanguageChange));

    /* Non top level widgets (embedded dialogs and the like) too */
    for (QWidget *widget : widgets) {
        QList<QWidget *> children = widget->findChildren<QWidget *>();
        for (QWidget *child : children)
            QApplication::postEvent(child, new QEvent(QEvent::LanguageChange));
    }
}

void LanguageManager::setPreference(Preference pref)
{
    if (preference_ == pref)
        return;

    preference_ = pref;
    saveToSettings();
    apply();
}
