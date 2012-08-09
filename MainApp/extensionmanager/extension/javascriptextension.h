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

#ifndef EXTENSIONMANAGER_EXTENSION_JAVASCRIPTEXTENSION_H
#define EXTENSIONMANAGER_EXTENSION_JAVASCRIPTEXTENSION_H

#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QtScript>

#include "main.h"

#include "extensionmanager/definitions/extensiondefinition.h"
#include "extensionmanager/extension/extension.h"

class JavaScriptExtension : public Extension
{
public:
    // This JavaScriptExtension class is compatible with api="javascript" extensions
    // conforming to the following api versions.
    static VersionNumber currentApiVersion()             { return VersionNumber(1, 0); }
    static VersionNumber oldestCompatibleApiVersion()    { return VersionNumber(1, 0); }

    explicit JavaScriptExtension(QSharedPointer<ExtensionDefinition> definition, QObject *parent = 0);

    ~JavaScriptExtension();

    bool isValid();

    // HVHS_INTERFACE_HOOKS
    qint64 suggestedHookPriority() const;
    qint64 pluginHook(const qint64 hook, QVariant &hookData);
    qint64 pluginHook(const qint64 hook);

    // HVHS_INTERFACE_STREAMS
    const QSharedPointer<QIODevice> openStream(QIODevice::OpenModeFlag openMode, const QString hurl);

private:
    QScopedPointer<QScriptEngine> _engine;
    QSharedPointer<ExtensionDefinition> _definition;

    bool _initialise();
    bool _initialised;

    bool _hasError(QScriptValue evalReturnValue = QScriptValue()) const;

signals:

public slots:

};

#endif // EXTENSIONMANAGER_EXTENSION_JAVASCRIPTEXTENSION_H
