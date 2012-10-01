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

#include "platformdependent.h"

// Singleton (Portability::p())
PlatformDependent *PlatformDependent::s_instance = NULL;

PlatformDependent::PlatformDependent()
{ }

PlatformDependent *
PlatformDependent::p()
{
    if (s_instance == NULL)
        s_instance = new PlatformDependent;           // alloc: Singleton object
    return s_instance;
}

void
PlatformDependent::setQAppLibraryPaths()
{
#ifdef DEPLOY
#   ifdef _WIN32
    // Ensure lib lookups happen ONLY in the applicationDirPath.
    // Only needed in Windows. Perhaps not even there.
    // According to the docs, the default list is only INSTALL/plugins,
    // where INSTALL is the Qt installation dir.
    qApp->setLibraryPaths(QStringList(qApp->applicationDirPath()));
#   endif // _WIN32
#endif // DEPLOY
}

QDir
PlatformDependent::extensionsDir(Scope scope)
{
#ifdef _WIN32
    switch (scope)
    {
    case System:
        return QDir(qApp->applicationDirPath() + "/extensions");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(
                        QDesktopServices::DataLocation)
                    + "/extensions");
        /* NOTREACHED */
    }
#endif // _WIN32
#ifdef __unix__
    switch (scope)
    {
    case System:
        //XXX: This is a placeholder value, only used in testing
        return QDir("/etc/hvhs/extensions");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(
                        QDesktopServices::DataLocation)
                    + "/extensions");
        /* NOTREACHED */
    }
#endif // __unix__
}

QDir
PlatformDependent::presetsDir(Scope scope)
{
#ifdef _WIN32
    switch (scope)
    {
    case System:
        return QDir(qApp->applicationDirPath() + "/presets");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(
                        QDesktopServices::DataLocation)
                    + "/presets");
        /* NOTREACHED */
    }
#endif // _WIN32
#ifdef __unix__
    switch (scope)
    {
    case System:
        return QDir("/etc/hvhs/presets");
        /* NOTREACHED */
    default:
    case User:
        return QDir(QDesktopServices::storageLocation(
                        QDesktopServices::DataLocation)
                    + "/presets");
        /* NOTREACHED */
    }
#endif // __unix__
}


QWeakPointer<QSettings>
PlatformDependent::newSettings(QObject * parent)
{
    // alloc: Has parent assigned from parameter above
#ifdef _WIN32
    return new QSettings(QDir::toNativeSeparators(
                             QDesktopServices::storageLocation(
                                 QDesktopServices::DataLocation)
                             + "/hvhs.ini"),
                         QSettings::IniFormat, parent);
#endif // _WIN32
#ifdef __unix__
    return new QSettings(QDir::toNativeSeparators(
                             QDesktopServices::storageLocation(
                                 QDesktopServices::DataLocation)
                             + "/hvhs.conf"),
                         QSettings::NativeFormat, parent);
#endif // __unix__
}
