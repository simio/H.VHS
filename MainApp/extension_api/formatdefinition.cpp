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

#include "formatdefinition.h"

FormatDefinition::FormatDefinition(QObject *parent) :
    Definition(parent)
{
    this->_completeness = NotEmpty;
}

FormatDefinition::FormatDefinition(const FormatDefinition &original, QObject *parent) :
    Definition(original, parent),
    _completeness(original._completeness)
{ }

FormatDefinition &FormatDefinition::operator =(const FormatDefinition &original)
{
    this->_completeness = original._completeness;
    return *this;
}

FormatDefinition::Completeness FormatDefinition::completeness() const                   { return this->_completeness; }
QString FormatDefinition::mimeType(int index) const
{
    return (index >= 0 && index < this->_mimeTypes.size() ? this->_mimeTypes.at(index) : QString());
}
QStringList FormatDefinition::mimeTypes() const                                         { return this->_mimeTypes; }

void FormatDefinition::setCompleteness(FormatDefinition::Completeness c)                { this->_completeness = c; }
void FormatDefinition::setCompleteness(QString str)
{
    QString normalised = str.toLower().trimmed();

    if (normalised == "notempty")
        this->setCompleteness(NotEmpty);
    else if (normalised == "metaonly")
        this->setCompleteness(MetaOnly);
    else if (normalised == "dataonly")
        this->setCompleteness(DataOnly);
    else if (normalised == "complete")
        this->setCompleteness(Complete);
    else
    {
        qWarning() << "Could not set FormatDefinition completeness to" << normalised
                   << "Using \"NotEmpty\" instead";
    }
}

void FormatDefinition::insertMimeType(int index, QString mimeType) {
    this->_mimeTypes.insert(index, mimeType);
}
