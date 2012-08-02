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

#include "platformdependent.h"

// Singleton (Portability::p())
PlatformDependent *PlatformDependent::s_instance = NULL;

PlatformDependent::PlatformDependent()
{ }

PlatformDependent *PlatformDependent::p()
{
    if (s_instance == NULL)
        s_instance = new PlatformDependent;                               // alloc: Singleton object
    return s_instance;
}

void PlatformDependent::setQAppLibraryPaths()
{
#ifdef DEPLOY
#   ifdef WIN32
        // Ensure lib lookups happen ONLY in the applicationDirPath.
        // Only needed in Windows. Perhaps not even there.
        // According to the docs, the default list is only INSTALL/plugins,
        // where INSTALL is the Qt installation dir.
        qApp->setLibraryPaths(QStringList(qApp->applicationDirPath()));
#   endif
#endif
}

QDir PlatformDependent::extensionsDir(Scope scope)
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
#elif unix
    switch (scope)
    {
    case System:
        //XXX: This is a placeholder value, only used in testing
        return QDir("/etc/hvhs/extensions");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/extensions");
        /* NOTREACHED */
    }
#endif
}

QDir PlatformDependent::presetsDir(Scope scope)
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
#elif unix
    switch (scope)
    {
    case System:
        return QDir("/etc/hvhs/presets");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/presets");
        /* NOTREACHED */
    }
#endif
}


QPointer<QSettings> PlatformDependent::makeSettings(QObject * parent)
{
#ifdef WIN32
    // alloc: Has parent
    return new QSettings(QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/hvhs.ini"),
                         QSettings::IniFormat, parent);
#elif unix
    // alloc: Has parent
    return new QSettings(QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/hvhs.conf"),
                         QSettings::NativeFormat, parent);
#endif
}
