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

#ifndef EXTENSIONMANAGER_DEFINITION_DEFINITIONDATA_H
#define EXTENSIONMANAGER_DEFINITION_DEFINITIONDATA_H

#include <QString>
#include <QDateTime>
#include <QSharedData>
#include <QHash>
#include <QVariant>

// We need access to Definition::DefinitionType
#include "extensionmanager/definitions/definition.h"

class DefinitionData : public QSharedData
{
public:
    /*  By using an enum for QHash keys, any use of invalid keys
     *  result in compile-time errors instead of failing silently.
     */
    enum Key {
        // DefinitionData
        Id,
        Name,
        Description,
        ReleaseDate,
        Type,

        // FormatDefinition
        Completeness,
        IsTextual,
        MimeTypes
    };

    DefinitionData()
    { }

    DefinitionData(const DefinitionData &original)
        : QSharedData(original),
          _map(original._map)
    { }

    // Return true iff this value is set.
    bool has(const Key &key) const
    {
        return this->_map.contains(key);
    }

    // Return value or QVariant() if it isn't set.
    QVariant get(const Key &key) const
    {
        return this->_map.value(key, QVariant());
    }

    // Add or update value. Return false if no change.
    bool set(const Key &key, const QVariant &value)
    {
        if (this->_map.contains(key) && this->_map.value(key) == value)
            return false;

        this->_map.insert(key, value);
        return true;
    }

    // Add new value, but don't replace existing values. Return true iff success.
    bool add(const Key &key, const QVariant &value)
    {
        if (this->_map.contains(key))
            return false;
        this->_map.insert(key, value);
        return true;
    }

    // Replace (only an) existing value. Return true iff success.
    bool replace(const Key &key, const QVariant &value)
    {
        if (! this->_map.contains(key))
            return false;
        this->_map.insert(key, value);
        return true;
    }

    // Remove existing value. Return true iff success.
    bool remove(const Key &key)
    {
        if (! this->_map.contains(key))
            return false;
        this->_map.remove(key);
        return true;
    }

private:
    QHash<Key,QVariant> _map;
};

#endif // EXTENSIONMANAGER_DEFINITION_DEFINITIONDATA_H
