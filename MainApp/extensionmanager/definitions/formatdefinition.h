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

#ifndef EXTENSIONMANAGER_DEFINITIONS_FORMATDEFINITION_H
#define EXTENSIONMANAGER_DEFINITIONS_FORMATDEFINITION_H

#include <QObject>
#include <QDateTime>

#include "main.h"

#include "definition.h"

/*  Since this class inherits Definition, which is implicitly shared
 *  through using a d pointer to QSharedData, none of the following
 *  may be defined inline: ctor, dtor, copy constructor, assignment
 *  operator.  Also, ctor and dtor must be defined.
 */

class FormatDefinition : public Definition
{
public:
    enum Completeness {
        Invalid,
        NotEmpty,           // Format may not contain nothing
        MetaOnly,           // Contains all information necessary to
                            // access and/or compile a complete file
        DataOnly,           // Does not contain all information
                            // necessary to compile a complete file
        Complete            // Contains a complete file
    };

    FormatDefinition(QString id,
                     QString name,
                     QString description,
                     QDateTime releaseDate,
                     QString completeness,
                     bool isTextual,
                     QStringList mimeTypes,
                     QObject *parent = 0);

    ~FormatDefinition();

    bool
    isValid() const;

    Completeness
    completeness() const;
    bool
    isTextual() const;

    // The MIME type list is prioritised. The first type is preferred
    // for output.
    QStringList
    mimeTypes() const;
    QString
    mimeType(int index = 0) const;

public slots:

signals:

private:
    FormatDefinition();

    FormatDefinition::Completeness
    _strToCompleteness(QString str);

};

#endif // EXTENSIONMANAGER_DEFINITIONS_FORMATDEFINITION_H
