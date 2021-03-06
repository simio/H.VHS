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

#ifndef EXTENSIONMANAGER_INTERFACES_STREAMS_H
#define EXTENSIONMANAGER_INTERFACES_STREAMS_H

#include <QIODevice>
#include <QSharedPointer>

#include "if_commondefines.h"

/* H.VHS QPlugin Extension Interface for QIODevices, version 1.0
 *
 * When the main application needs to call openStream(), it will
 * create a new instance of the extension. This new instance will be
 * put on a pluginHook() calling list, if the hooks interface is
 * supported by this extension. This means more than one instance of
 * every stream-creating extension might be called for each
 * hook. Avoid conflicts!
 */

class ExtensionInterfaceStreams
{
public:
    virtual
    ~ExtensionInterfaceStreams()
    { }

    virtual const QSharedPointer<QIODevice>
    openStream(QIODevice::OpenModeFlag openMode, const QString hurl)
    = 0;
};

Q_DECLARE_INTERFACE(ExtensionInterfaceStreams,
                    HVHS_INTERFACE_STREAMS)

#endif // EXTENSIONMANAGER_INTERFACES_STREAMS_H
