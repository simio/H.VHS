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

#include "hurl.h"

HUrl::HUrl()
{
    this->_set(QString(), QDateTime());         // Null QDateTime is not valid()
}

HUrl::HUrl(QString uri, QDateTime timestamp )
{
    this->_set(uri, timestamp);
}

HUrl::HUrl(QUrl url, QDateTime timestamp)
{
    this->_set(url.toString(), timestamp);
}

HUrl::HUrl(QFileInfo file, QDateTime timestamp)
{
    if (! timestamp.isValid())
        timestamp = file.lastModified();
    this->_set("file://" + file.canonicalFilePath(), timestamp);
}

QUrl
HUrl::toUrl() const
{
    return QUrl(this->_uri);
}

QString
HUrl::toString() const
{
    return this->_uri;
}

QFileInfo
HUrl::toFileInfo() const
{
    QString filename = this->_uri;
    if (! filename.startsWith("file://"))
        return QString();

    filename.remove(0, 7);
    return QFileInfo(filename);
}

QDateTime
HUrl::dateTime() const
{
    return this->_timestamp;
}

bool
HUrl::isValid() const
{
    return (! this->_uri.isEmpty() &&
	    this->_timestamp.isValid() &&
	    QRegExp("^[a-z]+:").indexIn(this->_uri) == 0
	);
}

void
HUrl::_set(QString uri, QDateTime timestamp)
{
    if (uri.startsWith("hvhs:"))
	uri.remove(0, 5);

    this->_uri = uri;

    if (timestamp.isNull() || ! timestamp.isValid())
        this->_timestamp = QDateTime::currentDateTimeUtc();
    else
        this->_timestamp = timestamp;
}
