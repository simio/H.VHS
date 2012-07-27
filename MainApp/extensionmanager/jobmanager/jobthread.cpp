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

#include "jobthread.h"

JobThread::JobThread(QObject *parent) :
    QObject(parent)
{
    //XXX: Create dummy job
    this->_job = new Job;
    this->_thread = new QThread;
    connect(this->_job, SIGNAL(ping(QString)), this, SLOT(receivePingFromJob(QString)));
    connect(this, SIGNAL(pingJob()), this->_job, SLOT(receivePing()));
    this->_job->moveToThread(this->_thread);
    this->_thread->start();
}

void JobThread::ping()
{
    qDebug() << QThread::currentThread() << "JobThread pinging job in" << this->_thread;
    emit pingJob();
}

void JobThread::receivePingFromJob(const QString &message)
{
    qDebug() << QThread::currentThread() << "JobThread got ping from job:" << message;
}
