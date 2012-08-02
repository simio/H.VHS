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

#ifndef CONFIG_CONFIGURATION_DEFAULTS_H
#define CONFIG_CONFIGURATION_DEFAULTS_H

#include <QString>

#include "config/configuration.h"

// This file contains default values used for settings in Configuration.
// All members are protected or private, and Configuration is its only friend.

class ConfigurationDefaults {
    friend class Configuration;

protected:
    QString searchQuery;        // Prepended to search terms to get a (complete) web search url. Encode in UTF-8 w/o url encoding.
    bool hideDevelInfo;         // Applies when #ifndef HIDE_DEVEL_INFO
    QUrl startPage;             // See issue #24

    ConfigurationDefaults() {
        this->searchQuery = QString("https://www.startpage.com/do/search?q=");
        this->hideDevelInfo = true;
        this->startPage = QUrl("about:blank");
    }

    ~ConfigurationDefaults()
    { }
};

#endif // CONFIG_CONFIGURATION_DEFAULTS_H
