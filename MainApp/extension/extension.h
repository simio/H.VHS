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

#include "transportdefinition.h"
#include "formatdefinition.h"

class Extension : public QObject
{
    Q_OBJECT
public:
    explicit Extension(QObject *parent = 0);
    Extension(const Extension &original, QObject *parent = 0);
    Extension &operator=(const Extension &original);

    QPointer<QDataStream> createWriter(QByteArray outputTransports, QByteArray outputFormat, QByteArray outputIdentifier);
    QPointer<QDataStream> createReader(QByteArray inputTransports, QByteArray inputFormat, QByteArray inputIdentifier);

    QByteArray uid() const;
    QVariant info(QString key, QVariant defaultValue = QVariant()) const;
    QString basePath() const;
    int version() const;

    void setUid(QByteArray uid);
    void addInfo(QString key, QVariant defaultValue = QVariant());
    void setBasePath(QString basePath);
    void setVersion(int version);

    bool setup();

    bool canWriteFormat(QByteArray uid) const;
    bool canReadFormat(QByteArray uid) const;
    bool canWriteTransport(QByteArray uid) const;
    bool canReadTransport(QByteArray uid) const;

    QList<QByteArray> inputTransports() const;
    QList<QByteArray> inputFormats() const;
    QList<QByteArray> outputTransports() const;
    QList<QByteArray> outputFormats() const;

    void addInputTransports(QList<QByteArray> transport);
    void addInputFormats(QList<QByteArray> format);
    void addOutputTransports(QList<QByteArray> transport);
    void addOutputFormats(QList<QByteArray> format);

signals:

public slots:

private:
    bool _setup();
    bool _isReady() const;

    QByteArray _uid;
    QString _basePath;
    int _version;

    QMap<QString,QVariant> _info;

    QList<QByteArray> _inputTransports;
    QList<QByteArray> _inputFormats;
    QList<QByteArray> _outputTransports;
    QList<QByteArray> _outputFormats;

    QMap<QString,QVariant> _setupData;
};

#endif // EXTENSION_H
