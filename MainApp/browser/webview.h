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

#ifndef BROWSER_WEBVIEW_H
#define BROWSER_WEBVIEW_H

#include <QWebView>

#include "main.h"

class WebView : public QWebView
{
    Q_OBJECT
public:
    explicit
    WebView(QWidget *parent = 0);

    ~WebView();

    QIcon
    icon() const;

    bool
    pluginsEnabled();

    bool
    javaEnabled();

signals:

public slots:
    void
    setPluginsEnabled(bool enabled);

    void
    setJavaEnabled(bool enabled);

private:
    void
    _readConfiguration();

    void
    _writeConfiguration();

    QVariant
    _setting(QString key);

    void
    _setSetting(QString key, QVariant value);

    QMap<QString,QVariant> _configuration;

};

#endif // BROWSER_WEBVIEW_H
