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

#include "extensionreader.h"

namespace VhsXml {

QList<QPointer<ExtensionDefinition> > ExtensionReader::parse(const QDomDocument &document, QObject *extensionParent)
{
    QList<QPointer<ExtensionDefinition> > result;

    QDomNode node = document.documentElement().firstChild();
    while (! node.isNull())
    {
        if (node.toElement().tagName() == "extensions")
        {
            QDomNode extensionNode = node.toElement().firstChild();
            while(!extensionNode.isNull())
            {
                if (extensionNode.toElement().tagName() == "extension")
                {
                    QPointer<ExtensionDefinition> extension = _parseExtension(extensionNode.toElement(), extensionParent);
                    if (! extension.isNull())
                        result << extension;
                }

                extensionNode = extensionNode.nextSiblingElement();
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

QPointer<ExtensionDefinition> ExtensionReader::_parseExtension(const QDomElement &extensionNode, QObject *extensionParent)
{
    QString id;                                     // Unique; required
    QDateTime releaseDate;                          // As attribute of id; required
    QString name;                                   // Localised; required
    QString description;                            // Localised; optional
    QList<Person> authors;                          // A minimum of one; required
    QString licenseName;                            // Required
    QUrl licenseUrl;                                // Optional
    bool enabled;                                   // Optional (set by application or extension repository)
    QString basePath;                               // Optional (set by application)
    ExtensionDefinition::Condition condition;       // Required
    QStringList interfaces;                         // Required
    Version apiVersion;                             // Required
    ExtensionDefinition::ApiInterface apiInterfaceClass;           // Required
    QString source;                                 // Source code for scripted extensions; optional
                                                    // (If source.isEmpty(), <basePath>/<id>.<apiInterface extension> will be used automatically.)
    QStringList inputTransports;                    // Required
    QStringList inputFormats;                       // Required
    QStringList outputTransports;                   // Required
    QStringList outputFormats;                      // Required
    QList<Person> audits;                           // Required, though it might contain zero <audit> elements
                                                    // (set by application or extension repository)

    QDomElement e = extensionNode.firstChildElement();
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

    //  <authors>
    //  <author username="xsd:NMTOKEN" copyright="xsd:token"
    //         email="xsd:token" website="xsd:anyURI">
    //      xsd:token
    //  </author>
    //  ...
    //  </authors>
    // At least one author is required.
    // All author attributes are optional.
    if (ElementParser::expect(e, "authors", ElementParser::Required))
    {
        QDomElement author = e.firstChildElement();
        while (ElementParser::expect(author, "author", ElementParser::Optional))
        {
            QString copyright = ElementParser::token(author.attribute("copyright", QString()));
            QString email = ElementParser::token(author.attribute("email", QString()));
            QUrl website = QUrl(author.attribute("website", QString()));
            QString username = ElementParser::nmtoken(author.attribute("username", QString()));
            QString name = ElementParser::nmtoken(author.text());
            Person person = Person(name, email, website, username);
            person.addInfo("copyright", copyright);
            authors << person;
            author = author.nextSiblingElement();
        }
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    // <license href="xsd:anyURI">xsd:token</license>
    // href is optional
    if (ElementParser::expect(e, "license", ElementParser::Required))
    {
        licenseUrl = QUrl(e.attribute("href", QString()));
        licenseName = ElementParser::token(e.text());
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    // <enabled> ( "true" | "false" ) </enabled>
    if (ElementParser::expect(e, "enabled", ElementParser::Optional))
    {
        enabled = ElementParser::boolean(e.text());
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return NULL;
    else
        enabled = false;

    // <basePath>xsd:string</basePath>
    if (ElementParser::expect(e, "basePath", ElementParser::Optional))
    {
        basePath = e.text();
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return NULL;

    // <condition> ( "broken" | "unstable" | "stable" | "testing" ) </condition>
    if (ElementParser::expect(e, "condition", ElementParser::Required))
    {
        QString str = ElementParser::nmtoken(e.text()).trimmed();
        if (str == "broken")
            condition = ExtensionDefinition::Broken;
        else if (str == "unstable")
            condition = ExtensionDefinition::Unstable;
        else if (str == "stable")
            condition = ExtensionDefinition::Stable;
        else if (str == "testing")
            condition = ExtensionDefinition::Testing;
        else
        {
            qDebug() << "ExtensionReader expected <condition> of broken, unstable, stable or testing, but got"
                     << str;
            qDebug() << "Discarding extension" << name;
            return NULL;
        }
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    // <interfaces>
    //     <interface>...</interface>
    //     (...)
    // </interface>
    if (ElementParser::expect(e, "interfaces", ElementParser::Required))
    {
        interfaces = ElementParser::tokenList(e, "interface");
        if (interfaces.count() < 1)
        {
            qDebug() << "ExtensionReader expected at least one interface, but got 0.";
            qDebug() << "Discarding extension" << name;
            return NULL;
        }
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    //      <source version="xsd:NMTOKEN" interfaceClass=" ( "qtplugin" | "javascript" ) " />
    // or   <source version="xsd:NMTOKEN" interfaceClass="javascript">xsd:string</source>
    if (ElementParser::expect(e, "source", ElementParser::Required))
    {
        apiVersion = Version(e.attribute("version", QString()));
        QString str = e.attribute("interfaceClass").trimmed();
        if (str == "qtplugin")
            apiInterfaceClass = ExtensionDefinition::QtPlugin;
        else if (str == "javascript")
            apiInterfaceClass = ExtensionDefinition::Javascript;
        else
        {
            qDebug() << "ExtensionReader expected interfaceClass qtplugin or javascript, but got" << str;
            qDebug() << "Discarding extension" << name;
            return NULL;
        }
        source = e.text().trimmed();
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    //  <convertsFrom>
    //      <transport>xsd:NMTOKEN</transport>
    //      ...
    //      <format>xsd:NMTOKEN</format>
    //      ...
    //  </convertsFrom>
    if (ElementParser::expect(e, "convertsFrom", ElementParser::Optional))
    {
        inputTransports = ElementParser::nmtokenList(e, "transport");
        inputFormats = ElementParser::nmtokenList(e, "format");
        if (inputTransports.count() < 1 || inputFormats.count() < 1)
        {
            qDebug() << "ExtensionReader expected at least one input transport and format, but got 0.";
            qDebug() << "Discarding extension" << name;
            return NULL;
        }
        e = e.nextSiblingElement();

        //  <convertsTo>
        //      <transport>xsd:NMTOKEN</transport>
        //      ...
        //      <format>xsd:NMTOKEN</format>
        //      ...
        //  </convertsTo>
        if (ElementParser::expect(e, "convertsTo", ElementParser::Required))
        {
            outputTransports = ElementParser::nmtokenList(e, "transport");
            outputFormats = ElementParser::nmtokenList(e, "format");
            if (outputTransports.count() < 1 || outputFormats.count() < 1)
            {
                qDebug() << "ExtensionReader expected at least one output transport and format, but got 0.";
                qDebug() << "Discarding extension" << name;
                return NULL;
            }
            e = e.nextSiblingElement();
        }
        else
            return NULL;
    }
    else if (e.isNull())
        return NULL;


    //  <audits>
    //      <audit email="xsd:token" website="xsd:anyURI" date="xsd:dateTime" username="xsd:token">xsd:token</audit>
    //      ...
    //  </audits>
    // All attributes are optional
    if (ElementParser::expect(e, "audits", ElementParser::Required))
    {
        QDomElement audit = e.firstChildElement();
        while (ElementParser::expect(audit, "audit", ElementParser::Optional))
        {
            QDateTime date = ElementParser::dateTime(audit.attribute("date"));
            QString email = ElementParser::token(audit.attribute("email", QString()));
            QUrl website = QUrl(audit.attribute("website", QString()));
            QString username = ElementParser::nmtoken(audit.attribute("username", QString()));
            QString name = ElementParser::nmtoken(audit.text());
            Person person = Person(name, email, website, username);
            person.addActivity(date, "audit");
            audits << person;
            audit = audit.nextSiblingElement();
        }
        e = e.nextSiblingElement();
    }
    else
        return NULL;

    // alloc: Has parent
    QPointer<ExtensionDefinition> extension = new ExtensionDefinition(id, name, description, releaseDate, authors, licenseName,
                                                                      licenseUrl, enabled, condition, basePath, interfaces, apiVersion,
                                                                      apiInterfaceClass, source, inputTransports, inputFormats,
                                                                      outputTransports, outputFormats, audits, extensionParent);
    if (extension->isValid())
        return extension;

    qDebug() << "ExtensionReader::parseExtension() discarded invalid extension definition.";
    delete extension;
    return NULL;
}


} // namespace VhsXml
