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

#include "formatdefinition.h"
#include "definitiondata.h"

FormatDefinition::FormatDefinition(QString id,
                                   QString name,
                                   QString description,
                                   QDateTime releaseDate,
                                   QString completeness,
                                   bool isText,
                                   QStringList mimeTypes,
                                   QObject *parent) :
    Definition(id,
               name,
               description,
               releaseDate,
               Definition::FormatDefinitionType,                // Set definition type
               parent)
{
    Definition::_d.data()->set(DefinitionData::FormatIsTextual, QVariant(isText));
    Definition::_d.data()->set(DefinitionData::FormatMimeTypes, QVariant(mimeTypes));

    // Cast to qint64 when storing enums
    Definition::_d.data()->set(DefinitionData::FormatCompleteness, QVariant((qint64)this->_strToCompleteness(completeness)));
}

FormatDefinition::~FormatDefinition()
{ }

bool FormatDefinition::isValid() const
{
    return (Definition::isValid()
            && this->completeness() != Invalid
            && Definition::_d.data()->has(DefinitionData::FormatIsTextual)
            && this->mimeTypes().count() > 0);
}

FormatDefinition::Completeness FormatDefinition::completeness() const
{
    return (FormatDefinition::Completeness)(Definition::_d.constData()->get(DefinitionData::FormatCompleteness).toLongLong());
}

bool FormatDefinition::isTextual() const
{
    return Definition::_d.constData()->get(DefinitionData::FormatIsTextual).toBool();
}

QStringList FormatDefinition::mimeTypes() const
{
    return Definition::_d.constData()->get(DefinitionData::FormatMimeTypes).toStringList();
}

QString FormatDefinition::mimeType(int index) const
{
    // Negative indexes do not exist. This will throw later on.
    if (index < 0)
        qFatal("FormatDefinition::mimeType(): Called with negative index.");

    // Same for indexes above range. (So don't you go off-by-one even once.)
    if (index >= this->mimeTypes().count())
        qFatal("FormatDefinition::mimeType(): Called with index above range.");

    // Index is within range
    return this->mimeTypes().at(index);
}

FormatDefinition::Completeness FormatDefinition::_strToCompleteness(QString str)
{
    Completeness c;
    QString normalised = str.toLower().trimmed();

    if (normalised == "notempty")
        c = NotEmpty;
    else if (normalised == "metaonly")
        c = MetaOnly;
    else if (normalised == "dataonly")
        c = DataOnly;
    else if (normalised == "complete")
        c = Complete;
    else
        qWarning() << "Could not set FormatDefinition completeness to" << normalised;

    return c;
}
