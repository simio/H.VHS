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

#ifndef EXTENSIONINTERFACE_H
#define EXTENSIONINTERFACE_H

#include <QPointer>
#include <QDataStream>

#include "cassette.h"

class ExtensionInterface
{
public:
    enum Control {
        Start               = 0,
        Abort               = 1,
        Pause               = 2,
        Restart             = 4
    };
    enum ErrorCode {
        Ok                  = 0,
        UnspecifiedError    = 1,
        ControlFailure      = 2,
        TransportFailure    = 4,
        FormatFailure       = 8
    };
    enum Direction {
        Read,
        Write
    };

    virtual ~ExtensionInterface() {}

    virtual bool control(Control control);
    virtual bool reset();
    virtual int error();
    virtual const char * errorStr();

    virtual QPointer<QDataStream> convert(Direction direction, Cassette cassette);
};

Q_DECLARE_INTERFACE(ExtensionInterface,
                    "org.huggpunkt.VHS.ExtensionInterface/1.0")

#endif // EXTENSIONINTERFACE_H
