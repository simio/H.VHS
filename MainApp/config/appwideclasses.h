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

#ifndef APPWIDECLASSES_H
#define APPWIDECLASSES_H

#include <QString>
#include <QVariant>
#include <QUrl>
#include <QMultiMap>
#include <QDateTime>
#include <QStringList>

class Person
{
public:
    Person(QString name, QString email, QUrl website, QString username);
    Person(const Person &original);
    virtual Person &operator=(const Person &original);

    QString name() const                                                        { return this->_name; }
    QString email() const                                                       { return this->_email; }
    QUrl website() const                                                        { return this->_website; }
    QString username() const                                                    { return this->_username; }
    QDateTime activity(QString activity) const                                  { return this->_activities.key(activity); }
    void addActivity(QDateTime timestamp, QString activity)                     { this->_activities.insert(timestamp, activity); }
    QMultiMap<QDateTime,QString> activities(QDateTime earliest, QDateTime latest) const;

    void addInfo(QString key, QVariant value)                                   { this->_info.insert(key, value); }
    QMultiMap<QString,QVariant> info(void) const                                { return this->_info; }
    QList<QVariant> info(QString key) const                                     { return this->_info.values(key); }

private:
    QString _name;
    QString _email;
    QUrl _website;
    QString _username;
    QMultiMap<QDateTime,QString> _activities;
    QMultiMap<QString,QVariant> _info;
};

class Version
{
public:
    Version()                                                   { this->_major = this->_minor = 0; }
    Version(QString version);
    Version(qint64 major, qint64 minor)                         { this->_major = major; this->_minor = minor; }
    Version(const Version &original)                          : _major(original._major), _minor(original._minor) {}
    virtual Version &operator=(const Version &original)         { this->_major = original._major; this->_minor = original._minor; return *this; }

    virtual bool operator==(const Version &other) const         { return (this->_major == other._major && this->_minor == other._minor); }
    virtual bool operator!=(const Version &other) const         { return !(*this == other); }
    virtual bool operator< (const Version &other) const         { return (this->_major < other._major)
                                                                            || (this->_major == other._major && this->_minor < other._minor); }
    virtual bool operator<=(const Version &other) const         { return (*this == other || *this < other); }
    virtual bool operator> (const Version &other) const         { return !(*this <= other); }
    virtual bool operator>=(const Version &other) const         { return !(*this < other); }

    Version toVersion(QString version);

    QString version() const                                     { return QString::number(this->_major) + "." + QString::number(this->_minor); }
    qint64 major() const                                        { return this->_major; }
    qint64 minor() const                                        { return this->_minor; }

private:
    qint64 _major;
    qint64 _minor;
};


#endif // APPWIDECLASSES_H
