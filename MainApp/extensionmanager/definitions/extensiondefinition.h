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

#ifndef EXTENSIONMANAGER_DEFINITIONS_EXTENSIONDEFINITION_H
#define EXTENSIONMANAGER_DEFINITIONS_EXTENSIONDEFINITION_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QIODevice>
#include <QStringList>

#include "main.h"

#include "definition.h"
#include "transportdefinition.h"
#include "formatdefinition.h"

/*  This class is expected to neither change nor expand much
 *  over time, which (probably) means the code is more easily
 *  maintained if it uses hardcoded vars rather than a QMap.
 */

class ExtensionDefinition : public Definition
{
public:
    enum Condition {
        Broken,
        Unstable,
        Stable,
        Testing
    };

    enum ApiInterface {
        JavaScript,
        QtPlugin
    };

    // Nice
    ExtensionDefinition(QString id,
                        QString name,
                        QString description,
                        QDateTime releaseDate,
                        QList<Person> authors,
                        QString licenseName,
                        QUrl licenseUrl,
                        QList<Person> maintainers,
                        bool enabled,
                        Condition condition,
                        QString basePath,
                        QStringList interfaces,
                        VersionNumber apiVersion,
                        ApiInterface api,
                        QString source,
                        QList<QString> inputTransports,
                        QList<QString> inputFormats,
                        QList<QString> outputTransports,
                        QList<QString> outputFormats,
                        QList<Person> audits,
                        QObject *parent = 0);

    ExtensionDefinition(const ExtensionDefinition &original);
    virtual ExtensionDefinition &operator=(const ExtensionDefinition &original);

    bool isEnabled() const                                                  { return this->_enabled; }
    bool isValid() const;
    bool implementsInterface(QString interface) const                       { return this->_interfaces.contains(interface); }
    QStringList interfaces() const                                          { return this->_interfaces; }
    ApiInterface api() const                                                { return this->_api; }
    VersionNumber apiVersion() const                                              { return this->_apiVersion; }
    QString source() const                                                  { return this->_source; }

    bool canWriteFormat(QString uid) const                                  { return this->_outputFormats.contains(uid); }
    bool canReadFormat(QString uid) const                                   { return this->_inputFormats.contains(uid); }
    bool canWriteTransport(QString uid) const                               { return this->_outputTransports.contains(uid); }
    bool canReadTransport(QString uid) const                                { return this->_inputTransports.contains(uid); }

    QList<QString> inputTransports() const                                  { return this->_inputTransports; }
    QList<QString> inputFormats() const                                     { return this->_inputFormats; }
    QList<QString> outputTransports() const                                 { return this->_outputTransports; }
    QList<QString> outputFormats() const                                    { return this->_outputFormats; }

signals:

public slots:

private:
    ExtensionDefinition();

    bool _enabled;                                  // Whether the extension is enabled on this machine/installation
    QList<Person> _authors;                         // Author(s) of this extension. Years of the (c)-notice is stored as 'copyright' activity in Person.
    QString _licenseName;                           // License statement.
    QUrl _licenseUrl;                               // If relative, ./ is the location of the extension definition XML file.
    QList<Person> _maintainers;
    Condition _condition;
    QString _basePath;
    QStringList _interfaces;
    VersionNumber _apiVersion;
    ApiInterface _api;
    QString _source;
    QList<Person> _audits;

    QStringList _inputTransports;
    QStringList _inputFormats;
    QStringList _outputTransports;
    QStringList _outputFormats;
};

#endif // EXTENSIONMANAGER_DEFINITIONS_EXTENSIONDEFINITION_H
