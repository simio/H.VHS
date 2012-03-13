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
    qDebug() << this->_loadMediaDefinitions() << "media definitions loaded.";
    qDebug() << this->_loadFormatDefinitions() << "format definitions loaded.";
    qDebug() << this->_loadExtensions() << "extensions loaded.";
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
        {
            //qDebug() << "Loading media definitions from URL:" << path;
        }
        else
        {
            QPointer<QFile> file = new QFile(path);
            if (file->exists())
            {
                qDebug() << "Loading media definitions from:" << path;
                VhsXml mediaFile(file);
                this->_knownMedia = mediaFile.getMediaDefinitions();
            }
            delete file;
        }
    }

    // Skriv här ned samtliga definitioner till
    // Configuration::pointer()->getStorageLocation(Configuration::UserMediaDefinitionStorageLocation)

    return this->_knownMedia.size();
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
        {
            //qDebug() << "Loading format definitions from URL:" << path;
        }
        else
        {
            QPointer<QFile> file = new QFile(path);
            if (file->exists())
            {
                qDebug() << "Loading format definitions from:" << path;
                VhsXml formatsFile(file);
                this->_knownFormats = formatsFile.getFormatDefinitions();
            }
            delete file;
        }
    }

    // Skriv här ned samtliga definitioner till
    // Configuration::pointer()->getStorageLocation(Configuration::UserFormatDefinitionStorageLocation)

    return this->_knownFormats.size();
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

    return this->_extensions.size();
}
