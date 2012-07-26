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
    this->_initialise();
}

ExtensionManager *ExtensionManager::p()
{
    if (s_instance == NULL)
        s_instance = new ExtensionManager;                              // alloc: Singleton object

    return s_instance;
}

QPointer<Extension> ExtensionManager::debugLoadExtension(QString id)
{
    QPointer<ExtensionDefinition> definition = (ExtensionDefinition*)(Definition*)this->_definitions.get(Definition::ExtensionDefinitionType, id);
    if (definition.isNull())
        return NULL;

    return this->_loadExtension(definition);
}

int ExtensionManager::callHook(const qint64 hook, QVariant &hookData)
{
    int result = 0;

    // values() returns in ascending order, so the iteration follows priority
    foreach(QPointer<Extension> extension, this->_persistentExtensions.values())
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
    return result;
}

int ExtensionManager::callHook(const qint64 hook)
{
    QVariant temp;
    return this->callHook(hook, temp);
}

void ExtensionManager::_initialise()
{
    /*
     *  Populate the first plugin hook ring with extensions
     */

    QHash<QString,QPointer<Definition> > extensions = this->_definitions.getAll(Definition::ExtensionDefinitionType);
    QHash<QString,QPointer<Definition> >::const_iterator i = extensions.begin();
    while (i != extensions.end())
    {
        QPointer<ExtensionDefinition> definition = (ExtensionDefinition*)(Definition*)i.value();
        if (definition->implementsInterface( HVHS_INTERFACE_HOOKS )
                && definition->isEnabled())
        {
            QPointer<Extension> extension = this->_loadExtension(definition);
            if (extension && extension->implementsInterface( HVHS_INTERFACE_HOOKS ))
            {
                qDebug() << "Extension" << definition->name() << "loaded to persistent circle.";
                qint64 priority = extension->suggestedHookPriority();
                if (priority == EXT_NO_HOOK_PRIORITY_SUGGESTION)
                    priority = 1000;
                this->_persistentExtensions.insert(priority, extension);
                extension->pluginHook(EXT_HOOK_INIT_EXTENSION_PERSISTENT);
            }
        }
        i++;
    }
}

// Load an extension and return a pointer to it (or NULL). Checking which
// interfaces are supported, or if the extension is enabled,
// is not done here.
QPointer<Extension> ExtensionManager::_loadExtension(QPointer<ExtensionDefinition> definition)
{
    QtPluginExtension * qtplugin;
    if (definition->api() == ExtensionDefinition::QtPlugin)
        qtplugin = new QtPluginExtension(definition, this);                      // alloc: Has parent
    else
    {
        qDebug() << "Unimplemented extension api:" << definition->api() << "for extension" << definition->id();
        return NULL;
    }

    if (qtplugin->isValid())
    {
        QPointer<Extension> extension;
        extension = (Extension*)(QtPluginExtension*)qtplugin;
        return extension;
    }
    else
    {
        delete qtplugin;
        return NULL;
    }
}
