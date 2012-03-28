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

#ifndef HURL_H
#define HURL_H

#include <QString>
#include <QUrl>
#include <QDateTime>

#include "main.h"

class HUrl
{
public:
    HUrl();
    HUrl(QString url, QDateTime timestamp = QDateTime());
    HUrl(QUrl url, QDateTime timestamp = QDateTime());

    inline bool isValid();
    inline QUrl toUrl();
    inline QString toString();
    inline QDateTime dateTime();


private:
    QString _url;
    QDateTime _timestamp;

    void _set(QString url, QDateTime timestamp);
};

#endif // HURL_H
