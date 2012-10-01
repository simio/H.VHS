/*
 * Copyright (c) 2012 Jesper Raftegard <jesper@huggpunkt.org>
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
    /* This class is registered with the QMetaType system.  Therefore,
     * a default public constructor, public copy constructor and
     * public assignment operator are required.
     */
    VersionNumber()
    {
        this->_majorVersion = this->_minorVersion = 0;
    }

    VersionNumber(QString version)
    {
        *this = VersionNumber::toVersion(version);
    }

    VersionNumber(qint64 majorVersion, qint64 minorVersion)
    {
        this->_majorVersion = majorVersion;
        this->_minorVersion = minorVersion;
    }

    // Using compiler generated public copy constructor
    // Using compiler generated public assignment operator

    virtual bool operator
    ==(const VersionNumber &other) const
    {
        return (this->_majorVersion == other._majorVersion &&
                this->_minorVersion == other._minorVersion);
    }

    virtual bool operator
    !=(const VersionNumber &other) const
    {
        return !(*this == other);
    }

    virtual bool operator
    <(const VersionNumber &other) const
    {
        return (this->_majorVersion < other._majorVersion) ||
            (this->_majorVersion == other._majorVersion &&
             this->_minorVersion < other._minorVersion);
    }

    virtual bool operator
    <=(const VersionNumber &other) const
    {
        return (*this == other || *this < other);
    }

    virtual bool operator
    >(const VersionNumber &other) const
    {
        return !(*this <= other);
    }

    virtual bool operator
    >=(const VersionNumber &other) const
    {
        return !(*this < other);
    }

    VersionNumber
    toVersion(QString toString);

    QString
    toString() const
    {
        return QString::number(this->_majorVersion)
            + "."
            + QString::number(this->_minorVersion);
    }

    qint64
    majorVersion() const
    {
        return this->_majorVersion;
    }

    qint64
    minorVersion() const
    {
        return this->_minorVersion;
    }

    bool
    isCompatible(VersionNumber high, VersionNumber low) const
    {
        return ((*this) >= low && (*this) <= high);
    }

private:
    qint64 _majorVersion;
    qint64 _minorVersion;
};

Q_DECLARE_METATYPE(VersionNumber)

#endif // APPWIDECLASSES_VERSIONNUMBER_H
