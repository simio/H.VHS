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

#include "documentreader.h"

namespace VhsXml {
    DocumentReader::DocumentReader(QFileInfo file,
                                   QObject *parent) :
        QObject(parent)
    {
        // Use a C++ pointer and set this as file parent. The file
        // will be used to construct a QXmlInputSource instance, which
        // might (or might not) read from it at any time until this
        // DocumentReader instance is destructed.
        // alloc: has parent
        QFile * p = new QFile(file.canonicalFilePath(), this);
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
            // <transportDefinitions> without at least one
            // <transportDefinition> are invalid, so there's no need
            // to look deeper into the tree.
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
    QList<QSharedPointer<Definition> >
    DocumentReader::definitions(Definition::DefinitionType defType) const
    {
        QList<QSharedPointer<FormatDefinition> > formats;
        QList<QSharedPointer<TransportDefinition> > transports;
        QList<QSharedPointer<ExtensionDefinition> > extensions;

        if (defType == Definition::FormatDefinitionType ||
            defType == Definition::NoDefinitionType)
            formats = FormatReader::parse(this->_xml);
        if (defType == Definition::TransportDefinitionType ||
            defType == Definition::NoDefinitionType)
            transports = TransportReader::parse(this->_xml);
        if (defType == Definition::ExtensionDefinitionType ||
            defType == Definition::NoDefinitionType)
            extensions = ExtensionReader::parse(this->_xml);

        QList<QSharedPointer<Definition> > result;
        foreach(QSharedPointer<FormatDefinition> format, formats)
            result << qSharedPointerDynamicCast<Definition>(format);
        foreach(QSharedPointer<TransportDefinition> transport, transports)
            result << qSharedPointerDynamicCast<Definition>(transport);
        foreach(QSharedPointer<ExtensionDefinition> extension, extensions)
            result << qSharedPointerDynamicCast<Definition>(extension);

        return result;
    }

    bool
    DocumentReader::_isCompatibleWith(QString foulString)
    {
        VersionNumber version = VersionNumber(foulString);

        return (DocumentReader::oldestCompatibleVersion() <= version &&
                DocumentReader::version() >= version);
    }

    bool
    DocumentReader::_initialise(QSharedPointer<QXmlInputSource> source)
    {
        bool success = false;
        this->_xml = QDomDocument("VhsXml");

        // alloc: QScopedPointer
        QScopedPointer<QString> error(new QString);
        // alloc: QScopedPointer
        QScopedPointer<int> errorLine(new int);
        // alloc: QScopedPointer
        QScopedPointer<int> errorColumn(new int);
        if (this->_xml.setContent(source.data(),
                                  true,
                                  error.data(),
                                  errorLine.data(),
                                  errorColumn.data()))
        {
            if (this->_xml.documentElement().tagName() == "vhsxml")
            {
                QString docVersion =
                    this->_xml.documentElement().attribute("version");
                if (this->_isCompatibleWith(docVersion))
                    success = true;
                else
                    qWarning() << "XML Parser error:"
                               << "Document contains VhsXml version"
                               << docVersion << "which is incompatible"
                               << " with this version of H.VHS.";
            }
        }
        else
            qWarning() << "XML Parser error:" << error.data() << endl
                       << "Line " << errorLine.data() << "column"
                       << errorColumn.data();

        if (!success)
            this->_xml.clear();

        return success;
    }

    bool
    DocumentReader::_initialise(QIODevice * device)
    {
        // alloc: QSharedPointer
        QSharedPointer<QXmlInputSource> source(
            new QXmlInputSource(device));
        bool retVal = this->_initialise(source);
        return retVal;
    }

    bool DocumentReader::_initialise(QFile * file)
    {
        if (file->exists())
        {
            // alloc: QSharedPointer
            QSharedPointer<QXmlInputSource> source(
                new QXmlInputSource(file));
            bool retVal = this->_initialise(source);
            return retVal;
        }
        qDebug() << "DocumentReader failed to initialise on"
                 << file->fileName();
        return false;
    }
} // namespace VhsXml
