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

#include "localfile.h"

LocalFileExtension::LocalFileExtension(QObject *parent) :
    QObject(parent)
{
}

LocalFileExtension::~LocalFileExtension()
{
    qDebug() << "LocalFileExtension() destructing.";
}

QPointer<QIODevice> LocalFileExtension::openStream(QIODevice::OpenModeFlag openMode, const QString hurl)
{
    if (this->_file != NULL)
    {
        qWarning() << "LocalFileExtension::openStream(): Bad. I was asked to open a stream when I already had one.";
        return NULL;
    }

    this->_file = this->_resolveHurl(hurl);
    if (! this->_file.isNull())
        if (this->_file->open(openMode))
        {
            qDebug() << "openStream for" << this->_file->fileName();
            return (QIODevice*)this->_file;
        }

    return NULL;
}

// We support fileuri and localfile transports, which means
// the hurl might be either a local filepath or a local filepath
// prepended by "file://".
QPointer<QFile> LocalFileExtension::_resolveHurl(QString hurl)
{
    QString prefix = "file://";
    if (hurl.startsWith(prefix))
        hurl.remove(0, prefix.length());

    qDebug() << "Trying to QFile(" << hurl;
    return new QFile(hurl, this);                                       // alloc: Has parent
}

Q_EXPORT_PLUGIN2(localfileextension, LocalFileExtension)
