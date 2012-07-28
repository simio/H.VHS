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

#include "javascriptextension.h"

JavaScriptExtension::JavaScriptExtension(QPointer<ExtensionDefinition> definition, QObject *parent) :
    Extension(parent)
{
    this->_initialised = this->_initialise(definition);
    if (! this->_initialised)
        qWarning() << "JavaScriptExtension object for definition" << definition->id() << "couldn't initialise";
}

JavaScriptExtension::~JavaScriptExtension()
{
    this->pluginHook( EXT_HOOK_BEFORE_KILL_EXT );

    //XXX: Delete qtscript environment etc
}

bool JavaScriptExtension::isValid()
{
    //XXX: More checks are needed
    return (Extension::isValid() && this->_initialised);
}

qint64 JavaScriptExtension::suggestedHookPriority() const
{
    if (! this->implementsInterface(HVHS_INTERFACE_HOOKS))
        return EXT_INTERFACE_NOT_SUPPORTED;

    // XXX: Fake it
    return -1;
}

qint64 JavaScriptExtension::pluginHook(const qint64 hook, QVariant &hookData)
{
    if (! this->implementsInterface(HVHS_INTERFACE_HOOKS))
        return EXT_INTERFACE_NOT_SUPPORTED;

    // XXX: Fake it
    return 0;
}

// For convenience
qint64 JavaScriptExtension::pluginHook(const qint64 hook)
{
    QVariant discardedReturnValue;
    return this->pluginHook(hook, discardedReturnValue);
}

QPointer<QIODevice> JavaScriptExtension::openStream(QIODevice::OpenModeFlag openMode, const QString hurl)
{
    // Javascript extensions cannot use this interface
    return NULL;
}

bool JavaScriptExtension::_initialise(QPointer<ExtensionDefinition> definition)
{
    if (! this->_engine.isNull())
    {
        qDebug() << "JavaScriptExtension::_initialise(): Replacing existing QScriptEngine to reinitialise for" << definition->id();
        this->_engine->deleteLater();
    }

    this->_engine = new QScriptEngine(this);                    // alloc: Has parent

    QScriptValue evalReturnValue;

    // Even if this file does not exist (with inlined script code), it will be used for error reporting.
    QFileInfo sourceFile = Configuration::p()->extensionRootFile(definition->id(), definition->apiVersion());

    // If definition->source() is not empty, evaluate it;
    if (! definition->source().isEmpty())
    {
        evalReturnValue = this->_engine->evaluate(definition->source(), sourceFile.canonicalFilePath());
        qDebug() << "JavaScriptExtension initialised with (inlined) source:";
        qDebug() << definition->source();
    }
    else
    {
        if (sourceFile.exists() && sourceFile.isReadable())
        {
            QFile scriptFile(sourceFile.canonicalFilePath());
            if (! scriptFile.open(QIODevice::ReadOnly))
            {
                qWarning() << "JavaScriptExtension::_initialise() could not read file" << sourceFile.canonicalFilePath();
                return false;
            }
            QTextStream stream(&scriptFile);
            QString source = stream.readAll();
            scriptFile.close();
            evalReturnValue = this->_engine->evaluate(source, sourceFile.canonicalFilePath());
            qDebug() << "JavaScriptExtension initialised with (standalone) source from file:";
            qDebug() << source;
        }
    }

    qDebug() << "JavaScriptExtension evaluation return value was" << evalReturnValue.toString();

    return true;
}
