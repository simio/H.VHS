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

#ifndef WEBWINDOW_H
#define WEBWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QProgressBar>
#include <QToolBar>
#include <QComboBox>
#include <QWebView>
#include <QShortcut>
#include <QWeakPointer>

#include "main.h"

#include "browser/webbrowsercombobox.h"
#include "browser/webview.h"

#include "extensionmanager/extensionmanager.h"

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

    void _setFocusOnAddressBar();
    void _setFocusOnSearchBox();

    void _launchConsoleWindow();

private:
    Ui::WebWindow *ui;

    void _setupGui();                                           // Located in webwindowguisetup.cpp
    void _setupWebView();
    void _updateBrowserIcon(const int &index, const bool &force = false);

    enum BrowserStatus {
        Idle,
        Busy
    };
    BrowserStatus _browserStatus;

    // All UI elements are parented
    QWeakPointer<WebView> _webView;

    QWeakPointer<QToolBar> _toolBarBrowser;
    QWeakPointer<QAction> _actionBrowseHome;
    QWeakPointer<QAction> _actionBrowseBack;
    QWeakPointer<QAction> _actionBrowseForward;
    QWeakPointer<QAction> _actionBrowseReload;
    QWeakPointer<QAction> _actionBrowseReloadAndBypassCache;
    QWeakPointer<QAction> _actionBrowseStop;

    QWeakPointer<WebBrowserComboBox> _comboBoxAddressBar;
    QWeakPointer<QComboBox> _comboBoxQuickPicker;
    QWeakPointer<QLineEdit> _lineEditSearch;
    QWeakPointer<QProgressBar> _browserProgressBar;

    QWeakPointer<QAction> _actionFocusAddressBar;
    QWeakPointer<QAction> _actionFocusSearchBox;

    // File menu
    QWeakPointer<QMenu> _menuFile;
    QWeakPointer<QAction> _actionQuit;

    // Settings menu
    QWeakPointer<QMenu> _menuSettings;
    QWeakPointer<QAction> _actionToggleWebViewPlugins;
    QWeakPointer<QAction> _actionToggleWebViewJava;

    // Advanced menu
    QWeakPointer<QMenu> _menuAdvanced;
    QWeakPointer<QAction> _actionClearStorageAndQuit;
    QWeakPointer<QAction> _actionLaunchConsoleWindow;

    // Info menu
    QWeakPointer<QMenu> _menuInfo;
    QWeakPointer<QAction> _actionAbout;

};

#endif // WEBWINDOW_H
