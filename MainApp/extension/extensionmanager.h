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

#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include <QObject>
#include <QList>
#include <QDirIterator>
#include <QDateTime>

#include "main.h"

#include "formatdefinition.h"
#include "transportdefinition.h"
#include "extension.h"
#include "vhsxml/reader.h"

class ExtensionManager : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionManager(QObject *parent = 0);
    static ExtensionManager *pointer();

    int count();

signals:

public slots:

private:
    static ExtensionManager *s_instance;

    int _loadAndMergeTransportDefinitions();
    int _loadAndMergeFormatDefinitions();
    int _loadAndMergeExtensions();

    QHash< QByteArray, Extension> _extensions;
    QHash< QByteArray, FormatDefinition> _formats;
    QHash< QByteArray, TransportDefinition > _transports;

};

#endif // EXTENSIONMANAGER_H