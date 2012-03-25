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

#ifndef CONVERSIONMANAGER_H
#define CONVERSIONMANAGER_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QDirIterator>
#include <QDateTime>

#include "main.h"

#include "converter/definition.h"
#include "converter/formatdefinition.h"
#include "converter/transportdefinition.h"
#include "converter/extensiondefinition.h"
#include "converter/definitiontable.h"

#include "vhsxml/documentreader.h"

class ConversionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConversionManager(QObject *parent = 0);
    static ConversionManager *pointer();

signals:

public slots:

private:
    static ConversionManager *s_instance;

    void _initialise();

    DefinitionTable _definitions;
};

#endif // CONVERSIONMANAGER_H
