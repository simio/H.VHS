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

#include "dummy.h"

DummyExtension::operator QObject
*()
{
    return qobject_cast<QObject*>(this);
}

DummyExtension::DummyExtension(QObject *parent) :
    QObject(parent)
{
}

qint64
DummyExtension::suggestedHookPriority() const
{
    return EXT_NO_HOOK_PRIORITY_SUGGESTION;
}

qint64
DummyExtension::pluginHook(const qint64 hook, QVariant &hookData)
{
    if (hook == EXT_HOOK_INIT_EXTENSION_PERSISTENT)
    {
        qDebug() << "Dummy extension: EXT_HOOK_INIT_EXTENSION_PERSISTENT";
        return EXT_RETVAL_DONE;
    }
    else if (hook == EXT_HOOK_AFTER_WEBWINDOW_CONSTRUCTOR)
    {
        qDebug() << "Dummy extension: EXT_HOOK_AFTER_WEBWINDOW_CONSTRUCTOR";
        return EXT_RETVAL_DONE;
    }

    return EXT_RETVAL_NOOP;
}

Q_EXPORT_PLUGIN2(dummyextension, DummyExtension)
