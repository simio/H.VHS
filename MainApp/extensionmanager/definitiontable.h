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

#ifndef EXTENSIONMANAGER_DEFINITIONTABLE_H
#define EXTENSIONMANAGER_DEFINITIONTABLE_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QHash>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#include "main.h"

#include "extensionmanager/definitions/definition.h"

#include "vhsxml/documentreader.h"

class DefinitionTable : public QObject
{
    Q_OBJECT
public:
    explicit DefinitionTable(QObject *parent = 0);

    int
    count(Definition::DefinitionType type = Definition::NoDefinitionType) const;

    // returns true iff definition table changed
    bool
    update(QSharedPointer<Definition> def);

    // returns number of changes to definition table
    int
    update(QList<QSharedPointer<Definition> > defs);

    bool
    contains(Definition::DefinitionType type, QString id) const;

    bool
    contains(QSharedPointer<Definition> def) const;

    QSharedPointer<Definition>
    get(Definition::DefinitionType type, QString id) const;

    QSharedPointer<Definition>
    get(QSharedPointer<Definition> def) const;

    QHash<QString,QSharedPointer<Definition> >
    getAll(Definition::DefinitionType type) const;

signals:

public slots:

private:
    // returns true if the total number of definitions changed
    bool _set(QSharedPointer<Definition> def);

    QHash<Definition::DefinitionType,
          QHash<QString, QSharedPointer<Definition> > > _definitions;
};

#endif // EXTENSIONMANAGER_DEFINITIONTABLE_H
