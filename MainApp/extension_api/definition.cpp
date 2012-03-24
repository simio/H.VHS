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

#include "definition.h"

Definition::Definition(QString id,
                       QString name,
                       QString description,
                       QDateTime releaseDate,
                       DefinitionType type,
                       QObject *parent) :
    QObject(parent)
{
    this->_id = id;
    this->_name = name;
    this->_description = description;
    this->_releaseDate = releaseDate;
    this->_type = type;
}

Definition::Definition(const Definition &original, QObject *parent) :
    QObject(parent),
    _id(original._id),
    _name(original._name),
    _description(original._description),
    _releaseDate(original._releaseDate),
    _type(original._type)
{ }

Definition &Definition::operator =(const Definition &original)
{
    this->_id = original._id;
    this->_name = original._name;
    this->_description = original._description;
    this->_releaseDate = original._releaseDate;
    this->_type = original._type;
    return *this;
}

// ==, !=, <= and >= compares both id and release dates, while < and > compare only dates.
bool Definition::operator ==(const Definition &other) const     { return this->_id == other._id
                                                                    && this->_releaseDate == other._releaseDate; }
bool Definition::operator !=(const Definition &other) const     { return !(*this == other); }
bool Definition::operator <(const Definition &other) const      { return this->_releaseDate < other._releaseDate; }
bool Definition::operator <=(const Definition &other) const     { return this->_id == other._id
                                                                    && this->_releaseDate <= other._releaseDate; }
bool Definition::operator >(const Definition &other) const      { return this->_releaseDate > other._releaseDate; }
bool Definition::operator >=(const Definition &other) const     { return this->_id == other._id
                                                                    && this->_releaseDate >= other._releaseDate; }

bool Definition::isValid() const
{
    return (!this->_id.isEmpty()
            && !this->_name.isEmpty()
            && this->_releaseDate.isValid()
            && this->_type != NoDefinitionType);
}
