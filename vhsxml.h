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

#ifndef VHSXML_H
#define VHSXML_H

#include <QObject>
#include <QPointer>
#include <QFile>
#include <QDomDocument>
#include <QXmlInputSource>

#include "main.h"

#include "formatdefinition.h"
#include "mediadefinition.h"

class VhsXml : public QObject
{
    Q_OBJECT
public:
    explicit VhsXml(QObject *parent = 0);
    VhsXml(QPointer<QFile> file, QObject *parent = 0);
    VhsXml(QPointer<QIODevice> device, QObject *parent = 0);

    QList< QPointer<FormatDefinition> > getFormatDefinitions();
    QList< QPointer<MediaDefinition> > getMediaDefinitions();

signals:

public slots:

private:
    bool _initialise(QPointer<QFile> file, QString name = QString());
    bool _initialise(QPointer<QIODevice> device, QString name = QString());
    bool _initialise(QXmlInputSource *source, QString name = QString());

    QDomDocument _xml;

};

#endif // VHSXML_H
