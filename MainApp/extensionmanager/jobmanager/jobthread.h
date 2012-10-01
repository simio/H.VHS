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

#ifndef EXTENSIONMANAGER_JOBMANAGER_JOBTHREAD_H
#define EXTENSIONMANAGER_JOBMANAGER_JOBTHREAD_H

#include <QObject>
#include <QThread>
#include <QScopedPointer>

#include "main.h"

#include "extensionmanager/jobmanager/job.h"

/* JobThread objects contain a job and a thread.
 * It is responsible for:
 *
 *  - Moving the job into the thread
 *  - Managing the thread
 *  - Keeping a job/thread state var (for reporting to JobManager)
 *  - Interfacing between the JobManager and the job.
 */

class JobThread : public QObject
{
    Q_OBJECT
public:
    explicit
    JobThread(QObject *parent = 0);
    ~JobThread();

    void
    ping();

private:
    QScopedPointer<QThread> _thread;
    QScopedPointer<Job> _job;

signals:
    void
    pingJob();

public slots:
    void
    receivePingFromJob(const QString &message);
};

#endif // EXTENSIONMANAGER_JOBMANAGER_JOBTHREAD_H
