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

#ifndef CASSETTE_H
#define CASSETTE_H

/* NOTICE: This class and the CassetteContent class are in circular
 * dependence. Inlining any members or adding CassetteContent typed
 * members will break the build.
 *
 * Instead, use pointers to CassetteContent.
 */

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMap>
#include <QHash>
#include <QSharedPointer>

#include "main.h"

#include "appwideclasses/hurl.h"

class CassetteContent;
class Cassette : public QSharedData
{
public:
    enum PresentationField {
        Title,
        Show,
        Season,
        Episode,
        Plot,
        ContentCreator,
        Publisher,
        Broadcaster
    };

    explicit
    Cassette();

    QString
    id() const;

    HUrl
    uri() const;

private:
    QString _id;
    HUrl _uri;

    QSharedDataPointer<CassetteContent> _content;

    QHash<QString,Cassette> _associatedCassettes;

    QMap<PresentationField,QString> _presentation;
};

#endif // CASSETTE_H
