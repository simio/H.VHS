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

#include "appwideclasses.h"

Person::Person(QString name, QString email, QUrl website, QString username)
{
    this->_name = name;
    this->_email = email;
    this->_website = website;
    this->_username = username;
}

Person::Person(const Person &original) :
    _name(original._name),
    _email(original._email),
    _website(original._website),
    _username(original._username),
    _activities(original._activities)
{ }

Person &Person::operator =(const Person &original)
{
    this->_name = original._name;
    this->_email = original._email;
    this->_website = original._website;
    this->_username = original._username;
    this->_activities = original._activities;
    return *this;
}

QMultiMap<QDateTime, QString> Person::activities(QDateTime earliest, QDateTime latest) const
{
    QMultiMap<QDateTime, QString> result;
    QMultiMap<QDateTime, QString>::const_iterator pair = this->_activities.constBegin();
    while (pair != this->_activities.constEnd())
    {
        // QMultiMap iterates over all pairs in ascending sorting order
        if (pair.key() > latest)
            return result;
        else if (pair.key() >= earliest)
            result.insert(pair.key(), pair.value());
        pair++;
    }
    return result;
}

Version::Version(QString version)
{
    *this = Version::toVersion(version);
}

Version Version::toVersion(QString version)
{
    qint64 major, minor;
    QStringList numbers = version.split(".", QString::SkipEmptyParts);
    if (numbers.isEmpty())
        minor = major = 0;
    else if (numbers.count() == 1)
    {
        major = numbers.first().toInt(); // Returns 0 if conversion fails
        minor = 0;
    }
    else
    {
        major = numbers.first().toInt(); // Returns 0 if conversion fails
        minor = numbers.at(1).toInt();   // as above
    }
    return Version(major, minor);
}
