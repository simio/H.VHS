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

#ifndef CONFIG_CONFIGURATION_H
#define CONFIG_CONFIGURATION_H

#include <QObject>
#include <QtGui/QApplication>
#include <QWeakPointer>
#include <QSettings>
#include <QDebug>
#include <QRegExp>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QMap>
#include <QStringList>

// May include neither "main.h" nor any files included in "main.h"
// below this file.

#include "version.h"
#include "config/platformdependent.h"
#include "config/messagehandler.h"
#include "appwideclasses/versionnumber.h"

#include "config/configurationdefaults.h"

class Configuration : public QObject
{
    Q_OBJECT
public:
    explicit
    Configuration(QObject *parent = 0);
    static Configuration *
    p();

    void
    setWriteBlock(bool blocked);

    bool
    clearStorage();

    enum Window {
        WebWindow,
        ConsoleWindow
    };

    enum StorageLocation {
        FaviconStorageLocation,
        SystemPresetsLocation,
        UserPresetsLocation,
        SystemExtensionsLocation,
        UserExtensionsLocation
    };

    enum BrowserStartPage {
        CompiledDefaultStartPage,
        UserDefaultStartPage,
        OneShotStartPage
    };

    QString
    locale(bool full = false) const;

    // App and version info
    QString
    appName(bool full = false) const;

    QString
    appVersion() const;

    QString
    fullAppName(bool fullVersion = false);

    // GUI states
    void
    saveWindow(const Window &window,
               const QByteArray &state,
               const QByteArray &geometry);

    QByteArray
    getWindowState(const Window &window) const;

    QByteArray
    getWindowGeometry(const Window &window) const;

    void
    saveWebViewSettings(QMap<QString,QVariant> settings);

    QMap<QString,QVariant>
    getWebViewSettings() const;

    // Browser stuff
    QUrl
    makeSearchUrl(const QString &query) const;

    QString
    getStartPage(const BrowserStartPage &page) const;

    void
    setStartPage(const BrowserStartPage &page,
                 const QString &page);

    // Filesystem things
    QDir
    getStorageLocation(const StorageLocation &type) const;

    QFileInfo
    extensionRootFile(const QString &id,
                      const VersionNumber &apiVersion) const;

    QDir
    getDefaultFileDialogPath() const;

    void
    setDefaultFileDialogPath(const QString &path);

    void
    setSessionFileDialogPath(const QString &path);

public slots:

signals:

private:
    static Configuration * s_instance;

    ConfigurationDefaults _defaults;

    void
    _setValue(const QString &key, const QVariant &value);

    QVariant
    _value(const QString &key,
           const QVariant &defaultValue = QVariant()) const;
    QString
    _wrapPublicAppTag(QString prepend = QString(),
                      QString append = QString()) const;
    QString
    _wrapGitInfo(QString prepend = QString(),
                 QString append = QString()) const;

    bool _writeBlock;
    bool _hideDevelInfo;
    QString _publicAppTag;
    QString _gitHash;
    QString _gitTag;
    QString _gitBranch;

    QWeakPointer<QSettings> _settings;

    QString _sessionFileDialogPath;
};

#endif // CONFIG_CONFIGURATION_H
