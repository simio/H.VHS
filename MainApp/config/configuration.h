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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QtGui/QApplication>
#include <QPointer>
#include <QSettings>
#include <QDebug>
#include <QRegExp>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QMap>
#include <QStringList>

#include "version.h"
#include "config/platformdependent.h"
#include "config/messagehandler.h"
#include "config/appwideclasses.h"

class Configuration : public QObject
{
    Q_OBJECT
public:
    explicit Configuration(QObject *parent = 0);
    static Configuration *pointer();

    void setWriteBlock(bool blocked);
    bool clearStorage();

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

    QString locale(bool full = false);

    // App and version info
    QString appName(bool full = false);
    QString appVersion();
    QString fullAppName(bool fullVersion = false);

    // GUI states
    void saveWindow(Window window, QByteArray state, QByteArray geometry);
    QByteArray getWindowState(Window window);
    QByteArray getWindowGeometry(Window window);
    void saveWebViewSettings(QMap<QString,QVariant> settings);
    QMap<QString,QVariant> getWebViewSettings();

    // Browser stuff
    QUrl getStartPage();
    QUrl makeSearchUrl(QString query);

    // Filesystem things
    QDir getStorageLocation(StorageLocation type);

public slots:

signals:

private:
    static Configuration * s_instance;

    void _setValue(const QString &key, const QVariant &value);
    QVariant _value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    QString _wrapPublicAppTag(QString prepend = QString(), QString append = QString());
    QString _wrapGitInfo(QString prepend = QString(), QString append = QString());

    bool _writeBlock;
    bool _hideDevelInfo;
    QString _publicAppTag;
    QString _gitHash;
    QString _gitTag;
    QString _gitBranch;

    QPointer<QSettings> _settings;
};

#endif // CONFIGURATION_H
