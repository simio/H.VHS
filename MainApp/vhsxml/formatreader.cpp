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

        node = node.nextSiblingElement();
    }
    return result;
}


/*
 *  While this way of parsing is not optimal (as in being nice and readable),
 *  the enforcing of xml element ordering is intentional. The order in which
 *  the elements appear is defined in the RELAX NG Schema.
 */

QPointer<FormatDefinition> FormatReader::_parseFormat(const QDomElement &formatNode)
{
    QString id;                                     // Unique; required
    QDateTime releaseDate;                          // As attribute of id; required
    QString name;                                   // Localised; required
    QString description;                            // Localised; optional
    QString completeness;                           // From presets; required
    QStringList mimeTypes;                          // Prioritised when outputting; required to specify at least one

    // <id releaseDate="xsd:dateTime">xsd:NMTOKEN</id>
    QDomElement e = formatNode.firstChildElement();
    if (ElementParser::expect(e, "id", ElementParser::Required))
    {
        releaseDate = ElementParser::dateTime(e.attribute("releaseDate"));
        id = ElementParser::nmtoken(e.text());
        e = e.nextSiblingElement();
    }
    else return NULL;

    // <name xml:lang="xsd:language">xsd:token</name>
    // (one or more, with unique xml:lang attributes)
    if (ElementParser::expect(e, "name", ElementParser::Required))
    {
        name = ElementParser::localisedString(e);
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    // <description xml:lang="xsd:language">xsd:token</description>
    // (zero or more, with unique xml:lang attributes)
    if (ElementParser::expect(e, "description", ElementParser::Optional))
    {
        description = ElementParser::localisedString(e);
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return NULL;

    // <completeness> ( notEmpty | metaOnly | dataOnly | complete ) </completeness>
    if (ElementParser::expect(e, "completeness", ElementParser::Required))
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
    if (ElementParser::expect(e, "mimeTypes", ElementParser::Required))
        mimeTypes = ElementParser::tokenList(e, "mimeType");
    e = e.nextSiblingElement();

    // alloc: Caller is responsible
    QPointer<FormatDefinition> format = new FormatDefinition(id, name, description, releaseDate, completeness, mimeTypes, 0);
    if (format->isValid())
        return format;

    qDebug() << "FormatReader::parseFormat() discarded invalid format definition.";
    delete format;
    return NULL;
}


} // namespace VhsXml
