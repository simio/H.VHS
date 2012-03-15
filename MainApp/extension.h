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

#include "mediadefinition.h"
#include "formatdefinition.h"

class Extension : public QObject
{
    Q_OBJECT
public:
    explicit Extension(QObject *parent = 0);

    QPointer<QDataStream> createWriter(QByteArray outputMedia, QByteArray outputFormat, QByteArray outputIdentifier);
    QPointer<QDataStream> createReader(QByteArray inputMedia, QByteArray inputFormat, QByteArray inputIdentifier);

    QByteArray uid();
    QVariant info(QString key, QVariant defaultValue = QVariant());
    QString basePath();
    int version();
    void setUid(QByteArray uid);
    void addInfo(QString key, QVariant defaultValue = QVariant());
    void setBasePath(QString basePath);
    void setVersion(int version);

    bool setup();

    bool canWriteFormat(QByteArray uid);
    bool canReadFormat(QByteArray uid);
    bool canWriteMedia(QByteArray uid);
    bool canReadMedia(QByteArray uid);

    QList<QByteArray> inputMedia();
    QList<QByteArray> inputFormats();
    QList<QByteArray> outputMedia();
    QList<QByteArray> outputFormats();

    void addInputMedia(QList<QByteArray> media);
    void addInputFormats(QList<QByteArray> format);
    void addOutputMedia(QList<QByteArray> media);
    void addOutputFormats(QList<QByteArray> format);

signals:

public slots:

private:
    QByteArray _uid;
    QMap<QString,QVariant> _info;
    QString _basePath;
    int _version;

    bool _setup();
    bool _isReady();
    QMap<QString,QVariant> _setupData;


    QList<QByteArray> _inputMedia;
    QList<QByteArray> _inputFormats;
    QList<QByteArray> _outputMedia;
    QList<QByteArray> _outputFormats;
};

#endif // EXTENSION_H
