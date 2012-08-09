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

#ifndef APPWIDECLASSES_VERSIONNUMBER_H
#define APPWIDECLASSES_VERSIONNUMBER_H

#include <QMetaType>
#include <QString>
#include <QStringList>

class VersionNumber
{
public:
    /*  This class is registered with the QMetaType system.
     *  Therefore, a default public constructor, public copy constructor and
     *  public assignment operator are required.
     */
    VersionNumber()                                                 { this->_major = this->_minor = 0; }
    VersionNumber(QString version)                                  { *this = VersionNumber::toVersion(version); }
    VersionNumber(qint64 major, qint64 minor)                       { this->_major = major; this->_minor = minor; }
    // Using compiler generated public copy constructor
    // Using compiler generated public assignment operator

    virtual bool operator==(const VersionNumber &other) const       { return (this->_major == other._major && this->_minor == other._minor); }
    virtual bool operator!=(const VersionNumber &other) const       { return !(*this == other); }
    virtual bool operator< (const VersionNumber &other) const       { return (this->_major < other._major)
                                                                        || (this->_major == other._major && this->_minor < other._minor); }
    virtual bool operator<=(const VersionNumber &other) const       { return (*this == other || *this < other); }
    virtual bool operator> (const VersionNumber &other) const       { return !(*this <= other); }
    virtual bool operator>=(const VersionNumber &other) const       { return !(*this < other); }

    VersionNumber toVersion(QString toString);

    QString toString() const                                     { return QString::number(this->_major) + "." + QString::number(this->_minor); }
    qint64 major() const                                        { return this->_major; }
    qint64 minor() const                                        { return this->_minor; }

    bool isCompatible(VersionNumber high, VersionNumber low) const  { return ((*this) >= low && (*this) <= high); }

private:
    qint64 _major;
    qint64 _minor;
};

Q_DECLARE_METATYPE(VersionNumber)

#endif // APPWIDECLASSES_VERSIONNUMBER_H
