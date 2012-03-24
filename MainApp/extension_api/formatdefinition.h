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

#ifndef FORMAT_H
#define FORMAT_H

#include <QObject>
#include <QDateTime>

#include "main.h"

#include "definition.h"

class FormatDefinition : public Definition
{
public:
    enum Completeness {
        Invalid,
        NotEmpty,           // Format may not contain nothing
        MetaOnly,           // Contains all information necessary to access and/or compile a complete file
        DataOnly,           // Does not contain all information necessary to compile a complete file
        Complete            // Contains a complete file
    };

    explicit FormatDefinition();
    FormatDefinition(QString id,
                     QString name,
                     QString description,
                     QDateTime releaseDate,
                     QString completeness,
                     QStringList mimeTypes,
                     QObject *parent = 0);
    FormatDefinition(const FormatDefinition &original, QObject *parent = 0);
    FormatDefinition &operator=(const FormatDefinition &original);

    bool isValid() const;

    // The MIME type list is prioritised. The first type is preferred for output.
    QString mimeType(int index = 0) const;
    QStringList mimeTypes() const;
    void insertMimeType(int index, QString mimeType);

    Completeness completeness() const;
    void setCompleteness(Completeness c);
    Completeness setCompleteness(QString contentType);

public slots:

signals:

private:
    Completeness _completeness;
    QStringList _mimeTypes;
};

#endif // FORMAT_H
