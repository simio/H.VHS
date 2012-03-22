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

#include "definitiontable.h"

DefinitionTable::DefinitionTable(QObject *parent) :
    QObject(parent)
{
}

int DefinitionTable::count(Definition::DefinitionType type)
{
    int sum = 0;
    if (type == Definition::NoDefinitionType)
    {
        foreach(Definition::DefinitionType iterator, this->_definitions.keys())
            sum += this->_definitions.value(iterator).size();
    }
    else
    {
        if (this->_definitions.contains(type))
            sum = this->_definitions.value(type).size();
        else
            sum = 0;
    }
    return sum;
}

bool DefinitionTable::update(QPointer<Definition> def)
{
    if (! this->contains(def))
        this->_set(def);            // Previously unknown definition.
    else if (this->get(def) <= def)
        this->_set(def);            // Had an older version in table. Update.
    else
        return false;               // Has an equally new or newer definition. Do nothing and tell about it.

    // The table was changed.
    return true;
}

int DefinitionTable::update(QList<QPointer<Definition> > defs)
{
    int sum;
    foreach(QPointer<Definition> newDef, defs)
        if (this->update(newDef))
            sum++;
    return sum;
}

bool DefinitionTable::contains(Definition::DefinitionType type, QString id)
{
    return this->_definitions.value(type).contains(id);
}

bool DefinitionTable::contains(QPointer<Definition> def)
{
    return this->contains(def->type(), def->id());
}

QPointer<Definition> DefinitionTable::get(Definition::DefinitionType type, QString id)
{
    return this->_definitions.value(type).value(id);
}

QPointer<Definition> DefinitionTable::get(QPointer<Definition> def)
{
    return this->get(def->type(), def->id());
}

bool DefinitionTable::_set(QPointer<Definition> def)
{
    bool overwrite = this->contains(def);
    def->setParent(this);
    this->_definitions[def->type()].insert(def->id(), def);
    return overwrite;
}
