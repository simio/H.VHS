/*
 * Copyright (c) 2012 Jesper Räftegård <jesper@huggpunkt.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
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

#ifndef WEBBROWSERCOMBOBOX_H
#define WEBBROWSERCOMBOBOX_H

#include <QLineEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QKeyEvent>

#include "main.h"

class WebBrowserLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit WebBrowserLineEdit(QWidget *parent = 0);

protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

signals:
    void completedInput(const QString &text);

public slots:

};

class WebBrowserComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit WebBrowserComboBox(QWidget *parent = 0);

    bool addUrl(const QIcon &icon, const QUrl &url);

protected:

signals:

public slots:
    void whenCompletedInput(const QString &text);

};

#endif // WEBBROWSERCOMBOBOX_H
