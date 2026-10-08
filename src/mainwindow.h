/*
 * WeAct Studio DAP Flasher
 *
 * Main window
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QString>

#include <QDragEnterEvent>
#include <QDropEvent>

#include "algorithmpicker.h"
#include "dap/probes.h"
#include "flash/algocatalog.h"
#include "flash/flashalgo.h"
#include "targetsession.h"
#include "wrappedlabel.h"

class QAction;
class QActionGroup;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QProgressBar;
class QPushButton;
class QTextEdit;
class QThread;
class QTimer;

class FlashWorker;

/**
 * Main window of the flasher.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

public Q_SLOTS:
    void showAbout();

private Q_SLOTS:
    void onRefreshProbes();
    void onProbeChanged(int index);
    void onBrowseAlgorithm();
    void onAlgorithmChanged();    void onBrowseFirmware();
    void onFirmwarePathChanged();
    void onStartFlash();
    void onCancelFlash();
    void onHaltTarget();
    void onRunTarget();
    void onResetTarget();
    void onClearLog();

    void onProgressUpdated(int percent, const QString &status);
    void onWorkerLog(const QString &message, bool isError);
    void onFlashFinished(bool success, const QString &message);

    void onThemeActionTriggered(QAction *action);
    void onThemeChanged();
    void onLanguageActionTriggered(QAction *action);
    void onLanguageChanged();

private:
    void setupUi();
    void createMenus();
    void retranslateUi();
    void loadSettings();
    void saveSettings();

    void refreshProbeList();
    void identifyTarget();

    /* Quiet counterpart of identifyTarget(), used after a programming run to
     * re-read a core state the run invalidated. It reconnects for real, but
     * writes nothing to the log - see targetIdentifyQuiet() for why. */
    void refreshTargetStatus();

    /* Start-up runs the very same identify, off the GUI thread.
     *
     * A target whose own program holds the SWD interface makes attachSwd()
     * walk its whole recovery ladder - resets, retries and a power-up poll
     * with a five second budget - and on the GUI thread that freezes the
     * window before it has painted anything, which is the white flash at
     * start-up. The list is filled synchronously; only the identify moves to
     * a thread, and its result comes back through a queued call.
     *
     * A second request while one is in flight is ignored: the answer for the
     * probe selected at the time is already on its way. */
    void identifyTargetAsync();
    void onTargetIdentified(const TargetInfo &info, const QString &error);

    /* The algorithm list is filled from a cache so that the start-up does not
     * have to walk the CMSIS packs; refreshAlgorithmList() rescans in the
     * background and updates the list when it is done. */
    void refreshAlgorithmList();
    void onCatalogScanned(const AlgoCatalog &catalog);
    void populateAlgorithmPicker(const QString &selectedPath);
    void addRecentAlgorithm(const QString &path);
    void loadAlgorithmCatalogue();

    /**
     * Load the algorithm the picker points at into algorithm_.
     *
     * @param announce  log the load, which is what the user wants when they
     *                  picked an algorithm. The picker is also rebuilt behind
     *                  the scenes (a language change, or setBusy() at the end
     *                  of a run) and that reload must stay out of the log: it
     *                  is not a new selection and "Algorithm loaded" printed
     *                  after every programming run reads like a new session
     *                  had started.
     */
    void reloadAlgorithm(bool announce);

    void updateProbeStatusText();
    void updateAlgorithmInfo();
    void updateFirmwareInfo();
    void updateProgressText();

    void setBusy(bool busy);
    void appendLog(const QString &message, bool isError = false);

    struct LogEntry
    {
        QString time;
        QString text;
        bool isError;
    };
    void appendLogLine(const LogEntry &entry);
    void renderLog();
    QString logTextColor() const;
    QString logErrorColor() const;
    QString statusColor(int kind) const;   /* 0=ok 1=warning 2=error 3=neutral */

    const DapProbeInfo *currentProbe() const;
    quint32 binBaseAddress() const;
    quint32 ramBaseAddress() const;
    quint32 clockFrequency() const;
    ResetMode resetMode() const;
    bool readHexWidget(QLineEdit *edit, quint32 fallback, quint32 &value) const;

    /* Device section */
    QComboBox *probeCombo_;
    QPushButton *refreshButton_;
    WrappedLabel *probeStatusLabel_;
    QLabel *probeStatusTitleLabel_;
    QLabel *targetCoreLabel_;
    QLabel *targetCoreTitleLabel_;

    /* Algorithm section */
    AlgorithmPicker *algoPicker_;
    QPushButton *algoBrowseButton_;
    WrappedLabel *algoInfoLabel_;
    QLabel *algoInfoTitleLabel_;

    /* Firmware section */
    QLineEdit *fileEdit_;
    QPushButton *fileBrowseButton_;
    WrappedLabel *firmwareInfoLabel_;
    QLabel *firmwareInfoTitleLabel_;
    QLineEdit *baseAddressEdit_;
    QLabel *baseAddressLabel_;

    /* Options */
    QComboBox *clockCombo_;
    QLineEdit *ramBaseEdit_;
    QCheckBox *eraseChipCheck_;
    QCheckBox *verifyCheck_;
    QCheckBox *resetRunCheck_;
    QComboBox *resetModeCombo_;
    QLabel *clockLabel_;
    QLabel *ramBaseLabel_;
    QLabel *resetModeLabel_;

    /* Progress and log */
    QProgressBar *progressBar_;
    QLabel *progressLabel_;
    QTextEdit *logView_;
    QLabel *logTitleLabel_;

    /* Buttons */
    QPushButton *startButton_;
    QPushButton *haltButton_;
    QPushButton *runButton_;
    QPushButton *resetButton_;
    QPushButton *exitButton_;

    /* Menus */
    QMenu *fileMenu_;
    QMenu *toolMenu_;
    QMenu *viewMenu_;
    QMenu *langMenu_;
    QMenu *helpMenu_;
    QAction *openAction_;
    QAction *openAlgoAction_;
    QAction *exitAction_;
    QAction *refreshAction_;
    QAction *reloadAlgoAction_;
    QAction *showAllHidAction_;
    QAction *clearLogAction_;
    QAction *aboutAction_;
    QActionGroup *langActionGroup_;
    QList<QAction *> langActions_;
    QActionGroup *themeActionGroup_;
    QList<QAction *> themeActions_;

    /* Group boxes and captions */
    QGroupBox *probeGroup_;
    QGroupBox *algoGroup_;
    QGroupBox *firmwareGroup_;
    QGroupBox *optionGroup_;
    QLabel *probeListLabel_;
    QLabel *filePathLabel_;

    /* State */
    QList<DapProbeInfo> probes_;
    AlgoCatalog algoCatalog_;
    QStringList recentAlgoPaths_;
    FlashAlgo algorithm_;
    QString algorithmError_;
    TargetInfo target_;
    bool targetIdentified_;

    FlashWorker *worker_;
    QThread *workerThread_;
    QThread *algoScanThread_;
    QThread *identifyThread_;
    bool flashing_;
    bool cancelled_;

    enum class ProgressState { Idle, Running, Succeeded, Failed };
    ProgressState progressState_;

    QList<LogEntry> logEntries_;
};

#endif /* MAINWINDOW_H */
