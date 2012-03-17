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

#ifndef SYSTEMDEPENDENT_H
#define SYSTEMDEPENDENT_H

#include <QApplication>
#include <QPointer>
#include <QDesktopServices>
#include <QDir>
#include <QString>
#include <QSettings>

class SystemDependent
{
public:
    SystemDependent();
    static SystemDependent *pointer();

    enum Scope {
        User,
        System
    };

    QFileInfo extensionsDir(Scope scope);
    QFileInfo presetsDir(Scope scope);
    QPointer<QSettings> makeSettings();

private:
    static SystemDependent *s_instance;

};

static const QString newl = "\n";

#endif // SYSTEMDEPENDENT_H
