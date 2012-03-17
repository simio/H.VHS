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

QString Portability::extensionsPathSystem(QString file)
{
    QString path;
#ifdef WIN32
    path = QDir::toNativeSeparators( qApp->applicationDirPath() + "/extensions/" + file);
#endif
    return path;
}

QString Portability::presetsPathSystem(QString file)
{
    QString path;
#ifdef WIN32
    path = QDir::toNativeSeparators( qApp->applicationDirPath() + "/presets/" + file);
#endif
    return path;
}

QString Portability::extensionsPathUser(QString file)
{
    QString path;
#ifdef WIN32
    path = QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/extensions/" + file);
#endif
    return path;
}

QString Portability::presetsPathUser(QString file)
{
    QString path;
#ifdef WIN32
    path = QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/presets/" + file);
#endif
    return path;
}

QString Portability::iniPathSystem(QString file)
{
    QString path;
#ifdef WIN32
    path = QDir::toNativeSeparators( qApp->applicationDirPath() + "/" + file);
#endif
    return path;
}

QString Portability::iniPathUser(QString file)
{
    QString path;
#ifdef WIN32
    path = QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/" + file);
#endif
    return path;
}
