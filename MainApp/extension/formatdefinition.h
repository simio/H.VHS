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

#ifndef FORMAT_H
#define FORMAT_H

// Format types: "Raw" (eg. any), "FLV", "VhsXml",

#include <QObject>
#include <QDateTime>

#include "main.h"

class FormatDefinition : public QObject
{
    Q_OBJECT
public:
    explicit FormatDefinition(QObject *parent = 0);
    FormatDefinition(const FormatDefinition &original, QObject *parent = 0);
    FormatDefinition &operator=(const FormatDefinition &original);

    enum ContentType {
        Unknown,
        Empty,              // No data or metadata
        Any,                // Any of the below
        Meta,               // Contains all information necessary to access or build a complete file
        Data,               // Does not contain enough all information necessary to access or build a complete file
        Complete            // Contains a complete file
    };

    QByteArray uid() const;
    QString prettyName() const;
    ContentType contentType() const;
    QDateTime dateTime() const;

    void setUid(QByteArray uid);
    void setPrettyName(QString prettyName);
    void setContentType(ContentType contentType);
    void setContentType(QString contentType);
    void setDateTime(QDateTime dateTime);

public slots:

signals:

private:
    QByteArray _uid;               // Used in extensions
    QString _prettyName;           // Defaults to _uid
    ContentType _contentType;
    QDateTime _dateTime;

};

#endif // FORMAT_H
