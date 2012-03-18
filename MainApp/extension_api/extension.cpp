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

#include "extension.h"

Extension::Extension(QObject *parent) :
    Definition(parent)
{
    this->_basePath = QString();
}

Extension::Extension(const Extension &original, QObject *parent) :
    Definition(original, parent),
    _basePath(original._basePath),
    _info(original._info),
    _inputTransports(original._inputTransports),
    _inputFormats(original._inputFormats),
    _outputTransports(original._outputTransports),
    _outputFormats(original._outputFormats)
{ }

Extension &Extension::operator=(const Extension &original)
{
    this->_basePath = original._basePath;
    this->_info = original._info;
    this->_inputTransports = original._inputTransports;
    this->_inputFormats = original._inputFormats;
    this->_outputTransports = original._outputTransports;
    this->_outputFormats = original._outputFormats;
    return *this;
}

QVariant Extension::info(QString key, QVariant defaultValue) const      { return this->_info.value(key, defaultValue); }
QString Extension::basePath() const                                     { return this->_basePath; }

void Extension::addInfo(QString key, QVariant value)                    { this->_info.insert(key, value); }
void Extension::setBasePath(QString basePath)                           { this->_basePath = basePath; }

bool Extension::setup()                                                 { return this->_setup(); }

bool Extension::canWriteFormat(QByteArray uid) const                    { return this->_outputFormats.contains(uid); }
bool Extension::canReadFormat(QByteArray uid) const                     { return this->_inputFormats.contains(uid); }
bool Extension::canWriteTransport(QByteArray uid) const                 { return this->_outputTransports.contains(uid); }
bool Extension::canReadTransport(QByteArray uid) const                  { return this->_inputTransports.contains(uid); }

QList<QByteArray> Extension::inputTransports() const                    { return this->_inputTransports; }
QList<QByteArray> Extension::inputFormats() const                       { return this->_inputFormats; }
QList<QByteArray> Extension::outputTransports() const                   { return this->_outputTransports; }
QList<QByteArray> Extension::outputFormats() const                      { return this->_outputFormats; }

void Extension::addInputTransports(QList<QByteArray> transport)         { this->_inputTransports.append(transport); }
void Extension::addInputFormats(QList<QByteArray> format)               { this->_inputFormats.append(format); }
void Extension::addOutputTransports(QList<QByteArray> transport)        { this->_outputTransports.append(transport); }
void Extension::addOutputFormats(QList<QByteArray> format)              { this->_outputFormats.append(format); }

bool Extension::_isReady() const                                        { return true; }

bool Extension::_setup()
{
    if (this->_isReady())
        return true;

    if (this->info("implementation").toString().toLower() == "dll")
    {
        qDebug() << "Found DLL extension:" << this->info("name");
    }

    return true;
}
