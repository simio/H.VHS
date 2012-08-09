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

QtPluginExtension::QtPluginExtension(QSharedPointer<ExtensionDefinition> definition, QObject *parent) :
    Extension(parent)
{
    QFileInfo pluginFile = Configuration::p()->extensionRootFile(definition.data()->id(), definition.data()->apiVersion());
    if (pluginFile.isReadable())
    {
        this->_loader.setFileName(pluginFile.canonicalFilePath());
        this->_loader.load();
        this->_qtplugin.reset(this->_loader.instance());
        if (! this->_qtplugin.isNull())
        {
            this->_interfaces = definition.data()->interfaces();
            this->_extensionId = definition.data()->id();
        }
    }
}

QtPluginExtension::~QtPluginExtension()
{
    this->pluginHook( EXT_HOOK_BEFORE_KILL_EXT );
    this->_loader.unload();
}

bool QtPluginExtension::isValid()
{
    return (Extension::isValid() && this->_loader.isLoaded() && ! this->_qtplugin.isNull());
}

qint64 QtPluginExtension::suggestedHookPriority() const
{
    if (! this->implementsInterface( HVHS_INTERFACE_HOOKS ))
        return EXT_INTERFACE_NOT_SUPPORTED;
    ExtensionInterfaceHooks *exIf = qobject_cast<ExtensionInterfaceHooks*>(this->_qtplugin.data());
    return exIf->suggestedHookPriority();
}

qint64 QtPluginExtension::pluginHook(const qint64 hook, QVariant &hookData)
{
    if (! this->implementsInterface( HVHS_INTERFACE_HOOKS ))
        return EXT_INTERFACE_NOT_SUPPORTED;
    ExtensionInterfaceHooks *exIf = qobject_cast<ExtensionInterfaceHooks*>(this->_qtplugin.data());
    return exIf->pluginHook(hook, hookData);
}

// For convenience
qint64 QtPluginExtension::pluginHook(const qint64 hook)
{
    QVariant discardedReturnValue;
    return this->pluginHook(hook, discardedReturnValue);
}

const QSharedPointer<QIODevice> QtPluginExtension::openStream(QIODevice::OpenModeFlag openMode, const QString hurl)
{
    if (! this->implementsInterface( HVHS_INTERFACE_STREAMS ))
        return QSharedPointer<QIODevice>();
    ExtensionInterfaceStreams *exIf = qobject_cast<ExtensionInterfaceStreams*>(this->_qtplugin.data());
    return QSharedPointer<QIODevice>(exIf->openStream(openMode, hurl));
}

const bool QtPluginExtension::setupFilter(QHash<QString, QVariant> setupData)
{
    if (! this->implementsInterface( HVHS_INTERFACE_FILTERS ))
        return false;
    ExtensionInterfaceFilters *exIf = qobject_cast<ExtensionInterfaceFilters*>(this->_qtplugin.data());
    return exIf->setupFilter(setupData);
}

const bool QtPluginExtension::filterIsReady() const
{
    if (! this->implementsInterface( HVHS_INTERFACE_FILTERS ))
        return false;
    ExtensionInterfaceFilters *exIf = qobject_cast<ExtensionInterfaceFilters*>(this->_qtplugin.data());
    return exIf->filterIsReady();
}

const QSharedPointer<QIODevice> QtPluginExtension::consumer()
{
    if (! this->implementsInterface( HVHS_INTERFACE_FILTERS ))
        return QSharedPointer<QIODevice>();
    ExtensionInterfaceFilters *exIf = qobject_cast<ExtensionInterfaceFilters*>(this->_qtplugin.data());
    return QSharedPointer<QIODevice>(exIf->consumer());
}

const QSharedPointer<QIODevice> QtPluginExtension::producer()
{
    if (! this->implementsInterface( HVHS_INTERFACE_FILTERS ))
        return QSharedPointer<QIODevice>();
    ExtensionInterfaceFilters *exIf = qobject_cast<ExtensionInterfaceFilters*>(this->_qtplugin.data());
    return QSharedPointer<QIODevice>(exIf->producer());
}
