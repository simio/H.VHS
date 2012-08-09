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

QList<QSharedPointer<ExtensionDefinition> > ExtensionReader::parse(const QDomDocument &document)
{
    QList<QSharedPointer<ExtensionDefinition> > result;

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
                    QSharedPointer<ExtensionDefinition> extension = _parseExtension(extensionNode.toElement());
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

QSharedPointer<ExtensionDefinition> ExtensionReader::_parseExtension(const QDomElement &extensionNode)
{
    QString id;                                     // Unique; required
    QDateTime releaseDate;                          // As attribute of id; required
    QString name;                                   // Localised; required
    QString description;                            // Localised; optional
    QList<Person> authors;                          // A minimum of one; required
    QString licenseName;                            // Required
    QUrl licenseUrl;                                // Optional
    QList<Person> maintainers;                      // A minimum of one; required
    bool enabled;                                   // Optional (set by application or extension repository)
    QString basePath;                               // Optional (set by application)
    ExtensionDefinition::Condition condition;       // Required
    QStringList interfaces;                         // Required
    VersionNumber apiVersion;                       // Required
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
    else
        return QSharedPointer<ExtensionDefinition>();

    // <name xml:lang="xsd:language">xsd:token</name>
    // (one or more, with unique xml:lang attributes)
    if (ElementParser::expect(e, "name", ElementParser::Required))
    {
        name = ElementParser::localisedString(e);
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<ExtensionDefinition>();

    // <description xml:lang="xsd:language">xsd:token</description>
    // (zero or more, with unique xml:lang attributes)
    if (ElementParser::expect(e, "description", ElementParser::Optional))
    {
        description = ElementParser::localisedString(e);
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return QSharedPointer<ExtensionDefinition>();

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
        return QSharedPointer<ExtensionDefinition>();

    // <license href="xsd:anyURI">xsd:token</license>
    // href is optional
    if (ElementParser::expect(e, "license", ElementParser::Required))
    {
        licenseUrl = QUrl(e.attribute("href", QString()));
        licenseName = ElementParser::token(e.text());
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<ExtensionDefinition>();

    if (ElementParser::expect(e, "maintainers", ElementParser::Required))
    {
        QDomElement maintainer = e.firstChildElement();
        while (ElementParser::expect(maintainer, "maintainer", ElementParser::Optional))
        {
            QString email = ElementParser::token(maintainer.attribute("email", QString()));
            QUrl website = QUrl(maintainer.attribute("website", QString()));
            QString username = ElementParser::nmtoken(maintainer.attribute("username", QString()));
            QString name = ElementParser::nmtoken(maintainer.text());
            Person person = Person(name, email, website, username);
            maintainers << person;
            maintainer = maintainer.nextSiblingElement();
        }
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<ExtensionDefinition>();

    // <enabled> ( "true" | "false" ) </enabled>
    if (ElementParser::expect(e, "enabled", ElementParser::Optional))
    {
        enabled = ElementParser::boolean(e.text());
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return QSharedPointer<ExtensionDefinition>();
    else
        enabled = false;

    // <basePath>xsd:string</basePath>
    if (ElementParser::expect(e, "basePath", ElementParser::Optional))
    {
        basePath = e.text();
        e = e.nextSiblingElement();
    }
    else if (e.isNull())
        return QSharedPointer<ExtensionDefinition>();

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
            return QSharedPointer<ExtensionDefinition>();
        }
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<ExtensionDefinition>();

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
            return QSharedPointer<ExtensionDefinition>();
        }
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<ExtensionDefinition>();

    //      <source apiVersion="xsd:NMTOKEN" api=" ( "qtplugin" | "javascript" ) " />
    // or   <source apiVersion="xsd:NMTOKEN" api="javascript">xsd:string</source>
    if (ElementParser::expect(e, "source", ElementParser::Required))
    {
        apiVersion = VersionNumber(e.attribute("apiVersion", QString()));
        QString str = e.attribute("api").trimmed();
        if (str == "qtplugin")
        {
            apiInterfaceClass = ExtensionDefinition::QtPlugin;
            if (! apiVersion.isCompatible(QtPluginExtension::currentApiVersion(),
                                          QtPluginExtension::oldestCompatibleApiVersion()))
            {
                qDebug() << "ExtensionReader discarding qtplugin" << name << "at apiVersion" << apiVersion.toString()
                         << "(compatibility span is" << QtPluginExtension::currentApiVersion().toString()
                         << "to" << QtPluginExtension::oldestCompatibleApiVersion().toString() << ")";
                return QSharedPointer<ExtensionDefinition>();
            }
        }
        else if (str == "javascript")
        {
            apiInterfaceClass = ExtensionDefinition::JavaScript;
            if (! apiVersion.isCompatible(JavaScriptExtension::currentApiVersion(),
                                          JavaScriptExtension::oldestCompatibleApiVersion()))
            {
                qDebug() << "ExtensionReader discarding javascript" << name << " at apiVersion" << apiVersion.toString()
                         << "(compatibility span is" << JavaScriptExtension::currentApiVersion().toString()
                         << "to" << JavaScriptExtension::oldestCompatibleApiVersion().toString() << ")";
                return QSharedPointer<ExtensionDefinition>();
            }
        }
        else
        {
            qDebug() << "ExtensionReader expected api qtplugin or javascript, but got" << str;
            qDebug() << "Discarding extension" << name;
            return QSharedPointer<ExtensionDefinition>();
        }
        source = e.text().trimmed();
        e = e.nextSiblingElement();
    }
    else
        return QSharedPointer<ExtensionDefinition>();

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
            return QSharedPointer<ExtensionDefinition>();
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
                return QSharedPointer<ExtensionDefinition>();
            }
            e = e.nextSiblingElement();
        }
        else
            return QSharedPointer<ExtensionDefinition>();
    }
    else if (e.isNull())
        return QSharedPointer<ExtensionDefinition>();


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
        return QSharedPointer<ExtensionDefinition>();

    // alloc: QSharedPointer
    QSharedPointer<ExtensionDefinition> extension(new ExtensionDefinition(id, name, description, releaseDate, authors, licenseName,
                                                                          licenseUrl, maintainers, enabled, condition, basePath,
                                                                          interfaces, apiVersion, apiInterfaceClass, source,
                                                                          inputTransports, inputFormats, outputTransports,
                                                                          outputFormats, audits, 0));
    if (extension->isValid())
        return extension;

    qDebug() << "ExtensionReader::parseExtension() discarded invalid extension definition.";
    return QSharedPointer<ExtensionDefinition>();
}


} // namespace VhsXml
