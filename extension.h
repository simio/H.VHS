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

#ifndef EXTENSION_H
#define EXTENSION_H

#include <QObject>

#include "main.h"

#include "mediadefinition.h"
#include "formatdefinition.h"

class Extension : public QObject
{
    Q_OBJECT
public:
    explicit Extension(QObject *parent = 0);

    enum ExtensionType {
        Empty,
        Internal,
        Dll,
        JavaScript,
        ChickenScheme
    };

signals:

public slots:

private:
    QByteArray _uid;
    QString _version;
    bool _enabled;
    QString _prettyName;
    QString _authorName;
    QString _authorEmail;
    QString _authorWebsite;

    ExtensionType _type;

    QList<MediaDefinition> _inputMedia;
    QList<FormatDefinition> _inputFormats;
    QList<MediaDefinition> _outputMedia;
    QList<FormatDefinition> _outputFormats;
};

#endif // EXTENSION_H
