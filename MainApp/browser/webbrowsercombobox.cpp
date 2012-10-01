/*
 * Copyright (c) 2012 Jesper Raftegard <jesper@huggpunkt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "webbrowsercombobox.h"

WebBrowserLineEdit::WebBrowserLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(onTextChanged(QString)));
}

void
WebBrowserLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);

    if (! this->hasSelectedText())
        this->selectAll();
}

void
WebBrowserLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);

    if (this->hasSelectedText())
        this->deselect();

    this->setCursorPosition(0);
}

void
WebBrowserLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    this->selectAll();
}

void
WebBrowserLineEdit::keyPressEvent(QKeyEvent *e)
{
    QLineEdit::keyPressEvent(e);

    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
    {
        this->selectAll();
        emit completedInput(this->text());
    }
}

WebBrowserComboBox::WebBrowserComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->setLineEdit(new WebBrowserLineEdit(this));         // alloc: Has owner
    this->setInsertPolicy(QComboBox::NoInsert);
    connect(this->lineEdit(),
            SIGNAL(completedInput(const QString &)),
            this,
            SLOT(whenCompletedInput(const QString &)));
}

void
WebBrowserComboBox::whenCompletedInput(const QString &text)
{
    emit activated(text);
}

bool
WebBrowserComboBox::addUrl(const QIcon &icon, const QUrl &url)
{
    // If there are items, and the top item has an identical host, it
    // is updated.  Otherwise, we add a new item at the top.
    if (url.host() == this->itemData(0).toUrl().host() && this->count() > 0)
    {
        this->setItemText(0, url.toString());
        this->setItemData(0, url);
        this->setItemIcon(0, icon);
    }
    else
    {
        this->insertItem(0, icon, url.toString(), url);
    }

    // Returns false if nothing was changed, which currently never happens.
    return true;
}

void
WebBrowserLineEdit::onTextChanged(const QString &text)
{
    if (! this->hasFocus())
        this->setCursorPosition(0);
}
