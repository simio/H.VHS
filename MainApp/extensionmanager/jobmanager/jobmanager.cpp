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

#include "jobmanager.h"

JobManager::JobManager(QObject *parent) :
    QObject(parent)
{
    //XXX: Dummy jobthread, remove code later
    QSharedPointer<JobThread> dummy(new JobThread);
    this->_jobQueue.append(dummy);

    this->pingRepeat();
}

JobManager::~JobManager()
{
}

void
JobManager::pingRepeat()
{
    QTimer::singleShot(10000, this, SLOT(pingRepeat()));
    foreach(QSharedPointer<JobThread> jt, this->_jobQueue)
        jt.data()->ping();
}

// Just a placeholder for now
int
JobManager::callHook(const qint64 hook, QVariant &hookData)
{
    return 0;
}
