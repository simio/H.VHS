/*
 * Copyright (c) 2012 Jesper R�fteg�rd <jesper@huggpunkt.org>
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

#ifndef PORTABILITY_H
#define PORTABILITY_H

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QString>

class Portability
{
public:
    Portability();
    static Portability *pointer();

    QString systemExtensionPath(QString file = QString());
    QString userExtensionPath(QString file = QString());
    QString userConfPath(QString file = QString());
    QString systemConfPath(QString file = QString());
    QString userIniFilePath(QString file = QString());
    QString systemIniFilePath(QString file = QString());

private:
    static Portability *s_instance;

};

static const QString newl = "\n";

#endif // PORTABILITY_H
