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

#ifndef VHSXML_READER_H
#define VHSXML_READER_H

#include <QObject>
#include <QSharedPointer>
#include <QFile>
#include <QDomDocument>
#include <QXmlInputSource>
#include <QDateTime>
#include <QHash>

#include "main.h"

#include "extensionmanager/definitions/definition.h"

#include "vhsxml/transportreader.h"
#include "vhsxml/formatreader.h"
#include "vhsxml/extensionreader.h"
#include "vhsxml/cassettereader.h"

namespace VhsXml {

class DocumentReader : public QObject
{
    Q_OBJECT
public:
    static VersionNumber version()                    { return VersionNumber(1, 1); }
    static VersionNumber oldestCompatibleVersion()    { return VersionNumber(1, 1); }

    DocumentReader(QFileInfo file, QObject *parent);
    DocumentReader(QSharedPointer<QXmlInputSource> source, QObject *parent) : QObject(parent)   { this->_initialise(source); }

    QList<QSharedPointer<Definition> > definitions(Definition::DefinitionType defType) const;

    bool isEmpty() const                                                    { return this->_xml.isNull(); }

    QList<Definition::DefinitionType> contentList() const;
    bool contains(Definition::DefinitionType contents) const                { return this->contentList().contains(contents); }

signals:

public slots:

private:
    static bool _isCompatibleWith(QString foulString);

    bool _initialise(QWeakPointer<QFile> file);
    bool _initialise(QWeakPointer<QIODevice> device);
    bool _initialise(QSharedPointer<QXmlInputSource> source);

    QDomDocument _xml;

};

} // namespace VhsXml

#endif // VHSXML_READER_H
