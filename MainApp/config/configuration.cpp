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

#include "configuration.h"

// Singleton (Configuration::p())
Configuration *Configuration::s_instance = NULL;

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{
    qApp->setOrganizationName( "Huggpunkt" );
    qApp->setOrganizationDomain( "huggpunkt.org" );
    qApp->setApplicationName( "H.VHS" );
    qApp->setApplicationVersion( APP_VER );

#ifdef DEPLOY
#   ifdef WIN32
        qApp->setLibraryPaths(QStringList(qApp->applicationDirPath()));
#   endif
#endif

#ifdef HIDE_DEVEL_INFO
    this->_hideDevelInfo = true;
#else
    this->_hideDevelInfo = false;
#endif

    this->_writeBlock = false;

    this->_settings = PlatformDependent::p()->makeSettings(this);
    this->_settings->setIniCodec("UTF-8");

    this->_publicAppTag = QString( PUBLIC_APP_TAG );
    this->_gitHash = QString( GIT_HASH );
    this->_gitBranch = QString( GIT_BRANCH );
    this->_gitTag = QString( GIT_TAG );
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

QString Configuration::locale(bool full)
{
    return ( full ? "sv" : "sv-se" );
}

Configuration *Configuration::p()
{
    if (s_instance == NULL)
        s_instance = new Configuration;                                             // alloc: Singleton object
    return s_instance;
}

QString Configuration::appName(bool full)
{
    return qApp->applicationName()
            + " " + this->appVersion()
            + this->_wrapPublicAppTag(" ")
            + (this->_hideDevelInfo ? "" : this->_wrapGitInfo());
}

QString Configuration::_wrapPublicAppTag(QString prepend, QString append)
{
    return (this->_publicAppTag.isEmpty() ? "" : prepend + this->_publicAppTag + append);
}

QString Configuration::_wrapGitInfo(QString prepend, QString append)
{
    QString info = (this->_gitTag.isEmpty() ? "" : ":" + this->_gitTag)
            + " ("
            + (this->_gitBranch.isEmpty() ? "" : this->_gitBranch + "/") + this->_gitHash
            + ")";
    return (info.isEmpty() ? "" : prepend + info + append);
}

QString Configuration::appVersion()
{
    return ( this->_hideDevelInfo
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
    this->_settings->beginGroup("WebBrowser");
    QUrl url = QUrl(this->_value("DefaultQuery", defaultQuery).toString() + query.replace(" ", "+"));
    this->_settings->endGroup();
    return url;
}

QDir Configuration::getStorageLocation(StorageLocation type)
{
    QDir location;
    switch (type)
    {
    case FaviconStorageLocation:
        location = QDir(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
        break;
    case SystemExtensionsLocation:
        location = QDir(PlatformDependent::p()->extensionsDir(PlatformDependent::System));
        break;
    case UserExtensionsLocation:
        location = QDir(PlatformDependent::p()->extensionsDir(PlatformDependent::User));
        break;
    case SystemPresetsLocation:
        location = QDir(PlatformDependent::p()->presetsDir(PlatformDependent::System));
        break;
    case UserPresetsLocation:
        location = QDir(PlatformDependent::p()->presetsDir(PlatformDependent::User));
        break;
    default:
        return QDir();
    }

    if (! location.isReadable() )
        return QDir();

    return location;
}

/*  Return the first file matching the pattern "extensions/<id>/<id><extension-apiversion-mejor>.*".
 *  For the "dummy" extension using API version 1.2, this means "extensions/dummy/dummy1.*".
 *  In other words: Extensions must provide exactly one file matching this pattern, and
 *  that file must contain root class/function of the extension.
 *
 *  Scripted extensions with the root class inlined in the XML file are not affected by this restriction.
 *
 *  Returns QFileInfo() if no such file was found.
 */
QFileInfo Configuration::extensionRootFile(QString id, Version apiVersion)
{
    QFileInfoList candidateFiles;
    QString pattern = id + QString::number(apiVersion.major()) + ".*";

    // Check the user extension repository first.
    QDir base = PlatformDependent::p()->extensionsDir(PlatformDependent::User);
    base.cd(id);
    candidateFiles = base.entryInfoList(QStringList(pattern), QDir::Files | QDir::Readable);

    // If there were no such files in the user extension repository, check the system repository.
    if (candidateFiles.isEmpty())
    {
        base = PlatformDependent::p()->extensionsDir(PlatformDependent::System);
        base.cd(id);
        candidateFiles = base.entryInfoList(QStringList(pattern), QDir::Files | QDir::Readable);
    }

    if (candidateFiles.isEmpty())
        return QFileInfo();
    else
        return candidateFiles.first();
}

void Configuration::_setValue(const QString &key, const QVariant &value)
{
    if (! this->_writeBlock)
    {
        this->_settings->setValue(key, value);
    }
}

QVariant Configuration::_value(const QString &key, const QVariant &defaultValue) const
{
    return this->_settings->value(key, defaultValue);
}
