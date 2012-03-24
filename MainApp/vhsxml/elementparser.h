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

#ifndef VHSXML_DATATYPE_H
#define VHSXML_DATATYPE_H

#include <QString>
#include <QDateTime>
#include <QDomElement>

#include "main.h"

namespace VhsXml {

class ElementParser
{
public:

    enum ElementAttribute {
        Required,
        Optional
    };

    // Returns true iff element.tagName() matches tagName.
    // Prints parsing errors if e.isNull().
    static bool expect(const QDomElement &element, const QString &tagName, ElementAttribute attribute);

    // Parse as ISO date (xsd:dateTime)
    static QDateTime dateTime(const QString &str);

    // Parse as boolean
    static bool boolean(const QString &str, bool defaultValue = false);

    // Parse as xsd:token
    static QString token(const QString &str);

    // Parse child elements and return list of text() from all childElementName childs, parsed as xsd:token
    static QStringList tokenList(const QDomElement &parent, const QString &childElementName);

    // Parse as xsd:NMTOKEN.
    static QString nmtoken(const QString &str);

    // Parse child elements and return list of text() from all childElementName childs, parsed as xsd:NMTOKEN
    static QStringList nmtokenList(const QDomElement &parent, const QString &childElementName);

    // Parse this and all following elements of the same tagname,
    // text() from the element that best matches the current locale.
    static QString localisedString(QDomElement &e);

signals:

public slots:

private:
    ElementParser();

};

} // namespace VhsXml

#endif // VHSXML_DATATYPE_H
