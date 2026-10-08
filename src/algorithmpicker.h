/*
 * WeAct Studio DAP Flasher
 *
 * Picking a flash algorithm out of the several hundred the CMSIS packs
 * install on a machine.
 *
 * A plain combo box is not usable at that size, so the picker is a drop-down
 * tree: the algorithms are grouped the way they are installed (the ones
 * shipped with Keil, one group per device family pack, the folders the user
 * added) and every group can be collapsed and expanded again, like a folder
 * in an explorer. A filter box narrows the list down without touching the
 * groups that do not match.
 *
 * The captions are handed in already translated (see AlgorithmChoiceGroup), so
 * this widget holds no user visible string of its own apart from the two
 * placeholders.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef ALGORITHMPICKER_H
#define ALGORITHMPICKER_H

#include <QString>
#include <QStringList>
#include <QToolButton>
#include <QVector>

class QFrame;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;

/** One selectable algorithm */
struct AlgorithmChoice
{
    QString label;    /* what the user reads, e.g. "STM32F103C8 128K Flash (…)" */
    QString path;     /* absolute file name of the .FLM / .xldr */
};

/** A collapsible group of choices */
struct AlgorithmChoiceGroup
{
    QString caption;
    QString tooltip;
    QVector<AlgorithmChoice> choices;
};

class AlgorithmPicker : public QToolButton
{
    Q_OBJECT

public:
    explicit AlgorithmPicker(QWidget *parent = nullptr);

    /** Replace the whole tree. The current selection is kept when it is still
     *  part of the new list. */
    void setGroups(const QVector<AlgorithmChoiceGroup> &groups);

    /** Point the picker at @p path without emitting changed() */
    void selectAlgorithm(const QString &path);

    /** The algorithm the user picked, or an empty string */
    QString currentPath() const { return currentPath_; }

    /** Re-apply the placeholders after a language change */
    void retranslate();

signals:
    /** The user picked another algorithm */
    void changed();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void ensurePopup();
    void showPicker();
    void rebuildTree();
    void applyFilter(const QString &text);
    void choose(QTreeWidgetItem *item);
    void updateLabel();
    QTreeWidgetItem *findItem(const QString &path) const;

    /** The label of @p path, looked up in the groups rather than in the tree,
     *  so that it is available before the popup has ever been built */
    QString labelFor(const QString &path) const;

    QFrame *popup_;
    QLineEdit *filter_;
    QTreeWidget *tree_;

    QVector<AlgorithmChoiceGroup> groups_;
    QString currentPath_;
    QString currentLabel_;

    /* Groups the user opened or closed by hand. Kept across rebuilds, so that
     * a group the user collapsed stays collapsed when the list is refreshed or
     * the picker is opened again. */
    QStringList expandedCaptions_;
    bool trackExpansion_;
    bool filtered_;
    bool expansionSeeded_;
    qint64 popupHiddenAt_;
};

#endif /* ALGORITHMPICKER_H */
