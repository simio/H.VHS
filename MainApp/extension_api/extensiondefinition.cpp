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
        Version apiVersion,
        ApiInterface apiInterface,
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
    this->_enabled = enabled;
    this->_condition = condition;
    this->_basePath = basePath;
    this->_apiVersion = apiVersion;
    this->_apiInterface = apiInterface;
    this->_source = source;
    this->_inputTransports = inputTransports;
    this->_inputFormats = inputFormats;
    this->_outputTransports = outputTransports;
    this->_outputFormats = outputFormats;
    this->_audits = audits;
}

ExtensionDefinition::ExtensionDefinition(const ExtensionDefinition &original, QObject *parent) :
    Definition(original, parent),
    _enabled(original._enabled),
    _authors(original._authors),
    _licenseName(original._licenseName),
    _licenseUrl(original._licenseUrl),
    _condition(original._condition),
    _basePath(original._basePath),
    _apiVersion(original._apiVersion),
    _apiInterface(original._apiInterface),
    _source(original._source),
    _audits(original._audits),
    _inputTransports(original._inputTransports),
    _inputFormats(original._inputFormats),
    _outputTransports(original._outputTransports),
    _outputFormats(original._outputFormats)
{ }

ExtensionDefinition &ExtensionDefinition::operator=(const ExtensionDefinition &original)
{
    this->_enabled = original._enabled;
    this->_authors = original._authors;
    this->_licenseName = original._licenseName;
    this->_licenseUrl = original._licenseUrl;
    this->_condition = original._condition;
    this->_basePath = original._basePath;
    this->_apiVersion = original._apiVersion;
    this->_apiInterface = original._apiInterface;
    this->_source = original._source;
    this->_inputTransports = original._inputTransports;
    this->_inputFormats = original._inputFormats;
    this->_outputFormats = original._outputFormats;
    this->_outputFormats = original._outputFormats;
    this->_audits = original._audits;
    return *this;
}

bool ExtensionDefinition::isValid() const
{
    return (Definition::isValid()
            &&  this->_authors.count() > 0
            && !this->_licenseName.isEmpty()
            &&  this->_apiVersion > Version(0, 0)
            &&  this->_inputTransports.count() > 0
            &&  this->_inputFormats.count() > 0
            &&  this->_outputTransports.count() > 0
            &&  this->_outputFormats.count() > 0);
}

bool ExtensionDefinition::_isReady() const
{
    // Fake it
    return true;
}

bool ExtensionDefinition::_setup()
{
    if (this->_isReady())
        return true;

    return true;
}
