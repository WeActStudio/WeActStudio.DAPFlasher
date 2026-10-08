/*
 * WeAct Studio DAP Flasher
 *
 * Main window implementation
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mainwindow.h"

#include "languagemanager.h"
#include "thememanager.h"

#include "dap/cmsisdap.h"
#include "dap/dapdefs.h"
#include "flash/algocatalog.h"
#include "flashworker.h"
#include "firmwareimage.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QMimeData>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QThread>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

namespace {

const char *kSettingsOrg = "WeActStudio";
const char *kSettingsApp = "WeAct Studio DAP Flasher";

/* Clock presets offered in the dropdown (label, Hz) */
struct ClockPreset
{
    const char *label;
    quint32 hz;
};

const ClockPreset kClockPresets[] = {
    { "100 kHz", 100000u },
    { "500 kHz", 500000u },
    { "1 MHz",   1000000u },
    { "2 MHz",   2000000u },
    { "4 MHz",   4000000u },
    { "8 MHz",   8000000u },
    { "10 MHz",  10000000u }
};

/* Smallest usable window: the rows that wrap need this much height */
const int kMinimumWindowWidth = 720;
const int kMinimumWindowHeight = 800;

const quint32 kDefaultClockHz = 1000000u;
const quint32 kDefaultRamBase = 0x20000000u;
const quint32 kDefaultBinBase = 0x08000000u;

QString hex32(quint32 value)
{
    return QStringLiteral("0x%1").arg(value, 8, 16, QLatin1Char('0'));
}

/** "08000000" from 0x08000000 */
QString hexDigits(quint32 value)
{
    return QString::number(value, 16).rightJustified(8, QLatin1Char('0')).toUpper();
}

} /* namespace */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , probeCombo_(nullptr)
    , refreshButton_(nullptr)
    , probeStatusLabel_(nullptr)
    , probeStatusTitleLabel_(nullptr)
    , targetCoreLabel_(nullptr)
    , targetCoreTitleLabel_(nullptr)
    , algoPicker_(nullptr)
    , algoBrowseButton_(nullptr)
    , algoInfoLabel_(nullptr)
    , algoInfoTitleLabel_(nullptr)
    , fileEdit_(nullptr)
    , fileBrowseButton_(nullptr)
    , firmwareInfoLabel_(nullptr)
    , firmwareInfoTitleLabel_(nullptr)
    , baseAddressEdit_(nullptr)
    , baseAddressLabel_(nullptr)
    , clockCombo_(nullptr)
    , ramBaseEdit_(nullptr)
    , eraseChipCheck_(nullptr)
    , verifyCheck_(nullptr)
    , resetRunCheck_(nullptr)
    , resetModeCombo_(nullptr)
    , clockLabel_(nullptr)
    , ramBaseLabel_(nullptr)
    , resetModeLabel_(nullptr)
    , progressBar_(nullptr)
    , progressLabel_(nullptr)
    , logView_(nullptr)
    , logTitleLabel_(nullptr)
    , startButton_(nullptr)
    , haltButton_(nullptr)
    , runButton_(nullptr)
    , resetButton_(nullptr)
    , exitButton_(nullptr)
    , fileMenu_(nullptr)
    , toolMenu_(nullptr)
    , viewMenu_(nullptr)
    , langMenu_(nullptr)
    , helpMenu_(nullptr)
    , openAction_(nullptr)
    , openAlgoAction_(nullptr)
    , exitAction_(nullptr)
    , refreshAction_(nullptr)
    , reloadAlgoAction_(nullptr)
    , showAllHidAction_(nullptr)
    , clearLogAction_(nullptr)
    , aboutAction_(nullptr)
    , langActionGroup_(nullptr)
    , themeActionGroup_(nullptr)
    , probeGroup_(nullptr)
    , algoGroup_(nullptr)
    , firmwareGroup_(nullptr)
    , optionGroup_(nullptr)
    , probeListLabel_(nullptr)
    , filePathLabel_(nullptr)
    , targetIdentified_(false)
    , worker_(nullptr)
    , workerThread_(nullptr)
    , algoScanThread_(nullptr)
    , identifyThread_(nullptr)
    , flashing_(false)
    , cancelled_(false)
    , progressState_(ProgressState::Idle)
{
    setupUi();
    createMenus();
    retranslateUi();
    loadSettings();

    setAcceptDrops(true);

    /* The probe list and the algorithms are enumerated once at start-up; a
     * probe list is never polled in the background, because opening a probe
     * that a debug session holds would disturb that session. */
    QTimer::singleShot(120, this, &MainWindow::onRefreshProbes);

    /* Walking the CMSIS packs takes a moment, so the list is filled from the
     * cache first and refreshed in the background. */
    QTimer::singleShot(0, this, &MainWindow::loadAlgorithmCatalogue);
}

MainWindow::~MainWindow()
{
    if (workerThread_ && workerThread_->isRunning()) {
        if (worker_)
            worker_->cancel();
        workerThread_->quit();
        if (!workerThread_->wait(3000)) {
            workerThread_->terminate();
            workerThread_->wait();
        }
    }

    /* The catalogue scan is read-only, but it must not touch the window after
     * the window is gone. */
    if (algoScanThread_ && algoScanThread_->isRunning()) {
        algoScanThread_->requestInterruption();
        algoScanThread_->wait(5000);
    }

    /* A start-up identify can still be inside attachSwd()'s recovery ladder
     * when the window is closed - that is the case it exists for. It holds a
     * probe open and would post its answer to a destroyed window, so wait for
     * it. It has no interruption point of its own; the budget is attachSwd's
     * power-up poll. */
    if (identifyThread_ && identifyThread_->isRunning())
        identifyThread_->wait(8000);

    dapProbesShutdown();
}

/* ==========================================================================
 * UI construction
 * ========================================================================== */

void MainWindow::setupUi()
{
    /* The height has to be enough for the rows that wrap (the probe status and
       the algorithm description, which runs over two lines): a window that is a
       few pixels short makes the layout squeeze one of the group boxes, and the
       wrapped text loses its last line. This also lifts a geometry that was
       saved by an older build up to a usable height. */
    setMinimumSize(kMinimumWindowWidth, kMinimumWindowHeight);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(10);

    /* ---------- Probe section ---------- */
    probeGroup_ = new QGroupBox(central);
    QFormLayout *probeLayout = new QFormLayout(probeGroup_);

    QHBoxLayout *probeRow = new QHBoxLayout();
    probeCombo_ = new QComboBox(probeGroup_);
    probeCombo_->setMinimumWidth(360);
    refreshButton_ = new QPushButton(probeGroup_);
    probeRow->addWidget(probeCombo_, 1);
    probeRow->addWidget(refreshButton_);

    probeListLabel_ = new QLabel(probeGroup_);
    probeLayout->addRow(probeListLabel_, probeRow);

    probeStatusLabel_ = new WrappedLabel(probeGroup_);
    probeStatusTitleLabel_ = new QLabel(probeGroup_);
    probeLayout->addRow(probeStatusTitleLabel_, probeStatusLabel_);

    targetCoreLabel_ = new QLabel(probeGroup_);
    targetCoreTitleLabel_ = new QLabel(probeGroup_);
    probeLayout->addRow(targetCoreTitleLabel_, targetCoreLabel_);

    mainLayout->addWidget(probeGroup_);

    /* ---------- Algorithm section ---------- */
    algoGroup_ = new QGroupBox(central);
    QFormLayout *algoLayout = new QFormLayout(algoGroup_);

    QHBoxLayout *algoRow = new QHBoxLayout();
    algoPicker_ = new AlgorithmPicker(algoGroup_);
    algoBrowseButton_ = new QPushButton(algoGroup_);
    algoRow->addWidget(algoPicker_, 1);
    algoRow->addWidget(algoBrowseButton_);

    algoInfoTitleLabel_ = new QLabel(algoGroup_);
    algoLayout->addRow(algoInfoTitleLabel_, algoRow);

    algoInfoLabel_ = new WrappedLabel(algoGroup_);
    algoLayout->addRow(QString(), algoInfoLabel_);

    mainLayout->addWidget(algoGroup_);

    /* ---------- Firmware section ---------- */
    firmwareGroup_ = new QGroupBox(central);
    QFormLayout *fwLayout = new QFormLayout(firmwareGroup_);

    QHBoxLayout *fileRow = new QHBoxLayout();
    fileEdit_ = new QLineEdit(firmwareGroup_);
    fileBrowseButton_ = new QPushButton(firmwareGroup_);
    fileRow->addWidget(fileEdit_, 1);
    fileRow->addWidget(fileBrowseButton_);

    filePathLabel_ = new QLabel(firmwareGroup_);
    fwLayout->addRow(filePathLabel_, fileRow);

    firmwareInfoLabel_ = new WrappedLabel(firmwareGroup_);
    firmwareInfoTitleLabel_ = new QLabel(firmwareGroup_);
    fwLayout->addRow(firmwareInfoTitleLabel_, firmwareInfoLabel_);

    baseAddressEdit_ = new QLineEdit(firmwareGroup_);
    baseAddressEdit_->setMaximumWidth(120);
    baseAddressLabel_ = new QLabel(firmwareGroup_);

    QHBoxLayout *baseRow = new QHBoxLayout();
    baseRow->addWidget(baseAddressLabel_);
    baseRow->addWidget(baseAddressEdit_);
    baseRow->addStretch(1);
    fwLayout->addRow(baseRow);

    mainLayout->addWidget(firmwareGroup_);

    /* ---------- Options ---------- */
    optionGroup_ = new QGroupBox(central);
    QVBoxLayout *optionLayout = new QVBoxLayout(optionGroup_);

    QHBoxLayout *optionRow1 = new QHBoxLayout();

    clockLabel_ = new QLabel(optionGroup_);
    clockCombo_ = new QComboBox(optionGroup_);
    for (const ClockPreset &preset : kClockPresets)
        clockCombo_->addItem(QLatin1String(preset.label), preset.hz);

    ramBaseLabel_ = new QLabel(optionGroup_);
    ramBaseEdit_ = new QLineEdit(optionGroup_);
    ramBaseEdit_->setMaximumWidth(110);

    eraseChipCheck_ = new QCheckBox(optionGroup_);
    verifyCheck_ = new QCheckBox(optionGroup_);

    optionRow1->addWidget(clockLabel_);
    optionRow1->addWidget(clockCombo_);
    optionRow1->addSpacing(16);
    optionRow1->addWidget(ramBaseLabel_);
    optionRow1->addWidget(ramBaseEdit_);
    optionRow1->addStretch(1);
    optionRow1->addWidget(eraseChipCheck_);
    optionRow1->addWidget(verifyCheck_);
    optionLayout->addLayout(optionRow1);

    /* The reset mode decides what the Reset button does and what happens
     * after programming, so it gets a row of its own instead of squeezing
     * the row above. */
    QHBoxLayout *optionRow2 = new QHBoxLayout();

    resetModeLabel_ = new QLabel(optionGroup_);
    resetModeCombo_ = new QComboBox(optionGroup_);
    /* The texts are filled in by retranslateUi(); the data is the mode */
    resetModeCombo_->addItem(QString(), static_cast<int>(ResetMode::Software));
    resetModeCombo_->addItem(QString(), static_cast<int>(ResetMode::Hardware));
    resetModeCombo_->addItem(QString(), static_cast<int>(ResetMode::Auto));

    resetRunCheck_ = new QCheckBox(optionGroup_);

    optionRow2->addWidget(resetModeLabel_);
    optionRow2->addWidget(resetModeCombo_);
    optionRow2->addStretch(1);
    optionRow2->addWidget(resetRunCheck_);
    optionLayout->addLayout(optionRow2);

    mainLayout->addWidget(optionGroup_);

    /* ---------- Progress ---------- */
    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressBar_ = new QProgressBar(central);
    progressBar_->setRange(0, 100);
    progressBar_->setValue(0);
    progressBar_->setTextVisible(true);
    progressLabel_ = new QLabel(central);
    progressLabel_->setMinimumWidth(220);
    progressLayout->addWidget(progressBar_, 1);
    progressLayout->addWidget(progressLabel_);
    mainLayout->addLayout(progressLayout);

    /* ---------- Log ---------- */
    logTitleLabel_ = new QLabel(central);
    mainLayout->addWidget(logTitleLabel_);

    logView_ = new QTextEdit(central);
    logView_->setReadOnly(true);
    QFont monoFont(QStringLiteral("Consolas"), 9);
    monoFont.setStyleHint(QFont::Monospace);
    logView_->setFont(monoFont);
    /* Keep this small enough that the layout does not have to squeeze another
       row when the window is at its default size; the view expands anyway. */
    logView_->setMinimumHeight(140);
    mainLayout->addWidget(logView_, 1);

    /* ---------- Buttons ---------- */
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    haltButton_ = new QPushButton(central);
    runButton_ = new QPushButton(central);
    resetButton_ = new QPushButton(central);
    startButton_ = new QPushButton(central);
    exitButton_ = new QPushButton(central);

    buttonLayout->addWidget(haltButton_);
    buttonLayout->addWidget(runButton_);
    buttonLayout->addWidget(resetButton_);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(startButton_);
    buttonLayout->addWidget(exitButton_);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(central);

    connect(refreshButton_, &QPushButton::clicked, this, &MainWindow::onRefreshProbes);
    connect(probeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onProbeChanged);
    connect(algoBrowseButton_, &QPushButton::clicked, this, &MainWindow::onBrowseAlgorithm);
    connect(algoPicker_, &AlgorithmPicker::changed, this, &MainWindow::onAlgorithmChanged);
    connect(fileBrowseButton_, &QPushButton::clicked, this, &MainWindow::onBrowseFirmware);
    connect(fileEdit_, &QLineEdit::textChanged, this, &MainWindow::onFirmwarePathChanged);
    connect(baseAddressEdit_, &QLineEdit::editingFinished, this, &MainWindow::onFirmwarePathChanged);
    connect(startButton_, &QPushButton::clicked, this, &MainWindow::onStartFlash);
    connect(exitButton_, &QPushButton::clicked, this, &MainWindow::close);
    connect(haltButton_, &QPushButton::clicked, this, &MainWindow::onHaltTarget);
    connect(runButton_, &QPushButton::clicked, this, &MainWindow::onRunTarget);
    connect(resetButton_, &QPushButton::clicked, this, &MainWindow::onResetTarget);
    connect(eraseChipCheck_, &QCheckBox::toggled, this, [this]() { updateAlgorithmInfo(); });
}

void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(QString());
    openAction_ = new QAction(this);
    openAlgoAction_ = new QAction(this);
    exitAction_ = new QAction(this);
    fileMenu_->addAction(openAction_);
    fileMenu_->addAction(openAlgoAction_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAction_);
    connect(openAction_, &QAction::triggered, this, &MainWindow::onBrowseFirmware);
    connect(openAlgoAction_, &QAction::triggered, this, &MainWindow::onBrowseAlgorithm);
    connect(exitAction_, &QAction::triggered, this, &MainWindow::close);

    toolMenu_ = menuBar()->addMenu(QString());
    refreshAction_ = new QAction(this);
    reloadAlgoAction_ = new QAction(this);
    showAllHidAction_ = new QAction(this);
    showAllHidAction_->setCheckable(true);
    clearLogAction_ = new QAction(this);
    toolMenu_->addAction(refreshAction_);
    toolMenu_->addAction(reloadAlgoAction_);
    toolMenu_->addSeparator();
    toolMenu_->addAction(showAllHidAction_);
    toolMenu_->addSeparator();
    toolMenu_->addAction(clearLogAction_);
    connect(refreshAction_, &QAction::triggered, this, &MainWindow::onRefreshProbes);
    connect(reloadAlgoAction_, &QAction::triggered, this, [this]() { refreshAlgorithmList(); });
    connect(showAllHidAction_, &QAction::toggled, this, [this]() { refreshProbeList(); });
    connect(clearLogAction_, &QAction::triggered, this, &MainWindow::onClearLog);

    /* ---------- Theme menu ---------- */
    viewMenu_ = menuBar()->addMenu(QString());
    themeActionGroup_ = new QActionGroup(this);
    themeActionGroup_->setExclusive(true);

    for (int i = 0; i < 3; ++i) {
        QAction *action = new QAction(this);
        action->setCheckable(true);
        action->setData(i);
        themeActionGroup_->addAction(action);
        viewMenu_->addAction(action);
        themeActions_.append(action);
    }
    connect(themeActionGroup_, &QActionGroup::triggered,
            this, &MainWindow::onThemeActionTriggered);
    connect(ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &MainWindow::onThemeChanged);

    /* ---------- Language menu ---------- */
    langMenu_ = menuBar()->addMenu(QString());
    langActionGroup_ = new QActionGroup(this);
    langActionGroup_->setExclusive(true);

    for (int i = 0; i < LanguageManager::preferenceNames().size(); ++i) {
        QAction *action = new QAction(this);
        action->setCheckable(true);
        action->setData(i);
        langActionGroup_->addAction(action);
        langMenu_->addAction(action);
        langActions_.append(action);
    }
    connect(langActionGroup_, &QActionGroup::triggered,
            this, &MainWindow::onLanguageActionTriggered);

    helpMenu_ = menuBar()->addMenu(QString());
    aboutAction_ = new QAction(this);
    helpMenu_->addAction(aboutAction_);
    connect(aboutAction_, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("WeAct Studio DAP Flasher"));

    probeGroup_->setTitle(tr("Debug Probe"));
    algoGroup_->setTitle(tr("Flash Algorithm"));
    firmwareGroup_->setTitle(tr("Firmware File"));
    optionGroup_->setTitle(tr("Options"));

    probeListLabel_->setText(tr("Probe:"));
    probeStatusTitleLabel_->setText(tr("Connection:"));
    targetCoreTitleLabel_->setText(tr("Target:"));
    algoInfoTitleLabel_->setText(tr("Algorithm File:"));
    filePathLabel_->setText(tr("File Path:"));
    firmwareInfoTitleLabel_->setText(tr("Firmware:"));
    baseAddressLabel_->setText(tr("BIN Load Address: 0x"));
    clockLabel_->setText(tr("SWD Clock:"));
    ramBaseLabel_->setText(tr("Algorithm RAM: 0x"));
    resetModeLabel_->setText(tr("Reset Mode:"));

    refreshButton_->setText(tr("Refresh(&R)"));
    algoBrowseButton_->setText(tr("Browse(&B)..."));
    fileBrowseButton_->setText(tr("Browse(&B)..."));

    eraseChipCheck_->setText(tr("Whole chip erase"));
    verifyCheck_->setText(tr("Verify"));
    resetRunCheck_->setText(tr("Reset and run"));

    /* The mode names are translated, so the entries are refilled whenever the
     * language changes; the item data keeps the selection in place. */
    for (int i = 0; i < resetModeCombo_->count(); ++i) {
        const ResetMode mode =
            static_cast<ResetMode>(resetModeCombo_->itemData(i).toInt());
        resetModeCombo_->setItemText(i, resetModeName(mode));
    }

    startButton_->setText(flashing_ ? tr("Cancel(&C)") : tr("Start Programming(&S)"));
    haltButton_->setText(tr("Halt(&P)"));
    runButton_->setText(tr("Run(&R)"));
    resetButton_->setText(tr("Reset(&T)"));
    exitButton_->setText(tr("Exit(&X)"));

    fileEdit_->setPlaceholderText(tr("Select or drop a .hex / .bin firmware file"));
    /* The picker keeps the full path as its own tool tip, which is more useful
     * than a general explanation of what the control does. */
    algoPicker_->retranslate();
    ramBaseEdit_->setToolTip(tr("RAM area the flash algorithm is copied to while it runs. "
                                "0x20000000 is correct for most Cortex-M devices."));
    baseAddressEdit_->setToolTip(tr("Loading address used for .bin files (ignored for .hex)"));
    resetModeCombo_->setToolTip(tr("Reset used by the Reset button and by \"Reset and run\". "
                                   "Software reset needs no extra wire, hardware reset pulses the "
                                   "nRESET line, automatic uses nRESET when the probe has one."));

    logTitleLabel_->setText(tr("Log"));

    if (fileMenu_) {
        fileMenu_->setTitle(tr("File(&F)"));
        openAction_->setText(tr("Open Firmware(&O)..."));
        openAlgoAction_->setText(tr("Select Algorithm(&A)..."));
        exitAction_->setText(tr("Exit(&X)"));

        toolMenu_->setTitle(tr("Tools(&T)"));
        refreshAction_->setText(tr("Refresh Probes(&R)"));
        reloadAlgoAction_->setText(tr("Rescan Algorithms(&L)"));
        showAllHidAction_->setText(tr("Show All USB HID Devices(&H)"));
        clearLogAction_->setText(tr("Clear Log(&C)"));

        viewMenu_->setTitle(tr("View(&V)"));
        langMenu_->setTitle(tr("Language(&L)"));
        helpMenu_->setTitle(tr("Help(&H)"));
        aboutAction_->setText(tr("About(&A)"));

        const QStringList names = LanguageManager::preferenceNames();
        for (int i = 0; i < langActions_.size() && i < names.size(); ++i)
            langActions_.at(i)->setText(names.at(i));

        for (int i = 0; i < themeActions_.size() && i < 3; ++i) {
            themeActions_.at(i)->setText(
                ThemeManager::modeName(static_cast<ThemeManager::Mode>(i)));
        }
    }

    /* The group captions of the algorithm list are translated, so the list has
     * to be rebuilt when the language changes */
    if (!algoCatalog_.isEmpty())
        populateAlgorithmPicker(algoPicker_->currentPath());

    updateProbeStatusText();
    updateAlgorithmInfo();
    updateFirmwareInfo();
    updateProgressText();
}

/* ==========================================================================
 * Language and theme
 * ========================================================================== */

void MainWindow::onLanguageActionTriggered(QAction *action)
{
    if (!action)
        return;

    LanguageManager::instance()->setPreference(
        static_cast<LanguageManager::Preference>(action->data().toInt()));
}

void MainWindow::onLanguageChanged()
{
    const int index = static_cast<int>(LanguageManager::instance()->preference());
    for (int i = 0; i < langActions_.size(); ++i)
        langActions_.at(i)->setChecked(i == index);
}

void MainWindow::onThemeActionTriggered(QAction *action)
{
    if (!action)
        return;

    ThemeManager::instance()->setMode(static_cast<ThemeManager::Mode>(action->data().toInt()));
}

void MainWindow::onThemeChanged()
{
    const int index = static_cast<int>(ThemeManager::instance()->mode());
    for (int i = 0; i < themeActions_.size(); ++i)
        themeActions_.at(i)->setChecked(i == index);

    renderLog();
    updateProbeStatusText();
    updateAlgorithmInfo();
    updateFirmwareInfo();
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();

    QMainWindow::changeEvent(event);
}

/* ==========================================================================
 * Settings
 * ========================================================================== */

void MainWindow::loadSettings()
{
    QSettings settings(QString::fromLatin1(kSettingsOrg), QString::fromLatin1(kSettingsApp));

    showAllHidAction_->setChecked(settings.value(QStringLiteral("showAllHid"), false).toBool());

    fileEdit_->setText(settings.value(QStringLiteral("firmwarePath")).toString());

    bool ok = false;
    quint32 value = settings.value(QStringLiteral("binBaseAddress"),
                                   hexDigits(kDefaultBinBase)).toString().toUInt(&ok, 16);
    baseAddressEdit_->setText(hexDigits(ok ? value : kDefaultBinBase));

    value = settings.value(QStringLiteral("ramBase"),
                           hexDigits(kDefaultRamBase)).toString().toUInt(&ok, 16);
    ramBaseEdit_->setText(hexDigits(ok ? value : kDefaultRamBase));

    value = settings.value(QStringLiteral("clockHz"), kDefaultClockHz).toUInt();
    int clockIndex = 2;
    for (int i = 0; i < clockCombo_->count(); ++i) {
        if (clockCombo_->itemData(i).toUInt() == value) {
            clockIndex = i;
            break;
        }
    }
    clockCombo_->setCurrentIndex(clockIndex);

    eraseChipCheck_->setChecked(settings.value(QStringLiteral("eraseWholeChip"), false).toBool());
    verifyCheck_->setChecked(settings.value(QStringLiteral("verify"), true).toBool());
    resetRunCheck_->setChecked(settings.value(QStringLiteral("resetAndRun"), true).toBool());

    /* Stored as a word, so that a settings file stays readable and an unknown
     * value simply falls back to the software reset. */
    ResetMode mode = ResetMode::Software;
    resetModeFromKey(settings.value(QStringLiteral("resetMode"),
                                    resetModeKey(ResetMode::Software)).toString(), mode);
    const int modeIndex = resetModeCombo_->findData(static_cast<int>(mode));
    resetModeCombo_->setCurrentIndex(modeIndex >= 0 ? modeIndex : 0);

    restoreGeometry(settings.value(QStringLiteral("geometry")).toByteArray());

    onLanguageChanged();
    onThemeChanged();
}

void MainWindow::saveSettings()
{
    QSettings settings(QString::fromLatin1(kSettingsOrg), QString::fromLatin1(kSettingsApp));

    settings.setValue(QStringLiteral("showAllHid"), showAllHidAction_->isChecked());
    settings.setValue(QStringLiteral("firmwarePath"), fileEdit_->text());
    settings.setValue(QStringLiteral("algoPath"), algoPicker_->currentPath());
    settings.setValue(QStringLiteral("binBaseAddress"), baseAddressEdit_->text().trimmed());
    settings.setValue(QStringLiteral("ramBase"), ramBaseEdit_->text().trimmed());
    settings.setValue(QStringLiteral("clockHz"), clockFrequency());
    settings.setValue(QStringLiteral("eraseWholeChip"), eraseChipCheck_->isChecked());
    settings.setValue(QStringLiteral("verify"), verifyCheck_->isChecked());
    settings.setValue(QStringLiteral("resetAndRun"), resetRunCheck_->isChecked());
    settings.setValue(QStringLiteral("resetMode"), resetModeKey(resetMode()));
    settings.setValue(QStringLiteral("geometry"), saveGeometry());
}

/* ==========================================================================
 * Probes
 * ========================================================================== */

const DapProbeInfo *MainWindow::currentProbe() const
{
    const int index = probeCombo_->currentIndex();
    if (!probeCombo_->isEnabled() || index < 0 || index >= probes_.size())
        return nullptr;

    return &probes_.at(index);
}

void MainWindow::refreshProbeList()
{
    QString error;
    const QList<DapProbeInfo> found = dapEnumerateProbes(showAllHidAction_->isChecked(), &error);

    /* Remember the selection so that a refresh does not move the user away */
    QString selectedKey;
    if (const DapProbeInfo *current = currentProbe())
        selectedKey = current->key();

    probes_ = found;

    probeCombo_->blockSignals(true);
    probeCombo_->clear();

    if (probes_.isEmpty()) {
        probeCombo_->addItem(tr("(no probe found)"));
        probeCombo_->setEnabled(false);
        probeCombo_->setToolTip(error);
    } else {
        for (const DapProbeInfo &probe : probes_) {
            probeCombo_->addItem(probe.displayName());
            probeCombo_->setItemData(probeCombo_->count() - 1, probe.description(), Qt::ToolTipRole);
        }
        probeCombo_->setEnabled(true);

        int select = 0;
        for (int i = 0; i < probes_.size(); ++i) {
            if (probes_.at(i).key() == selectedKey) {
                select = i;
                break;
            }
        }
        probeCombo_->setCurrentIndex(select);

        /* The closed combo shows the probe that is selected right now, so its
         * own tooltip has to carry that probe's details. The signal that does
         * this on a manual change (onProbeChanged) never fires here - the
         * populate above runs with the signals blocked, and the index often
         * does not change at all - so without this the tooltip stayed empty
         * until the user happened to switch to another probe. The per item
         * Qt::ToolTipRole above only dresses the drop down list, not the
         * widget once it is collapsed. */
        probeCombo_->setToolTip(probes_.at(select).description());
    }

    probeCombo_->blockSignals(false);

    /* The identify goes to a thread. It is the one step here that can block
     * for seconds - attachSwd()'s recovery ladder, on a target that runs its
     * own program - and this runs before the window has painted, so doing it
     * inline is what turns start-up into a white window. See
     * identifyTargetAsync(). */
    if (!probes_.isEmpty())
        identifyTargetAsync();
    else {
        target_ = TargetInfo();
        targetIdentified_ = false;
        updateProbeStatusText();
    }
}

void MainWindow::onRefreshProbes()
{
    refreshProbeList();
}

void MainWindow::onProbeChanged(int index)
{
    if (index < 0 || index >= probes_.size())
        return;

    probeCombo_->setToolTip(probes_.at(index).description());
    identifyTarget();
}

void MainWindow::identifyTarget()
{
    /* A start-up identify may still be in flight. Two sessions on the one
     * probe must not overlap, and the queued answer of the old one would
     * overwrite what this call is about to read - so it is waited out. This
     * is a user action (a combo change, a button), which is why the wait here
     * is acceptable while the start-up path had to avoid it. */
    if (identifyThread_ && identifyThread_->isRunning())
        identifyThread_->wait(8000);

    const DapProbeInfo *probe = currentProbe();

    if (!probe) {
        target_ = TargetInfo();
        targetIdentified_ = false;
        updateProbeStatusText();
        return;
    }

    appendLog(tr("Identifying the target over %1 ...").arg(probe->displayName()));

    target_ = targetIdentify(*probe, clockFrequency(),
                            [this](const QString &message, bool isError) {
                                appendLog(message, isError);
                            });
    targetIdentified_ = target_.ok;

    if (!target_.ok && !target_.error.isEmpty())
        appendLog(target_.error, true);

    updateProbeStatusText();
}

void MainWindow::refreshTargetStatus()
{
    const DapProbeInfo *probe = currentProbe();

    if (!probe) {
        target_ = TargetInfo();
        targetIdentified_ = false;
        updateProbeStatusText();
        return;
    }

    /* A real reconnect, but silent: the point is to re-read a state the
     * programming run changed, not to announce a new session to the user.
     * See targetIdentifyQuiet().
     *
     * A failed refresh keeps the previous status rather than replacing it
     * with an error. This runs a moment after the result box, which is
     * exactly when the probe may still be releasing its interface - and
     * "programming complete" followed by a connection failure the user
     * never asked for would be worse than a status that is merely a few
     * seconds stale. */
    QString error;
    const TargetInfo info = targetIdentifyQuiet(*probe, clockFrequency(), error);

    if (!info.ok)
        return;

    target_ = info;
    targetIdentified_ = true;
    updateProbeStatusText();
}

void MainWindow::identifyTargetAsync()
{
    if (identifyThread_)
        return;              /* one identify is already in flight */

    const DapProbeInfo *probe = currentProbe();
    if (!probe) {
        target_ = TargetInfo();
        targetIdentified_ = false;
        updateProbeStatusText();
        return;
    }

    /* Capture everything the worker needs by value: probes_ can be rebuilt by
     * the next refresh long before this finishes. */
    const DapProbeInfo wanted = *probe;
    const quint32 clockHz = clockFrequency();

    /* Say what is going on before the window can freeze on anything. The
     * status shows that work is in progress rather than the stale answer from
     * the previous probe, which is also what makes a slow attach look like a
     * slow attach instead of a hang. */
    target_ = TargetInfo();
    targetIdentified_ = false;
    probeStatusLabel_->setRichText(
        tr("<font color='%1'>Identifying the target ...</font>").arg(statusColor(1)));
    probeStatusLabel_->setToolTip(QString());
    targetCoreLabel_->setText(tr("-"));
    targetCoreLabel_->setToolTip(QString());

    identifyThread_ = QThread::create([this, wanted, clockHz]() {
        QString error;
        const TargetInfo info = targetIdentifyQuiet(wanted, clockHz, error);

        QMetaObject::invokeMethod(
            this, [this, info, error]() { onTargetIdentified(info, error); },
            Qt::QueuedConnection);
    });

    connect(identifyThread_, &QThread::finished, this, [this]() {
        QThread *thread = identifyThread_;
        identifyThread_ = nullptr;
        thread->deleteLater();
    });

    identifyThread_->start();
}

void MainWindow::onTargetIdentified(const TargetInfo &info, const QString &error)
{
    /* The probe may have changed while this was running - a refresh or a
     * combo change starts its own identify, and whichever answer arrives
     * last must not overwrite a newer one. The in-flight request is the only
     * thing that can be current, and identifyThread_ is cleared when it
     * finishes, so a stale answer is the one that arrives with a request
     * already superseded. */
    if (!probeCombo_->isEnabled() || probes_.isEmpty())
        return;

    if (info.ok) {
        target_ = info;
        targetIdentified_ = true;
    } else {
        target_ = TargetInfo();
        targetIdentified_ = false;
        if (!error.isEmpty())
            target_.error = error;
    }

    updateProbeStatusText();
}

QString MainWindow::statusColor(int kind) const
{
    const bool dark = ThemeManager::instance()->isDark();
    switch (kind) {
    case 0:
        return dark ? QStringLiteral("#5CC45C") : QStringLiteral("#008000");
    case 1:
        return dark ? QStringLiteral("#E8A33D") : QStringLiteral("#C06000");
    case 2:
        return dark ? QStringLiteral("#FF7B7B") : QStringLiteral("#A00000");
    default:
        break;
    }
    return palette().color(QPalette::WindowText).name();
}

void MainWindow::updateProbeStatusText()
{
    if (probes_.isEmpty()) {
        probeStatusLabel_->setRichText(
            tr("<font color='%1'>No probe found</font>").arg(statusColor(2)));
        probeStatusLabel_->setToolTip(tr("Connect a CMSIS-DAP probe and press Refresh."));
        targetCoreLabel_->setText(tr("-"));
        targetCoreLabel_->setToolTip(QString());
        return;
    }

    if (target_.ok) {
        probeStatusLabel_->setRichText(
            tr("<font color='%1'>Connected (%2)</font>")
                .arg(statusColor(0))
                .arg(target_.probeName.isEmpty() ? tr("SWD") : target_.probeName));
        probeStatusLabel_->setToolTip(
            tr("CMSIS-DAP firmware: %1").arg(target_.probeFirmware.isEmpty()
                                                 ? tr("unknown")
                                                 : target_.probeFirmware));

        targetCoreLabel_->setText(target_.coreName);
        targetCoreLabel_->setToolTip(
            tr("CPUID %1, DP IDCODE %2, core %3")
                .arg(hex32(target_.cpuid),
                     hex32(target_.dpIdCode),
                     target_.halted ? tr("halted") : tr("running")));
    } else {
        probeStatusLabel_->setRichText(
            tr("<font color='%1'>Probe present, target not identified</font>")
                .arg(statusColor(1)));
        probeStatusLabel_->setToolTip(target_.error);
        targetCoreLabel_->setText(tr("-"));
        targetCoreLabel_->setToolTip(target_.error);
    }
}

/* ==========================================================================
 * Algorithms
 *
 * The list is grouped the way the algorithms are installed, and shown in a
 * collapsible tree (see AlgorithmPicker) because a full pack installation
 * holds several hundred of them:
 *
 *     [Recently used]                               <- open by default
 *     [Keil built-in algorithms]                    <- open by default
 *     [STMicroelectronics.stm32c5xx_dfp  2.0.0]     <- one CMSIS device family pack
 *     ...
 *     [Additional folder: ...]                      <- folders the user added
 *
 * Walking every pack touches hundreds of directories and opening each
 * algorithm to read its FlashDevice structure takes another moment, so the
 * result is cached in %APPDATA%. The window fills the list from that cache,
 * which is immediate, and only rescans when the cache is missing, stale or the
 * user asks for it. The scan itself runs in a worker thread.
 * ========================================================================== */

/** How long a scan result is trusted before it is refreshed in the background */
const int kCacheMaxAgeDays = 7;

void MainWindow::loadAlgorithmCatalogue()
{
    QSettings settings(QString::fromLatin1(kSettingsOrg), QString::fromLatin1(kSettingsApp));

    const QString savedPath = settings.value(QStringLiteral("algoPath")).toString();
    const QDateTime now = QDateTime::currentDateTime();

    /* 1. Instant: whatever the last run left behind */
    bool haveCache = algoLoadCache(algoCacheFilePath(), algoCatalog_);

    if (haveCache) {
        populateAlgorithmPicker(savedPath);

        const bool stale = !algoCatalog_.scannedAt.isValid()
                           || algoCatalog_.scannedAt.daysTo(now) >= kCacheMaxAgeDays
                           || !algoCacheRootsMatch(algoCatalog_);
        if (stale)
            refreshAlgorithmList();
        return;
    }

    /* 2. No cache yet (first run): the Keil folder is a single flat directory,
     *    so reading it now costs nothing and gives the user something to pick
     *    while the packs are scanned in the background. */
    algoCatalog_ = algoScanCatalog(algoBuiltinRoots(), QStringList(), algoUserRoots());
    populateAlgorithmPicker(savedPath);

    refreshAlgorithmList();
}

void MainWindow::refreshAlgorithmList()
{
    if (algoScanThread_)
        return;              /* a scan is already running */

    const QStringList builtinRoots = algoBuiltinRoots();
    const QStringList packRoots = algoPackRoots();
    const QStringList userRoots = algoUserRoots();

    appendLog(tr("Scanning flash algorithms (%1 pack folders) ...").arg(packRoots.size()));

    algoScanThread_ = QThread::create([this, builtinRoots, packRoots, userRoots]() {
        QThread *self = QThread::currentThread();

        AlgoCatalog catalog = algoScanCatalog(
            builtinRoots, packRoots, userRoots,
            [self]() { return self->isInterruptionRequested(); });

        if (self->isInterruptionRequested())
            return;

        algoSaveCache(algoCacheFilePath(), catalog);

        /* Hand the result over to the GUI thread. If the window is gone by
         * then the queued call is dropped together with it. */
        QMetaObject::invokeMethod(
            this, [this, catalog]() { onCatalogScanned(catalog); },
            Qt::QueuedConnection);
    });

    connect(algoScanThread_, &QThread::finished, this, [this]() {
        QThread *thread = algoScanThread_;
        algoScanThread_ = nullptr;
        if (thread)
            thread->deleteLater();
    });

    algoScanThread_->start();
}

void MainWindow::onCatalogScanned(const AlgoCatalog &catalog)
{
    if (catalog.isEmpty() && !algoCatalog_.isEmpty())
        return;              /* an interrupted or failed scan must not empty the list */

    const QString selected = algoPicker_->currentPath();

    algoCatalog_ = catalog;
    populateAlgorithmPicker(selected);

    appendLog(tr("%1 flash algorithms found in %2 groups.")
                  .arg(catalog.entryCount())
                  .arg(catalog.groups.size()));

    if (catalog.invalidCount > 0) {
        appendLog(tr("%1 file(s) were skipped: they carry no usable FlashDevice "
                     "structure.").arg(catalog.invalidCount));

        for (const QString &path : catalog.invalidFiles)
            appendLog(QStringLiteral("    %1").arg(QDir::toNativeSeparators(path)));
    }
}

/**
 * Rebuild the algorithm picker from algoCatalog_.
 *
 * The caption of every group and the label of every entry are built here, so
 * that all the visible text of the list lives in one place (the group captions
 * of the packs come from AlgoGroup::title(), which is translated as well).
 */
void MainWindow::populateAlgorithmPicker(const QString &selectedPath)
{
    QSettings settings(QString::fromLatin1(kSettingsOrg), QString::fromLatin1(kSettingsApp));

    QString wanted = selectedPath;
    if (wanted.isEmpty())
        wanted = settings.value(QStringLiteral("algoPath")).toString();

    recentAlgoPaths_ = settings.value(QStringLiteral("algoRecent")).toStringList();

    /* The device name is what the user recognises; the file name is what Keil
     * shows, so keep both. */
    const auto toChoice = [](const AlgoEntry &entry) {
        QString label = entry.deviceName.trimmed();
        if (label.isEmpty()) {
            label = entry.fileName;
        } else if (label.compare(QFileInfo(entry.fileName).completeBaseName(),
                                 Qt::CaseInsensitive) != 0) {
            label = QStringLiteral("%1  (%2)").arg(label, entry.fileName);
        }

        AlgorithmChoice choice;
        choice.label = label;
        choice.path = entry.path;
        return choice;
    };

    QVector<AlgorithmChoiceGroup> groups;

    const auto appendGroup = [&groups, &toChoice](const QString &caption,
                                                  const QString &tooltip,
                                                  const QVector<AlgoEntry> &entries) {
        if (entries.isEmpty())
            return;

        AlgorithmChoiceGroup group;
        group.caption = caption;
        group.tooltip = tooltip;
        for (const AlgoEntry &entry : entries)
            group.choices.append(toChoice(entry));

        groups.append(group);
    };

    /* 1. recently used: the algorithm of the last session is one click away */
    {
        QVector<AlgoEntry> recent;
        for (const QString &path : recentAlgoPaths_) {
            if (!QFileInfo(path).isFile())
                continue;

            bool duplicate = false;
            for (const AlgoEntry &entry : recent) {
                if (entry.path.compare(path, Qt::CaseInsensitive) == 0) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate)
                continue;

            const AlgoEntry *known = algoCatalog_.find(path);
            if (known) {
                recent.append(*known);
                continue;
            }

            AlgoEntry entry;
            if (algoInspectFile(path, entry))
                recent.append(entry);
        }

        appendGroup(tr("Recently used"), QString(), recent);
    }

    /* 2. Keil's own algorithms, then the device family packs, then the folders
     *    the user added, which is the order in which they usually matter */
    const AlgoGroup::Kind order[] = { AlgoGroup::Builtin, AlgoGroup::Pack, AlgoGroup::User };
    for (AlgoGroup::Kind kind : order) {
        for (const AlgoGroup &group : algoCatalog_.groups) {
            if (group.kind != kind)
                continue;

            QString caption;
            switch (kind) {
            case AlgoGroup::Builtin:
                caption = group.title();          /* "Keil built-in algorithms" */
                break;
            case AlgoGroup::Pack:
                caption = QStringLiteral("%1.%2  %3").arg(group.vendor, group.pack,
                                                          group.version);
                break;
            case AlgoGroup::User:
                caption = tr("Additional folder: %1")
                              .arg(QDir::toNativeSeparators(group.root));
                break;
            }

            appendGroup(caption, QDir::toNativeSeparators(group.root), group.entries);
        }
    }

    /* 3. the remembered algorithm also has to be visible when it does not
     *    belong to any of the search roots (a file that was dropped in) */
    if (!wanted.isEmpty() && QFileInfo(wanted).isFile()) {
        bool present = false;
        for (const AlgorithmChoiceGroup &group : groups) {
            for (const AlgorithmChoice &choice : group.choices) {
                if (choice.path.compare(wanted, Qt::CaseInsensitive) == 0) {
                    present = true;
                    break;
                }
            }
            if (present)
                break;
        }

        if (!present) {
            AlgoEntry entry;
            if (algoInspectFile(wanted, entry))
                appendGroup(tr("Last used"), QString(), QVector<AlgoEntry>() << entry);
        }
    }

    algoPicker_->setGroups(groups);
    algoPicker_->selectAlgorithm(wanted);

    /* selectAlgorithm() only moves the picker; it does not emit changed(), so
     * algorithm_ has to be brought in line by hand. Silent: this list is
     * rebuilt for reasons the user did not ask for as a selection - the
     * language changing, or setBusy() running at the end of every programming
     * run - and a second "Algorithm loaded" line in the log there reads as if
     * a new session had started on its own. */
    reloadAlgorithm(false);
}

void MainWindow::addRecentAlgorithm(const QString &path)
{
    if (path.isEmpty())
        return;

    const QString clean = QFileInfo(path).absoluteFilePath();

    QStringList list;
    list.append(clean);
    for (const QString &item : recentAlgoPaths_) {
        if (list.size() >= 8)
            break;
        if (!list.contains(item, Qt::CaseInsensitive))
            list.append(item);
    }

    recentAlgoPaths_ = list;

    QSettings settings(QString::fromLatin1(kSettingsOrg), QString::fromLatin1(kSettingsApp));
    settings.setValue(QStringLiteral("algoRecent"), recentAlgoPaths_);
}

void MainWindow::onBrowseAlgorithm()
{
    QString start = algoPicker_->currentPath();
    start = QFileInfo(start).absolutePath();

    const QString path = QFileDialog::getOpenFileName(
        this,
        tr("Select a Keil Flash Algorithm"),
        start,
        algoFileFilter());

    if (path.isEmpty())
        return;

    /* The folder is remembered, so that the next scan finds it on its own */
    algoRememberRoot(QFileInfo(path).absolutePath());
    addRecentAlgorithm(path);

    populateAlgorithmPicker(path);
}

void MainWindow::onAlgorithmChanged()
{
    /* The slot behind the picker's changed() signal, so this is the user
     * picking an algorithm and the load belongs in the log. */
    reloadAlgorithm(true);
}

void MainWindow::reloadAlgorithm(bool announce)
{
    const QString path = algoPicker_->currentPath();

    algorithm_ = FlashAlgo();
    algorithmError_.clear();

    if (path.isEmpty()) {
        updateAlgorithmInfo();
        return;
    }

    QString error;
    if (!algorithm_.load(path, error)) {
        algorithmError_ = error;
        appendLog(tr("The algorithm %1 could not be loaded: %2")
                      .arg(QFileInfo(path).fileName(), error), true);
    } else if (announce) {
        appendLog(tr("Algorithm loaded: %1 (%2)")
                      .arg(algorithm_.fileName(), algorithm_.info().deviceName));
    }

    updateAlgorithmInfo();
}

void MainWindow::updateAlgorithmInfo()
{
    if (!algorithmError_.isEmpty()) {
        algoInfoLabel_->setRichText(
            tr("<font color='%1'>%2</font>")
                .arg(statusColor(2), algorithmError_.toHtmlEscaped()));
        return;
    }

    if (!algorithm_.isValid()) {
        algoInfoLabel_->setRichText(
            tr("<font color='%1'>No algorithm selected</font>").arg(statusColor(2)));
        return;
    }

    const FlashAlgoInfo &info = algorithm_.info();

    QString text = tr("<b>%1</b><br>%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors")
                       .arg(info.deviceName.toHtmlEscaped(),
                            algorithm_.deviceTypeName())
                       .arg(info.deviceAddress, 8, 16, QLatin1Char('0'))
                       .arg(info.deviceAddress + info.deviceSize - 1, 8, 16, QLatin1Char('0'))
                       .arg(firmware_format_size(info.deviceSize))
                       .arg(info.pageSize)
                       .arg(info.sectors.size());

    if (eraseChipCheck_->isChecked() && !algorithm_.hasFunction(QStringLiteral("EraseChip"))) {
        text += tr("<br><font color='%1'>This algorithm has no chip erase; "
                   "the sectors are erased instead.</font>")
                    .arg(statusColor(1));
    }

    algoInfoLabel_->setRichText(text);
}

/* ==========================================================================
 * Firmware
 * ========================================================================== */

void MainWindow::onBrowseFirmware()
{
    const QString start = QFileInfo(fileEdit_->text()).absolutePath();

    const QString path = QFileDialog::getOpenFileName(
        this,
        tr("Select Firmware File"),
        start,
        tr("Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
           "Binary Files (*.bin);;All Files (*.*)"));

    if (!path.isEmpty())
        fileEdit_->setText(path);
}

void MainWindow::onFirmwarePathChanged()
{
    updateFirmwareInfo();
}

void MainWindow::updateFirmwareInfo()
{
    const QString path = fileEdit_->text().trimmed();

    if (path.isEmpty() || !QFileInfo(path).exists()) {
        firmwareInfoLabel_->setRichText(
            tr("<font color='%1'>No valid firmware file selected</font>").arg(statusColor(2)));
        return;
    }

    QString error;
    FirmwareImage image;

    if (!firmware_load(path, binBaseAddress(), image, error)) {
        firmwareInfoLabel_->setRichText(
            tr("<font color='%1'>%2</font>").arg(statusColor(2), error.toHtmlEscaped()));
        return;
    }

    /* The size shown is the payload, not the address span: a HEX file with a
     * bootloader at 0x08000000 and an application at 0x08020000 covers 448 KB
     * of address space but only carries 325 KB of data, and the payload is
     * what is programmed. */
    firmwareInfoLabel_->setRichText(
        tr("%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6")
            .arg(image.fromHex ? tr("Intel HEX") : tr("BIN"),
                 firmware_format_size(image.payloadSize()))
            .arg(image.segments.size())
            .arg(image.startAddress, 8, 16, QLatin1Char('0'))
            .arg(image.endAddress, 8, 16, QLatin1Char('0'))
            .arg(image.checksum, 8, 16, QLatin1Char('0')));
}

/* ==========================================================================
 * Value helpers
 * ========================================================================== */

bool MainWindow::readHexWidget(QLineEdit *edit, quint32 fallback, quint32 &value) const
{
    QString text = edit->text().trimmed();
    if (text.startsWith(QLatin1String("0x"), Qt::CaseInsensitive))
        text.remove(0, 2);

    if (text.isEmpty()) {
        value = fallback;
        return true;
    }

    bool ok = false;
    const quint32 parsed = text.toUInt(&ok, 16);
    value = ok ? parsed : fallback;
    return ok;
}

quint32 MainWindow::binBaseAddress() const
{
    quint32 value = kDefaultBinBase;
    readHexWidget(baseAddressEdit_, kDefaultBinBase, value);
    return value;
}

quint32 MainWindow::ramBaseAddress() const
{
    quint32 value = kDefaultRamBase;
    readHexWidget(ramBaseEdit_, kDefaultRamBase, value);
    return value;
}

quint32 MainWindow::clockFrequency() const
{
    const QVariant data = clockCombo_->currentData();
    return data.isValid() ? data.toUInt() : kDefaultClockHz;
}

ResetMode MainWindow::resetMode() const
{
    const QVariant data = resetModeCombo_->currentData();
    return data.isValid() ? static_cast<ResetMode>(data.toInt())
                          : ResetMode::Software;
}

/* ==========================================================================
 * Target control buttons
 * ========================================================================== */

void MainWindow::onHaltTarget()
{
    const DapProbeInfo *probe = currentProbe();
    if (!probe) {
        QMessageBox::warning(this, tr("No Probe"), tr("Select a debug probe first."));
        return;
    }

    /* One session, not three. This used to run targetIdentify() first to see
     * whether the target answered at all, then targetHalt() to do the work,
     * then identifyTarget() to refresh the status - three probe opens, three
     * power-up negotiations, six "Probe: / SWD clock:" lines and two
     * redundant connect sequences for one button press.
     *
     * targetHalt() opens the probe itself and fails with the same accuracy
     * when it cannot, so the first session only ever duplicated it. The
     * status comes back out of the one session that did the work. */
    TargetInfo info;
    QString error;

    if (!targetHalt(*probe, clockFrequency(),
                    [this](const QString &message, bool isError) { appendLog(message, isError); },
                    &info, error)) {
        appendLog(error, true);
        QMessageBox::critical(this, tr("Halt Failed"), error);
        return;
    }

    appendLog(tr("The core is halted."));

    target_ = info;
    targetIdentified_ = true;
    updateProbeStatusText();
}

void MainWindow::onRunTarget()
{
    const DapProbeInfo *probe = currentProbe();
    if (!probe) {
        QMessageBox::warning(this, tr("No Probe"), tr("Select a debug probe first."));
        return;
    }

    TargetInfo info;
    QString error;

    if (!targetRun(*probe, clockFrequency(),
                   [this](const QString &message, bool isError) { appendLog(message, isError); },
                   &info, error)) {
        appendLog(error, true);
        QMessageBox::critical(this, tr("Run Failed"), error);
        return;
    }

    appendLog(tr("The core is running."));

    target_ = info;
    targetIdentified_ = true;
    updateProbeStatusText();
}

void MainWindow::onResetTarget()
{
    const DapProbeInfo *probe = currentProbe();
    if (!probe) {
        QMessageBox::warning(this, tr("No Probe"), tr("Select a debug probe first."));
        return;
    }

    TargetInfo info;
    QString error;

    if (!targetReset(*probe, clockFrequency(), resetMode(), false,
                     [this](const QString &message, bool isError) { appendLog(message, isError); },
                     &info, error)) {
        appendLog(error, true);
        QMessageBox::critical(this, tr("Reset Failed"), error);
        return;
    }

    appendLog(tr("The target was reset and is running."));

    target_ = info;
    targetIdentified_ = true;
    updateProbeStatusText();
}

/* ==========================================================================
 * Programming
 * ========================================================================== */

void MainWindow::setBusy(bool busy)
{
    flashing_ = busy;

    probeCombo_->setEnabled(!busy && !probes_.isEmpty());
    refreshButton_->setEnabled(!busy);
    algoPicker_->setEnabled(!busy);
    algoBrowseButton_->setEnabled(!busy);
    fileEdit_->setEnabled(!busy);
    fileBrowseButton_->setEnabled(!busy);
    baseAddressEdit_->setEnabled(!busy);
    ramBaseEdit_->setEnabled(!busy);
    clockCombo_->setEnabled(!busy);
    eraseChipCheck_->setEnabled(!busy);
    verifyCheck_->setEnabled(!busy);
    resetRunCheck_->setEnabled(!busy);
    resetModeCombo_->setEnabled(!busy);

    haltButton_->setEnabled(!busy);
    runButton_->setEnabled(!busy);
    resetButton_->setEnabled(!busy);

    retranslateUi();
}

void MainWindow::onStartFlash()
{
    /* The same button cancels a running operation */
    if (flashing_) {
        onCancelFlash();
        return;
    }

    const DapProbeInfo *probe = currentProbe();
    if (!probe) {
        QMessageBox::warning(this, tr("No Probe"),
                             tr("Select a CMSIS-DAP probe first."));
        return;
    }

    if (!algorithm_.isValid()) {
        QMessageBox::warning(this, tr("No Algorithm"),
                             tr("Select the Keil .FLM flash algorithm that matches the device."));
        return;
    }

    const QString path = fileEdit_->text().trimmed();
    if (path.isEmpty() || !QFileInfo(path).exists()) {
        QMessageBox::warning(this, tr("No Firmware"),
                             tr("Select a valid firmware file first."));
        return;
    }

    FlashRequest request;
    request.probe = *probe;
    request.algoPath = algorithm_.path();
    request.firmwarePath = path;
    request.firmwareBase = binBaseAddress();
    request.ramBase = ramBaseAddress();
    request.clockHz = clockFrequency();
    request.eraseWholeChip = eraseChipCheck_->isChecked();
    request.verify = verifyCheck_->isChecked();
    request.resetAfterProgram = resetRunCheck_->isChecked();
    request.resetMode = resetMode();

    logEntries_.clear();
    logView_->clear();
    progressBar_->setValue(0);
    progressState_ = ProgressState::Running;
    cancelled_ = false;
    progressLabel_->setText(tr("Preparing ..."));
    setBusy(true);

    worker_ = new FlashWorker(request);
    workerThread_ = new QThread(this);
    worker_->moveToThread(workerThread_);

    connect(workerThread_, &QThread::started, worker_, &FlashWorker::run);
    connect(worker_, &FlashWorker::progress, this, &MainWindow::onProgressUpdated);
    connect(worker_, &FlashWorker::logMessage, this, &MainWindow::onWorkerLog);
    connect(worker_, &FlashWorker::finished, this, &MainWindow::onFlashFinished);
    connect(worker_, &FlashWorker::finished, worker_, &FlashWorker::deleteLater);
    connect(worker_, &FlashWorker::finished, workerThread_, &QThread::quit);
    connect(workerThread_, &QThread::finished, workerThread_, &QThread::deleteLater);

    workerThread_->start();
}

void MainWindow::onCancelFlash()
{
    if (!flashing_ || !worker_)
        return;

    cancelled_ = true;
    worker_->cancel();
    appendLog(tr("Cancelling ..."), true);
}

void MainWindow::onProgressUpdated(int percent, const QString &status)
{
    progressBar_->setValue(percent);
    progressLabel_->setText(status);
}

void MainWindow::onWorkerLog(const QString &message, bool isError)
{
    appendLog(message, isError);
}

void MainWindow::onFlashFinished(bool success, const QString &message)
{
    setBusy(false);
    worker_ = nullptr;
    workerThread_ = nullptr;

    if (success) {
        progressBar_->setValue(100);
        progressState_ = ProgressState::Succeeded;
        progressLabel_->setText(tr("Programming complete"));
        /* No appendLog() here: the worker already put the summary line in the
         * log through its note() before it reported success, and printing the
         * same message a second time made every run end with the summary
         * duplicated. A failure is the other way round - that path has no
         * note() of its own - so the two branches below do log. */
        QMessageBox::information(this, tr("Complete"),
                                 tr("The firmware was programmed successfully.\n\n%1").arg(message));
    } else if (cancelled_) {
        progressState_ = ProgressState::Failed;
        progressLabel_->setText(tr("Cancelled"));
        appendLog(message, true);
    } else {
        progressState_ = ProgressState::Failed;
        progressLabel_->setText(tr("Programming failed"));
        appendLog(message, true);
        QMessageBox::critical(this, tr("Failed"), message);
    }

    /* The core state changed underneath us: read it back - silently, because
     * the log has just finished and a full identify sequence printed here
     * reads as if a new connection had started on its own.
     *
     * Only after a success. A failed or cancelled run leaves the target in a
     * state this tool did not choose, and reconnecting a moment later is
     * exactly when the probe can still be busy; there is nothing useful to
     * show either way, so the status simply keeps its last known value. */
    if (success)
        QTimer::singleShot(300, this, &MainWindow::refreshTargetStatus);
}

void MainWindow::updateProgressText()
{
    switch (progressState_) {
    case ProgressState::Running:
        break;
    case ProgressState::Succeeded:
        progressLabel_->setText(tr("Programming complete"));
        break;
    case ProgressState::Failed:
        progressLabel_->setText(tr("Programming failed"));
        break;
    case ProgressState::Idle:
    default:
        progressLabel_->setText(tr("Ready"));
        break;
    }
}

/* ==========================================================================
 * Log
 * ========================================================================== */

QString MainWindow::logTextColor() const
{
    if (ThemeManager::instance()->isDark())
        return QStringLiteral("#DDDDDD");
    return palette().color(QPalette::Text).name();
}

QString MainWindow::logErrorColor() const
{
    return ThemeManager::instance()->isDark() ? QStringLiteral("#FF8080")
                                              : QStringLiteral("#C00000");
}

void MainWindow::appendLogLine(const LogEntry &entry)
{
    const QString color = entry.isError ? logErrorColor() : logTextColor();
    const QString line = QStringLiteral("<span style='color:%1'>[%2] %3</span>")
                             .arg(color,
                                  entry.time.toHtmlEscaped(),
                                  entry.text.toHtmlEscaped());
    logView_->append(line);
    logView_->ensureCursorVisible();
}

void MainWindow::appendLog(const QString &message, bool isError)
{
    LogEntry entry;
    entry.time = QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"));
    entry.text = message;
    entry.isError = isError;

    logEntries_.append(entry);
    while (logEntries_.size() > 2000)
        logEntries_.removeFirst();

    appendLogLine(entry);
}

void MainWindow::renderLog()
{
    logView_->clear();
    for (const LogEntry &entry : logEntries_)
        appendLogLine(entry);
}

void MainWindow::onClearLog()
{
    logEntries_.clear();
    logView_->clear();
}

/* ==========================================================================
 * About, drag and drop, closing
 * ========================================================================== */

void MainWindow::showAbout()
{
    QMessageBox box(this);
    box.setWindowTitle(tr("About"));
    box.setIconPixmap(QPixmap(QStringLiteral(":/icons/logo_128"))
                          .scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    box.setText(
        tr("<h3>WeAct Studio DAP Flasher</h3>"
           "<p>Version: %1</p>"
           "<p><b>WeAct Studio</b><br>"
           "Copyright (C) 2026 WeAct Studio. All rights reserved.</p>"
           "<p>Programs Cortex-M devices through a CMSIS-DAP probe (USB HID or "
           "USB bulk / WinUSB) using the Keil .FLM flash algorithms.</p>"
           "<p>This program is free software: you can redistribute it and/or modify it "
           "under the terms of the GNU General Public License as published by the Free "
           "Software Foundation, either version 3 of the License, or (at your option) "
           "any later version. It is distributed in the hope that it will be useful, but "
           "WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY "
           "or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for "
           "more details.</p>"
           "<p>License: GPL-3.0-or-later<br>"
           "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>"
           "https://www.gnu.org/licenses/gpl-3.0.html</a></p>"
           "<p>Built with Qt %2, statically compiled with MSYS2 (MINGW64).</p>")
            .arg(QApplication::applicationVersion(), QString::fromLatin1(qVersion())));
    box.setTextFormat(Qt::RichText);
    box.setStandardButtons(QMessageBox::Ok);
    box.exec();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    const QString path = urls.first().toLocalFile();
    if (path.isEmpty())
        return;

    if (algoIsAlgorithmFile(path)) {
        /* Dropping an algorithm file adds its folder to the search roots and
         * puts the file itself into the "recently used" group */
        algoRememberRoot(QFileInfo(path).absolutePath());
        addRecentAlgorithm(path);
        populateAlgorithmPicker(path);
    } else {
        fileEdit_->setText(path);
    }

    event->acceptProposedAction();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (flashing_) {
        const int answer = QMessageBox::warning(
            this, tr("Programming in Progress"),
            tr("Programming is still running; stopping now can leave the device with "
               "an incomplete firmware.\n\nExit anyway?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (answer != QMessageBox::Yes) {
            event->ignore();
            return;
        }

        if (worker_)
            worker_->cancel();
    }

    saveSettings();
    event->accept();
}
