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

#ifndef DEFINITION_H
#define DEFINITION_H

#include <QObject>

// Till mig själv: Syftet med detta är att göra en virtuell klass som
// som FormatDefinition, TransportDefinition och Extension (och senare Cassette?)
// kan ärva, så att ExtensionManager kan ladda alla samtidigt.
// Format- och TransportDefinition har i nuläget datum ist.f. versionnuffror,
// vilket behöver åtgärdas i kod och relaxng. God morgon!

class Definition : public QObject
{
    Q_OBJECT
public:
    explicit Definition(QObject *parent = 0);
    Definition(const Definition &original, QObject *parent = 0);

    virtual Definition &operator=(const Definition &original);
    virtual bool operator==(const Definition &original) const;
    virtual bool operator!=(const Definition &original) const;
    virtual bool operator<(const Definition &original) const;
    virtual bool operator<=(const Definition &original) const;
    virtual bool operator>(const Definition &original) const;
    virtual bool operator>=(const Definition &original) const;

signals:

public slots:

private:
    QByteArray _id;                 // Unique ID
    QString _name;                  // Localised name
    QString _description;           // Localised description
    int _versionMajor;
    int _versionMinor;
};

#endif // DEFINITION_H
