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

#include "vhsxml/transportreader.h"

namespace VhsXml {

QList<QSharedPointer<TransportDefinition> > TransportReader::parse(const QDomDocument &document, QObject *definitionParent)
{
    QList<QSharedPointer<TransportDefinition> > result;

    QDomNode node = document.documentElement().firstChild();
    while (! node.isNull())
    {
        if (node.toElement().tagName() == "transportDefinitions")
        {
            QDomNode transportNode = node.toElement().firstChild();
            while(!transportNode.isNull())
            {
                if (transportNode.toElement().tagName() == "transportDefinition")
                {
                    QSharedPointer<TransportDefinition> transport = _parseTransport(transportNode.toElement(), definitionParent);
                    if (! transport.isNull())
                        result << transport;
                }

                transportNode = transportNode.nextSiblingElement();
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

QSharedPointer<TransportDefinition> TransportReader::_parseTransport(const QDomElement &transportNode, QObject *definitionParent)
{
    QString id;                                     // Unique; required
    QDateTime releaseDate;                          // As attribute of id; required
    QString name;                                   // Localised; required
    QString description;                            // Localised; optional

    QDomElement e = transportNode.firstChildElement();
    if (ElementParser::expect(e, "id", ElementParser::Required))
    {
        releaseDate = ElementParser::dateTime(e.attribute("releaseDate"));
        id = ElementParser::nmtoken(e.text());
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<TransportDefinition>();

    // <name xml:lang="xsd:language">xsd:token</name>
    // (one or more, with unique xml:lang attributes)
    if (ElementParser::expect(e, "name", ElementParser::Required))
    {
        name = ElementParser::localisedString(e);
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<TransportDefinition>();

    // <description xml:lang="xsd:language">xsd:token</description>
    // (zero or more, with unique xml:lang attributes)
    if (ElementParser::expect(e, "description", ElementParser::Optional))
    {
        description = ElementParser::localisedString(e);
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return QSharedPointer<TransportDefinition>();

    // alloc: Has parent
    QSharedPointer<TransportDefinition> transport(new TransportDefinition(id, name, description, releaseDate, definitionParent));
    if (transport->isValid())
        return transport;

    qDebug() << "TransportReader::parseTransport() discarded invalid transport definition.";
    return QSharedPointer<TransportDefinition>();
}


} // namespace VhsXml
