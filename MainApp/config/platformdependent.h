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

#ifndef SYSTEMDEPENDENT_H
#define SYSTEMDEPENDENT_H

#include <QApplication>
#include <QPointer>
#include <QDesktopServices>
#include <QDir>
#include <QString>
#include <QSettings>

class PlatformDependent
{
public:
    PlatformDependent();
    static PlatformDependent *p();

    enum Scope {
        User,
        System
    };

    QDir extensionsDir(Scope scope);
    QDir presetsDir(Scope scope);
    QPointer<QSettings> makeSettings(QObject *parent);

private:
    static PlatformDependent *s_instance;

};

static const QString newl = "\n";

#endif // SYSTEMDEPENDENT_H
