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

#ifndef VHSXML_FORMATREADER_H
#define VHSXML_FORMATREADER_H

#include <QList>
#include <QPointer>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "main.h"
#include "vhsxml/elementparser.h"
#include "extensionmanager/definitions/formatdefinition.h"

namespace VhsXml {

class FormatReader
{
public:
    static QList<QPointer<FormatDefinition> > parse(const QDomDocument &document, QObject *definitionParent);

private:
    FormatReader();

    static QPointer<FormatDefinition> _parseFormat(const QDomElement &formatNode, QObject *definitionParent);
};

} // namespace VhsXml

#endif // VHSXML_FORMATREADER_H
