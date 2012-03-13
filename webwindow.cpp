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

WebWindow::WebWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebWindow)
{
    ui->setupUi(this);

    this->_browserStatus = Idle;

    this->_setupGui();

    this->_loadPage(Configuration::pointer()->getStartPage());
}

WebWindow::~WebWindow()
{
    delete ui;
}

void WebWindow::_loadPage(const QUrl &url)
{
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
    Configuration::pointer()->setWriteBlock(true);
    Configuration::pointer()->clearStorage();
    this->_quit();
}

void WebWindow::_whenWebViewLoadFinished(bool ok)
{
    this->_browserStatus = Idle;
    this->_browserProgressBar->setEnabled(false);
    this->_browserProgressBar->setValue(0);
}

void WebWindow::_whenWebViewLoadStarted()
{
    this->_browserStatus = Busy;
    this->_browserProgressBar->setEnabled(true);
}

void WebWindow::_whenWebViewUrlChanged(const QUrl &url)
{
    this->_comboBoxAddressBar->addUrl(this->_webView->icon(), url);
    this->_comboBoxAddressBar->clearEditText();
    this->_comboBoxAddressBar->setCurrentIndex(0);
}

void WebWindow::_whenWebViewIconChanged()
{
    this->_updateBrowserIcon(0);
}

void WebWindow::_whenWebViewTitleChanged(const QString &title)
{
    this->setWindowTitle(title + (title.isEmpty() ? "" : " - ") + Configuration::pointer()->fullAppName());
}

void WebWindow::_whenSearchBoxReturnPressed()
{
    this->_loadPage(Configuration::pointer()->makeSearchUrl(this->_lineEditSearch->text()));
}

void WebWindow::_receiveStatusBarMessage(const QString &text)
{
    ui->statusBar->showMessage(text);
}

void WebWindow::_receiveBrowserProgress(int progress)
{
    this->_browserProgressBar->setValue(progress);
}

void WebWindow::_updateBrowserIcon(int index, bool force)
{
    if (force || this->_comboBoxAddressBar->itemIcon(index).isNull())
        this->_comboBoxAddressBar->setItemIcon(index, this->_webView->settings()->iconForUrl(QUrl(this->_comboBoxAddressBar->itemText(index))));
}

void WebWindow::closeEvent(QCloseEvent *event)
{
    Configuration::pointer()->saveWindow(Configuration::WebWindow, this->saveState(), this->saveGeometry());
    QMainWindow::closeEvent(event);
}
