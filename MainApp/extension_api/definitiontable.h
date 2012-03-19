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

#ifndef DEFINITIONTABLE_H
#define DEFINITIONTABLE_H

#include <QObject>
#include <QPointer>
#include <QByteArray>

#include "main.h"

#include "extension_api/definition.h"

class DefinitionTable : public QObject
{
    Q_OBJECT
public:
    explicit DefinitionTable(QObject *parent = 0);

    int count(Definition::DefinitionType type = Definition::NoDefinitionType);
    bool update(QPointer<Definition> def);                                      // returns true iff definition table changed
    int update(QList<QPointer<Definition> > defs);                              // returns number of changes to definition table
    bool contains(Definition::DefinitionType type, QByteArray id);
    bool contains(QPointer<Definition> def);
    QPointer<Definition> get(Definition::DefinitionType type, QByteArray id);
    QPointer<Definition> get(QPointer<Definition> def);

signals:

public slots:

private:
    bool _set(QPointer<Definition> def);                                      // returns true if the total number of definitions changed

    QHash<Definition::DefinitionType, QHash<QByteArray, QPointer<Definition> > > _definitions;

};

#endif // DEFINITIONTABLE_H
