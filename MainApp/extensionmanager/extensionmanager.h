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

#ifndef EXTENSIONMANAGER_EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_EXTENSIONMANAGER_H

/*  All extensions are accessed through the ExtensionManager.
 *  This singleton's work is centered around two lists.
 *
 *   - The definition list, kept in a DefinitionTable, which is a
 *     list of all definitions of Formats, Transports and Extensions.
 *
 *   - The persistent list of extensions, i.e. a list containing
 *     one instance of each extension implementing the
 *     ExtensionInterfaceHooks interface. These are kept until
 *     the singleton is destructed. They are only ever accessed
 *     through the ExtensionInterfaceHooks interface.
 *
 *  The extension instances contained in the persistent list are
 *  the instances that will receive plugin hook calls.
 *
 *  For any uses other than calling plugin hooks, the extension
 *  manager creates new instances of suitable extensions on request.
 */

#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QList>
#include <QMultiMap>
#include <QDirIterator>
#include <QDateTime>
#include <QPluginLoader>

#include "main.h"

#include "extensionmanager/interfaces/hooks_if.h"

#include "extensionmanager/definitions/definition.h"
#include "extensionmanager/definitions/formatdefinition.h"
#include "extensionmanager/definitions/transportdefinition.h"
#include "extensionmanager/definitions/extensiondefinition.h"
#include "extensionmanager/definitiontable.h"

#include "extensionmanager/extension/qtpluginextension.h"
#include "extensionmanager/extension/javascriptextension.h"

#include "jobmanager/jobmanager.h"

#include "vhsxml/documentreader.h"

class ExtensionManager : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionManager(QObject *parent = 0);
    static ExtensionManager *p();

    QSharedPointer<Extension> debugLoadExtension(QString id);

    // Run "hook" on all plugins with supplied hookData and return the number of non-NOOP responses.
    int callHook(const qint64 hook, QVariant &hookData);
    int callHook(const qint64 hook);

signals:

public slots:

private:
    static ExtensionManager *s_instance;

    void _initialise();
    bool _initialised;

    DefinitionTable _definitions;

    static const qint64 _defaultPluginHookPriority = 1000;

    // These extensions are loaded at startup and kept for as long
    // as the ExtensionManager is around.
    QMultiMap<qint64,QSharedPointer<Extension> > _persistentExtensions;

    QScopedPointer<JobManager> _jobManager;

    QSharedPointer<Extension> _loadExtension(QSharedPointer<ExtensionDefinition> definition);
};

#endif // EXTENSIONMANAGER_EXTENSIONMANAGER_H
