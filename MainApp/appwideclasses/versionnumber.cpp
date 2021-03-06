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

#include "versionnumber.h"

VersionNumber VersionNumber::toVersion(QString version)
{
    qint64 majorVersion, minorVersion;
    QStringList numbers = version.trimmed().split(".", QString::SkipEmptyParts);
    if (numbers.isEmpty())
        minorVersion = majorVersion = 0;
    else if (numbers.count() == 1)
    {
        majorVersion = numbers.first().toInt(); // Returns 0 if conversion fails
        minorVersion = 0;
    }
    else
    {
        majorVersion = numbers.first().toInt(); // Returns 0 if conversion fails
        minorVersion = numbers.at(1).toInt();   // as above
    }
    return VersionNumber(majorVersion, minorVersion);
}
