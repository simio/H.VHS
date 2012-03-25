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

#ifndef DEFINITION_H
#define DEFINITION_H

#include <QObject>
#include <QDate>
#include <QUuid>

class Definition : public QObject
{
    Q_OBJECT
public:
    enum DefinitionType {
        NoDefinitionType,
        TransportDefinitionType,
        FormatDefinitionType,
        ExtensionDefinitionType,
        CassetteDefinitionType
    };
    Definition(QString id,
               QString name,
               QString description,
               QDateTime releaseDate,
               DefinitionType type,
               QObject *parent = 0);

    // ==, !=, <= and >= compares both id and release dates, while < and > compare only dates.
    virtual bool operator==(const Definition &original) const;
    virtual bool operator!=(const Definition &original) const;
    virtual bool operator<(const Definition &original) const;
    virtual bool operator<=(const Definition &original) const;
    virtual bool operator>(const Definition &original) const;
    virtual bool operator>=(const Definition &original) const;

    virtual bool isValid() const;

    QString id() const                                  { return this->_id; }
    QString name() const                                { return this->_name; }
    QString description() const                         { return this->_description; }
    QDateTime releaseDate() const                       { return this->_releaseDate; }
    Definition::DefinitionType type() const             { return this->_type; }

signals:

public slots:

private:
    explicit Definition();
    QString _id;                 // Unique ID (generated from QUuid by default)
    QString _name;                  // Localised name
    QString _description;           // Localised description
    QDateTime _releaseDate;
    DefinitionType _type;
};

#endif // DEFINITION_H
