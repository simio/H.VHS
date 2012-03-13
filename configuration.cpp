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

// Singleton (Configuration::pointer())
Configuration *Configuration::s_instance = NULL;

Configuration::Configuration()
{
    qApp->setOrganizationName( "Huggpunkt" );
    qApp->setOrganizationDomain( "huggpunkt.org" );
    qApp->setApplicationName( "H.VHS" );
    qApp->setApplicationVersion( APP_VER );

    this->_settings = new QSettings("hvhs.ini", QSettings::IniFormat, 0);
    this->_settings->setIniCodec("UTF-8");
    qDebug() << "Using INI-file at" << this->_settings->fileName();
}

Configuration *Configuration::pointer()
{
    if (s_instance == NULL)
        s_instance = new Configuration;
    return s_instance;
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
    this->_settings->setValue(QString("Layout/WindowState") + QString::number((int)window), state);
    this->_settings->setValue(QString("Layout/WindowGeometry") + QString::number((int)window), geometry);
}

QByteArray Configuration::getWindowState(Window window)
{
    return this->_settings->value(QString("Layout/WindowState") + QString::number((int)window), "").toByteArray();
}

QByteArray Configuration::getWindowGeometry(Window window)
{
    return this->_settings->value(QString("Layout/WindowGeometry") + QString::number((int)window), "").toByteArray();
}

void Configuration::saveWebViewSettings(QMap<QString,QVariant> webViewSettings)
{
    QMap<QString,QVariant>::iterator webViewSetting;
    this->_settings->beginGroup("WebView");
    for (webViewSetting = webViewSettings.begin(); webViewSetting != webViewSettings.end(); webViewSetting++)
    {
        qDebug() << "Writing config pair" << webViewSetting.key() << webViewSetting.value();
        this->_settings->setValue(webViewSetting.key(), webViewSetting.value());
    }
    this->_settings->endGroup();
}

QMap<QString,QVariant> Configuration::getWebViewSettings()
{
    QMap<QString,QVariant> result;

    this->_settings->beginGroup("WebView");
    QStringList keys = this->_settings->childKeys();
    QStringList::iterator iterator;
    for (iterator = keys.begin(); iterator != keys.end(); iterator++)
    {
        result.insert(*iterator, this->_settings->value(*iterator));
        qDebug() << "Reading config pair" << *iterator <<  this->_settings->value(*iterator);
    }
    this->_settings->endGroup();

    return result;
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
