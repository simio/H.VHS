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

#include "datatype.h"

namespace VhsXml {

QDateTime DataType::dateTime(const QString &str)
{
    return QDateTime::fromString(str, Qt::ISODate);
}

QStringList DataType::tokenList(const QDomElement &parent, const QString &childElementName)
{
    QStringList result;
    QDomElement element = parent.firstChildElement();
    while (! element.isNull())
    {
        result << element.text().replace("\\s", " ");
        element = element.nextSiblingElement();
    }
    return result;
}

QString DataType::nmtoken(const QString &str)
{
    QString result;
    QChar const* ch = str.constData();
    while (*ch != NULL)
    {
        if (ch->isLetterOrNumber()
                || ch->category() == QChar::Punctuation_Connector
                || ch->category() == QChar::Punctuation_Dash)
            result.append(*ch);
        ch++;
    }

    return result;
}

QString DataType::localisedString(QDomElement &e)
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
        if (e.attribute("xml:lang") == Configuration::pointer()->locale(true))
            return e.text();
        else if (e.attribute("xml:lang") == Configuration::pointer()->locale(false))
            bestMatch = e.text();
    }

    // Leave element pointer at last matching element for consistency.
    return bestMatch;
}


} // namespace VhsXml
