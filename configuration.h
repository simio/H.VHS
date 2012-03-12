/*
 * Copyright (c) 2012 Jesper R�fteg�rd <jesper@huggpunkt.org>
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QtGui/QApplication>
#include <QPointer>
#include <QSettings>
#include <QDebug>
#include <QRegExp>
#include <QDesktopServices>
#include <QUrl>

class Configuration
{
public:
    Configuration();

    enum Window {
        WebWindow,
        ConsoleWindow
    };

    enum StorageLocation {
        FaviconStorageLocation
    };

    QString appName();
    QString appVersion(bool full = false);
    QString fullAppName(bool fullVersion = false);

    void saveWindow(Window window, QByteArray state, QByteArray geometry);
    QByteArray getWindowState(Window window);
    QByteArray getWindowGeometry(Window window);

    QUrl getStartPage();
    QUrl makeSearchUrl(QString query);

    QString getStorageLocation(StorageLocation location);

private:
    QPointer<QSettings> _settings;
    QVariant _getValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void _setValue(const QString &key, const QVariant &value);};

static Configuration config;

#endif // CONFIGURATION_H
