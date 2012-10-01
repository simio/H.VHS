/*
 * Copyright (c) 2012 Jesper Raftegard <jesper@huggpunkt.org>
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

#ifndef EXTENSIONMANAGER_EXTENSION_QTPLUGINEXTENSION_H
#define EXTENSIONMANAGER_EXTENSION_QTPLUGINEXTENSION_H

#include <QObject>
#include <QWeakPointer>
#include <QSharedPointer>
#include <QStringList>
#include <QIODevice>
#include <QVariant>
#include <QFileInfo>
#include <QPluginLoader>

#include "main.h"

#include "extensionmanager/definitions/extensiondefinition.h"
#include "extensionmanager/extension/extension.h"

class QtPluginExtension : public Extension
{
public:
    // This QtPluginExtension class is compatible with api="qtplugin"
    // extensions conforming to the following api versions.
    static VersionNumber
    currentApiVersion()
    {
        return VersionNumber(1, 0);
    }

    static VersionNumber
    oldestCompatibleApiVersion()
    {
        return VersionNumber(1, 0);
    }

    QtPluginExtension(QSharedPointer<ExtensionDefinition> definition,
                      QObject *parent = 0);
    ~QtPluginExtension();

    bool
    isValid();

    // HVHS_INTERFACE_HOOKS
    qint64
    suggestedHookPriority() const;

    qint64
    pluginHook(const qint64 hook, QVariant &hookData);

    qint64
    pluginHook(const qint64 hook);

    // HVHS_INTERFACE_STREAMS
    const QSharedPointer<QIODevice>
    openStream(QIODevice::OpenModeFlag openMode, const QString hurl);

    // HVHS_INTERFACE_FILTERS
    virtual const bool
    setupFilter(QHash<QString,QVariant> setupData);

    virtual const bool
    filterIsReady() const;

    virtual const QSharedPointer<QIODevice>
    consumer();

    virtual const QSharedPointer<QIODevice>
    producer();

signals:

public slots:

private:
    QScopedPointer<QObject> _qtplugin;
    QPluginLoader _loader;
};

#endif // EXTENSIONMANAGER_EXTENSION_QTPLUGINEXTENSION_H
