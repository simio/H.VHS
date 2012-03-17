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

#include "transportdefinition.h"

TransportDefinition::TransportDefinition(QObject *parent) :
    QObject(parent)
{
    this->_uid = QByteArray();
    this->_prettyName = QString(this->_uid);
    this->_dateTime = QDateTime();
}

TransportDefinition::TransportDefinition(const TransportDefinition &original, QObject *parent) :
    QObject(parent),
    _uid(original._uid),
    _prettyName(original._prettyName),
    _dateTime(original._dateTime) { }

TransportDefinition &TransportDefinition::operator=(const TransportDefinition &original)
{
    this->_uid = original._uid;
    this->_prettyName = original._prettyName;
    this->_dateTime = original._dateTime;
    return *this;
}

QByteArray TransportDefinition::uid() const                                             { return this->_uid; }
QString TransportDefinition::prettyName() const                                         { return this->_prettyName; }
QDateTime TransportDefinition::dateTime() const                                         { return this->_dateTime; }

void TransportDefinition::setUid(QByteArray uid)                                        { this->_uid = uid; }
void TransportDefinition::setPrettyName(QString prettyName)                             { this->_prettyName = prettyName; }
void TransportDefinition::setDateTime(QDateTime dateTime)                               { this->_dateTime = dateTime; }

