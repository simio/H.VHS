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

WebWindow::WebWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebWindow)                        // alloc: Deleted in destructor
{
    ui->setupUi(this);

    this->_browserStatus = Idle;

    // For testing -- without this, it won't initialise until used somewhere.
    ExtensionManager::p();

    this->_setupGui();

    this->_setupWebView();

    ExtensionManager::p()->callHook(EXT_HOOK_AFTER_WEBWINDOW_CONSTRUCTOR);
}

WebWindow::~WebWindow()
{
    delete ui;

    // Note: QWidget attribute Qt::WA_DeleteOnClose is set on this,
    // which should result in deletion of this after closing the window.
}

void WebWindow::_loadPage(const QUrl &url)
{
    QVariant vUrl = QVariant(url);
    ExtensionManager::p()->callHook(EXT_HOOK_WEBVIEW_LOADREQUESTED, vUrl);
    this->_webView->load(url);
    this->_webView->setFocus();
}

void WebWindow::_loadPage(const QString &dirtyUrl)
{
    if (! dirtyUrl.contains(QRegExp("^[a-z0-9-]+:")))
        this->_loadPage(QUrl(("http://" + dirtyUrl)));
    else
        this->_loadPage(QUrl(dirtyUrl));
}

void WebWindow::_quit()
{
    qApp->closeAllWindows();
    qApp->exit(0);
}

void WebWindow::_clearStorageAndQuit()
{
    Configuration::p()->setWriteBlock(true);
    Configuration::p()->clearStorage();
    this->_quit();
}

void WebWindow::_whenWebViewLoadFinished(bool ok)
{
    QVariant vOk = QVariant(ok);
    ExtensionManager::p()->callHook(EXT_HOOK_WEBVIEW_LOADFINISHED, vOk);
    this->_browserStatus = Idle;
    this->_browserProgressBar->setEnabled(false);
    this->_browserProgressBar->setValue(0);
}

void WebWindow::_whenWebViewLoadStarted()
{
    ExtensionManager::p()->callHook(EXT_HOOK_WEBVIEW_LOADSTARTED);
    this->_browserStatus = Busy;
    this->_browserProgressBar->setEnabled(true);
}

void WebWindow::_whenWebViewUrlChanged(const QUrl &url)
{
    this->_comboBoxAddressBar->addUrl(
        this->_webView->icon(), url);
    this->_comboBoxAddressBar->clearEditText();
    this->_comboBoxAddressBar->setCurrentIndex(0);
}

void WebWindow::_whenWebViewIconChanged()
{
    this->_updateBrowserIcon(0);
}

void WebWindow::_whenWebViewTitleChanged(const QString &title)
{
    this->setWindowTitle(title + (title.isEmpty() ? "" : " - ") + Configuration::p()->appName());
}

void WebWindow::_whenSearchBoxReturnPressed()
{
    this->_loadPage(Configuration::p()->makeSearchUrl(this->_lineEditSearch->text()));
}

void WebWindow::_receiveStatusBarMessage(const QString &text)
{
    ui->statusBar->showMessage(text);
}

void WebWindow::_receiveBrowserProgress(int progress)
{
    this->_browserProgressBar->setValue(progress);
}

void WebWindow::_setFocusOnAddressBar()
{
    this->_comboBoxAddressBar->setFocus();
}

void WebWindow::_setFocusOnSearchBox()
{
    this->_lineEditSearch->setFocus();
}

void WebWindow::_launchConsoleWindow()
{
    MessageHandler::p()->createConsoleWindow();
}

void WebWindow::_selectDefaultFileDialogPath()
{
    QString path = QFileDialog::getExistingDirectory(
        this,
        tr("Select Default Save/Open Folder"),
        Configuration::p()->getDefaultFileDialogPath().absolutePath());

    if (! path.isEmpty())
    {
        Configuration::p()->setDefaultFileDialogPath(path);
        Configuration::p()->setSessionFileDialogPath(path);
    }
}

void WebWindow::_setupWebView()
{
    HUrl startPage = this->_pickStartPage();
    qDebug() << "Startpage is" << startPage.toString();
    this->_loadPage(startPage.toUrl());
}

HUrl WebWindow::_pickStartPage()
{
    QStringList candidates;
    candidates << (QApplication::arguments().count() > 1
                   ? QApplication::arguments().at(1)
                   : "")
               << QApplication::clipboard()->text(QClipboard::Clipboard)
               << QApplication::clipboard()->text(QClipboard::Selection)
               << Configuration::p()->getStartPage(Configuration::OneShotStartPage)
        //XXX: Insert startpage string from http get api here (issue #64)
               << Configuration::p()->getStartPage(Configuration::UserDefaultStartPage);

    foreach(QString cand, candidates)
        if (! cand.trimmed().isEmpty() && HUrl(cand).isValid())
        {
            if (cand == Configuration::p()->getStartPage(Configuration::OneShotStartPage))
                Configuration::p()->setStartPage(Configuration::OneShotStartPage, "");
            qDebug() << "WebWindow::_pickStartPage(): picked" << cand;
            return HUrl(cand);
        }

    QString page = Configuration::p()->getStartPage(
        Configuration::CompiledDefaultStartPage);
    qDebug() << "WebWindow::pickStartPage(): picked compiled default value"
             << page;
    return HUrl(page);
}

void WebWindow::_updateBrowserIcon(const int &index, const bool &force)
{
    if (force || this->_comboBoxAddressBar->itemIcon(index).isNull())
        this->_comboBoxAddressBar->setItemIcon(
            index,
            this->_webView->settings()->iconForUrl(
                QUrl(this->_comboBoxAddressBar->itemText(index))));
}

void WebWindow::closeEvent(QCloseEvent *event)
{
    Configuration::p()->saveWindow(Configuration::WebWindow,
                                   this->saveState(),
                                   this->saveGeometry());
    QMainWindow::closeEvent(event);
}
