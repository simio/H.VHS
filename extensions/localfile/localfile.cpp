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
    this->_stream = NULL;
    this->_textStream = NULL;
}

LocalFileExtension::~LocalFileExtension()
{
    if (this->_stream != NULL)
        delete this->_stream;
    else if (this->_textStream != NULL)
        delete this->_textStream;
}

QDataStream * LocalFileExtension::createStream(QIODevice::OpenModeFlag openMode, const QString hurl)
{
    if (this->_textStream != NULL || this->_stream != NULL)
    {
        qWarning() << "LocalFileExtension::createStream(): Bad. I was asked to open a stream when I already had one.";
        return NULL;
    }

    QFileInfo fileinfo = this->_resolveHurl(hurl);
    this->_file = new QFile(fileinfo.canonicalFilePath(), this);                    // alloc: Has parent
    if (this->_file->open(openMode))
    {
        this->_stream = new QDataStream(this->_file);           // alloc: Deleted in destructor or here
        if (this->_stream->status() != QDataStream::Ok)
        {
            delete this->_stream;
            this->_stream = NULL;
        }
        else
            return this->_stream;
    }

    return NULL;
}

QTextStream * LocalFileExtension::createTextStream(QIODevice::OpenModeFlag openMode, const QString hurl)
{
    if (this->_textStream != NULL || this->_stream != NULL)
    {
        qWarning() << "LocalFileExtension::createTextStream(): Bad. I was asked to open a stream when I already had one.";
        return NULL;
    }

    QFileInfo fileinfo = this->_resolveHurl(hurl);
    this->_file = new QFile(fileinfo.canonicalFilePath(), this);                    // alloc: Has parent
    if (this->_file->open(openMode))
    {
        this->_textStream = new QTextStream(this->_file);       // alloc: Deleted in destructor or here
        if (this->_textStream->status() != QTextStream::Ok)
        {
            delete this->_textStream;
            this->_textStream = NULL;
        }
        else
            return this->_textStream;
    }

    return NULL;
}

// We support fileuri and localfile transports, which means
// the hurl might be either a local filepath or a local filepath
// prepended by "file://".
QFileInfo LocalFileExtension::_resolveHurl(QString hurl)
{
    QString prefix = "file://";
    if (hurl.startsWith(prefix))
        hurl.remove(0, prefix.length());

    return QFileInfo(hurl);
}

Q_EXPORT_PLUGIN2(localfileextension, LocalFileExtension)
