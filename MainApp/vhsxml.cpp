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

#include "vhsxml.h"

VhsXml::VhsXml(QObject *parent) :
    QObject(parent)
{
}

VhsXml::VhsXml(QPointer<QFile> file, QObject *parent) :
    QObject(parent)
{
    this->_initialise(file);
}

VhsXml::VhsXml(QPointer<QIODevice> device, QObject *parent) :
    QObject(parent)
{
    this->_initialise(device);
}

// Allocates new FormatDefinitions. Caller is responsible for deletion.
QList< QPointer<FormatDefinition> > VhsXml::getFormatDefinitions()
{
    QList< QPointer<FormatDefinition> > result;

    QDomElement documentElement = this->_xml.documentElement();
    // Find the first <FormatDefinitions>
    QDomNode definitionsNode = documentElement.firstChild();
    while (! definitionsNode.isNull())
    {
        if (definitionsNode.toElement().tagName() == "FormatDefinitions")
        {
            if (! definitionsNode.toElement().hasAttribute("asOf"))
                qWarning() << "Invalid 'FormatDefinitions' element found; element has no 'asOf' attribute.";
            else
            {
                QDateTime defaultDateTime = QDateTime::fromString(definitionsNode.toElement().attribute("asOf"), Qt::ISODate);

                // Loop through all <Definition> elements within it
                QDomNode node = definitionsNode.firstChild();
                while (! node.isNull())
                {
                    if (node.toElement().tagName() == "Definition")
                    {
                        QPointer<FormatDefinition> newDef = new FormatDefinition;
                        if (node.toElement().hasAttribute("asOf"))
                            newDef->setDateTime(QDateTime::fromString(node.toElement().attribute("asOf"), Qt::ISODate));
                        else
                            newDef->setDateTime(defaultDateTime);
                        newDef->setUid(node.toElement().elementsByTagName("uid").item(0).toElement().text().toAscii());
                        newDef->setPrettyName(node.toElement().elementsByTagName("Name").item(0).toElement().text());
                        newDef->setContentType(node.toElement().elementsByTagName("ContentType").item(0).toElement().text());
                        result << newDef;
                    }
                    node = node.nextSibling();
                }
            }
        }
        definitionsNode = definitionsNode.nextSibling();
    }
    return result;
}

// Allocates new MediaDefinitions. Caller is responsible for deletion.
QList< QPointer<MediaDefinition> > VhsXml::getMediaDefinitions()
{
    QList< QPointer<MediaDefinition> > result;

    QDomElement documentElement = this->_xml.documentElement();
    // Find the first <MediaDefinition>
    QDomNode definitionsNode = documentElement.firstChild();
    while (! definitionsNode.isNull())
    {
        if (definitionsNode.toElement().tagName() == "MediaDefinitions")
        {
            if (! definitionsNode.toElement().hasAttribute("asOf"))
                qWarning() << "Invalid 'MediaDefinitions' element found; element has no 'asOf' attribute.";
            else
            {
                QDateTime defaultDateTime = QDateTime::fromString(definitionsNode.toElement().attribute("asOf"), Qt::ISODate);

                // Loop through all <Definition> elements within it
                QDomNode node = definitionsNode.firstChild();
                while (! node.isNull())
                {
                    if (node.toElement().tagName() == "Definition")
                    {
                        QPointer<MediaDefinition> newDef = new MediaDefinition;
                        if (node.toElement().hasAttribute("asOf"))
                            newDef->setDateTime(QDateTime::fromString(node.toElement().attribute("asOf"), Qt::ISODate));
                        else
                            newDef->setDateTime(defaultDateTime);
                        newDef->setUid(node.toElement().elementsByTagName("uid").item(0).toElement().text().toAscii());
                        newDef->setPrettyName(node.toElement().elementsByTagName("Name").item(0).toElement().text());
                        result << newDef;
                    }
                    node = node.nextSibling();
                }
            }
        }
        definitionsNode = definitionsNode.nextSibling();
    }
    return result;
}

QList<QByteArray> VhsXml::_readNamedChildrenToByteArrayList(QDomElement node, QString tagName)
{
    QDomNodeList nodeList = node.elementsByTagName(tagName);
    QList<QByteArray> result;
    for (int index = 0; index < nodeList.size(); index++)
        if (! nodeList.item(index).toElement().isNull())
            result << nodeList.item(index).toElement().text().toAscii();
    return result;
}

QList< QPointer<Extension> > VhsXml::getExtensions()
{
    QList< QPointer<Extension> > result;

    QDomElement documentElement = this->_xml.documentElement();
    // Find the first <Extensions>
    QDomNode definitionsNode = documentElement.firstChild();
    while (! definitionsNode.isNull())
    {
        if (definitionsNode.toElement().tagName() == "Extensions")
        {
            // Loop through all <Extension> elements within it
            QDomNode node = definitionsNode.firstChild();
            while (! node.isNull())
            {
                if (node.toElement().tagName() == "Extension")
                {
                    QPointer<Extension> newDef = new Extension;

                    QDomNode extNode = node.firstChild().toElement();
                    while (! extNode.toElement().isNull())
                    {
                        QDomElement e = extNode.toElement();
                        if (e.tagName() == "uid")
                            newDef->setUid(e.text().toAscii());
                        else if (e.tagName() == "Version")
                            newDef->setVersion(e.text().toInt());
                        else if (e.tagName() == "Author")
                        {
                            newDef->addInfo("Author", e.text());
                            newDef->addInfo("AuthorEmail", e.attribute("email"));
                            newDef->addInfo("AuthorWebsite", e.attribute("website"));
                        }
                        else if (e.tagName() == "Enabled")
                        {
                            newDef->addInfo("Enabled", e.text() == "true");
                        }
                        else if (e.tagName() == "InputMedia")
                            newDef->addInputMedia(this->_readNamedChildrenToByteArrayList(e, "Media"));
                        else if (e.tagName() == "InputFormats")
                            newDef->addInputFormats(this->_readNamedChildrenToByteArrayList(e, "Format"));
                        else if (e.tagName() == "OutputMedia")
                            newDef->addOutputMedia(this->_readNamedChildrenToByteArrayList(e, "Media"));
                        else if (e.tagName() == "OutputFormats")
                            newDef->addOutputFormats(this->_readNamedChildrenToByteArrayList(e, "Format"));
                        else
                            newDef->addInfo(e.tagName(), e.text());

                        extNode = extNode.nextSibling();
                    }

                    result << newDef;
                }
                node = node.nextSibling();
            }
        }
        definitionsNode = definitionsNode.nextSibling();
    }
    return result;
}

// Cannot use QPointer for source
bool VhsXml::_initialise(QXmlInputSource *source, QString name)
{
    QString *error = new QString;
    int *errorLine = new int;
    int *errorColumn = new int;

    bool success = false;
    this->_xml = QDomDocument(name);
    if (this->_xml.setContent(source, true, error, errorLine, errorColumn))
    {
        if (this->_xml.documentElement().tagName() == "HuggPlay")
        {
            if ((this->_xml.documentElement().attribute("version").toInt() >= this->_legacyXmlVersion)
                    && (this->_xml.documentElement().attribute("version").toInt() <= this->_currentXmlVersion))
                success = true;
            else
                qWarning() << "XML Parser error:"
                           << "Version mismatch."
                           << this->_xml.documentElement().attribute("version").toInt()
                           << "is above" << this->_currentXmlVersion << "or below" << this->_legacyXmlVersion;
        }

    }
    else
        qWarning() << "XML Parser error:" << *error << endl
                   << "Line " << *errorLine << "column" << *errorColumn;


    if (!success)
        this->_xml.clear();

    delete error;
    delete errorLine;
    delete errorColumn;

    return false;
}

bool VhsXml::_initialise(QPointer<QIODevice> device, QString name)
{
    // Cannot use QPointer for source
    QXmlInputSource *source = new QXmlInputSource(device);
    bool retVal = this->_initialise(source, name);
    delete source;
    return retVal;
}

bool VhsXml::_initialise(QPointer<QFile> file, QString name)
{
    if (file->exists())
    {
        QXmlInputSource *source = new QXmlInputSource(file);
        bool retVal = this->_initialise(source, (name.isNull() ? file->fileName() : name));
        delete source;
        return retVal;
    }
    return false;
}
