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

#ifndef EXTENSIONMANAGER_JOBMANAGER_JOB_H
#define EXTENSIONMANAGER_JOBMANAGER_JOB_H

#include <QObject>
#include <QPointer>
#include <QLinkedList>
#include <QBuffer>
#include <QThread>

#include "main.h"

#include "extensionmanager/interfaces/streams.h"

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QObject *parent = 0);

private:
    QPointer<ExtensionInterfaceStreams> _source;
    QPointer<ExtensionInterfaceStreams> _target;
    QLinkedList<QPointer<ExtensionInterfaceStreams> > _converters;

signals:
    void ping(const QString &message);

public slots:
    void receivePing();

};

#endif // EXTENSIONMANAGER_JOBMANAGER_JOB_H