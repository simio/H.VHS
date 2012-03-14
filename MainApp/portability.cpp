/*
 * Copyright (c) 2012 Jesper Räftegård <jesper@huggpunkt.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
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

#include "portability.h"

// Singleton (Portability::pointer())
Portability *Portability::s_instance = NULL;

Portability::Portability()
{
}

Portability *Portability::pointer()
{
    if (s_instance == NULL)
        s_instance = new Portability;
    return s_instance;
}

QString Portability::systemExtensionPath(QString file)
{
#ifndef RELEASE
    QString base = QString( PRO_PATH );
#elif WIN32
    QString base = qApp->applicationDirPath();
#endif
    return QDir::toNativeSeparators( base + "/extensions/" + file);
}

QString Portability::systemConfPath(QString file)
{
#ifndef RELEASE
    QString base = QString( PRO_PATH );
#elif WIN32
    QString base = qApp->applicationDirPath();
#endif
    return QDir::toNativeSeparators( base + "/conf/" + file);
}

QString Portability::userExtensionPath(QString file)
{
#ifdef WIN32
    return QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/extensions/" + file);
#endif
}

QString Portability::userConfPath(QString file)
{
#ifdef WIN32
    return QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/conf/" + file);
#endif
}
