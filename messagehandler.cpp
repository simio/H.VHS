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

#include "messagehandler.h"

// Singleton (MessageHandler::pointer())
MessageHandler *MessageHandler::s_instance = NULL;

MessageHandler::MessageHandler(QObject *parent) :
    QObject(parent)
{
}

MessageHandler *MessageHandler::pointer()
{
    if (s_instance == NULL)
        s_instance = new MessageHandler;

    return s_instance;
}

void MessageHandler::message(QtMsgType type, const char *msg)
{
    QString message;

    switch (type)
    {
    case QtDebugMsg:        message = "";         break;
    case QtWarningMsg:      message = tr("Warning:");       break;
    case QtCriticalMsg:     message = tr("Critical:");      break;
    case QtFatalMsg:        message = tr("Fatal:");         break;
    default:                message = tr("Message:");
    }

    message += QString(" ") + QString(msg);

    emit deliverMessage(message);

    std::cerr << message.toStdString() << std::endl;

    if (type == QtFatalMsg)
        abort();
}

QPointer<ConsoleWindow> MessageHandler::createConsoleWindow()
{
    if (this->_consoleWindow.isNull())
        this->_consoleWindow = new ConsoleWindow(0);

    this->_consoleWindow->show();
    this->_consoleWindow->raise();
    this->_consoleWindow->activateWindow();

    if (QObject::connect(MessageHandler::pointer(), SIGNAL(deliverMessage(QString)), this->_consoleWindow, SLOT(printMessage(QString))))
        qDebug() << tr("Console connected.");
    else
        qWarning() << tr("Could not connect console.");

    return this->_consoleWindow;

}

// OBS: Inte del av klass
void messageHandler(QtMsgType type, const char *msg)
{
    MessageHandler::pointer()->message(type, msg);
}
