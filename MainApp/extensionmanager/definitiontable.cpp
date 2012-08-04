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
    /*  Read all transport, format and extension definitions and put them inte to the table.
     *
     *  update() replaces definitions where the definition currently in the table has the same
     *  id and an older releaseDate than the one evaluated. On the other hand, if also the
     *  releaseDates are identical, no replacing occurs. Therefore, make sure directories are
     *  loaded in the correct order.
     */
    QStringList locations;
    locations << Configuration::p()->getStorageLocation(Configuration::SystemPresetsLocation).canonicalPath()
              << Configuration::p()->getStorageLocation(Configuration::UserPresetsLocation).canonicalPath();

    QDir systemBase = Configuration::p()->getStorageLocation(Configuration::SystemExtensionsLocation);
    foreach(QString folder, systemBase.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        locations << systemBase.canonicalPath() + "/" + folder;

    QDir userBase = Configuration::p()->getStorageLocation(Configuration::UserExtensionsLocation);
    foreach(QString folder, userBase.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        locations << userBase.canonicalPath() + "/" + folder;

    QFileInfoList files;
    foreach (QString dir, locations)
        files.append(QDir(dir).entryInfoList(QStringList("*.xml"), QDir::Files | QDir::Readable));

    foreach (QFileInfo file, files)
    {
        // alloc: QScopedPointer
        QScopedPointer<VhsXml::DocumentReader> xml(new VhsXml::DocumentReader(file, 0));
        this->update(xml->definitions(Definition::NoDefinitionType, this));
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

bool DefinitionTable::update(QSharedPointer<Definition> def)
{
    if (! this->contains(def))
    {
        this->_set(def);            // Previously unknown definition.
        qDebug() << "DefinitionTable: Type" << def.data()->prettyType()
                 << "definition added:    " << def.data()->releaseDate() << def.data()->name();
    }
    // Check if both date and id is identical
    else if (this->get(def).data() == def.data())
    {
        // Since definitions should be added in prioritised order (from system-wide to user-specific, etc),
        // definitions with id:s and ages identical to a definition already in the table are ignored.
        // This ensures system-wide definitions are not replaced by half-assed edits of user specific
        // definitions, which would complicate bug reporting.
        qDebug() << "DefinitionTable: Type" << def.data()->prettyType() << "definition ignored:  "
                 << def.data()->releaseDate() << def.data()->name() << "(identical definition already in table)";
    }
    // Since == is already tested, this test is true only for (this->get(def) <= def && !(this->get(def) == def))
    else if (this->get(def).data() <= def.data())
    {
        this->_set(def);            // Had an older version in table. Update.
        qDebug() << "DefinitionTable: Type" << def.data()->prettyType()
                 << "definition updated:  " << def.data()->releaseDate() << def.data()->name();
    }
    else
    {
        qDebug() << "DefinitionTable: Type" << def.data()->prettyType()
                 << "definition discarded:" << def.data()->releaseDate() << def.data()->name();
        return false;               // Has an equally new or newer definition. Do nothing and tell about it.
    }

    // The table was changed.
    return true;
}

int DefinitionTable::update(QList<QSharedPointer<Definition> > defs)
{
    int sum = 0;
    foreach(QSharedPointer<Definition> newDef, defs)
        if (this->update(newDef))
            sum++;
    return sum;
}

bool DefinitionTable::contains(Definition::DefinitionType type, QString id) const
{
    return this->_definitions.value(type).contains(id);
}

bool DefinitionTable::contains(QSharedPointer<Definition> def) const
{
    return this->contains(def.data()->type(), def.data()->id());
}

QSharedPointer<Definition> DefinitionTable::get(Definition::DefinitionType type, QString id) const
{
    return this->_definitions.value(type).value(id);
}

QSharedPointer<Definition> DefinitionTable::get(QSharedPointer<Definition> def) const
{
    return this->get(def.data()->type(), def.data()->id());
}

QHash<QString, QSharedPointer<Definition> > DefinitionTable::getAll(Definition::DefinitionType type) const
{
    return this->_definitions.value(type, QHash<QString,QSharedPointer<Definition> >());
}

bool DefinitionTable::_set(QSharedPointer<Definition> def)
{
    bool overwrite = this->contains(def);
    def->setParent(this);
    this->_definitions[def.data()->type()].insert(def.data()->id(), def);
    // Since the (possibly) overwritten definition is managed by QSharedPointer,
    // it isn't deleted here (or anywhere).
    return overwrite;
}
