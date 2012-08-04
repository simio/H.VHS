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

#include "extensionmanager.h"

ExtensionManager *ExtensionManager::s_instance = NULL;

ExtensionManager::ExtensionManager(QObject *parent) :
    QObject(parent)
{
    this->_initialised = false;
    this->_initialise();
}

ExtensionManager *ExtensionManager::p()
{
    if (s_instance == NULL)
        s_instance = new ExtensionManager;                              // alloc: Singleton object

    return s_instance;
}

QSharedPointer<Extension> ExtensionManager::debugLoadExtension(QString id)
{
    QSharedPointer<ExtensionDefinition> definition
            = qSharedPointerDynamicCast<ExtensionDefinition>(this->_definitions.get(Definition::ExtensionDefinitionType, id));
    if (definition.isNull())
        return QSharedPointer<Extension>();

    return this->_loadExtension(definition);
}

int ExtensionManager::callHook(const qint64 hook, QVariant &hookData)
{
    int result = 0;

    // Call plugin hook on all extensions in the persistent list.
    // values() returns in ascending order, so the iteration follows priority
    foreach(QSharedPointer<Extension> extension, this->_persistentExtensions.values())
    {
        switch (extension->pluginHook(hook, hookData))
        {
        case EXT_RETVAL_NOOP:
            break;
        case EXT_RETVAL_BLOCK:
            return ++result;
            /* NOTREACHED */
        case EXT_RETVAL_DONE:
        case EXT_RETVAL_DATA_MODIFIED:
        default:
            result++;
        }
    }

    // Persistent extension instances take priority over instances
    // currently present in the JobManager. This is to ensure that
    // any hookData modifications take place before the JobManager
    // extension instances are called.
    result += this->_jobManager->callHook(hook, hookData);

    return result;
}

// Convenience member, for calling hooks where the
// QVariant &hookData parameter is not used.
int ExtensionManager::callHook(const qint64 hook)
{
    QVariant discardedReturnValue;
    return this->callHook(hook, discardedReturnValue);
}

void ExtensionManager::_initialise()
{
    if (this->_initialised)
        qFatal("ExtensionManager::_initialise() called twice!");
    this->_initialised = true;

    //  Populate the first plugin hook ring with extensions
    QHash<QString,QSharedPointer<Definition> > extensions = this->_definitions.getAll(Definition::ExtensionDefinitionType);
    QHash<QString,QSharedPointer<Definition> >::const_iterator i = extensions.begin();
    while (i != extensions.end())
    {
        QSharedPointer<ExtensionDefinition> definition = qSharedPointerDynamicCast<ExtensionDefinition>(i.value());
        if (definition->implementsInterface( HVHS_INTERFACE_HOOKS )
                && definition->isEnabled())
        {
            QSharedPointer<Extension> extension = this->_loadExtension(definition);
            if (extension && extension->implementsInterface( HVHS_INTERFACE_HOOKS ))
            {
                qDebug() << "ExtensionManager: " << definition->name() << "loaded to persistent extension list.";
                qint64 priority = extension->suggestedHookPriority();
                if (priority == EXT_NO_HOOK_PRIORITY_SUGGESTION)
                    priority = this->_defaultPluginHookPriority;
                this->_persistentExtensions.insert(priority, extension);
                extension->pluginHook(EXT_HOOK_INIT_EXTENSION_PERSISTENT);
            }
        }
        i++;
    }

    // Set up a JobManager
    this->_jobManager.reset(new JobManager(this));                                   // alloc: Has parent & QScopedPointer
}

// Load an extension and return a pointer to it (or NULL). Checking which
// interfaces are supported, or if the extension is enabled,
// is not done here.
QSharedPointer<Extension> ExtensionManager::_loadExtension(QSharedPointer<ExtensionDefinition> definition)
{
    //XXX: More type casting might simplify this code greatly
    switch (definition->api())
    {
    case ExtensionDefinition::QtPlugin:
    {
        QScopedPointer<QtPluginExtension> qtplugin(new QtPluginExtension(definition, this));      // alloc: Has parent & is scoped
        if (qtplugin.data()->isValid())
        {
            // Cast back to Extension and take ownership
            QSharedPointer<Extension> extension(qSharedPointerCast<Extension>(QSharedPointer<QtPluginExtension>(qtplugin.take())));
            return extension;
        }
        else
        {
            return QSharedPointer<Extension>();
        }
        /* NOTREACHED */
    }

    case ExtensionDefinition::JavaScript:
    {
        QScopedPointer<JavaScriptExtension> jsplugin(new JavaScriptExtension(definition, this));  // alloc: Has parent & is scoped
        if (jsplugin.data()->isValid())
        {
            // Cast back to Extension and take ownership
            QSharedPointer<Extension> extension(qSharedPointerCast<Extension>(QSharedPointer<JavaScriptExtension>(jsplugin.take())));
            return extension;
        }
        else
        {
            return QSharedPointer<Extension>();
        }
        /* NOTREACHED */
    }
    default:
        qWarning() << "ExtensionManager: Unimplemented extension api:" << definition->api() << "for extension" << definition->id();
        return QSharedPointer<Extension>();
    }
}
