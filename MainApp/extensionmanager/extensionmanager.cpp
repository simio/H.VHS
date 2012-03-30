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
        if (extension->implementsInterface( HVHS_HOOKS_INTERFACE ) && ! this->_persistentExtensions.contains(extension->id()))
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
                        ExtensionInterfaceHooks * exIf = qobject_cast<ExtensionInterfaceHooks*>(plugin);
                        if (exIf)
                        {
                            qDebug() << "Extension" << extension->name() << "loaded.";
                            this->_persistentExtensions.insert(extension->id(), exIf);
                        }
                    }
                }
            }
        }
        i++;
    }
}
