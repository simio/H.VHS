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

#ifndef EXTENSION_H
#define EXTENSION_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QDataStream>

#include "main.h"

#include "definition.h"
#include "transportdefinition.h"
#include "formatdefinition.h"

class Extension : public Definition
{
public:
    enum Condition {
        Broken,
        Unstable,
        Stable,
        Testing
    };

    enum ApiInterface {
        Javascript,
        Qt
    };

    // Nice
    Extension(QString id,
              QString name,
              QString description,
              QDateTime releaseDate,
              QList<Person> authors,
              QString statement,
              bool enabled,
              Condition condition,
              QString basePath,
              Version apiVersion,
              ApiInterface apiInterface,
              QString source,
              Version highestCompatibleVersion,
              Version lowestCompatibleVersion,
              QList<QString> inputTransports,
              QList<QString> inputFormats,
              QList<QString> outputTransports,
              QList<QString> outputFormats,
              QList<Person> audits,
              QObject *parent = 0);
    Extension(const Extension &original, QObject *parent = 0);
    Extension &operator=(const Extension &original);

    bool isEnabled() const                                                  { return this->_enabled; }

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
    Extension();
    bool _setup();
    bool _isReady() const;

    bool _enabled;
    QList<Person> _authors;
    QString _statement;
    Condition _condition;
    QString _basePath;
    Version _apiVersion;
    ApiInterface _apiInterface;
    QString _source;
    Version _highestCompatibleVersion;
    Version _lowestCompatibleVersion;
    QList<Person> _audits;

    QList<QString> _inputTransports;
    QList<QString> _inputFormats;
    QList<QString> _outputTransports;
    QList<QString> _outputFormats;
};

#endif // EXTENSION_H
