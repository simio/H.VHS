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

#include "webwindow.h"
#include "ui_webwindow.h"

void WebWindow::_setupGui()
{
    this->setMinimumSize(300, 100);

    // Create, configure and add webView
    this->_webView = new WebView(this);
    this->_webView->settings()->setIconDatabasePath(Configuration::pointer()->getStorageLocation(Configuration::FaviconStorageLocation));
    ui->horizontalLayoutMiddle->addWidget(this->_webView);

    // Create, configure and add browser toolbar
    this->_toolBarBrowser = new QToolBar(tr("Browser Navigation"), this);
    this->_toolBarBrowser->layout()->setSpacing(3);
    this->_toolBarBrowser->layout()->setObjectName("BrowserToolBarLayout");
    this->_toolBarBrowser->setObjectName("BrowserToolBar");
    this->_toolBarBrowser->setIconSize(QSize(24, 24));
    this->addToolBar(this->_toolBarBrowser);

    // Create browser toolbar actions
    this->_actionBrowseBack = this->_webView->pageAction(QWebPage::Back);
    this->_actionBrowseForward = this->_webView->pageAction(QWebPage::Forward);
    this->_actionBrowseReload = this->_webView->pageAction(QWebPage::Reload);
    this->_actionBrowseReloadAndBypassCache = this->_webView->pageAction(QWebPage::ReloadAndBypassCache);
    this->_actionBrowseStop = this->_webView->pageAction(QWebPage::Stop);
    this->_actionBrowseHome = new QAction(tr("Go Home"), this);

    this->_actionFocusAddressBar = new QAction(tr("Set Focus on Address Bar"), this);
    this->_actionFocusSearchBox = new QAction(tr("Set Focus on Search Box"), this);

    // ...and widgets
    this->_comboBoxAddressBar = new WebBrowserComboBox;
    this->_comboBoxQuickPicker = new QComboBox;
    this->_lineEditSearch = new QLineEdit;
    this->_browserProgressBar = new QProgressBar;

    // Configure actions
    this->_actionBrowseBack->setIcon(QIcon(":/icons/browseBack"));
    this->_actionBrowseBack->setShortcuts(QKeySequence::Back);
    this->_actionBrowseForward->setIcon(QIcon(":/icons/browseForward"));
    this->_actionBrowseForward->setShortcuts(QKeySequence::Forward);
    this->_actionBrowseReload->setIcon(QIcon(":/icons/browseReload"));
    QList<QKeySequence> actionBrowseReloadShortcuts;
    actionBrowseReloadShortcuts << QKeySequence(tr("F5")) << QKeySequence(tr("Ctrl+R"));
    this->_actionBrowseReload->setShortcuts(QKeySequence::Refresh);
    QList<QKeySequence> actionBrowseReloadAndBypassCacheShortcuts;
    actionBrowseReloadAndBypassCacheShortcuts << QKeySequence(tr("Shift+F5")) << QKeySequence(tr("Ctrl+Shift+R"));
    if (! this->_actionBrowseReloadAndBypassCache.isNull())
    {
        this->_actionBrowseReloadAndBypassCache->setShortcuts(actionBrowseReloadAndBypassCacheShortcuts);
        qDebug() << "ReloadAndBypassCache is enabled.";
    }
    else
        qDebug() << "ReloadAndBypassCache is disabled.";
    this->_actionBrowseStop->setIcon(QIcon(":/icons/browseStop"));
    this->_actionBrowseStop->setShortcut(QKeySequence(Qt::Key_Escape));
    this->_actionBrowseHome->setIcon(QIcon(":/icons/browseHome"));
    this->_actionBrowseHome->setShortcut(QKeySequence(tr("Ctrl+H")));

    QList<QKeySequence> actionFocusAddressBarShortcuts;
    actionFocusAddressBarShortcuts << QKeySequence(tr("F8")) << QKeySequence(tr("Ctrl+L")) << QKeySequence(tr("Alt+D"));
    this->_actionFocusAddressBar->setShortcuts(actionFocusAddressBarShortcuts);
    this->_actionFocusSearchBox->setShortcut(QKeySequence(tr("Ctrl+E")));

    // Configure widgets
    this->_comboBoxAddressBar->setSizePolicy(QSizePolicy::Expanding, this->_comboBoxAddressBar->sizePolicy().verticalPolicy());
    this->_comboBoxAddressBar->setItemIcon(0, this->_webView->icon());
    this->_lineEditSearch->setSizePolicy(QSizePolicy::Minimum, this->_lineEditSearch->sizePolicy().verticalPolicy());
    this->_lineEditSearch->setMinimumWidth(200);
    this->_lineEditSearch->setPlaceholderText(tr("Search..."));
    this->_browserProgressBar->setTextVisible(false);
    this->_browserProgressBar->setMaximumWidth(80);
    this->_browserProgressBar->setMaximumHeight(20);

    // Connect actions and widgets
    connect(this->_comboBoxAddressBar,      SIGNAL(activated(const QString&)),          this, SLOT(_loadPage(const QString&)));
    connect(this->_webView,                 SIGNAL(loadProgress(int)),                  this, SLOT(_receiveBrowserProgress(int)));
    connect(this->_webView,                 SIGNAL(loadStarted()),                      this, SLOT(_whenWebViewLoadStarted()));
    connect(this->_webView,                 SIGNAL(loadFinished(bool)),                 this, SLOT(_whenWebViewLoadFinished(bool)));
    connect(this->_webView,                 SIGNAL(iconChanged()),                      this, SLOT(_whenWebViewIconChanged()));
    connect(this->_webView,                 SIGNAL(urlChanged(const QUrl &)),           this, SLOT(_whenWebViewUrlChanged(const QUrl&)));
    connect(this->_webView,                 SIGNAL(statusBarMessage(const QString &)),  this, SLOT(_receiveStatusBarMessage(QString)));
    connect(this->_webView,                 SIGNAL(titleChanged(const QString &)),      this, SLOT(_whenWebViewTitleChanged(QString)));
    connect(this->_lineEditSearch,          SIGNAL(returnPressed()),                    this, SLOT(_whenSearchBoxReturnPressed()));
    connect(this->_actionFocusAddressBar,   SIGNAL(triggered()),                        this, SLOT(_setFocusOnAddressBar()));
    connect(this->_actionFocusSearchBox,    SIGNAL(triggered()),                        this, SLOT(_setFocusOnSearchBox()));

    // Add actions and widgets to browser toolbar
    this->_toolBarBrowser->addAction(this->_actionBrowseBack);
    this->_toolBarBrowser->addAction(this->_actionBrowseForward);
    this->_toolBarBrowser->addAction(this->_actionBrowseReload);
    this->_toolBarBrowser->addAction(this->_actionBrowseStop);
    this->_toolBarBrowser->addAction(this->_actionBrowseHome);
    this->_toolBarBrowser->addWidget(this->_comboBoxAddressBar);
    this->_toolBarBrowser->addWidget(this->_comboBoxQuickPicker);
    this->_toolBarBrowser->addWidget(this->_lineEditSearch);
    this->_toolBarBrowser->addWidget(this->_browserProgressBar);

    this->addAction(this->_actionFocusAddressBar);
    this->addAction(this->_actionFocusSearchBox);

    // Restore window state and geometry, or set default geometry if no previous settings exist
    if (! Configuration::pointer()->getWindowState(Configuration::WebWindow).isEmpty())
        this->restoreState(Configuration::pointer()->getWindowState(Configuration::WebWindow));
    if (! Configuration::pointer()->getWindowGeometry(Configuration::WebWindow).isEmpty())
        this->restoreGeometry(Configuration::pointer()->getWindowGeometry(Configuration::WebWindow));
    else
    {
        int defaultWidth  = ((QApplication::desktop()->availableGeometry().width() >= 1024)  ? 1024 : QApplication::desktop()->availableGeometry().width());
        int defaultHeight = ((QApplication::desktop()->availableGeometry().height() >= 740) ? 740   : QApplication::desktop()->availableGeometry().height());
        this->setGeometry(30, 50, defaultWidth, defaultHeight);
    }

    // Create top menus
    this->_menuFile = new QMenu(tr("&File"), ui->menuBar);
    this->_menuSettings = new QMenu(tr("&Settings"), ui->menuBar);
    this->_menuAdvanced = new QMenu(tr("&Advanced"), ui->menuBar);
    this->_menuInfo = new QMenu(tr("&Info"), ui->menuBar);

    // FILE MENU
    // += Quit
    this->_actionQuit = new QAction(QIcon(":/icons/exit"), tr("&Quit"), this->_menuFile);
    this->_actionQuit->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(this->_actionQuit, SIGNAL(triggered()), this, SLOT(_quit()));
    this->_menuFile->addAction(this->_actionQuit);

    // SETTINGS MENU
    // +- Toggle Plugins
    this->_actionToggleWebViewPlugins = new QAction(tr("Allow Browser &Plugins"), this->_menuSettings);
    this->_actionToggleWebViewPlugins->setCheckable(true);
    this->_actionToggleWebViewPlugins->setChecked(this->_webView->pluginsEnabled());
    this->_actionToggleWebViewPlugins->setShortcut(QKeySequence(tr("Ctrl+Alt+P")));
    connect(this->_actionToggleWebViewPlugins, SIGNAL(toggled(bool)), this->_webView, SLOT(setPluginsEnabled(bool)));
    this->_menuSettings->addAction(this->_actionToggleWebViewPlugins);
    // +- Toggle Java
    this->_actionToggleWebViewJava = new QAction(tr("Allow &Java"), this->_menuSettings);
    this->_actionToggleWebViewJava->setCheckable(true);
    this->_actionToggleWebViewJava->setChecked(this->_webView->javaEnabled());
    this->_actionToggleWebViewJava->setShortcut(QKeySequence(tr("Ctrl+Alt+J")));
    connect(this->_actionToggleWebViewJava, SIGNAL(toggled(bool)), this->_webView, SLOT(setJavaEnabled(bool)));
    this->_menuSettings->addAction(this->_actionToggleWebViewJava);

    // ADVANCED MENU
    // += Launch Console Window
    this->_actionLaunchConsoleWindow = new QAction(tr("Launch &Console Window"), this->_menuAdvanced);
    this->_actionLaunchConsoleWindow->setShortcut(QKeySequence(tr("F12")));
    connect(this->_actionLaunchConsoleWindow, SIGNAL(triggered()), this, SLOT(_launchConsoleWindow()));
    this->_menuAdvanced->addAction(this->_actionLaunchConsoleWindow);
    // += Clear storage and quit
    this->_actionClearStorageAndQuit = new QAction(tr("Clear &Storage and Quit"), this->_menuAdvanced);
    this->_actionClearStorageAndQuit->setShortcut(QKeySequence(tr("Ctrl+Alt+Q")));
    connect(this->_actionClearStorageAndQuit, SIGNAL(triggered()), this, SLOT(_clearStorageAndQuit()));
    this->_menuAdvanced->addAction(this->_actionClearStorageAndQuit);

    // INFO MENU
    // += About
    this->_actionAbout = new QAction(QIcon(":/icons/bitmapVideoCassette"),
                                     tr("&About") + QString(" ") + Configuration::pointer()->fullAppName(),
                                     this->_menuSettings);
    this->_menuInfo->addAction(this->_actionAbout);

    // Add top menus
    ui->menuBar->addMenu(this->_menuFile);
    ui->menuBar->addMenu(this->_menuSettings);
    ui->menuBar->addMenu(this->_menuAdvanced);
    ui->menuBar->addMenu(this->_menuInfo);
}
