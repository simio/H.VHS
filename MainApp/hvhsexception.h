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

#ifndef HVHSEXCEPTION_H
#define HVHSEXCEPTION_H

#include <QtGlobal>
#include <QDebug>
#include <QString>

// This class should include no project files.

class HvhsException
{
public:
    enum Severity {
        Recoverable,
        Unrecoverable,
        Fatal
    };

    HvhsException(QString humaneMessage, Severity severity)
    {
        this->_humaneMessage = humaneMessage; this->_severity = severity;
    }

    QString humaneMessage()
    {
        return this->_humaneMessage;
    }

    Severity severity()
    {
        return this->_severity;
    }

    void report()
    {
        switch (this->_severity)
        {
        case Recoverable:
            qWarning() << "Recoverable exception:" << this->_humaneMessage;
            break;
        case Unrecoverable:
            qCritical() << "Unrecoverable exception:" << this->_humaneMessage;
            break;
        case Fatal:
            qFatal(QString("Fatal exception:"
                           + this->_humaneMessage).toAscii().constData());
            break;
        default:
            qCritical() << "Unclassified exception" << this->_humaneMessage;
        }
    }

private:
    QString _humaneMessage;
    Severity _severity;
};

#endif // HVHSEXCEPTION_H
