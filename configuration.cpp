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

#include "configuration.h"

Configuration::Configuration()
{
    qApp->setOrganizationName( "Huggpunkt" );
    qApp->setOrganizationDomain( "huggpunkt.org" );
    qApp->setApplicationName( "H.VHS" );
    qApp->setApplicationVersion( APP_VER );

    this->_settings = new QSettings("hvhs.ini", QSettings::IniFormat, 0);
    this->_settings->setIniCodec("UTF-8");
}

QString Configuration::appName()
{
    return qApp->applicationName();
}

QString Configuration::appVersion(bool full)
{
    return (full
            ? qApp->applicationVersion()
            : qApp->applicationVersion().remove(QRegExp("[0.]+$")));
}

QString Configuration::fullAppName(bool fullVersion)
{
    return this->appName() + " " + this->appVersion(fullVersion);
}

void Configuration::saveWindow(Window window, QByteArray state, QByteArray geometry)
{
    this->_setValue(QString("WindowState") + QString::number((int)window), state);
    this->_setValue(QString("WindowGeometry") + QString::number((int)window), geometry);
}

QByteArray Configuration::getWindowState(Window window)
{
    return this->_getValue(QString("WindowState") + QString::number((int)window), "").toByteArray();
}

QByteArray Configuration::getWindowGeometry(Window window)
{
    return this->_getValue(QString("WindowGeometry") + QString::number((int)window), "").toByteArray();
}

QUrl Configuration::getStartPage()
{
    return QUrl("about:blank");
}

QUrl Configuration::makeSearchUrl(QString query)
{
    return QUrl(QString("https://www.startpage.com/do/search?q=") + query.replace(" ", "+"));
}

QString Configuration::getStorageLocation(StorageLocation location)
{
    switch (location)
    {
    case FaviconStorageLocation:
        return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
        /* NOTREACHED */
    default:
        return QString();
    }
}

void Configuration::_setValue(const QString &key, const QVariant &value)
{
    this->_settings->setValue(key, value);
}

QVariant Configuration::_getValue(const QString &key, const QVariant &defaultValue) const
{
    return this->_settings->value(key, defaultValue);
}
