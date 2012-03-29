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

#ifndef EXTENSIONINTERFACES_HOOKS_H
#define EXTENSIONINTERFACES_HOOKS_H

#include <QPointer>
#include <QVariant>
#include <QHash>
#include <QString>

#include "defines.h"

/*  H.VHS QPlugin Extension Interface for plugin hooks, version 1.0
 *
 *  The application will create exactly one instance of each extension at
 *  startup and put it on the pluginHook() calling list. The createStream()
 *  member of this instance will not be called.
 */

class ExtensionInterfaceHooks
{

public:
    virtual ~ExtensionInterfaceHooks() {}

    /* A suggestion to what priority this extension should have
     * when constructing the pluginHook calling list.
     *
     *  1           means F1RST PLZ!                        (Use only if blocking all other extensions.)
     *  2           means I FIX BROKEN THINGS               (Use only if fixing broken things.)
     *  0,3-9       are reserved for future use.            (Don't use them at all.)
     *
     *  100         Use this value if the extension doesn't care about ordering.
     *
     * The main application (and user) may and quite possibly will
     * change the call order to their own preferences.
     */
    virtual qint64 suggestedHookPriority() const = 0;

    /* This member will get called exactly once (per instance of this class)
     * each time the main application hits a plugin hook. The return value
     * should specify how this extension handles the hook. Blocking results
     * in no more extensions being called. The order in which extensions are
     * called is decided in the main application.
     *
     * Hook names and possible return values are defined in extensionhooks.h
     * Extensions are (for some hooks) allowed or required to modify hookData
     */
    virtual qint64 pluginHook(const qint64 hook, QHash<QString,QVariant> &hookData) = 0;
};

Q_DECLARE_INTERFACE(ExtensionInterfaceHooks,
                    "org.huggpunkt.VHS.Hooks/1.0")

#endif // EXTENSIONINTERFACES_HOOKS_H
