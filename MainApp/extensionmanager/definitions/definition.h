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

#ifndef EXTENSIONMANAGER_DEFINITIONS_DEFINITION_H
#define EXTENSIONMANAGER_DEFINITIONS_DEFINITION_H

#include <QMetaObject>
#include <QObject>
#include <QSharedDataPointer>
#include <QDate>
#include <QUuid>

class DefinitionData;

/*  Since this class inherits Definition, which is implicitly shared
 *  through using a d pointer to QSharedData, none of the following
 *  may be defined inline: ctor, dtor, copy constructor, assignment
 *  operator.  Also, ctor and dtor must be defined.
 */

class Definition : public QObject
{
    Q_OBJECT
public:
    // Adding a new definition type here includes updating prettyType()
    enum DefinitionType {
        // Use to forcibly make definition invalid
        NoDefinitionType,
        // For instances of Definition (non-polymorphic)
        BaseDefinitionType,
        // For instances of TransportDefinition
        TransportDefinitionType,
        // For instances of FormatDefinition
        FormatDefinitionType,
        // For instances of ExtensionDefinition
        ExtensionDefinitionType,
        // For instances of CassetteDefinition
        CassetteDefinitionType
    };

    Definition(QString id,
               QString name,
               QString description,
               QDateTime releaseDate,
               DefinitionType type,
               QObject *parent = 0);

    ~Definition();

    Definition(const Definition &original);

    virtual Definition &operator
    =(const Definition &original);

    // ==, !=, <= and >= compares both id and release dates, while <
    // and > compare only dates.
    virtual bool operator
    ==(const Definition &original) const;

    virtual bool operator
    !=(const Definition &original) const;

    virtual bool operator
    <(const Definition &original) const;

    virtual bool operator
    <=(const Definition &original) const;

    virtual bool operator
    >(const Definition &original) const;

    virtual bool operator
    >=(const Definition &original) const;

    virtual bool
    isValid() const;

    QString
    id() const;

    QString
    name() const;

    QString
    description() const;

    QDateTime
    releaseDate() const;

    Definition::DefinitionType
    type() const;

    QString
    prettyType() const;

signals:

public slots:

protected:
    QSharedDataPointer<DefinitionData> _d;

private:
    Definition();
};

#endif // EXTENSIONMANAGER_DEFINITIONS_DEFINITION_H
