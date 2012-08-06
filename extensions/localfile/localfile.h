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

#ifndef LOCALFILE_H
#define LOCALFILE_H

#define HVHS_EXTENSION

#include <QtGui/QtGui>
#include <QObject>
#include <QSharedPointer>
#include <QIODevice>
#include <QFileInfo>

#include "common_if.h"
#include "streams_if.h"

/*  This extension will currently only allow one file per instance,
 *  and should be rewritten to either forget about opened files as
 *  soon as they've been returned (as QWeakPointer) to the caller,
 *  or keep them in a QList<QSharedPointer<QFile> >. Keeping a table
 *  would allow the extension to run some kind of maintenance, but is
 *  that really needed?
 */

class LocalFileExtension :
        public QObject,
        public ExtensionInterfaceCommon,
        public ExtensionInterfaceStreams
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterfaceCommon ExtensionInterfaceStreams)

public:
    operator QObject*();
    explicit LocalFileExtension(QObject *parent = 0);
    ~LocalFileExtension();

    const QSharedPointer<QIODevice> openStream(QIODevice::OpenModeFlag openMode, const QString hurl);

private:
    QSharedPointer<QFile> _file;

    const QSharedPointer<QFile> _resolveHurl(QString hurl);

};

#endif // LOCALFILE_H
