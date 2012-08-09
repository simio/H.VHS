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

#ifndef EXTENSIONMANAGER_INTERFACES_FILTERS_H
#define EXTENSIONMANAGER_INTERFACES_FILTERS_H

#include <QIODevice>
#include <QHash>
#include <QString>
#include <QVariant>
#include <QSharedPointer>

#include "if_commondefines.h"

/*  H.VHS QPlugin Extension Interface for Filters, version 1.0
 *
 *  Filter objects transform data streams.
 *
 *      setup()     is (optionally) called to configure the filter
 *      isReady()   is self-explanatory
 *      consumer()  returns a pointer to the consuming end of the filter
 *      producer()  returns a pointer to the producing end of the filter
 */

class ExtensionInterfaceFilters
{
public:
    virtual ~ExtensionInterfaceFilters() {}

    virtual const bool setupFilter(QHash<QString,QVariant> setupData) = 0;
    virtual const bool filterIsReady() const = 0;
    virtual const QSharedPointer<QIODevice> consumer() = 0;
    virtual const QSharedPointer<QIODevice> producer() = 0;
};

Q_DECLARE_INTERFACE(ExtensionInterfaceFilters,
                    HVHS_INTERFACE_FILTERS)

#endif // EXTENSIONMANAGER_INTERFACES_FILTERS_H
