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

#ifndef QTPLUGINWRAPPER_H
#define QTPLUGINWRAPPER_H

#include <QObject>
#include <QPointer>
#include <QStringList>
#include <QIODevice>
#include <QVariant>
#include <QFileInfo>
#include <QPluginLoader>

#include "main.h"

#include "cassette.h"
#include "extensionmanager/definitions/extensiondefinition.h"
#include "extensionmanager/extension/extension.h"

class QtPluginExtension : public Extension
{
public:
    QtPluginExtension(QPointer<ExtensionDefinition> definition, QObject *parent = 0);
    ~QtPluginExtension();

    bool isValid();

    // Return StringList of supported interfaces
    QStringList interfaces();
    bool implementsInterface(QString interface) const;

    // HVHS_INTERFACE_HOOKS
    qint64 suggestedHookPriority() const;
    qint64 pluginHook(const qint64 hook, QVariant &hookData);

    // HVHS_INTERFACE_STREAMS
    QPointer<QDataStream> createStream(QIODevice::OpenModeFlag openMode, const QPointer<Cassette> cassette);

signals:

public slots:

private:
    QPointer<QObject> _plugin;
    QPluginLoader _loader;
    QString _extensionId;
    QStringList _interfaces;
};

#endif // QTPLUGINWRAPPER_H