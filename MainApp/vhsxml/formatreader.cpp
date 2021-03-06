/*
 * Copyright (c) 2012 Jesper Raftegard <jesper@huggpunkt.org>
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
    QList<QSharedPointer<FormatDefinition> >
    FormatReader::parse(const QDomDocument &document)
    {
        QList<QSharedPointer<FormatDefinition> > result;

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
                        QSharedPointer<FormatDefinition> format =
                            _parseFormat(formatNode.toElement());
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


    /* While this way of parsing is not optimal (as in being nice and
     * readable), the enforcing of xml element ordering is
     * intentional. The order in which the elements appear is defined
     * in the RELAX NG Schema.
     */

    QSharedPointer<FormatDefinition>
    FormatReader::_parseFormat(const QDomElement &formatNode)
    {
        QString id;                             // Unique; required
        QDateTime releaseDate;                  // As attribute of id; required
        QString name;                           // Localised; required
        QString description;                    // Localised; optional
        QString completeness;                   // From enum; required
        bool isText;                            // True or false; required
        // Prioritised when outputting; required to specify at least one:
        QStringList mimeTypes;

        // <id releaseDate="xsd:dateTime">xsd:NMTOKEN</id>
        QDomElement e = formatNode.firstChildElement();
        if (ElementParser::expect(e, "id", ElementParser::Required))
        {
            releaseDate = ElementParser::dateTime(e.attribute("releaseDate"));
            id = ElementParser::nmtoken(e.text());
            e = e.nextSiblingElement();
        }
        else
            return QSharedPointer<FormatDefinition>();

        // <name xml:lang="xsd:language">xsd:token</name>
        // (one or more, with unique xml:lang attributes)
        if (ElementParser::expect(e, "name", ElementParser::Required))
        {
            name = ElementParser::localisedString(e);
            e = e.nextSiblingElement();
        }
        else
            return QSharedPointer<FormatDefinition>();

        // <description xml:lang="xsd:language">xsd:token</description>
        // (zero or more, with unique xml:lang attributes)
        if (ElementParser::expect(e, "description", ElementParser::Optional))
        {
            description = ElementParser::localisedString(e);
            e = e.nextSiblingElement();
        }
        else if (e.isNull())
            return QSharedPointer<FormatDefinition>();

        // <completeness>(notEmpty|metaOnly|dataOnly|complete)</completeness>
        if (ElementParser::expect(e, "completeness", ElementParser::Required))
        {
            completeness = e.text();
            e = e.nextSiblingElement();
        }
        else
            return QSharedPointer<FormatDefinition>();

        // <isText> ( true | false ) </isText>
        if (ElementParser::expect(e, "isText", ElementParser::Required))
        {
            isText = ElementParser::boolean(e.text());
            e = e.nextSiblingElement();
        }
        else
            return QSharedPointer<FormatDefinition>();

        // <mimeTypes>
        //     <mimeType>xxx/yyy</mimeType>
        //     ...
        // <mimeTypes>
        if (ElementParser::expect(e, "mimeTypes", ElementParser::Required))
            mimeTypes = ElementParser::tokenList(e, "mimeType");
        e = e.nextSiblingElement();

        // alloc: QSharedPointer
        QSharedPointer<FormatDefinition> format(
            new FormatDefinition(id,
                                 name,
                                 description,
                                 releaseDate,
                                 completeness,
                                 isText,
                                 mimeTypes,
                                 0));
        if (format->isValid())
            return format;

        qDebug() << "FormatReader::parseFormat() discarded invalid format"
                 << " definition.";
        return QSharedPointer<FormatDefinition>();
    }
} // namespace VhsXml
