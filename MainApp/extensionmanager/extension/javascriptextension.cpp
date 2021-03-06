/*
 * Copyright (c) 2012 Jesper Raftegard <jesper@huggpunkt.org>
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

/*  This object makes a copy of the supplied ExtensionDefinition on
 *  construction.  The ExtensionDefinition will never change nor be
 *  swapped for another one.  Create another instance of
 *  JavaScriptExtension to use another definition.
 */

JavaScriptExtension::JavaScriptExtension(
    QSharedPointer<ExtensionDefinition> definition, QObject *parent) :
    Extension(parent)
{
    /*  Copying the object rather than the pointer ensures updates to
     *  the definition won't affect any already running extensions.
     */
    // alloc: QSharedPointer
    this->_definition =
	QSharedPointer<ExtensionDefinition>(
	    new ExtensionDefinition(*(definition.data())));     

    this->_initialised = this->_initialise();
    if (! this->_initialised)
    {
        qWarning() << "JavaScriptExtension object for definition"
		   << definition->id() << "couldn't initialise.";
    }
}

JavaScriptExtension::~JavaScriptExtension()
{
    this->pluginHook( EXT_HOOK_BEFORE_KILL_EXT );

    // nodelete: this->_engine is QScopedPointer
    // nodelete: this->_definition is QSharedPointer
}

bool
JavaScriptExtension::isValid()
{
    //XXX: More checks are needed
    return (Extension::isValid() && this->_initialised);
}

qint64
JavaScriptExtension::suggestedHookPriority() const
{
    if (! this->implementsInterface(HVHS_INTERFACE_HOOKS))
        return EXT_INTERFACE_NOT_SUPPORTED;

    //XXX: Fake it
    return -1;
}

qint64
JavaScriptExtension::pluginHook(const qint64 hook, QVariant &hookData)
{
    if (! this->implementsInterface(HVHS_INTERFACE_HOOKS))
        return EXT_INTERFACE_NOT_SUPPORTED;

    //XXX: Fake it
    return 0;
}

// For convenience
qint64
JavaScriptExtension::pluginHook(const qint64 hook)
{
    QVariant discardedReturnValue;
    return this->pluginHook(hook, discardedReturnValue);
}

/*  Try to initialise the script environment, and return true or false
 *  depending on if the script extension is valid. (Validity holds
 *  true if this function and Extension::isValid() returns true, see
 *  isValid() above.)
 *
 *  Currently, the script source code is only validated here,
 *  including if any source code is present at all. This is probably
 *  bad. Checking if any source is present should happen in checking
 *  the validity of the ExtensionDefinition, since the source code
 *  should be considered part of the definition, rather than the
 *  actual plugin.
 *
 *  From the same reasoning follows that checking whether the script
 *  evaluates properly also should happen on definition level. That
 *  might not work out too well though, because of memory, speed
 *  etc. Also, new instances of extensions are created on demand,
 *  making definition level script evaluation totally useless for any
 *  other purpose than checking whether evaluation fails or not.
 *
 *  This function is also too long and should be broken up.
 */
bool
JavaScriptExtension::_initialise()
{
    // Load source code from definition, or from file if
    // definition->source() is empty.
    QString source = QString();
    QString sourceFilename = QString();
    if (! this->_definition->source().isEmpty())
    {
        qDebug() << "JavaScriptExtension: Inline source for"
		 << this->_definition->id();
        source = this->_definition->source();
        //XXX: This is a hack. Doing it properly would include having
        //     the definition provide its own source filename.
        sourceFilename = this->_definition->id() + ".xml";
    }
    else
    {
        QFile scriptFile(
	    Configuration::p()->extensionRootFile(
		this->_definition->id(),
		this->_definition->apiVersion()).canonicalFilePath());
        if (! scriptFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "JavaScriptExtension: Extension"
		       << this->_definition->id()
		       << "has no inlined source, but a source file could not"
		       << " be found or opened for reading.";
            return false;
        }

        //XXX: AFAIK the following will fail to report input errors.
        //     (http://qt-project.org/doc/qt-4.8/qiodevice.html#readAll)
        QTextStream stream(&scriptFile);
        source = stream.readAll();
        scriptFile.close();
        sourceFilename = scriptFile.fileName();
    }

    // Delete the current script engine, if there is one, and create a new.
    if (! this->_engine.isNull())
    {
        qDebug() << "JavaScriptExtension::_initialise(): Replacing"
		 << "existing QScriptEngine to reinitialise for"
                 << this->_definition->id();
    }
    // alloc: QScopedPointer
    this->_engine.reset(new QScriptEngine(0));                  

    // Setup up an environment for the actual extension script here

    // Evaluate source now
    qDebug() << "JavaScriptExtension: Evaluating source for "
	     << this->_definition->id();

    if (this->_hasError(this->_engine.data()->evaluate(source, sourceFilename)))
    {
        // Since we don't know what the user of this instance does if
        // initialisation fails and this->isValid() returns false, the
        // allocated engine is deleted here.
        this->_engine.take()->deleteLater();
        return false;
    }

    return true;
}

/* Check for uncaught exceptions. Return true and print stuff if
 * there were any.
 */
bool
JavaScriptExtension::_hasError(QScriptValue evalReturnValue) const
{
    if (this->_engine.isNull())
    {
        qCritical() << "JavaScriptExtension::_hasError(): "
		    << "Called without engine!";
        return true;
    }
    if (! this->_engine.data()->hasUncaughtException())
        return false;

    QString retvalstr = (evalReturnValue.isNull()
			 ? "In"
			 : evalReturnValue.toString() + " in");

    qWarning() << "JavaScriptExtension::_hasError():" << retvalstr
               << "source for" << this->_definition->id()
               << "line" << this->_engine.data()->uncaughtExceptionLineNumber();
    qWarning() << "  " << this->_engine.data()->uncaughtException().toString();
    qWarning() << "Trace follows:";
    foreach(QString line, this->_engine.data()->uncaughtExceptionBacktrace())
        qWarning() << "  |" << line;

    return true;
}
