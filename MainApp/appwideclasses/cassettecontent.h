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

#ifndef CASSETTECONTENT_H
#define CASSETTECONTENT_H

/*  NOTICE: This class and the Cassette class are in
 *  circular dependence. Inlining any members or adding
 *  Cassette typed members will break the build.
 *
 *  Instead, use QPointer<Cassette>.
 */

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QHash>
#include <QSharedData>
#include <QSharedPointer>

#include "main.h"

class Cassette;
class CassetteContent : public QSharedData
{
public:
    CassetteContent();

    enum ContentForm {
        Empty,
        Text,
        Binary,
        SubCassettes
    };

    bool isEmpty() const;
    ContentForm form() const;

    QString textContent() const;
    QByteArray binaryContent() const;
    QSharedDataPointer<Cassette> subCassette(QString id) const;
    QHash<QString,QSharedDataPointer<Cassette> > subCassettes() const;

    void empty();
    void setContent(QString str);
    void setContent(QByteArray binaryData);
    void setContent(QList<QSharedDataPointer<Cassette> > cassettes);

private:
    ContentForm _form;

    QString _textContent;
    QByteArray _binaryData;
    QHash<QString,QSharedDataPointer<Cassette> > _subCassettes;

    void _empty();
};

#endif // CASSETTECONTENT_H
