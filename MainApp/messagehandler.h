/*
 * Copyright (c) 2012 Jesper R�fteg�rd <jesper@huggpunkt.org>
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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>

#include <QObject>
#include <QDebug>
#include <QPointer>

#include "consolewindow.h"

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageHandler(QObject *parent = 0);
    static MessageHandler *pointer();

    void message(QtMsgType type, const char *msg);
    QPointer<ConsoleWindow> createConsoleWindow();

public slots:

signals:
    void deliverMessage(QString message);

private:
    static MessageHandler *s_instance;

    QPointer<ConsoleWindow> _consoleWindow;
};

void messageHandler(QtMsgType type, const char *msg);

#endif // MESSAGEHANDLER_H