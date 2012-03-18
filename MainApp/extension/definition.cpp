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
    this->_id = QUuid::createUuid().toString().toAscii();
    this->_name = QString(tr("Undefined definition. :-)"));
    this->_description = QString();
    this->_releaseDate = QDateTime(QDate(1712, 2, 30));             // http://tinyurl.com/feb30
}

Definition::Definition(const Definition &original, QObject *parent) :
    QObject(parent),
    _id(original._id),
    _name(original._name),
    _description(original._description),
    _releaseDate(original._releaseDate)
{ }

Definition &Definition::operator =(const Definition &original)
{
    this->_id = original._id;
    this->_name = original._name;
    this->_description = original._description;
    this->_releaseDate = original._releaseDate;
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

QByteArray Definition::id() const               { return this->_id; }
QString Definition::name() const                { return this->_name; }
QString Definition::description() const         { return this->_description; }
QDateTime Definition::releaseDate() const           { return this->_releaseDate; }
