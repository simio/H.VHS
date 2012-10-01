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

#ifndef EXTENSIONMANAGER_INTERFACES_HOOKS_IF_H
#define EXTENSIONMANAGER_INTERFACES_HOOKS_IF_H

#include <QVariant>
#include <QHash>
#include <QString>

#include "if_commondefines.h"

/* H.VHS QPlugin Extension Interface for plugin hooks, version 1.0
 *
 * The application will create exactly one instance of each extension
 * implementing this interface, which will be called for each plugin
 * hook. These instances are never accessed through any other plugin
 * interfaces.
 */

class ExtensionInterfaceHooks
{

public:
    virtual
    ~ExtensionInterfaceHooks()
    { }

    /* A suggestion to what priority this extension should have when
     * ordering a pluginHook calling list.
     *
     *  0           SPECIAL: Suggesting 0 as a priority means the extension
     *              isn't interested in answering hook calls at this time,
     *              and doesn't want to be on this particular calling list.
     *
     *  1           For blockers. The extension wants to be called first. Use
     *              iff the extension's purpose is to block one or more hooks
     *              from ever reaching any other extensions.
     *
     *  2           For hotfixes. Use iff this extension fixes stuff that
     *              other extensions might need fixed.
     *
     *  3-9         reserved (don't use them)
     *
     *  EXT_NO_HOOK_PRIORITY_SUGGESTION
     *              Use this predefined value if call ordering doesn't matter
     *              to this extension.
     *
     * Suggesting a hook priority of 0 might lead to the instance
     * being deleted.
     *
     * The main application (and user) may and quite possibly will
     * change the call order to their own preferences.
     */
    virtual qint64
    suggestedHookPriority() const = 0;

    /* This member will get called exactly once (per instance of this
     * class) each time the main application hits a plugin hook. The
     * return value should specify how this extension handles the
     * hook. Blocking results in no more extensions being called. The
     * order in which extensions are called is decided in the main
     * application.
     *
     * Hook names and possible return values are defined in
     * if_commondefines.h Extensions are (for some hooks) allowed or
     * required to modify hookData
     */
    virtual qint64
    pluginHook(const qint64 hook, QVariant &hookData) = 0;
};

Q_DECLARE_INTERFACE(ExtensionInterfaceHooks,
                    HVHS_INTERFACE_HOOKS)

#endif // EXTENSIONMANAGER_INTERFACES_HOOKS_IF_H
