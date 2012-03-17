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

#ifdef DEPLOY
    qApp->setLibraryPaths(QStringList(qApp->applicationDirPath()));
#endif

#ifdef HIDE_COMMIT_COUNT
    this->_hideCommitCount = true;
#else
    this->_hideCommitCount = false;
#endif

    this->_writeBlock = false;

    this->_settings = new QSettings(Portability::pointer()->iniPathUser("hvhs.ini"), QSettings::IniFormat, 0);
    this->_settings->setIniCodec("UTF-8");
}

void Configuration::setWriteBlock(bool blocked)
{
    this->_writeBlock = blocked;
}

bool Configuration::clearStorage()
{
    this->_settings->clear();

    if (this->_settings->format() == QSettings::IniFormat)
        QFile(this->_settings->fileName()).remove();

    return true;
}

Configuration *Configuration::pointer()
{
    if (s_instance == NULL)
        s_instance = new Configuration;
    return s_instance;
}

QString Configuration::appName(bool full)
{
    return qApp->applicationName() + " " + this->appVersion() + this->appTag("-");
}

QString Configuration::appTag(QString prepend)
{
    QString tag( APP_TAG );
    return (tag.isEmpty() ? "" : prepend + tag);
}

QString Configuration::appVersion()
{
    return ( this->_hideCommitCount
            ? qApp->applicationVersion().remove(QRegExp("\\.[0-9]+$"))
            : qApp->applicationVersion());
}

void Configuration::saveWindow(Window window, QByteArray state, QByteArray geometry)
{
    this->_settings->beginGroup("Layout");
    this->_setValue(QString("WindowState") + QString::number((int)window), state);
    this->_setValue(QString("WindowGeometry") + QString::number((int)window), geometry);
    this->_settings->endGroup();
}

QByteArray Configuration::getWindowState(Window window)
{
    this->_settings->beginGroup("Layout");
    QByteArray retVal = this->_value(QString("WindowState") + QString::number((int)window), "").toByteArray();
    this->_settings->endGroup();
    return retVal;
}

QByteArray Configuration::getWindowGeometry(Window window)
{
    this->_settings->beginGroup("Layout");
    QByteArray retVal = this->_value(QString("WindowGeometry") + QString::number((int)window), "").toByteArray();
    this->_settings->endGroup();
    return retVal;
}

void Configuration::saveWebViewSettings(QMap<QString,QVariant> webViewSettings)
{
    QMap<QString,QVariant>::iterator webViewSetting;
    this->_settings->beginGroup("WebView");
    for (webViewSetting = webViewSettings.begin(); webViewSetting != webViewSettings.end(); webViewSetting++)
    {
        this->_setValue(webViewSetting.key(), webViewSetting.value());
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
        result.insert(*iterator, this->_value(*iterator));
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
    QString defaultQuery("https://www.startpage.com/do/search?q=");
    this->_settings->beginGroup("WebSearch");
    QUrl url = QUrl(this->_value("DefaultQuery", defaultQuery).toString() + query.replace(" ", "+"));
    this->_settings->endGroup();
    return url;
}

QString Configuration::getStorageLocation(StorageLocation location)
{
    switch (location)
    {
    case FaviconStorageLocation:                            return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
                                                            /* NOTREACHED */
    case SystemExtensionsStorageLocation:                   return Portability::pointer()->extensionsPathSystem();
                                                            /* NOTREACHED */
    case UserExtensionsStorageLocation:                     return Portability::pointer()->extensionsPathUser();
                                                            /* NOTREACHED */
    case SystemTransportDefinitionStorageLocation:              return Portability::pointer()->presetsPathSystem("transport.xml");
                                                            /* NOTREACHED */
    case UserTransportDefinitionStorageLocation:                return Portability::pointer()->presetsPathUser("transport.xml");
                                                            /* NOTREACHED */
    case SystemFormatDefinitionStorageLocation:             return Portability::pointer()->presetsPathSystem("format.xml");
                                                            /* NOTREACHED */
    case UserFormatDefinitionStorageLocation:               return Portability::pointer()->presetsPathUser("format.xml");
                                                            /* NOTREACHED */
    default:
        return QString();
    }
}

void Configuration::_setValue(const QString &key, const QVariant &value)
{
    if (! this->_writeBlock)
    {
        this->_settings->setValue(key, value);
        //qDebug() << "Config write(" << this->_settings->group() << "): " << key << value;
    }
}

QVariant Configuration::_value(const QString &key, const QVariant &defaultValue) const
{
    //qDebug() << "Config read(" << this->_settings->group() << "):" << key
    //         << this->_settings->value(key, defaultValue) << "(default:" << defaultValue << ")";
    return this->_settings->value(key, defaultValue);
}
