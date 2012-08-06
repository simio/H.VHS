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
#include "definitiondata.h"

Definition::Definition(QString id,
                       QString name,
                       QString description,
                       QDateTime releaseDate,
                       DefinitionType type,
                       QObject * parent)
    : QObject(parent)
{
    this->_d = QSharedDataPointer<DefinitionData>(new DefinitionData);
    this->_d.data()->add(DefinitionData::DefinitionId, QVariant(id));
    this->_d.data()->add(DefinitionData::DefinitionName, QVariant(name));
    this->_d.data()->add(DefinitionData::DefinitionDescription, QVariant(description));
    this->_d.data()->add(DefinitionData::DefinitionReleaseDate, QVariant(releaseDate));
    this->_d.data()->add(DefinitionData::DefinitionType, QVariant((qint64)type));
}

Definition::~Definition()
{ }

Definition::Definition(const Definition &original)
    : _d(original._d)
{ }

Definition &Definition::operator =(const Definition &original)
{
    this->_d = original._d;
    return *this;
}

// ==, !=, <= and >= compares both id and release dates, while < and > compare only dates.
bool Definition::operator ==(const Definition &other) const
{
    return this->id() == other.id()
            && this->releaseDate() == other.releaseDate();
}

bool Definition::operator !=(const Definition &other) const
{
    // Invert operator ==
    return ! (*this == other);
}

bool Definition::operator <(const Definition &other) const
{
    return this->releaseDate() < other.releaseDate();
}

bool Definition::operator <=(const Definition &other) const
{
    return this->id() == other.id()
            && this->releaseDate() <= other.releaseDate();
}

bool Definition::operator >(const Definition &other) const
{
    return this->releaseDate() > other.releaseDate();
}

bool Definition::operator >=(const Definition &other) const
{
    return this->id() == other.id()
            && this->releaseDate() >= other.releaseDate();
}

bool Definition::isValid() const
{
    return (! this->id().isEmpty()
            && ! this->name().isEmpty()
            && this->releaseDate().isValid()
            && this->type() != Definition::NoDefinitionType);
}

QString Definition::id() const
{
    return this->_d.constData()->get(DefinitionData::DefinitionId).toString();
}

QString Definition::name() const
{
    return this->_d.constData()->get(DefinitionData::DefinitionName).toString();
}

QString Definition::description() const
{
    return this->_d.constData()->get(DefinitionData::DefinitionDescription).toString();
}

QDateTime Definition::releaseDate() const
{
    return this->_d.constData()->get(DefinitionData::DefinitionReleaseDate).toDateTime();
}

Definition::DefinitionType Definition::type() const
{
    return (Definition::DefinitionType)this->_d.constData()->get(DefinitionData::DefinitionType).toLongLong();
}

QString Definition::prettyType() const
{
    switch (this->type())
    {
    case Definition::NoDefinitionType:                  return "none (invalid)"; break;
    case Definition::TransportDefinitionType:           return "transport"; break;
    case Definition::FormatDefinitionType:              return "format"; break;
    case Definition::ExtensionDefinitionType:           return "extension"; break;
    case Definition::CassetteDefinitionType:            return "cassette"; break;
    default:                                            return QString::number(this->type()).prepend("#"); break;
    }
}
