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

#ifndef DUMMY_H
#define DUMMY_H

#define HVHS_EXTENSION

#include <iostream>

#include <QtGui/QtGui>
#include <QObject>
#include <QHash>
#include <QString>
#include <QVariant>

#include "common_if.h"
#include "hooks_if.h"

class DummyExtension :
    public QObject,
    public ExtensionInterfaceCommon,
    public ExtensionInterfaceHooks
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterfaceCommon ExtensionInterfaceHooks)

    public:
    operator QObject
    *();

    explicit
    DummyExtension(QObject *parent = 0);

    qint64
    suggestedHookPriority() const;
    qint64
    pluginHook(const qint64 hook, QVariant &hookData);

private:

};

#endif // DUMMY_H
