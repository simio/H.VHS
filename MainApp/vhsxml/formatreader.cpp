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

#include "formatreader.h"

namespace VhsXml {

QList<QPointer<FormatDefinition> > FormatReader::parse(const QDomDocument &document)
{
    QList<QPointer<FormatDefinition> > result;

    QDomNode node = document.documentElement().firstChild();
    while (! node.isNull())
    {
        if (node.toElement().tagName() == "formatDefinitions")
        {
            QDomNode formatNode = node.toElement().firstChild();
            while(!formatNode.isNull())
            {
                if (formatNode.toElement().tagName() == "formatDefinition")
                {
                    QPointer<FormatDefinition> format = _parseFormat(formatNode.toElement());
                    if (! format.isNull())
                        result << format;
                }

                formatNode = formatNode.nextSiblingElement();
            }
        }
        else
            qDebug() << "FormatReader::parse() found no <formatDefinitions> tag.";

        node = node.nextSiblingElement();
    }
    qDebug() << "FormatReader::parse() returning" << result.count() << "formats.";
    return result;
}

bool FormatReader::_expectElement(const QDomElement &element, const QString &tagName, bool mandatory)
{
    if (element.isNull())
        return false;
    else if (tagName == element.tagName())
        return true;
    else if (mandatory)
        qDebug() << "FormatReader error: Expected" << tagName << "element but got" << element.tagName()
                    << "in line" << element.lineNumber();

    return false;
}

QPointer<FormatDefinition> FormatReader::_parseFormat(const QDomElement &formatNode)
{
    QString id;                                     // Unique; mandatory
    QDateTime releaseDate;                          // As attribute of id; mandatory
    QString name;                                   // Localised; mandatory
    QString description;                            // Localised; optional
    QString completeness;                           // From presets; mandatory
    QStringList mimeTypes;                          // Prioritised when outputting; mandatory to specify at least one

    // <id releaseDate="xsd:dateTime">xsd:NMTOKEN</id>
    QDomElement e = formatNode.firstChildElement();
    if (FormatReader::_expectElement(e, "id"))
    {
        releaseDate = DataType::dateTime(e.attribute("releaseDate"));
        id = DataType::nmtoken(e.text());
        e = e.nextSiblingElement();
    }
    else return NULL;

    // <name xml:lang="xsd:language">xsd:token</name>
    // (one or more, with unique xml:lang attributes)
    if (FormatReader::_expectElement(e, "name"))
    {
        name = DataType::localisedString(e);
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    // <description xml:lang="xsd:language">xsd:token</description>
    // (zero or more, with unique xml:lang attributes)
    if (FormatReader::_expectElement(e, "description", false))
    {
        description = DataType::localisedString(e);
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return NULL;

    // <completeness> ( notEmpty | metaOnly | dataOnly | complete ) </completeness>
    if (FormatReader::_expectElement(e, "completeness"))
    {
        completeness = e.text();
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return NULL;

    // <mimeTypes>
    //     <mimeType>xxx/yyy</mimeType>
    //     ...
    // <mimeTypes>
    if (FormatReader::_expectElement(e, "mimeTypes"))
        mimeTypes = DataType::tokenList(e, "mimeType");

    // new: Caller is responsible
    QPointer<FormatDefinition> format = new FormatDefinition(id, name, description, releaseDate, completeness, mimeTypes, 0);
    if (format->isValid())
        return format;

    qDebug() << "FormatReader::parseFormat() discarded invalid format definition.";
    delete format;
    return NULL;
}


} // namespace VhsXml
