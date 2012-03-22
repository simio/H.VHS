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
    explicit Extension(QObject *parent = 0);
    Extension(QString id, QString name, QString description, QDateTime releaseDate, QString basePath, QObject *parent = 0);
    Extension(QString basePath, QObject *parent = 0);
    Extension(const Extension &original, QObject *parent = 0);
    Extension &operator=(const Extension &original);

    QVariant info(QString key, QVariant defaultValue = QVariant()) const    { return this->_info.value(key, defaultValue); }
    void setInfo(QString key, QVariant value)                               { this->_info.insert(key, value); }
    QFileInfo basePath() const                                              { return this->_basePath; }
    bool isEnabled() const                                                  { return this->_enabled; }

    bool canWriteFormat(QString uid) const                               { return this->_writeFormats.contains(uid); }
    bool canReadFormat(QString uid) const                                { return this->_readFormats.contains(uid); }
    bool canWriteTransport(QString uid) const                            { return this->_writeTransports.contains(uid); }
    bool canReadTransport(QString uid) const                             { return this->_readTransports.contains(uid); }

    QList<QString> readTransports() const                                { return this->_readTransports; }
    QList<QString> readFormats() const                                   { return this->_readFormats; }
    QList<QString> writeTransports() const                               { return this->_writeTransports; }
    QList<QString> writeFormats() const                                  { return this->_writeFormats; }

    void addReadTransport(QString transport)                             { this->_readTransports.append(transport); }
    void addReadFormat(QString format)                                   { this->_readFormats.append(format); }
    void addWriteTransport(QString transport)                            { this->_writeTransports.append(transport); }
    void addWriteFormat(QString format)                                  { this->_writeFormats.append(format); }

signals:

public slots:

private:
    bool _setup();
    bool _isReady() const;

    bool _enabled;
    QString _basePath;

    QMap<QString,QVariant> _info;

    QList<QString> _readTransports;
    QList<QString> _readFormats;
    QList<QString> _writeTransports;
    QList<QString> _writeFormats;
};

#endif // EXTENSION_H
