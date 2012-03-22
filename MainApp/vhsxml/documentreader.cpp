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

#include "vhsxml/documentreader.h"

namespace VhsXml {

DocumentReader::DocumentReader(QFileInfo file, QObject *parent) : QObject(parent)
{
    QPointer<QFile> p = new QFile(file.canonicalFilePath());
    this->_initialise(p);
}

QList<Definition::DefinitionType> DocumentReader::contentList() const
{
    if (this->isEmpty())
        return QList<Definition::DefinitionType>();

    QHash<Definition::DefinitionType,bool> isPresent;
    QDomElement doc = this->_xml.documentElement();
    QDomNode node = doc.firstChild();
    while (! node.isNull())
    {
        QString name =  node.toElement().tagName();
        // <transportDefinitions> without at least one <transportDefinition> are invalid,
        // so there's no need to look deeper into the tree.
        if (name == "formatDefinitions")
            isPresent.insert(Definition::FormatDefinitionType, true);
        else if (name == "transportDefinitions")
            isPresent.insert(Definition::TransportDefinitionType, true);
        else if (name == "extensions")
            isPresent.insert(Definition::ExtensionDefinitionType, true);
        else if (name == "cassettes")
            isPresent.insert(Definition::CassetteDefinitionType, true);
    }
    return isPresent.keys();
}

// Dispatch request to appropriate reader(s).
QList<QPointer<Definition> > DocumentReader::definitions(Definition::DefinitionType defType) const
{
    QList<QPointer<FormatDefinition> > formats;
    QList<QPointer<TransportDefinition> > transports;
    QList<QPointer<Extension> > extensions;
    //QList<QPointer<CassetteDefinition> > cassettes;

    if (defType == Definition::FormatDefinitionType || defType == Definition::NoDefinitionType)
        formats = FormatReader::parse(this->_xml);
    if (defType == Definition::TransportDefinitionType || defType == Definition::NoDefinitionType)
        transports = TransportReader::parse(this->_xml);
    if (defType == Definition::ExtensionDefinitionType || defType == Definition::NoDefinitionType)
        extensions = ExtensionReader::parse(this->_xml);
    //else if (defType == Definition::CassetteDefinitionType || defType == Definition::NoDefinitionType)
    //    result.append(CassetteReader::parse(this->_xml);

    QList<QPointer<Definition> > result;
    foreach(QPointer<FormatDefinition> format, formats)
        result << qobject_cast<Definition*>(format);
    foreach(QPointer<TransportDefinition> transport, transports)
        result << qobject_cast<Definition*>(transport);
    foreach(QPointer<Extension> extension, extensions)
        result << qobject_cast<Definition*>(extension);
    //foreach(QPointer<FormatDefinition> cassette, cassettes)
    //    result << qobject_cast<QPointer<Definition> >(cassette);

    return result;
}

bool DocumentReader::_isCompatibleWith(QString version)
{
    QRegExp rx("([0-9]+)\\.([0-9]+)");
    if (rx.indexIn(version.trimmed()) < 0)
        return false;
    int major = rx.cap(1).toInt();
    int minor = rx.cap(2).toInt();

    if (major < this->_legacyXmlMajor
            || major > this->_currentXmlMajor
            || (major == this->_legacyXmlMajor && minor < this->_legacyXmlMinor)
            || (major == this->_currentXmlMajor && minor > this->_currentXmlMinor))
        return false;

    return true;
}

// Cannot use QPointer for source
bool DocumentReader::_initialise(QXmlInputSource *source)
{
    QString *error = new QString;
    int *errorLine = new int;
    int *errorColumn = new int;

    bool success = false;
    this->_xml = QDomDocument("VhsXml");
    if (this->_xml.setContent(source, true, error, errorLine, errorColumn))
    {
        if (this->_xml.documentElement().tagName() == "vhsxml")
        {
            QString docVersion = this->_xml.documentElement().attribute("version");
            if (this->_isCompatibleWith(docVersion))
                success = true;
            else
                qWarning() << "XML Parser error!"
                           << "Document contains VhsXml version" << docVersion
                           << "which is incompatible with this version of H.VHS.";
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

    return success;
}

bool DocumentReader::_initialise(QPointer<QIODevice> device)
{
    // Cannot use QPointer for source
    QXmlInputSource *source = new QXmlInputSource(device);
    bool retVal = this->_initialise(source);
    delete source;
    return retVal;
}

bool DocumentReader::_initialise(QPointer<QFile> file)
{
    if (file->exists())
    {
        QXmlInputSource *source = new QXmlInputSource(file);
        bool retVal = this->_initialise(source);
        delete source;
        qDebug() << "DocumentReader initialised for" << file->fileName();
        return retVal;
    }
    qDebug() << "DocumentReader failed to initialise on" << file->fileName();
    return false;
}

} // namespace VhsXml
