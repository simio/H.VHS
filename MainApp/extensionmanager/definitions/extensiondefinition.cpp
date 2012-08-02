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
        QList<Person> maintainers,
        bool enabled,
        ExtensionDefinition::Condition condition,
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
        QObject *parent) :
    Definition(id, name, description, releaseDate, Definition::ExtensionDefinitionType, parent)
{
    this->_authors = authors;
    this->_licenseName = licenseName;
    this->_licenseUrl = licenseUrl;
    this->_maintainers = maintainers;
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

ExtensionDefinition::ExtensionDefinition(const ExtensionDefinition &original) :
    Definition(original),
    _enabled(original._enabled),
    _authors(original._authors),
    _licenseName(original._licenseName),
    _licenseUrl(original._licenseUrl),
    _maintainers(original._maintainers),
    _condition(original._condition),
    _basePath(original._basePath),
    _interfaces(original._interfaces),
    _apiVersion(original._apiVersion),
    _api(original._api),
    _source(original._source),
    _audits(original._audits),
    _inputTransports(original._inputTransports),
    _inputFormats(original._inputFormats),
    _outputTransports(original._outputTransports),
    _outputFormats(original._outputFormats)
{ }

ExtensionDefinition &ExtensionDefinition::operator =(const ExtensionDefinition &original)
{
    Definition::operator=(original);
    this->_authors = original._authors;
    this->_licenseName = original._licenseName;
    this->_licenseUrl = original._licenseUrl;
    this->_maintainers = original._maintainers;
    this->_enabled = original._enabled;
    this->_condition = original._condition;
    this->_basePath = original._basePath;
    this->_interfaces = original._interfaces;
    this->_apiVersion = original._apiVersion;
    this->_api = original._api;
    this->_source = original._source;
    this->_inputTransports = original._inputTransports;
    this->_inputFormats = original._inputFormats;
    this->_outputTransports = original._outputTransports;
    this->_outputFormats = original._outputFormats;
    this->_audits = original._audits;
    return *this;
}

bool ExtensionDefinition::isValid() const
{
    return (Definition::isValid()
            &&  this->_authors.count() > 0
            &&  this->_maintainers.count() > 0
            && !this->_licenseName.isEmpty()
            &&  this->_apiVersion > VersionNumber(0, 0)
            && (! (this->_inputTransports.count() || this->_inputFormats.count() || this->_outputTransports.count() || this->_outputFormats.count())
                || (this->_inputTransports.count() && this->_inputFormats.count() && this->_outputTransports.count() && this->_outputFormats.count())));
}
