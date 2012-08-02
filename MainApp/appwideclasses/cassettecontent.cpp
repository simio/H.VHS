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

// May not include main.h

#include "appwideclasses/cassettecontent.h"
#include "appwideclasses/cassette.h"

CassetteContent::CassetteContent()
{
    this->_form = CassetteContent::Empty;
}

bool CassetteContent::isEmpty() const
{
    return (this->_form == CassetteContent::Empty);
}

CassetteContent::ContentForm CassetteContent::form() const
{
    return this->_form;
}

QString CassetteContent::textContent() const
{
    if (this->_form == CassetteContent::Text)
        return this->_textContent;
    else
    {
        qWarning() << "CassetteContent::textContent() accessed on non-text CassetteContent."
                   << "Form is type" << (int)this->_form;
        return QString();
    }
}

QByteArray CassetteContent::binaryContent() const
{
    if (this->_form == CassetteContent::Binary)
        return this->_binaryData;
    else
    {
        qWarning() << "CassetteContent::binaryContent() accessed on non-binary CassetteContent."
                   << "Form is type" << (int)this->_form;
        return QByteArray();
    }
}

QSharedDataPointer<Cassette> CassetteContent::subCassette(QString id) const
{
    if (this->_form == CassetteContent::SubCassettes)
        return this->_subCassettes.value(id, QSharedDataPointer<Cassette>());
    else
    {
        qWarning() << "CassetteContent::subCassette(id) accessed on non-subCassette CassetteContent."
                   << "Form is type" << (int)this->_form;
        return QSharedDataPointer<Cassette>();
    }
}

QHash<QString, QSharedDataPointer<Cassette> > CassetteContent::subCassettes() const
{
    if (this->_form == CassetteContent::SubCassettes)
        return this->_subCassettes;
    else
    {
        qWarning() << "CassetteContent::subCassettes(id) accessed on non-subCassette CassetteContent."
                   << "Form is type" << (int)this->_form;
        return QHash<QString,QSharedDataPointer<Cassette> >();
    }
}

void CassetteContent::empty()
{
    this->_empty();
}

void CassetteContent::setContent(QString str)
{
    this->_form = CassetteContent::Text;
    this->_empty();
    this->_textContent = str;
}

void CassetteContent::setContent(QByteArray binaryData)
{
    this->_form = CassetteContent::Binary;
    this->_empty();
    this->_binaryData = binaryData;
}

void CassetteContent::setContent(QList<QSharedDataPointer<Cassette> > cassettes)
{
    this->_form = CassetteContent::SubCassettes;
    this->_empty();
    foreach(QSharedDataPointer<Cassette> cassette, cassettes)
    {
        this->_subCassettes.insert(cassette->id(), cassette);
    }
}

void CassetteContent::_empty()
{
    if (this->_form == CassetteContent::SubCassettes)
    {
        QHash<QString,QSharedDataPointer<Cassette> >::iterator cassette = this->_subCassettes.begin();
        while (cassette != this->_subCassettes.end())
            delete cassette;
        this->_subCassettes.clear();
    }
    else if (this->_form == CassetteContent::Binary)
    {
        this->_binaryData = QByteArray();
    }
    else if (this->_form == CassetteContent::Text)
    {
        this->_textContent = QString();
    }
    else if (this->_form == CassetteContent::Empty)
    {
        // No action needed.
    }
    else
    {
        qWarning() << "CassetteContent::_empty() called on object with illegal internal form. Form type is" << this->_form;
    }
}
