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

#include "extensiondefinition.h"

ExtensionDefinition::ExtensionDefinition(
        QString id,
        QString name,
        QString description,
        QDateTime releaseDate,
        QList<Person> authors,
        QString licenseName,
        QUrl licenseUrl,
        bool enabled,
        ExtensionDefinition::Condition condition,
        QString basePath,
        QStringList interfaces,
        Version apiVersion,
        ApiInterface api,
        QString source,
        QList<QString> inputTransports,
        QList<QString> inputFormats,
        QList<QString> outputTransports,
        QList<QString> outputFormats,
        QList<Person> audits, QObject *parent) :
    Definition(id, name, description, releaseDate, Definition::ExtensionDefinitionType, parent)
{
    this->_authors = authors;
    this->_licenseName = licenseName;
    this->_licenseUrl = licenseUrl;
    this->_enabled = enabled;
    this->_condition = condition;
    this->_basePath = basePath;
    this->_interfaces = interfaces;
    this->_apiVersion = apiVersion;
    this->_api = api;
    this->_source = source;
    this->_inputTransports = inputTransports;
    this->_inputFormats = inputFormats;
    this->_outputTransports = outputTransports;
    this->_outputFormats = outputFormats;
    this->_audits = audits;
}

bool ExtensionDefinition::isValid() const
{
    return (Definition::isValid()
            &&  this->_authors.count() > 0
            && !this->_licenseName.isEmpty()
            &&  this->_apiVersion > Version(0, 0)
            && (! (this->_inputTransports.count() || this->_inputFormats.count() || this->_outputTransports.count() || this->_outputFormats.count())
                || (this->_inputTransports.count() && this->_inputFormats.count() && this->_outputTransports.count() && this->_outputFormats.count())));
}