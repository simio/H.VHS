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

#ifndef CONFIG_CONFIGURATIONDEFAULTS_H
#define CONFIG_CONFIGURATIONDEFAULTS_H

#include <QString>
#include <QDir>
#include <QStandardPaths>

#include "config/configuration.h"

// This file contains default values used for settings in
// Configuration.  All members are protected or private, and
// Configuration is its only friend.

class ConfigurationDefaults {
    friend class Configuration;

protected:
    // Prepended to search terms to get a (complete) web search
    // url. Encode in UTF-8 w/o url encoding.
    QString searchQuery;
    // Applies when #ifndef HIDE_DEVEL_INFO
    bool hideDevelInfo;
    // "Factory default" web browser start page
    QString startPage;
    // "Factory default" save/open file dialog path
    QString fileDialogPath;

    ConfigurationDefaults() {
        this->searchQuery = QString("https://www.startpage.com/do/search?q=");
        this->hideDevelInfo = true;
        this->startPage = QString("about:blank");
	this->fileDialogPath = QDir::rootPath();

	// Use the first existing path for fileDialogPath
	QStringList candidates;
	candidates
        << QStandardPaths::writableLocation(
        QStandardPaths::DownloadLocation)
        << QStandardPaths::writableLocation(
        QStandardPaths::MoviesLocation)
        << QStandardPaths::writableLocation(
        QStandardPaths::DesktopLocation)
        << QStandardPaths::writableLocation(
        QStandardPaths::HomeLocation);

	foreach (QString cand, candidates)
	    if (QDir(cand).exists())
	    {
		this->fileDialogPath = cand;
		break;
	    }

	qDebug() << "ConfigurationDefaults(): fileDialogPath factory default is"
		 << this->fileDialogPath;
    }

    ~ConfigurationDefaults()
    { }
};

#endif // CONFIG_CONFIGURATIONDEFAULTS_H
