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

#ifndef WEBWINDOW_H
#define WEBWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QProgressBar>
#include <QToolBar>
#include <QComboBox>
#include <QWebView>

#include "main.h"

#include "webbrowsercombobox.h"
#include "webview.h"

namespace Ui {
class WebWindow;
}

class WebWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WebWindow(QWidget *parent = 0);
    ~WebWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void _loadPage(const QUrl &url);
    void _loadPage(const QString &dirtyUrl);

    void _quit();
    void _clearStorageAndQuit();

    void _whenWebViewLoadStarted();
    void _whenWebViewLoadFinished(bool ok);
    void _whenWebViewUrlChanged(const QUrl &url);
    void _whenWebViewIconChanged();
    void _whenWebViewTitleChanged(const QString &title);
    void _whenSearchBoxReturnPressed();

    void _receiveStatusBarMessage(const QString &text);
    void _receiveBrowserProgress(int progress);

private:
    Ui::WebWindow *ui;

    void _setupWebView();
    void _setupGui();
    void _updateBrowserIcon(int index, bool force = false);

    enum BrowserStatus { Idle, Busy };
    BrowserStatus _browserStatus;

    QPointer<WebView> _webView;

    QPointer<QToolBar> _toolBarBrowser;
    QPointer<QAction> _actionBrowseHome;
    QPointer<QAction> _actionBrowseBack;
    QPointer<QAction> _actionBrowseForward;
    QPointer<QAction> _actionBrowseReload;
    QPointer<QAction> _actionBrowseStop;

    QPointer<WebBrowserComboBox> _comboBoxAddressBar;
    QPointer<QComboBox> _comboBoxQuickPicker;
    QPointer<QLineEdit> _lineEditSearch;
    QPointer<QProgressBar> _browserProgressBar;

    // File menu
    QPointer<QMenu> _menuFile;
    QPointer<QAction> _actionQuit;

    // Settings menu
    QPointer<QMenu> _menuSettings;
    QPointer<QAction> _actionToggleWebViewPlugins;
    QPointer<QAction> _actionToggleWebViewJava;

    // Advanced menu
    QPointer<QMenu> _menuAdvanced;
    QPointer<QAction> _actionClearStorageAndQuit;

    // Info menu
    QPointer<QMenu> _menuInfo;
    QPointer<QAction> _actionAbout;


};

#endif // WEBWINDOW_H
