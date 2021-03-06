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

#ifndef EXTENSIONMANAGER_EXTENSION_EXTENSION_H
#define EXTENSIONMANAGER_EXTENSION_EXTENSION_H

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QIODevice>

#include "main.h"

#include "extensionmanager/interfaces/if_commondefines.h"
#include "extensionmanager/interfaces/common_if.h"
#include "extensionmanager/interfaces/hooks_if.h"
#include "extensionmanager/interfaces/streams_if.h"
#include "extensionmanager/interfaces/filters_if.h"

/* Base class for all extensions. Must provide dummy implementations
 * of all interfaces.
 */

class Extension : public QObject
{
    Q_OBJECT
public:
    virtual
    ~Extension()
    {
    }

    virtual bool
    isValid()
    {
        return this->implementsInterface( HVHS_INTERFACE_COMMON );
    }

    // Return StringList of supported interfaces
    QString
    id()
    {
        return this->_extensionId;
    }

    QStringList
    interfaces() const
    {
        return this->_interfaces;
    }

    bool implementsInterface(QString interface) const
    {
        return this->_interfaces.contains(QString(interface));
    }

    // HVHS_INTERFACE_HOOKS
    virtual qint64
    suggestedHookPriority() const
    {
        return EXT_NO_HOOK_PRIORITY_SUGGESTION;
    }

    virtual qint64
    pluginHook(const qint64 hook, QVariant &hookData)
    {
        Q_UNUSED(hook); Q_UNUSED(hookData);
        return EXT_RETVAL_NOOP;
    }

    virtual qint64
    pluginHook(const qint64 hook)
    {
        QVariant temp; return this->pluginHook(hook, temp);
    }

    // HVHS_INTERFACE_STREAMS
    virtual const QSharedPointer<QIODevice>
    openStream(QIODevice::OpenModeFlag openMode, const QString hurl)
    {
        Q_UNUSED(openMode); Q_UNUSED(hurl);
        return QSharedPointer<QIODevice>();
    }

    // HVHS_INTERFACE_FILTERS
    virtual const bool
    setupFilter(QHash<QString,QVariant> setupData)
    {
        Q_UNUSED(setupData);
        return false;
    }

    virtual const bool
    filterIsReady() const
    {
        return false;
    }

    virtual const QSharedPointer<QIODevice>
    consumer()
    {
        return QSharedPointer<QIODevice>();
    }

    virtual const QSharedPointer<QIODevice>
    producer()
    {
        return QSharedPointer<QIODevice>();
    }

protected:
    explicit Extension(QObject *parent = 0) : QObject(parent)
    {
    }

    QString _extensionId;
    QStringList _interfaces;
};

#endif // EXTENSIONMANAGER_EXTENSION_EXTENSION_H
