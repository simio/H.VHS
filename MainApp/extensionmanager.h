/*
 * Copyright (c) 2012 Jesper R�fteg�rd <jesper@huggpunkt.org>
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

#include "main.h"

#include "formatdefinition.h"
#include "mediadefinition.h"
#include "extension.h"
#include "vhsxml.h"

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

    int _loadMediaDefinitions();
    int _loadFormatDefinitions();
    int _loadExtensions();

    QList< QPointer<Extension> > _extensions;
    QList< QPointer<FormatDefinition> > _knownFormats;
    QList< QPointer<MediaDefinition> > _knownMedia;

};

#endif // EXTENSIONMANAGER_H