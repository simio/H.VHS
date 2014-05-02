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

#ifndef CONFIG_MESSAGEHANDLER_H
#define CONFIG_MESSAGEHANDLER_H

#include <iostream>

#include <QObject>
#include <QDebug>
#include <QString>
#include <QQueue>
#include <QDateTime>

#include "consolewindow.h"

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageHandler(QObject *parent = 0);
    static MessageHandler *
    p();

    void
    message(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    ConsoleWindow * createConsoleWindow();

public slots:
    void
    notifyQObjectDestroyed(QObject * obj);

signals:
    void
    deliverMessage(QString message);

private:
    static MessageHandler *s_instance;

    QQueue<QString> _buffer;
    static const qint64 _bufferLength = 10240;      // Lines

    // Independent Window; won't get parent
    ConsoleWindow * _consoleWindow;
};

void
messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // CONFIG_MESSAGEHANDLER_H
