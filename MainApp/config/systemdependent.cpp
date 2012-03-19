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

#include "systemdependent.h"

// Singleton (Portability::pointer())
SystemDependent *SystemDependent::s_instance = NULL;

SystemDependent::SystemDependent()
{
}

SystemDependent *SystemDependent::pointer()
{
    if (s_instance == NULL)
        s_instance = new SystemDependent;
    return s_instance;
}

QDir SystemDependent::extensionsDir(Scope scope)
{
#ifdef WIN32
    switch (scope)
    {
    case System:
        return QDir(qApp->applicationDirPath() + "/extensions");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/extensions");
        /* NOTREACHED */
    }
#endif
}

QDir SystemDependent::presetsDir(Scope scope)
{
#ifdef WIN32
    switch (scope)
    {
    case System:
        return QDir(qApp->applicationDirPath() + "/presets");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/presets");
        /* NOTREACHED */
    }
#endif
}


QPointer<QSettings> SystemDependent::makeSettings()
{
#ifdef WIN32
    return new QSettings(QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/hvhs.ini"),
                         QSettings::IniFormat);
#endif
}
