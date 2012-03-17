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

#include "formatdefinition.h"

FormatDefinition::FormatDefinition(QObject *parent) :
    QObject(parent)
{
    this->_uid = QByteArray();
    this->_prettyName = QString(this->_uid);
    this->_contentType = Empty;
    this->_dateTime = QDateTime();
}

FormatDefinition::FormatDefinition(const FormatDefinition &original, QObject *parent) :
    QObject(parent),
    _uid(original._uid),
    _prettyName(original._prettyName),
    _contentType(original._contentType),
    _dateTime(original._dateTime) { }

FormatDefinition &FormatDefinition::operator =(const FormatDefinition &original)
{
    this->_uid = original._uid;
    this->_prettyName = original._prettyName;
    this->_contentType = original._contentType;
    this->_dateTime = original._dateTime;
    return *this;
}

QByteArray FormatDefinition::uid() const                                            { return this->_uid; }
QString FormatDefinition::prettyName() const                                        { return this->_prettyName; }
FormatDefinition::ContentType FormatDefinition::contentType() const                 { return this->_contentType; }
QDateTime FormatDefinition::dateTime() const                                        { return this->_dateTime; }

void FormatDefinition::setUid(QByteArray uid)                                       { this->_uid = uid; }
void FormatDefinition::setPrettyName(QString prettyName)                            { this->_prettyName = prettyName; }
void FormatDefinition::setDateTime(QDateTime dateTime)                              { this->_dateTime = dateTime; }
void FormatDefinition::setContentType(FormatDefinition::ContentType content)        { this->_contentType = content; }
void FormatDefinition::setContentType(QString contentType)
{
    QString normalised = contentType.toLower().trimmed();

    if (normalised == "empty")
        this->setContentType(Empty);
    else if (normalised == "any")
        this->setContentType(Any);
    else if (normalised == "meta")
        this->setContentType(Meta);
    else if (normalised == "data")
        this->setContentType(Data);
    else if (normalised == "complete")
        this->setContentType(Complete);
    else
        this->setContentType(Unknown);
}
