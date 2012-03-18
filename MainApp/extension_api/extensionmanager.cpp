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
    qDebug() << this->_loadAndMergeTransportDefinitions() << "transport definitions loaded.";
    qDebug() << this->_loadAndMergeFormatDefinitions() << "format definitions loaded.";
    qDebug() << this->_loadAndMergeExtensions() << "extensions loaded.";
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

int ExtensionManager::_loadAndMergeTransportDefinitions()
{
    QList<QFileInfo> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemTransportDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserTransportDefinitionStorageLocation);

    QFileInfo location;
    foreach (location, locations)
    {
        QString path = location.canonicalFilePath();
        QPointer<QFile> file = new QFile(path);
        if (file->exists())
        {
            VhsXml::Reader transportFile(file);
            QList<TransportDefinition> newTransports = transportFile.getTransports();
            TransportDefinition newTransport;
            foreach(newTransport, newTransports)
            {
                QByteArray id = newTransport.id();
                if (! this->_transports.contains(id))
                {
                    // Transport is not yet known, so add it
                    qDebug() << "Found previously unknown transport definition:" << id << "(adding)";
                    this->_transports.insert(id, newTransport);
                }
                else if (this->_transports.value(id).releaseDate() > newTransport.releaseDate())
                {
                    // A newer definition is already known, so delete the one we just got.
                    qDebug() << "Found older definition of already known transport:" << id << "(deleting)";
                    //delete newTransport;
                }
                else
                {
                    // An older definition is known. Delete the old one and replace it with the new one.
                    qDebug() << "Found newer definition of already known transport:" << id << "(replacing)";
                    //delete this->_transports.take(uid);
                    this->_transports.insert(id, newTransport);
                }
            }
        }
        delete file;
    }

    return this->_transports.size();
}

int ExtensionManager::_loadAndMergeFormatDefinitions()
{
    QList<QFileInfo> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemFormatDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserFormatDefinitionStorageLocation);

    QFileInfo location;
    foreach (location, locations)
    {
        QString path = location.canonicalFilePath();
        QPointer<QFile> file = new QFile(path);
        if (file->exists())
        {
            VhsXml::Reader formatsFile(file);
            QList<FormatDefinition> newFormats = formatsFile.getFormats();
            FormatDefinition newFormat;
            foreach(newFormat, newFormats)
            {
                QByteArray id = newFormat.id();
                if (! this->_formats.contains(id))
                {
                    // Format is not yet known, so add it
                    qDebug() << "Found previously unknown format definition:" << id << "(adding)";
                    this->_formats.insert(id, newFormat);
                }
                else if (this->_formats.value(id).releaseDate() > newFormat.releaseDate())
                {
                    // A newer definition is already known, so delete the one we just got.
                    qDebug() << "Found older definition of already known format:" << id << "(deleting)";
                    //delete newFormat;
                }
                else
                {
                    // An older definition is known. Delete the old one and replace it with the new one.
                    qDebug() << "Found newer definition of already known format:" << id << "(replacing)";
                    //delete this->_formats.take(uid);
                    this->_formats.insert(id, newFormat);
                }
            }
        }
        delete file;
    }

    // Skriv här ned samtliga definitioner till
    // Configuration::pointer()->getStorageLocation(Configuration::UserFormatDefinitionStorageLocation)

    return this->_formats.size();
}

int ExtensionManager::_loadAndMergeExtensions()
{
    QList<QFileInfo> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemExtensionsStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserExtensionsStorageLocation);

    QFileInfo location;
    foreach (location, locations)
    {
        QString path = location.canonicalFilePath();
        // Nya extensionsdefinitioner med redan registrerade uid ska ersätta de föregående.
        if (QFileInfo(path).isDir())
        {
            QDirIterator dir(path);
            while (dir.hasNext())
            {
                dir.next();
                if (dir.fileName() != ".." && dir.fileName() != "." && QFileInfo(dir.filePath()).isDir())
                {
                    QFileInfo fileInfo(QDir::toNativeSeparators(dir.filePath() + "/" + dir.fileName() + ".xml"));
                    if (fileInfo.isFile())
                    {
                        QPointer<QFile> file = new QFile(fileInfo.canonicalFilePath());
                        VhsXml::Reader extensionsFile(file);
                        QList<Extension> newExtensions = extensionsFile.getExtensions();
                        Extension newExtension;
                        foreach(newExtension, newExtensions)
                        {
                            QByteArray id = newExtension.id();
                            if (! this->_extensions.contains(id))
                            {
                                // Extension is not yet known, so add it
                                qDebug() << "Found previously unknown extension:" << id << "(adding)";
                                this->_extensions.insert(id, newExtension);
                            }
                            else if (this->_extensions.value(id).releaseDate() > newExtension.releaseDate())
                            {
                                // A newer version of this extension is already known, so delete the one we just got.
                                qDebug() << "Found older version of already known extension:" << id << "(deleting)";
                                //delete newExtension;
                            }
                            else
                            {
                                // An older definition is known. Delete the old one and replace it with the new one.
                                qDebug() << "Found newer version of already known extension:" << id << "(replacing)";
                                //delete this->_extensions.take(uid);
                                this->_extensions.insert(id, newExtension);
                            }
                        }
                        delete file;
                    }
                }
            }
        }
    }

    return this->_extensions.size();
}
