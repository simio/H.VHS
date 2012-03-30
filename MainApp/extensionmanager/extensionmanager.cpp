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

ExtensionManager *ExtensionManager::pointer()
{
    if (s_instance == NULL)
        s_instance = new ExtensionManager;                              // alloc: Singleton object

    return s_instance;
}

int ExtensionManager::callHook(const qint64 hook, QMultiMap<QString, QVariant> &hookData)
{
    int result = 0;

    // values() returns in ascending order, so the iteration follows priority
    foreach(ExtensionInterfaceHooks *exIf, this->_persistentExtensions.values())
    {
        switch (exIf->pluginHook(hook, hookData))
        {
        case EXT_RETVAL_NOOP:
            break;
        case EXT_RETVAL_BLOCK:
            return ++result;
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
    QMultiMap<QString,QVariant> temp;
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
        QPointer<ExtensionDefinition> extension = (ExtensionDefinition*)(Definition*)i.value();
        if (extension->implementsInterface( HVHS_HOOKS_INTERFACE )
                && extension->isEnabled())
        {
            QList<QFileInfo> pluginFiles = Configuration::p()->extensionQPluginFiles(extension->id());
            foreach(QFileInfo pluginFile, pluginFiles)
            {
                if (pluginFile.isReadable())
                {
                    QPluginLoader loader(pluginFile.canonicalFilePath());
                    QPointer<QObject> plugin = loader.instance();
                    if (plugin)
                    {
                        ExtensionInterfaceHooks *exIf = qobject_cast<ExtensionInterfaceHooks*>(plugin);
                        if (exIf)
                        {
                            qDebug() << "Extension" << extension->name() << "loaded.";
                            if (exIf->suggestedHookPriority())
                            {
                                QMultiMap<QString,QVariant> temp;
                                exIf->pluginHook(EXT_HOOK_INIT_EXTENSION_PERSISTENT, temp);
                                this->_persistentExtensions.insert(exIf->suggestedHookPriority(), exIf);
                            }
                        }
                    }
                }
            }
        }
        i++;
    }
}
