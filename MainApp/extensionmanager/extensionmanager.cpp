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

#include "extensionmanager.h"

ExtensionManager *ExtensionManager::s_instance = NULL;

ExtensionManager::ExtensionManager(QObject *parent) :
    QObject(parent)
{
    this->_initialise();
}

ExtensionManager *ExtensionManager::pointer()
{
    if (s_instance == NULL)
        s_instance = new ExtensionManager;                              // alloc: Singleton object

    return s_instance;
}

void ExtensionManager::_initialise()
{
    QStringList locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemPresetsLocation).canonicalPath()
              << Configuration::pointer()->getStorageLocation(Configuration::UserPresetsLocation).canonicalPath();

    QDir base = Configuration::pointer()->getStorageLocation(Configuration::SystemExtensionsLocation);
    foreach(QString folder, base.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        locations << base.canonicalPath() + "/" + folder;

    base = Configuration::pointer()->getStorageLocation(Configuration::UserExtensionsLocation);
    foreach(QString folder, base.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        locations << base.canonicalPath() + "/" + folder;

    QFileInfoList files;
    foreach (QString dir, locations)
        files.append(QDir(dir).entryInfoList(QStringList("*.xml"), QDir::Files | QDir::Readable));

    foreach (QFileInfo file, files)
    {
        // alloc: Has parent and is deleted here.
        QPointer<VhsXml::DocumentReader> xml = new VhsXml::DocumentReader(QFile(file.canonicalFilePath()), this);
        this->_definitions.update(xml->definitions(Definition::NoDefinitionType, &(this->_definitions)));
        delete xml;
    }
}