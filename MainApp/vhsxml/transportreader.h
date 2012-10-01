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

#ifndef VHSXML_TRANSPORTREADER_H
#define VHSXML_TRANSPORTREADER_H

#include <QList>
#include <QSharedPointer>
#include <QDomDocument>

#include "main.h"
#include "vhsxml/elementparser.h"
#include "extensionmanager/definitions/transportdefinition.h"

namespace VhsXml {
    class TransportReader
    {
    public:
        static QList<QSharedPointer<TransportDefinition> >
        parse(const QDomDocument &document);

    private:
        TransportReader();

        static QSharedPointer<TransportDefinition>
        _parseTransport(const QDomElement &transportNode);
    };
} // namespace VhsXml

#endif // VHSXML_TRANSPORTREADER_H
