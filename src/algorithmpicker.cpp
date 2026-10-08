/*
 * WeAct Studio DAP Flasher
 *
 * The collapsible algorithm picker. See algorithmpicker.h for the rationale.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "algorithmpicker.h"

#include <QCursor>
#include <QDateTime>
#include <QDir>
#include <QEvent>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QLineEdit>
#include <QScreen>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace {

/* The popup is as tall as this unless the screen is smaller */
const int kPopupHeight = 430;
const int kPopupMinWidth = 540;

/* Reopening the picker right after it was closed by the click that closed it
 * looks like a flicker, so a click that arrives this soon is ignored */
const qint64 kReopenGuardMs = 180;

} /* namespace */

AlgorithmPicker::AlgorithmPicker(QWidget *parent)
    : QToolButton(parent)
    , popup_(nullptr)
    , filter_(nullptr)
    , tree_(nullptr)
    , trackExpansion_(true)
    , filtered_(false)
    , expansionSeeded_(false)
    , popupHiddenAt_(0)
{
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    setArrowType(Qt::DownArrow);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumWidth(320);
    setFocusPolicy(Qt::StrongFocus);

    connect(this, &QToolButton::clicked, this, [this]() { showPicker(); });

    updateLabel();
}

/* ==========================================================================
 * Popup
 * ========================================================================== */

void AlgorithmPicker::ensurePopup()
{
    if (popup_)
        return;

    popup_ = new QFrame(this, Qt::Popup);
    popup_->setObjectName(QStringLiteral("algorithmPickerPopup"));
    popup_->setFrameShape(QFrame::StyledPanel);
    popup_->installEventFilter(this);

    QVBoxLayout *layout = new QVBoxLayout(popup_);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(5);

    filter_ = new QLineEdit(popup_);
    filter_->setClearButtonEnabled(true);
    filter_->installEventFilter(this);
    layout->addWidget(filter_);

    tree_ = new QTreeWidget(popup_);
    tree_->setHeaderHidden(true);
    tree_->setRootIsDecorated(true);
    tree_->setUniformRowHeights(true);
    tree_->setIndentation(16);
    tree_->setSelectionMode(QAbstractItemView::SingleSelection);
    tree_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    layout->addWidget(tree_, 1);

    connect(filter_, &QLineEdit::textChanged, this, &AlgorithmPicker::applyFilter);

    connect(tree_, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item, int) {
                if (item->childCount() == 0) {
                    choose(item);
                    return;
                }

                /* Clicking the branch arrow already toggled the group; only a
                 * click on the caption toggles it here. */
                const int arrowZone = tree_->indentation() * 2;
                if (tree_->viewport()->mapFromGlobal(QCursor::pos()).x() < arrowZone)
                    return;

                item->setExpanded(!item->isExpanded());
            });

    connect(tree_, &QTreeWidget::itemExpanded, this, [this](QTreeWidgetItem *item) {
        if (trackExpansion_ && !expandedCaptions_.contains(item->text(0)))
            expandedCaptions_.append(item->text(0));
    });
    connect(tree_, &QTreeWidget::itemCollapsed, this, [this](QTreeWidgetItem *item) {
        if (trackExpansion_)
            expandedCaptions_.removeAll(item->text(0));
    });
}

void AlgorithmPicker::showPicker()
{
    ensurePopup();

    if (popup_->isVisible()) {
        popup_->hide();
        return;
    }

    if (popupHiddenAt_ != 0
        && QDateTime::currentMSecsSinceEpoch() - popupHiddenAt_ < kReopenGuardMs) {
        return;
    }

    rebuildTree();
    filter_->clear();
    filter_->setPlaceholderText(tr("Filter algorithms ..."));

    QTreeWidgetItem *current = findItem(currentPath_);
    if (current) {
        for (QTreeWidgetItem *parent = current->parent(); parent; parent = parent->parent())
            parent->setExpanded(true);
        tree_->setCurrentItem(current);
    }

    /* At least as wide as the button, but wide enough for a full caption even
     * when the window is narrow */
    const int width = qMax(this->width(), kPopupMinWidth);

    const QPoint below = mapToGlobal(QPoint(0, height() + 2));
    QScreen *screen = QGuiApplication::screenAt(below);
    if (!screen)
        screen = QGuiApplication::primaryScreen();

    const QRect area = screen ? screen->availableGeometry()
                              : QRect(below, QSize(width, kPopupHeight));

    const int height = qMin(kPopupHeight, qMax(180, area.height() - 40));

    int y = below.y();
    if (y + height > area.bottom())
        y = qMax(area.top(), mapToGlobal(QPoint(0, 0)).y() - height - 2);

    int x = below.x();
    if (x + width > area.right())
        x = qMax(area.left(), area.right() - width);

    popup_->setGeometry(x, y, width, height);
    popup_->show();

    if (current)
        tree_->scrollToItem(current);

    filter_->setFocus(Qt::PopupFocusReason);
}

/* ==========================================================================
 * The tree
 * ========================================================================== */

void AlgorithmPicker::rebuildTree()
{
    if (!tree_)
        return;

    trackExpansion_ = false;
    tree_->blockSignals(true);
    tree_->clear();

    for (const AlgorithmChoiceGroup &group : groups_) {
        if (group.choices.isEmpty())
            continue;

        QTreeWidgetItem *header = new QTreeWidgetItem(tree_);
        header->setText(0, group.caption);
        header->setToolTip(0, group.tooltip.isEmpty() ? group.caption : group.tooltip);
        header->setFlags(Qt::ItemIsEnabled);        /* a caption, not a choice */

        QFont font = header->font(0);
        font.setBold(true);
        header->setFont(0, font);

        for (const AlgorithmChoice &choice : group.choices) {
            QTreeWidgetItem *leaf = new QTreeWidgetItem(header);
            leaf->setText(0, choice.label);
            leaf->setToolTip(0, QDir::toNativeSeparators(choice.path));
            leaf->setData(0, Qt::UserRole, choice.path);
        }

        header->setExpanded(expandedCaptions_.contains(group.caption));
    }

    /* "Recently used" is the most convenient group, so it starts open — but
     * only until the user closes it, from then on the choice is remembered */
    if (!expansionSeeded_ && tree_->topLevelItemCount() > 0) {
        expansionSeeded_ = true;
        tree_->topLevelItem(0)->setExpanded(true);
        expandedCaptions_.append(tree_->topLevelItem(0)->text(0));
    }

    tree_->blockSignals(false);
    trackExpansion_ = true;
}

void AlgorithmPicker::applyFilter(const QString &text)
{
    if (!tree_)
        return;

    const QString needle = text.trimmed();
    if (needle.isEmpty()) {
        /* Everything comes back with the expansion state the user left behind */
        if (filtered_) {
            filtered_ = false;
            rebuildTree();
        }
        return;
    }

    filtered_ = true;

    trackExpansion_ = false;
    for (int i = 0; i < tree_->topLevelItemCount(); ++i) {
        QTreeWidgetItem *header = tree_->topLevelItem(i);

        int hits = 0;
        for (int c = 0; c < header->childCount(); ++c) {
            QTreeWidgetItem *leaf = header->child(c);
            const bool hit =
                leaf->text(0).contains(needle, Qt::CaseInsensitive)
                || leaf->data(0, Qt::UserRole).toString().contains(needle, Qt::CaseInsensitive);
            leaf->setHidden(!hit);
            if (hit)
                ++hits;
        }

        header->setHidden(hits == 0);
        if (hits > 0)
            header->setExpanded(true);
    }
    trackExpansion_ = true;
}

QTreeWidgetItem *AlgorithmPicker::findItem(const QString &path) const
{
    if (!tree_ || path.isEmpty())
        return nullptr;

    const QString clean = QDir::cleanPath(path);

    for (int i = 0; i < tree_->topLevelItemCount(); ++i) {
        QTreeWidgetItem *header = tree_->topLevelItem(i);
        for (int c = 0; c < header->childCount(); ++c) {
            QTreeWidgetItem *leaf = header->child(c);
            const QString candidate = leaf->data(0, Qt::UserRole).toString();
            if (candidate.compare(clean, Qt::CaseInsensitive) == 0
                || candidate.compare(path, Qt::CaseInsensitive) == 0) {
                return leaf;
            }
        }
    }

    return nullptr;
}

QString AlgorithmPicker::labelFor(const QString &path) const
{
    if (path.isEmpty())
        return QString();

    const QString clean = QDir::cleanPath(path);

    for (const AlgorithmChoiceGroup &group : groups_) {
        for (const AlgorithmChoice &choice : group.choices) {
            if (choice.path.compare(clean, Qt::CaseInsensitive) == 0
                || choice.path.compare(path, Qt::CaseInsensitive) == 0) {
                return choice.label;
            }
        }
    }

    return QString();
}

/* ==========================================================================
 * Selection
 * ========================================================================== */

void AlgorithmPicker::setGroups(const QVector<AlgorithmChoiceGroup> &groups)
{
    groups_ = groups;

    if (tree_)
        rebuildTree();

    /* The device name of the current algorithm may have changed */
    if (!currentPath_.isEmpty()) {
        const QString label = labelFor(currentPath_);
        currentLabel_ = label.isEmpty() ? QFileInfo(currentPath_).fileName() : label;
        updateLabel();
    }
}

void AlgorithmPicker::selectAlgorithm(const QString &path)
{
    if (path.isEmpty()) {
        currentPath_.clear();
        currentLabel_.clear();
        updateLabel();
        return;
    }

    const QFileInfo info(path);
    currentPath_ = info.absoluteFilePath();

    /* The popup is created lazily, so the label cannot come from the tree */
    const QString label = labelFor(currentPath_);
    currentLabel_ = label.isEmpty() ? info.fileName() : label;

    if (QTreeWidgetItem *item = findItem(currentPath_)) {
        for (QTreeWidgetItem *parent = item->parent(); parent; parent = parent->parent())
            parent->setExpanded(true);
        tree_->setCurrentItem(item);
    }

    updateLabel();
}

void AlgorithmPicker::choose(QTreeWidgetItem *item)
{
    const QString path = item->data(0, Qt::UserRole).toString();
    if (path.isEmpty())
        return;

    currentPath_ = path;
    currentLabel_ = item->text(0);
    updateLabel();

    if (popup_)
        popup_->hide();

    emit changed();
}

void AlgorithmPicker::updateLabel()
{
    const int available = qMax(80, width() - 46);   /* room for the arrow */

    if (currentLabel_.isEmpty()) {
        setText(tr("Select algorithm ..."));
        setToolTip(QString());
        return;
    }

    setText(fontMetrics().elidedText(currentLabel_, Qt::ElideMiddle, available));
    setToolTip(QDir::toNativeSeparators(currentPath_));
}

void AlgorithmPicker::retranslate()
{
    if (filter_)
        filter_->setPlaceholderText(tr("Filter algorithms ..."));

    updateLabel();
}

void AlgorithmPicker::resizeEvent(QResizeEvent *event)
{
    QToolButton::resizeEvent(event);
    updateLabel();
}

bool AlgorithmPicker::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == popup_ && event->type() == QEvent::Hide)
        popupHiddenAt_ = QDateTime::currentMSecsSinceEpoch();

    if (watched == filter_) {
        if (event->type() == QEvent::KeyPress) {
            const int key = static_cast<QKeyEvent *>(event)->key();

            if (key == Qt::Key_Down) {
                /* Walk into the list without leaving the keyboard */
                for (int i = 0; i < tree_->topLevelItemCount(); ++i) {
                    QTreeWidgetItem *header = tree_->topLevelItem(i);
                    if (header->isHidden())
                        continue;
                    for (int c = 0; c < header->childCount(); ++c) {
                        if (!header->child(c)->isHidden()) {
                            tree_->setCurrentItem(header->child(c));
                            tree_->setFocus(Qt::PopupFocusReason);
                            return true;
                        }
                    }
                }
                return true;
            }

            if (key == Qt::Key_Return || key == Qt::Key_Enter) {
                /* Take the first algorithm the filter left over */
                for (int i = 0; i < tree_->topLevelItemCount(); ++i) {
                    QTreeWidgetItem *header = tree_->topLevelItem(i);
                    if (header->isHidden())
                        continue;
                    for (int c = 0; c < header->childCount(); ++c) {
                        QTreeWidgetItem *leaf = header->child(c);
                        if (!leaf->isHidden()) {
                            choose(leaf);
                            return true;
                        }
                    }
                }
                return true;
            }
        }
    }

    return QToolButton::eventFilter(watched, event);
}
