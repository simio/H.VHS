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

    PlatformDependent::p()->setQAppLibraryPaths();

#ifdef HIDE_DEVEL_INFO
    this->_hideDevelInfo = this->_defaults.hideDevelInfo;
#else
    this->_hideDevelInfo = false;
#endif

    this->_writeBlock = false;

    this->_settings = PlatformDependent::p()->newSettings(this);
    this->_settings.data()->setIniCodec("UTF-8");

    this->_publicAppTag = QString( PUBLIC_APP_TAG );
    this->_gitHash = QString( GIT_HASH );
    this->_gitBranch = QString( GIT_BRANCH );
    this->_gitTag = QString( GIT_TAG );
}

void
Configuration::setWriteBlock(bool blocked)
{
    this->_writeBlock = blocked;
}

bool
Configuration::clearStorage()
{
    this->_settings.data()->clear();

    // Remove the file
    if (this->_settings.data()->format() == QSettings::IniFormat)
        QFile(this->_settings.data()->fileName()).remove();

    return true;
}

QString
Configuration::locale(bool full) const
{
    //XXX: Fixing this is and actually detecting the proper locale is issue #7
    return ( full ? "sv" : "sv-se" );
}

Configuration *
Configuration::p()
{
    if (s_instance == NULL)
        s_instance = new Configuration;        // alloc: Singleton object
    return s_instance;
}

QString
Configuration::appName(bool full) const
{
    return qApp->applicationName()
        + " " + this->appVersion()
        + this->_wrapPublicAppTag(" ")
        + (this->_hideDevelInfo ? "" : this->_wrapGitInfo());
}

QString
Configuration::_wrapPublicAppTag(QString prepend, QString append) const
{
    return (this->_publicAppTag.isEmpty()
            ? ""
            : prepend + this->_publicAppTag + append);
}

QString
Configuration::_wrapGitInfo(QString prepend, QString append) const
{
    QString info = (this->_gitTag.isEmpty() ? "" : ":" + this->_gitTag)
        + " ("
        + (this->_gitBranch.isEmpty()
           ? ""
           : this->_gitBranch + "/")
        + this->_gitHash
        + ")";
    return (info.isEmpty() ? "" : prepend + info + append);
}

QString
Configuration::appVersion() const
{
    return ( this->_hideDevelInfo
             ? qApp->applicationVersion().remove(QRegExp("\\.[0-9]+$"))
             : qApp->applicationVersion());
}

void
Configuration::saveWindow(const Window &window,
                          const QByteArray &state,
                          const QByteArray &geometry)
{
    this->_settings.data()->beginGroup("Layout");
    this->_setValue(QString("WindowState")
                    + QString::number((int)window), state);
    this->_setValue(QString("WindowGeometry")
                    + QString::number((int)window), geometry);
    this->_settings.data()->endGroup();
}

QByteArray
Configuration::getWindowState(const Window &window) const
{
    this->_settings.data()->beginGroup("Layout");
    QByteArray retVal =
        this->_value(QString("WindowState")
                     + QString::number((int)window), "").toByteArray();
    this->_settings.data()->endGroup();
    return retVal;
}

QByteArray
Configuration::getWindowGeometry(const Window &window) const
{
    this->_settings.data()->beginGroup("Layout");
    QByteArray retVal =
        this->_value(QString("WindowGeometry")
                     + QString::number((int)window), "").toByteArray();
    this->_settings.data()->endGroup();
    return retVal;
}

void
Configuration::saveWebViewSettings(QMap<QString,QVariant> webViewSettings)
{
    QMap<QString,QVariant>::iterator webViewSetting;
    this->_settings.data()->beginGroup("WebView");
    for (webViewSetting = webViewSettings.begin();
         webViewSetting != webViewSettings.end();
         webViewSetting++)
    {
        this->_setValue(webViewSetting.key(), webViewSetting.value());
    }
    this->_settings.data()->endGroup();
}

QMap<QString,QVariant>
Configuration::getWebViewSettings() const
{
    QMap<QString,QVariant> result;

    this->_settings.data()->beginGroup("WebView");
    QStringList keys = this->_settings.data()->childKeys();
    QStringList::iterator iterator;
    for (iterator = keys.begin(); iterator != keys.end(); iterator++)
    {
        result.insert(*iterator, this->_value(*iterator));
    }
    this->_settings.data()->endGroup();

    return result;
}

QUrl
Configuration::makeSearchUrl(const QString &query) const
{
    QString q(query);
    this->_settings.data()->beginGroup("WebBrowser");
    QUrl url = QUrl(this->_value("DefaultQuery",
                                 this->_defaults.searchQuery).toString()
                    + q.replace(" ", "+"));
    this->_settings.data()->endGroup();
    return url;
}

QString
Configuration::getStartPage(const BrowserStartPage &page) const
{
    switch (page)
    {
    case Configuration::UserDefaultStartPage:
	return this->_value("userStartPage", QString()).toString();
	/* NOTREACHED */
    case Configuration::OneShotStartPage:
	return this->_value("oneShotStartPage", QString()).toString();
	/* NOTREACHED */
    case Configuration::CompiledDefaultStartPage:
    default:
	return this->_defaults.startPage;
	/* NOTREACHED */
    }
}

void
Configuration::setStartPage(const BrowserStartPage &page,
			    const QString &value)
{
    switch (page)
    {
    case Configuration::UserDefaultStartPage:
	this->_setValue("userStartPage", value);
	break;
    case Configuration::OneShotStartPage:
	this->_setValue("oneShotStartPage", value);
	break;
    default:
	qWarning() << "Configuration::setStartPage(): cannot set start"
		   << "page of type" << page;
    }
}

QDir
Configuration::getStorageLocation(const StorageLocation &type) const
{
    QDir location;
    switch (type)
    {
    case FaviconStorageLocation:
        location = QDir(QDesktopServices::storageLocation(
                            QDesktopServices::CacheLocation));
        break;
    case SystemExtensionsLocation:
        location = QDir(PlatformDependent::p()->extensionsDir(
                            PlatformDependent::System));
        break;
    case UserExtensionsLocation:
        location = QDir(PlatformDependent::p()->extensionsDir(
                            PlatformDependent::User));
        break;
    case SystemPresetsLocation:
        location = QDir(PlatformDependent::p()->presetsDir(
                            PlatformDependent::System));
        break;
    case UserPresetsLocation:
        location = QDir(PlatformDependent::p()->presetsDir(
                            PlatformDependent::User));
        break;
    default:
        return QDir();
    }

    if (! location.isReadable() )
        return QDir();

    return location;
}

/* Return the first file matching the pattern
 * "extensions/<id>/<id><extension-apiversion-mejor>.*". For the
 * "dummy" extension using API version 1.2, this means
 * "extensions/dummy/dummy1.*".  In other words: Extensions must
 * provide exactly one file matching this pattern, and that file must
 * contain root class/function of the extension.
 *
 * Scripted extensions with the root class inlined in the XML file are
 * not affected by this restriction.
 *
 * Returns QFileInfo() if no such file was found.
 */
QFileInfo
Configuration::extensionRootFile(const QString &id,
				 const VersionNumber &apiVersion) const
{
    QFileInfoList candidateFiles;
    QString pattern = id + QString::number(apiVersion.majorVersion()) + ".*";

    // Check the user extension repository first.
    QDir base = PlatformDependent::p()->extensionsDir(PlatformDependent::User);
    base.cd(id);
    candidateFiles = base.entryInfoList(QStringList(pattern),
                                        QDir::Files | QDir::Readable);

    // If there were no such files in the user extension repository,
    // check the system repository.
    if (candidateFiles.isEmpty())
    {
        base = PlatformDependent::p()->extensionsDir(PlatformDependent::System);
        base.cd(id);
        candidateFiles = base.entryInfoList(QStringList(pattern),
                                            QDir::Files | QDir::Readable);
    }

    if (candidateFiles.isEmpty())
        return QFileInfo();
    else
        return candidateFiles.first();
}

QDir
Configuration::getDefaultFileDialogPath() const
{
    return this->_value("defaultFileDialogPath",
			this->_defaults.fileDialogPath).toString();
}

void
Configuration::setDefaultFileDialogPath(const QString &path)
{
    this->_setValue("defaultFileDialogPath", path);
    qDebug() << "Configuration::setDefaultFileDialogPath():"
	     << "path is now" << path;
}


void
Configuration::_setValue(const QString &key, const QVariant &value)
{
    if (! this->_writeBlock)
    {
        this->_settings.data()->setValue(key, value);
    }
}

QVariant
Configuration::_value(const QString &key,
                      const QVariant &defaultValue) const
{
    return this->_settings.data()->value(key, defaultValue);
}
