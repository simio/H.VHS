/*
 * Copyright (c) 2012 Jesper R�fteg�rd <jesper@huggpunkt.org>
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
    QObject(parent)
{
    this->_uid = QByteArray();
}

QByteArray Extension::uid()                                             { return this->_uid; }
QVariant Extension::info(QString key, QVariant defaultValue)            { return this->_info.value(key, defaultValue); }
void Extension::addInfo(QString key, QVariant value)                    { this->_info.insert(key, value); }

bool Extension::setup()                                                 { return this->_setup(); }

bool Extension::canWriteFormat(QByteArray uid)                          { return this->_outputFormats.contains(uid); }
bool Extension::canReadFormat(QByteArray uid)                           { return this->_inputFormats.contains(uid); }
bool Extension::canWriteMedia(QByteArray uid)                           { return this->_outputMedia.contains(uid); }
bool Extension::canReadMedia(QByteArray uid)                            { return this->_inputMedia.contains(uid); }

QList<QByteArray> Extension::inputMedia()                               { return this->_inputMedia; }
QList<QByteArray> Extension::inputFormats()                             { return this->_inputFormats; }
QList<QByteArray> Extension::outputMedia()                              { return this->_outputMedia; }
QList<QByteArray> Extension::outputFormats()                            { return this->_outputFormats; }

void Extension::setUid(QByteArray uid)                                  { this->_uid = uid; }
void Extension::addInputMedia(QList<QByteArray> media)                  { this->_inputMedia.append(media); }
void Extension::addInputFormats(QList<QByteArray> format)               { this->_inputFormats.append(format); }
void Extension::addOutputMedia(QList<QByteArray> media)                 { this->_outputMedia.append(media); }
void Extension::addOutputFormats(QList<QByteArray> format)              { this->_outputFormats.append(format); }

bool Extension::_isReady()                                              { return (this->_setupData.size() > 0); }

QString Extension::_findExtensionFile(QString filename, QString suffix)
{
    QStringList possibleLocations;
    possibleLocations << Configuration::pointer()->getStorageLocation(Configuration::UserExtensionsStorageLocation)
                      << Configuration::pointer()->getStorageLocation(Configuration::SystemExtensionsStorageLocation);

    QString path = QDir::toNativeSeparators(QString(this->uid()) + "/" + filename + "-" + this->info("Version").toString() + suffix);

    QString location;
    foreach (location, possibleLocations)
        if (QFile(location + path).exists())
            return location + path;

    return QString();
}

bool Extension::_setup()
{
    if (this->_isReady())
        return true;

    if (this->info("Implementation").toString().toLower() == "dll")
    {
        this->_setupData.insert("dllFile", this->_findExtensionFile(this->uid(), ".dll"));
    }

    qDebug() << "DLL location:" << this->_setupData.value("dllFile");

    return true;
}


/*
QPointer<QDataStream> Extension::createWriter(QByteArray outputMedia, QByteArray outputFormat, QByteArray outputIdentifier)
{

}

QPointer<QDataStream> Extension::createReader(QByteArray inputMedia, QByteArray inputFormat, QByteArray inputIdentifier)
{

}
*/