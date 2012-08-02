/*
 * Copyright (c) 2012 Jesper Räftegård <jesper@huggpunkt.org>
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

#include "messagehandler.h"

// OBS: Inte del av klass!
// Anropas av Qt.
void messageHandler(QtMsgType type, const char *msg)
{
    MessageHandler::p()->message(type, msg);
}

// Singleton (MessageHandler::p())
MessageHandler *MessageHandler::s_instance = NULL;

MessageHandler::MessageHandler(QObject *parent) :
    QObject(parent)
{
    this->message(QtDebugMsg, "READY.");
}

MessageHandler *MessageHandler::p()
{
    if (s_instance == NULL)
        s_instance = new MessageHandler;                                // alloc: Singleton object

    return s_instance;
}

void MessageHandler::message(QtMsgType type, const char *msg)
{
    QString message;

    message = QDateTime::currentDateTime().toString(Qt::ISODate) + " ";

    switch (type)
    {
    case QtDebugMsg:        message += "";              break;
    case QtWarningMsg:      message += "Warning:";      break;
    case QtCriticalMsg:     message += "Critical:";     break;
    case QtFatalMsg:        message += "Fatal:";        break;
    default:                message += "Message:";
    }

    message += QString(" ") + QString(msg);

    emit deliverMessage(message);

    this->_buffer.enqueue(message);
    if (this->_buffer.length() > this->_bufferLength)
        this->_buffer.dequeue();

    std::cerr << message.toStdString() << std::endl;

    if (type == QtFatalMsg)
        abort();
}

QPointer<ConsoleWindow> MessageHandler::createConsoleWindow()
{
    if (! this->_consoleWindow.isNull())
        return this->_consoleWindow;

    this->_consoleWindow = new ConsoleWindow(0);                // alloc: ConsoleWindow deletes itself upon close()

    if (! QObject::connect(MessageHandler::p(), SIGNAL(deliverMessage(QString)), this->_consoleWindow, SLOT(printMessage(QString))))
        qWarning() << "Could not connect console.";
    QObject::connect(this->_consoleWindow.data(), SIGNAL(destroyed(QObject*)), this, SLOT(notifyQObjectDestroyed(QObject*)));

    this->_consoleWindow->show();
    this->_consoleWindow->raise();
    this->_consoleWindow->activateWindow();

    foreach(QString m, this->_buffer)
        emit deliverMessage(m);

    return this->_consoleWindow;
}

void MessageHandler::notifyQObjectDestroyed(QObject *obj)
{
    qDebug() << "MessageHandler: Received destroyed() event from" << obj;
}
