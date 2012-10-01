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

#ifndef APPWIDECLASSES_HURL_H
#define APPWIDECLASSES_HURL_H

#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QFileInfo>

#include "config/configuration.h"
#include "config/messagehandler.h"

/* Url class to hold anything. File paths and names should be
 * interpretable by QFileInfo. While being stored in the _url QString,
 * they are prepended with file://
 */

class HUrl
{
public:
    HUrl();
    HUrl(QString uri, QDateTime timestamp = QDateTime());
    HUrl(QUrl url, QDateTime timestamp = QDateTime());
    HUrl(QFileInfo file, QDateTime timestamp = QDateTime());

    inline bool
    isValid() const;

    inline QUrl
    toUrl() const;

    inline QString
    toString() const;

    inline QFileInfo
    toFileInfo() const;

    inline QDateTime
    dateTime() const;


private:
    QString _uri;
    QDateTime _timestamp;

    void
    _set(QString uri, QDateTime timestamp);
};

#endif // APPWIDECLASSES_HURL_H
