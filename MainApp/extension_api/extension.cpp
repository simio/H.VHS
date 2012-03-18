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

#include "extension.h"

Extension::Extension(QObject *parent) :
    Definition(parent)
{
    this->_basePath = QString();
}

Extension::Extension(const Extension &original, QObject *parent) :
    Definition(original, parent),
    _basePath(original._basePath),
    _info(original._info),
    _readTransports(original._readTransports),
    _readFormats(original._readFormats),
    _writeTransports(original._writeTransports),
    _writeFormats(original._writeFormats)
{ }

Extension &Extension::operator=(const Extension &original)
{
    this->_basePath = original._basePath;
    this->_info = original._info;
    this->_readTransports = original._readTransports;
    this->_readFormats = original._readFormats;
    this->_writeTransports = original._writeTransports;
    this->_writeFormats = original._writeFormats;
    return *this;
}

bool Extension::_isReady() const
{
    // Fake it
    return true;
}

bool Extension::_setup()
{
    if (this->_isReady())
        return true;

    if (this->info("implementation").toString().toLower() == "dll")
    {
        qDebug() << "Found DLL extension:" << this->info("name");
    }

    return true;
}
