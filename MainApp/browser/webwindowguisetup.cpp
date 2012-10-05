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

#include "webwindow.h"
#include "ui_webwindow.h"

void
WebWindow::_setupGui()
{
    this->setMinimumSize(300, 100);

    // Create, configure and add webView
    // alloc: Has parent
    this->_webView = QWeakPointer<WebView>(new WebView(this));
    this->_webView.data()->settings()->setIconDatabasePath(
        Configuration::p()->getStorageLocation(
            Configuration::FaviconStorageLocation).canonicalPath());
    ui->horizontalLayoutMiddle->addWidget(this->_webView.data());

    // Create, configure and add browser toolbar
    // alloc: Has parent
    this->_toolBarBrowser =
        QWeakPointer<QToolBar>(new QToolBar(tr("Browser Navigation"), this));
    this->_toolBarBrowser.data()->layout()->setSpacing(3);
    this->_toolBarBrowser.data()->layout()->setObjectName("BrowserToolBarLayout");
    this->_toolBarBrowser.data()->setObjectName("BrowserToolBar");
    this->_toolBarBrowser.data()->setIconSize(QSize(24, 24));
    this->addToolBar(this->_toolBarBrowser.data());

    // Create browser toolbar actions
    this->_actionBrowseBack =
        QWeakPointer<QAction>(
            this->_webView.data()->pageAction(QWebPage::Back));
    this->_actionBrowseForward =
        QWeakPointer<QAction>(this->_webView.data()->pageAction(QWebPage::Forward));
    this->_actionBrowseReload =
        QWeakPointer<QAction>(this->_webView.data()->pageAction(QWebPage::Reload));
    this->_actionBrowseReloadAndBypassCache =
        QWeakPointer<QAction>(this->_webView.data()->pageAction(QWebPage::ReloadAndBypassCache));
    this->_actionBrowseStop =
        QWeakPointer<QAction>(this->_webView.data()->pageAction(QWebPage::Stop));
    // alloc: Has parent
    this->_actionBrowseHome = QWeakPointer<QAction>(new QAction(tr("Go Home"), this));

    // alloc: Has parent
    this->_actionFocusAddressBar =
        QWeakPointer<QAction>(new QAction(tr("Set Focus on Address Bar"), this));
    // alloc: Has parent
    this->_actionFocusSearchBox =
        QWeakPointer<QAction>(new QAction(tr("Set Focus on Search Box"), this));

    // ...and widgets
    // alloc: Has parent
    this->_comboBoxAddressBar =
        QWeakPointer<WebBrowserComboBox>(new WebBrowserComboBox(this));
    // alloc: Has parent
    this->_comboBoxQuickPicker = QWeakPointer<QComboBox>(new QComboBox(this));
    // alloc: Has parent
    this->_lineEditSearch = QWeakPointer<QLineEdit>(new QLineEdit(this));
    // alloc: Has parent
    this->_browserProgressBar =
        QWeakPointer<QProgressBar>(new QProgressBar(this));

    // Configure actions
    this->_actionBrowseBack.data()->setIcon(QIcon(":/icons/browseBack"));
    this->_actionBrowseBack.data()->setShortcuts(QKeySequence::Back);
    this->_actionBrowseForward.data()->setIcon(QIcon(":/icons/browseForward"));
    this->_actionBrowseForward.data()->setShortcuts(QKeySequence::Forward);
    this->_actionBrowseReload.data()->setIcon(QIcon(":/icons/browseReload"));
    QList<QKeySequence> actionBrowseReloadShortcuts;
    actionBrowseReloadShortcuts << QKeySequence(tr("F5"))
                                << QKeySequence(tr("Ctrl+R"));
    this->_actionBrowseReload.data()->setShortcuts(QKeySequence::Refresh);
    QList<QKeySequence> actionBrowseReloadAndBypassCacheShortcuts;
    actionBrowseReloadAndBypassCacheShortcuts
        << QKeySequence(tr("Shift+F5"))
        << QKeySequence(tr("Ctrl+Shift+R"));
    // Add this only if setting it above was successful
    if (! this->_actionBrowseReloadAndBypassCache.isNull())
    {
        this->_actionBrowseReloadAndBypassCache.data()->setShortcuts(
            actionBrowseReloadAndBypassCacheShortcuts);
        qDebug() << "WebWindow::_setupGui(): ReloadAndBypassCache is enabled.";
    }
    else
        qDebug() << "WebWindow::_setupGui(): ReloadAndBypassCache is disabled.";
    this->_actionBrowseStop.data()->setIcon(QIcon(":/icons/browseStop"));
    this->_actionBrowseStop.data()->setShortcut(QKeySequence(Qt::Key_Escape));
    this->_actionBrowseHome.data()->setIcon(QIcon(":/icons/browseHome"));
    this->_actionBrowseHome.data()->setShortcut(QKeySequence(tr("Ctrl+H")));

    QList<QKeySequence> actionFocusAddressBarShortcuts;
    actionFocusAddressBarShortcuts << QKeySequence(tr("F8"))
                                   << QKeySequence(tr("Ctrl+L"))
                                   << QKeySequence(tr("Alt+D"));
    this->_actionFocusAddressBar.data()->setShortcuts(actionFocusAddressBarShortcuts);
    this->_actionFocusSearchBox.data()->setShortcut(QKeySequence(tr("Ctrl+E")));

    // Configure widgets
    this->_comboBoxAddressBar.data()->setSizePolicy(QSizePolicy::Expanding,
        this->_comboBoxAddressBar.data()->sizePolicy().verticalPolicy());
    this->_comboBoxAddressBar.data()->setItemIcon(0, this->_webView.data()->icon());
    this->_lineEditSearch.data()->setSizePolicy(
        QSizePolicy::Minimum,
        this->_lineEditSearch.data()->sizePolicy().verticalPolicy());
    this->_lineEditSearch.data()->setMinimumWidth(200);
    this->_lineEditSearch.data()->setPlaceholderText(tr("Search..."));
    this->_browserProgressBar.data()->setTextVisible(false);
    this->_browserProgressBar.data()->setMaximumWidth(80);
    this->_browserProgressBar.data()->setMaximumHeight(20);

    // Connect actions and widgets
    connect(this->_comboBoxAddressBar.data(), SIGNAL(activated(const QString&)),
            this, SLOT(_loadPage(const QString&)));
    connect(this->_webView.data(), SIGNAL(loadProgress(int)),
            this, SLOT(_receiveBrowserProgress(int)));
    connect(this->_webView.data(), SIGNAL(loadStarted()),
            this, SLOT(_whenWebViewLoadStarted()));
    connect(this->_webView.data(), SIGNAL(loadFinished(bool)),
            this, SLOT(_whenWebViewLoadFinished(bool)));
    connect(this->_webView.data(), SIGNAL(iconChanged()),
            this, SLOT(_whenWebViewIconChanged()));
    connect(this->_webView.data(), SIGNAL(urlChanged(const QUrl &)),
            this, SLOT(_whenWebViewUrlChanged(const QUrl&)));
    connect(this->_webView.data(), SIGNAL(statusBarMessage(const QString &)),
            this, SLOT(_receiveStatusBarMessage(QString)));
    connect(this->_webView.data(), SIGNAL(titleChanged(const QString &)),
            this, SLOT(_whenWebViewTitleChanged(QString)));
    connect(this->_lineEditSearch.data(), SIGNAL(returnPressed()),
            this, SLOT(_whenSearchBoxReturnPressed()));
    connect(this->_actionFocusAddressBar.data(), SIGNAL(triggered()),
            this, SLOT(_setFocusOnAddressBar()));
    connect(this->_actionFocusSearchBox.data(), SIGNAL(triggered()),
            this, SLOT(_setFocusOnSearchBox()));

    // Add actions and widgets to browser toolbar
    this->_toolBarBrowser.data()->addAction(this->_actionBrowseBack.data());
    this->_toolBarBrowser.data()->addAction(this->_actionBrowseForward.data());
    this->_toolBarBrowser.data()->addAction(this->_actionBrowseReload.data());
    this->_toolBarBrowser.data()->addAction(this->_actionBrowseStop.data());
    this->_toolBarBrowser.data()->addAction(this->_actionBrowseHome.data());
    this->_toolBarBrowser.data()->addWidget(this->_comboBoxAddressBar.data());
    this->_toolBarBrowser.data()->addWidget(this->_comboBoxQuickPicker.data());
    this->_toolBarBrowser.data()->addWidget(this->_lineEditSearch.data());
    this->_toolBarBrowser.data()->addWidget(this->_browserProgressBar.data());

    this->addAction(this->_actionFocusAddressBar.data());
    this->addAction(this->_actionFocusSearchBox.data());

    // Restore window state and geometry, or set default geometry if
    // no previous settings exist
    if (! Configuration::p()->getWindowState(Configuration::WebWindow).isEmpty())
        this->restoreState(Configuration::p()->getWindowState(Configuration::WebWindow));
    if (! Configuration::p()->getWindowGeometry(Configuration::WebWindow).isEmpty())
        this->restoreGeometry(Configuration::p()->getWindowGeometry(Configuration::WebWindow));
    else
    {
        int defaultWidth =
            ((QApplication::desktop()->availableGeometry().width() >= 1024)
             ? 1024
             : QApplication::desktop()->availableGeometry().width());
        int defaultHeight =
            ((QApplication::desktop()->availableGeometry().height() >= 740)
             ? 740
             : QApplication::desktop()->availableGeometry().height());
        this->setGeometry(30, 50, defaultWidth, defaultHeight);
    }

    // Create top menus
    // alloc: Has parent
    this->_menuFile = QWeakPointer<QMenu>(new QMenu(tr("&File"), ui->menuBar));
    // alloc: Has parent
    this->_menuSettings = QWeakPointer<QMenu>(new QMenu(tr("&Settings"), ui->menuBar));
    // alloc: Has parent
    this->_menuAdvanced = QWeakPointer<QMenu>(new QMenu(tr("&Advanced"), ui->menuBar));
    // alloc: Has parent
    this->_menuInfo = QWeakPointer<QMenu>(new QMenu(tr("&Info"), ui->menuBar));

    // FILE MENU
    // += Quit
    // alloc: Has parent
    this->_actionQuit =
        QWeakPointer<QAction>(new QAction(QIcon(":/icons/exit"),
                                          tr("&Quit"),
                                          this->_menuFile.data()));
    this->_actionQuit.data()->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(this->_actionQuit.data(), SIGNAL(triggered()), this, SLOT(_quit()));
    this->_menuFile.data()->addAction(this->_actionQuit.data());

    // SETTINGS MENU
    // +- Toggle Plugins
    // alloc: Has parent
    this->_actionToggleWebViewPlugins =
        QWeakPointer<QAction>(new QAction(tr("Allow Browser &Plugins"),
                                          this->_menuSettings.data()));
    this->_actionToggleWebViewPlugins.data()->setCheckable(true);
    this->_actionToggleWebViewPlugins.data()->setChecked(this->_webView.data()->pluginsEnabled());
    this->_actionToggleWebViewPlugins.data()->setShortcut(QKeySequence(tr("Ctrl+Alt+P")));
    connect(this->_actionToggleWebViewPlugins.data(), SIGNAL(toggled(bool)),
            this->_webView.data(), SLOT(setPluginsEnabled(bool)));

    this->_menuSettings.data()->addAction(this->_actionToggleWebViewPlugins.data());
    // +- Toggle Java
    // alloc: Has parent
    this->_actionToggleWebViewJava = QWeakPointer<QAction>(new QAction(tr("Allow &Java"),
								       this->_menuSettings.data()));
    this->_actionToggleWebViewJava.data()->setCheckable(true);
    this->_actionToggleWebViewJava.data()->setChecked(this->_webView.data()->javaEnabled());
    this->_actionToggleWebViewJava.data()->setShortcut(QKeySequence(tr("Ctrl+Alt+J")));
    connect(this->_actionToggleWebViewJava.data(),
            SIGNAL(toggled(bool)),
            this->_webView.data(),
            SLOT(setJavaEnabled(bool)));
    this->_menuSettings.data()->addAction(this->_actionToggleWebViewJava.data());

    // ADVANCED MENU
    // +- Launch Console Window
    // alloc: Has parent
    this->_actionLaunchConsoleWindow =
        QWeakPointer<QAction>(new QAction(tr("Launch &Console Window"),
                                          this->_menuAdvanced.data()));
    this->_actionLaunchConsoleWindow.data()->setShortcut(QKeySequence(tr("F12")));
    connect(this->_actionLaunchConsoleWindow.data(), SIGNAL(triggered()),
            this, SLOT(_launchConsoleWindow()));
    this->_menuAdvanced.data()->addAction(this->_actionLaunchConsoleWindow.data());
    // +- Clear storage and quit
    // alloc: Has parent
    this->_actionClearStorageAndQuit =
        QWeakPointer<QAction>(new QAction(tr("Clear &Storage and Quit"),
                                          this->_menuAdvanced.data()));
    this->_actionClearStorageAndQuit.data()->setShortcut(QKeySequence(tr("Ctrl+Alt+Q")));
    connect(this->_actionClearStorageAndQuit.data(), SIGNAL(triggered()),
            this, SLOT(_clearStorageAndQuit()));
    this->_menuAdvanced.data()->addAction(this->_actionClearStorageAndQuit.data());

    // INFO MENU
    // += About
    // alloc: Has parent
    this->_actionAbout = QWeakPointer<QAction>(
        new QAction(QIcon(":/icons/bitmapVideoCassette"),
                    tr("&About")
                    + QString(" ")
                    + Configuration::p()->appName(true),
                    this->_menuSettings.data()));
    this->_menuInfo.data()->addAction(this->_actionAbout.data());

    // Add top menus to Window
    ui->menuBar->addMenu(this->_menuFile.data());
    ui->menuBar->addMenu(this->_menuSettings.data());
    ui->menuBar->addMenu(this->_menuAdvanced.data());
    ui->menuBar->addMenu(this->_menuInfo.data());
}
