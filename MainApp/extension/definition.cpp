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

#include "definition.h"

Definition::Definition(QObject *parent) :
    QObject(parent)
{
    this->_id = QByteArray();
    this->_name = QString(this->_id);
    this->_description = QString();
    this->_versionMajor = 0;
    this->_versionMinor = 0;
}

Definition::Definition(const Definition &original, QObject *parent) :
    QObject(parent),
    _id(original._id),
    _name(original._name),
    _description(original._description),
    _versionMajor(original._versionMajor),
    _versionMinor(original._versionMinor)
{ }

Definition &Definition::operator =(const Definition &original)
{
    this->_id = original._id;
    this->_name = original._name;
    this->_description = original._description;
    this->_versionMajor = original._versionMajor;
    this->_versionMinor = original._versionMinor;
    return *this;
}

bool Definition::operator ==(const Definition &other) const
{
    return this->_id == other._id
            && this->_versionMajor == other._versionMajor
            && this->_versionMinor == other._versionMinor;
}

bool Definition::operator !=(const Definition &other) const
{
    return !(*this == other);
}

bool Definition::operator <(const Definition &other) const
{
    return this->_versionMajor < other._versionMajor || (this->_versionMajor == other._versionMajor && this->_versionMinor < other._versionMinor);
}

bool Definition::operator <=(const Definition &other) const
{
    return *this < other || (this->_versionMajor == other._versionMajor && this->_versionMinor == other._versionMinor);
}

bool Definition::operator >(const Definition &other) const
{
    return !(*this <= other);
}

bool Definition::operator >=(const Definition &other) const
{
    return !(*this < other);
}
