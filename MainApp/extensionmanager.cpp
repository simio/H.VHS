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
    qDebug() << this->_loadAndMergeMediaDefinitions() << "media definitions loaded.";
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

int ExtensionManager::_loadAndMergeMediaDefinitions()
{
    QList<QString> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemMediaDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserMediaDefinitionStorageLocation);

    QString path;
    foreach (path, locations)
    {
        QPointer<QFile> file = new QFile(path);
        if (file->exists())
        {
            qDebug() << "Reading MediaDefinitions from:" << path;
            VhsXml mediaFile(file);
            QList< QPointer<MediaDefinition> > newMedia = mediaFile.getMediaDefinitions();
            QPointer<MediaDefinition> newMedium;
            foreach(newMedium, newMedia)
            {
                QByteArray uid = newMedium->uid();
                if (! this->_media.contains(uid))
                {
                    // Medium is not yet known, so add it
                    qDebug() << "Found previously unknown medium definition:" << uid << "(adding)";
                    this->_media.insert(uid, newMedium);
                }
                else if (this->_media.value(uid)->dateTime() > newMedium->dateTime())
                {
                    // A newer definition is already known, so delete the one we just got.
                    qDebug() << "Found older definition of already known medium:" << uid << "(deleting)";
                    delete newMedium;
                }
                else
                {
                    // An older definition is known. Delete the old one and replace it with the new one.
                    qDebug() << "Found newer definition of already known medium:" << uid << "(replacing)";
                    delete this->_media.take(uid);
                    this->_media.insert(uid, newMedium);
                }
            }
        }
        delete file;
    }

    return this->_media.size();
}

int ExtensionManager::_loadAndMergeFormatDefinitions()
{
    QList<QString> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemFormatDefinitionStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserFormatDefinitionStorageLocation);

    QString path;
    foreach (path, locations)
    {
        QPointer<QFile> file = new QFile(path);
        if (file->exists())
        {
            qDebug() << "Reading FormatDefinitions from:" << path;
            VhsXml formatsFile(file);
            QList< QPointer<FormatDefinition> > newFormats = formatsFile.getFormatDefinitions();
            QPointer<FormatDefinition> newFormat;
            foreach(newFormat, newFormats)
            {
                QByteArray uid = newFormat->uid();
                if (! this->_formats.contains(uid))
                {
                    // Format is not yet known, so add it
                    qDebug() << "Found previously unknown format definition:" << uid << "(adding)";
                    this->_formats.insert(uid, newFormat);
                }
                else if (this->_formats.value(uid)->dateTime() > newFormat->dateTime())
                {
                    // A newer definition is already known, so delete the one we just got.
                    qDebug() << "Found older definition of already known format:" << uid << "(deleting)";
                    delete newFormat;
                }
                else
                {
                    // An older definition is known. Delete the old one and replace it with the new one.
                    qDebug() << "Found newer definition of already known format:" << uid << "(replacing)";
                    delete this->_formats.take(uid);
                    this->_formats.insert(uid, newFormat);
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
    QList<QString> locations;
    locations << Configuration::pointer()->getStorageLocation(Configuration::SystemExtensionsStorageLocation)
              << Configuration::pointer()->getStorageLocation(Configuration::UserExtensionsStorageLocation);

    QString path;
    foreach (path, locations)
    {
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
                        qDebug() << "Found extension definition file:" << fileInfo.absoluteFilePath();
                        QPointer<QFile> file = new QFile(fileInfo.absoluteFilePath());
                        VhsXml extensionsFile(file);
                        QList< QPointer<Extension> > newExtensions = extensionsFile.getExtensions();
                        QPointer<Extension> newExtension;
                        foreach(newExtension, newExtensions)
                        {
                            QByteArray uid = newExtension->uid();
                            if (! this->_extensions.contains(uid))
                            {
                                // Extension is not yet known, so add it
                                qDebug() << "Found previously unknown extension:" << uid << "(adding)";
                                this->_extensions.insert(uid, newExtension);
                            }
                            else if (this->_extensions.value(uid)->version() > newExtension->version())
                            {
                                // A newer version of this extension is already known, so delete the one we just got.
                                qDebug() << "Found older version of already known extension:" << uid << "(deleting)";
                                delete newExtension;
                            }
                            else
                            {
                                // An older definition is known. Delete the old one and replace it with the new one.
                                qDebug() << "Found newer version of already known extension:" << uid << "(replacing)";
                                delete this->_extensions.take(uid);
                                this->_extensions.insert(uid, newExtension);
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
