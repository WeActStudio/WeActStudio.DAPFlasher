/*
 * WeAct Studio DAP Flasher
 *
 * QLabel whose height follows the text it wraps.
 *
 * Copyright (C) 2026 WeAct Studio
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef WRAPPEDLABEL_H
#define WRAPPEDLABEL_H

#include <QLabel>
#include <QLayout>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>

/*
 * A word wrapped QLabel for a QFormLayout.
 *
 * QFormLayout gives a row the height of the field's sizeHint(), and for a
 * wrapped label that hint is the height of a single line. Every further line is
 * then cut off - and whether the row happens to be laid out correctly depends
 * on the order of the layout passes, so the same text is rendered properly on
 * one start and clipped on the next.
 *
 * Two details make this work:
 *
 *  - The height to reserve is sizeHint().height(). QLabel::heightForWidth() is
 *    *not* usable here: for rich text it returns the height of a single line no
 *    matter how often the text wraps. Advertising it through the size policy
 *    makes QFormLayout lay the row out for one line and squeeze the group box
 *    below the height it needs.
 *  - A row that has already been laid out keeps its geometry. Reserving the
 *    height only helps once the window is laid out again, so do that as well -
 *    after the pending layout pass, when the width is final.
 */
class WrappedLabel : public QLabel
{
public:
    /* Delay of the second layout pass, in milliseconds */
    enum { kSettleMs = 150 };

    explicit WrappedLabel(QWidget *parent = nullptr)
        : QLabel(parent)
    {
        setWordWrap(true);
    }

    /** setText(), with the height for the wrapped text reserved right away */
    void setRichText(const QString &text)
    {
        QLabel::setText(text);
        reserveHeight();

        /* The rows are filled one after another (the probe list, the algorithm
           catalogue), and the layout request of one of them gets coalesced away
           when the next row is still empty at that moment. So measure and lay
           out again once the event loop is idle, and once more after the other
           rows have had their turn. */
        QTimer::singleShot(0, this, [this]() { reserveHeight(); relayoutWindow(); });
        QTimer::singleShot(kSettleMs, this, [this]() { reserveHeight(); relayoutWindow(); });
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QLabel::resizeEvent(event);
        reserveHeight();
    }

    void showEvent(QShowEvent *event) override
    {
        QLabel::showEvent(event);
        reserveHeight();
        relayoutWindow();
    }

private:
    void reserveHeight()
    {
        /* The height the text needs at the width it currently has - the same
           quantity the row is sized from */
        const int needed = sizeHint().height();
        if (needed > 0 && needed != minimumHeight())
            setMinimumHeight(needed);
    }

    /** Lay the widget chain out again after the height changed */
    void relayoutWindow()
    {
        /* A layout request travels up the parent chain, and one that gets
           coalesced away leaves a row with the height it had before the text
           arrived. Walking the chain bottom-up re-sizes every row from the
           size hints that are correct by now. */
        for (QWidget *widget = this; widget; widget = widget->parentWidget()) {
            if (QLayout *layout = widget->layout()) {
                layout->invalidate();
                layout->activate();
            }
        }

        /* And keep the window tall enough for the rows it has to show: the size
           it starts with is derived from the layout while these texts are still
           empty, so it can be a line too short for them. */
        QWidget *top = window();
        if (!top)
            return;

        if (QLayout *layout = top->layout()) {
            const int needed = layout->minimumSize().height();
            if (needed > 0 && top->minimumHeight() < needed)
                top->setMinimumHeight(needed);
        }
    }
};

#endif /* WRAPPEDLABEL_H */
