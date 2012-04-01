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

#ifndef LOCALFILE_H
#define LOCALFILE_H

#define HVHS_EXTENSION

#include <QtGui/QtGui>
#include <QObject>
#include <QPointer>
#include <QDataStream>

#include "streams.h"

class LocalFileExtension : public QObject, ExtensionInterfaceStreams
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterfaceStreams)

public:
    explicit LocalFileExtension(QObject *parent = 0);
    ~LocalFileExtension();

    QDataStream * createStream(QIODevice::OpenModeFlag openMode, const QString hurl);
    QTextStream * createTextStream(QIODevice::OpenModeFlag openMode, const QString hurl);

signals:

public slots:

private:
    QPointer<QFile> _file;
    QDataStream * _stream;
    QTextStream * _textStream;

    QFileInfo _resolveHurl(QString hurl);

};

#endif // LOCALFILE_H
