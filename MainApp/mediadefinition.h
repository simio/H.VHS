/*
 * Copyright (c) 2012 Jesper R�fteg�rd <jesper@huggpunkt.org>
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

#ifndef MEDIA_H
#define MEDIA_H

// Media types: "QTextStream", "QDataStream", "http", "https", "ftp", "rtmp", "rtmpe", "FileProtocol", "LocalFile", etc

#include <QObject>

#include "main.h"

class MediaDefinition : public QObject
{
    Q_OBJECT
public:
    explicit MediaDefinition(QObject *parent = 0);

    QByteArray uid();
    QString prettyName();
    void setUid(QByteArray uid);
    void setPrettyName(QString prettyName);

signals:

public slots:

private:
    QByteArray _uid;           // Used in extensions
    QString _prettyName;    // Defaults to _uid
};

#endif // MEDIA_H