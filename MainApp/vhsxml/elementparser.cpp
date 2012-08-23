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

#include "elementparser.h"

namespace VhsXml {

QDateTime ElementParser::dateTime(const QString &str)
{
    return QDateTime::fromString(str, Qt::ISODate);
}

bool ElementParser::boolean(const QString &str, bool defaultValue)
{
    if (str.trimmed() == "true")
        return true;
    else if (str.trimmed() == "false")
        return false;

    qDebug() << "ElementParser error: Expected \"true\" or \"false\" but got" << str;
    qDebug() << "Assuming" << (defaultValue ? "true" : "false");

    return false;
}

QString ElementParser::token(const QString &str)
{
    QString result = str;
    return result.replace("\\s+", " ");
}

bool ElementParser::expect(const QDomElement &element, const QString &tagName, ElementParser::ElementAttribute attribute)
{
    if (element.isNull())
        return false;
    else if (tagName == element.tagName())
        return true;
    else if (attribute == ElementParser::Required)
        qDebug() << "ElementParser error: Expected" << tagName << "element but got" << element.tagName()
                    << "in line" << element.lineNumber();
    return false;
}

QStringList ElementParser::tokenList(const QDomElement &parent, const QString &childElementName)
{
    QStringList result;
    QDomElement element = parent.firstChildElement();
    while (! element.isNull())
    {
        result << ElementParser::token(element.text());
        element = element.nextSiblingElement();
    }
    return result;
}

QString ElementParser::nmtoken(const QString &str)
{
    QString result;
    QChar const* ch = str.constData();              // Qt guarantees null termination.
    while (! ch->isNull())
    {
        if (ch->isLetterOrNumber()
                || ch->category() == QChar::Punctuation_Connector
                || ch->category() == QChar::Punctuation_Dash)
            result.append(*ch);
        ch++;
    }

    return result;
}

QStringList ElementParser::nmtokenList(const QDomElement &parent, const QString &childElementName)
{
    QStringList result;
    QDomElement element = parent.firstChildElement();
    while (! element.isNull())
    {
        result << ElementParser::nmtoken(element.text());
        element = element.nextSiblingElement();
    }
    return result;
}

QString ElementParser::localisedString(QDomElement &e)
{
    if (e.isNull())
        return QString();

    // Read first text() and use as initial bestMatch
    QString bestMatch = e.text();
    QString name = e.tagName();

    // Now loop through all following elements with same tagName()
    while (!e.nextSiblingElement().isNull() && e.nextSiblingElement().tagName() == name)
    {
        e = e.nextSiblingElement();
        if (e.attribute("xml:lang") == Configuration::p()->locale(true))
            return e.text();
        else if (e.attribute("xml:lang") == Configuration::p()->locale(false))
            bestMatch = e.text();
    }

    // Leave element pointer at last matching element for consistency.
    return bestMatch;
}


} // namespace VhsXml
