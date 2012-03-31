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

#include "qtpluginextension.h"

QtPluginExtension::QtPluginExtension(QPointer<ExtensionDefinition> definition, QObject *parent) :
    Extension(parent)
{
    QFileInfo pluginFile = Configuration::p()->extensionRootFile(definition->id(), definition->apiVersion());
    if (pluginFile.isReadable())
    {
        this->_loader.setFileName(pluginFile.canonicalFilePath());
        this->_loader.load();
        this->_plugin = this->_loader.instance();
        if (this->_plugin)
        {
            this->_interfaces = definition->interfaces();
            this->_extensionId = definition->id();
        }
    }
}

QtPluginExtension::~QtPluginExtension()
{
    this->_loader.unload();
}

bool QtPluginExtension::isValid()
{
    return this->_loader.isLoaded();
}

QStringList QtPluginExtension::interfaces()
{
    return this->_interfaces;
}

bool QtPluginExtension::implementsInterface(QString interface) const
{
    return this->_interfaces.contains(interface);
}

qint64 QtPluginExtension::suggestedHookPriority() const
{
    if (! this->implementsInterface(HVHS_INTERFACE_HOOKS))
        return EXT_INTERFACE_NOT_SUPPORTED;
    ExtensionInterfaceHooks *exIf = qobject_cast<ExtensionInterfaceHooks*>(this->_plugin);
    return exIf->suggestedHookPriority();
}

qint64 QtPluginExtension::pluginHook(const qint64 hook, QVariant &hookData)
{
    if (! this->implementsInterface(HVHS_INTERFACE_HOOKS))
        return EXT_INTERFACE_NOT_SUPPORTED;
    ExtensionInterfaceHooks *exIf = qobject_cast<ExtensionInterfaceHooks*>(this->_plugin);
    return exIf->pluginHook(hook, hookData);
}

QPointer<QDataStream> QtPluginExtension::createStream(QIODevice::OpenModeFlag openMode, const QPointer<Cassette> cassette)
{
    if (! this->implementsInterface(HVHS_INTERFACE_STREAMS))
        return QPointer<QDataStream>();
    ExtensionInterfaceStreams *exIf = qobject_cast<ExtensionInterfaceStreams*>(this->_plugin);
    return exIf->createStream(openMode, cassette);
}
