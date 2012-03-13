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

#include "extensionmanager.h"

ExtensionManager *ExtensionManager::s_instance = NULL;

ExtensionManager::ExtensionManager(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Found" << this->_loadMediaDefinitions() << "media definitions.";
    qDebug() << "Found" << this->_loadFormatDefinitions() << "format definitions.";
    qDebug() << "Found" << this->_loadExtensions() << "extensions.";
}

ExtensionManager *ExtensionManager::pointer()
{
    if (s_instance == NULL)
        s_instance = new ExtensionManager;

    return s_instance;
}

int ExtensionManager::count()
{
    return this->_extensions.count();
}

int ExtensionManager::_loadMediaDefinitions()
{
    QList<QString> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemMediaDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserMediaDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::RemoteMediaDefinitionStorageLocation);

    QString path;
    foreach (path, locations)
    {
        // Nya mediadefinitioner med redan registrerade uid ska ersätta de föregående.
        if (path.startsWith("http://") || path.startsWith("https://"))
            qDebug() << "Loading media definitions from URL:" << path;
        else
            qDebug() << "Loading media definitions from:" << path;
    }

    // Skriv här ned samtliga definitioner till
    // Configuration::pointer()->getStorageLocation(Configuration::UserMediaDefinitionStorageLocation)

    return 0;
}

int ExtensionManager::_loadFormatDefinitions()
{
    QList<QString> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemFormatDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserFormatDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::RemoteMediaDefinitionStorageLocation);

    QString path;
    foreach (path, locations)
    {
        // Nya formatdefinitioner med redan registrerade uid ska ersätta de föregående.
        if (path.startsWith("http://") || path.startsWith("https://"))
            qDebug() << "Loading format definitions from URL:" << path;
        else
            qDebug() << "Loading format definitions from:" << path;
    }

    // Skriv här ned samtliga definitioner till
    // Configuration::pointer()->getStorageLocation(Configuration::UserFormatDefinitionStorageLocation)

    return 0;
}


int ExtensionManager::_loadExtensions()
{
    QList<QString> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemExtensionsStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserExtensionsStorageLocation);

    QString path;
    foreach (path, locations)
    {
        qDebug() << "Loading extensions from" << path;
    }

    return 0;
}
