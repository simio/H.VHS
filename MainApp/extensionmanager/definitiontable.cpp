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

#include "definitiontable.h"

DefinitionTable::DefinitionTable(QObject *parent) :
    QObject(parent)
{
    /*  Read all transport, format and extension definitions
     *  and put them inte to the table.
     */
    QStringList locations;
    locations << Configuration::p()->getStorageLocation(Configuration::SystemPresetsLocation).canonicalPath()
              << Configuration::p()->getStorageLocation(Configuration::UserPresetsLocation).canonicalPath();

    QDir base = Configuration::p()->getStorageLocation(Configuration::SystemExtensionsLocation);
    foreach(QString folder, base.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        locations << base.canonicalPath() + "/" + folder;

    base = Configuration::p()->getStorageLocation(Configuration::UserExtensionsLocation);
    foreach(QString folder, base.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        locations << base.canonicalPath() + "/" + folder;

    QFileInfoList files;
    foreach (QString dir, locations)
        files.append(QDir(dir).entryInfoList(QStringList("*.xml"), QDir::Files | QDir::Readable));

    foreach (QFileInfo file, files)
    {
        // alloc: Has parent and is deleted here.
        QPointer<VhsXml::DocumentReader> xml = new VhsXml::DocumentReader(file, this);
        this->update(xml->definitions(Definition::NoDefinitionType, this));
        delete xml;
    }

    qDebug() << "DefinitionTable constructed with" << this->_definitions.size() << "definition types";
}

int DefinitionTable::count(Definition::DefinitionType type) const
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
    {
        this->_set(def);            // Previously unknown definition.
        qDebug() << "DefinitionTable: Type" << def->prettyType()
                 << "definition added:    " << def->releaseDate() << def->name();
    }
    // Check if both date and id is identical
    else if (this->get(def) == def)
    {
        // Since definitions should be added in prioritised order (from system-wide to user-specific, etc),
        // definitions with id:s and ages identical to a definition already in the table are ignored.
        // This ensures system-wide definitions are not replaced by half-assed edits of user specific
        // definitions, which would complicate bug reporting.
        qDebug() << "DefinitionTable: Type" << def->prettyType() << "definition ignored:  "
                 << def->releaseDate() << def->name() << "(identical definition already in table)";
    }
    // Since == is already tested, this test is true only for (this->get(def) <= def && !(this->get(def) == def))
    else if (this->get(def) <= def)
    {
        this->_set(def);            // Had an older version in table. Update.
        qDebug() << "DefinitionTable: Type" << def->prettyType()
                 << "definition updated:  " << def->releaseDate() << def->name();
    }
    else
    {
        qDebug() << "DefinitionTable: Type" << def->prettyType()
                 << "definition discarded:" << def->releaseDate() << def->name();
        return false;               // Has an equally new or newer definition. Do nothing and tell about it.
    }

    // The table was changed.
    return true;
}

int DefinitionTable::update(QList<QPointer<Definition> > defs)
{
    int sum = 0;
    foreach(QPointer<Definition> newDef, defs)
        if (this->update(newDef))
            sum++;
    return sum;
}

bool DefinitionTable::contains(Definition::DefinitionType type, QString id) const
{
    return this->_definitions.value(type).contains(id);
}

bool DefinitionTable::contains(QPointer<Definition> def) const
{
    return this->contains(def->type(), def->id());
}

QPointer<Definition> DefinitionTable::get(Definition::DefinitionType type, QString id) const
{
    return this->_definitions.value(type).value(id);
}

QPointer<Definition> DefinitionTable::get(QPointer<Definition> def) const
{
    return this->get(def->type(), def->id());
}

QHash<QString, QPointer<Definition> > DefinitionTable::getAll(Definition::DefinitionType type) const
{
    return this->_definitions.value(type, QHash<QString,QPointer<Definition> >());
}

bool DefinitionTable::_set(QPointer<Definition> def)
{
    bool overwrite = this->contains(def);
    def->setParent(this);
    this->_definitions[def->type()].insert(def->id(), def);
    return overwrite;
}
