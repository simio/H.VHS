/*
 * Copyright (c) 2012 Jesper Räftegård <jesper@huggpunkt.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
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

#ifndef EXTENSIONINTERFACE_H
#define EXTENSIONINTERFACE_H

#include <QPointer>
#include <QDataStream>

#include "extensionhooks.h"
#include "cassette.h"

/*  H.VHS QPlugin Extension Interface, version 1.0
 *
 *  The application will create exactly one instance of each extension at
 *  startup. This instance will be called exactly once per plugin hook in
 *  the main application.
 *
 *  If, during the startup process, an extension instance suggests a hook
 *  call priority of zero, it might get deleted.
 *
 *
 *
 */

class ExtensionPrivate;

class ExtensionInterface
{
protected:
    QPointer<ExtensionPrivate> d;
public:
    virtual ~ExtensionInterface();

    /*
     *      PLUGIN HOOK EXTENSION INTERFACE
     */

    /* A suggestion to what priority this extension should have
     * when constructing the pluginHook calling list.
     *
     *  0           means GO AWAY                           (Use if this instance will not handle hooks.)
     *  1           means F1RST PLZ!                        (Use only if blocking all other extensions.)
     *  2           means I FIX BROKEN THINGS               (Use only if fixing broken things.)
     *  3-9         are reserved for future use.            (Don't use them at all.)
     *
     *  100         means JUST CALL ME, THAT ALL            (Use if the extension doesn't care about ordering.)
     *
     * The main application (and user) may and quite possibly will
     * change the call order to their own preferences.
     */
    virtual qint64 suggestedHookPriority() const;

    /* This member will get called exactly once each time the main application
     * hits a plugin hook. The return value should specify how this extension
     * handles the hook. Blocking results in no more extensions being called.
     * The order in which extensions are called is decided in the main application.
     *
     * The hook names are defined in extensionhooks.h
     */
    virtual QString pluginHook(const QString hookName, QHash<QString,QVariant> hookData);

    // Returns NULL and sets error()/errorStr() on failure
    virtual QPointer<QDataStream> pluginStream(QIODevice::OpenModeFlag openMode, const QPointer<const Cassette> cassette);

    virtual bool reset();                               // Reset extension to an empty, ready state. Returns true on success.
    virtual qint64 error() const;                       // Returns the current state(s) of the extension.
    virtual QString errorStr() const;                   // Human readable description of current state. For prettyprinting only.
};

Q_DECLARE_INTERFACE(ExtensionInterface,
                    "org.huggpunkt.VHS.ExtensionInterface/1.0")

#endif // EXTENSIONINTERFACE_H
