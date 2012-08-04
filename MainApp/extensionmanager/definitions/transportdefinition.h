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

#ifndef EXTENSIONMANAGER_DEFINITIONS_TRANSPORTDEFINITION_H
#define EXTENSIONMANAGER_DEFINITIONS_TRANSPORTDEFINITION_H

#include <QObject>
#include <QDateTime>

#include "main.h"
#include "definition.h"

// Since this class inherits Definition, which is implicitly shared through using a d pointer to QSharedData,
// none of the following may be defined inline: ctor, dtor, copy constructor, assignment operator.
// Also, ctor and dtor must be defined.

class TransportDefinition : public Definition
{
public:
    TransportDefinition(QString id,
                        QString name,
                        QString description,
                        QDateTime releaseDate,
                        QObject *parent = 0);

    ~TransportDefinition();

signals:

public slots:

private:
    TransportDefinition();

};

#endif // EXTENSIONMANAGER_DEFINITIONS_TRANSPORTDEFINITION_H
