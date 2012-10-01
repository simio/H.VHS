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

#include "extensiondefinition.h"
#include "definitiondata.h"

/* The below QVariant type juggling suggests accessing DefinitionData
 * could be done in a better way.
 */

ExtensionDefinition::ExtensionDefinition(
    QString id,
    QString name,
    QString description,
    QDateTime releaseDate,
    QList<Person> authors,
    QString licenseName,
    QUrl licenseUrl,
    QList<Person> maintainers,
    bool enabled,
    ExtensionDefinition::Condition condition,
    QString basePath,
    QStringList interfaces,
    VersionNumber apiVersion,
    ExtensionDefinition::ApiInterface apiInterface,
    QString source,
    QList<QString> inputTransports,
    QList<QString> inputFormats,
    QList<QString> outputTransports,
    QList<QString> outputFormats,
    QList<Person> audits,
    QObject *parent) :
Definition(id,
           name,
           description,
           releaseDate,
           Definition::ExtensionDefinitionType,     // Set definition type
           parent)
{
    Definition::_d.data()->set(DefinitionData::ExtensionLicenseName,
                               QVariant(licenseName));
    Definition::_d.data()->set(DefinitionData::ExtensionLicenseUrl,
                               QVariant(licenseUrl));
    Definition::_d.data()->set(DefinitionData::ExtensionEnabled,
                               QVariant(enabled));
    Definition::_d.data()->set(DefinitionData::ExtensionBasePath,
                               QVariant(basePath));
    Definition::_d.data()->set(DefinitionData::ExtensionInterfaces,
                               QVariant(interfaces));
    Definition::_d.data()->set(DefinitionData::ExtensionSource,
                               QVariant(source));
    Definition::_d.data()->set(DefinitionData::ExtensionInputTransports,
                               QVariant(inputTransports));
    Definition::_d.data()->set(DefinitionData::ExtensionInputFormats,
                               QVariant(inputFormats));
    Definition::_d.data()->set(DefinitionData::ExtensionOutputTransports,
                               QVariant(outputTransports));
    Definition::_d.data()->set(DefinitionData::ExtensionOutputFormats,
                               QVariant(outputFormats));

    // Cast to qint64 when storing enums
    Definition::_d.data()->set(DefinitionData::ExtensionApi,
                               QVariant((qint64)apiInterface));
    Definition::_d.data()->set(DefinitionData::ExtensionCondition,
                               QVariant((qint64)condition));

    // Use QVariant::fromValue where no QVariant constructor is available.
    // (These are registered with the QMetaType-system.)
    Definition::_d.data()->set(DefinitionData::ExtensionAuthors,
                               QVariant::fromValue(authors));
    Definition::_d.data()->set(DefinitionData::ExtensionAudits,
                               QVariant::fromValue(audits));
    Definition::_d.data()->set(DefinitionData::ExtensionApiVersion,
                               QVariant::fromValue(apiVersion));
    Definition::_d.data()->set(DefinitionData::ExtensionMaintainers,
                               QVariant::fromValue(maintainers));
}

ExtensionDefinition::~ExtensionDefinition()
{ }

bool
ExtensionDefinition::isEnabled() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionEnabled).toBool();
}

QStringList
ExtensionDefinition::interfaces() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionInterfaces).toStringList();
}

bool
ExtensionDefinition::implementsInterface(QString interface) const
{
    return this->interfaces().contains(interface);
}

ExtensionDefinition::ApiInterface
ExtensionDefinition::api() const
{
    return (ExtensionDefinition::ApiInterface)
        Definition::_d.constData()->get(
            DefinitionData::ExtensionApi).toLongLong();
}

VersionNumber
ExtensionDefinition::apiVersion() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionApiVersion).value<VersionNumber>();
}

QString
ExtensionDefinition::licenseName() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionLicenseName).toString();
}

QString
ExtensionDefinition::source() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionSource).toString();
}

QList<Person>
ExtensionDefinition::authors() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionAuthors).value<QList<Person> >();
}

QList<Person>
ExtensionDefinition::maintainers() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionMaintainers).value<QList<Person> >();
}

QList<Person>
ExtensionDefinition::audits() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionAudits).value<QList<Person> >();
}

bool
ExtensionDefinition::canWriteFormat(QString uid) const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionOutputFormats).toStringList().contains(uid);
}

bool
ExtensionDefinition::canReadFormat(QString uid) const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionInputFormats).toStringList().contains(uid);
}

bool
ExtensionDefinition::canWriteTransport(QString uid) const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionOutputTransports).toStringList().contains(uid);
}

bool
ExtensionDefinition::canReadTransport(QString uid) const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionInputTransports).toStringList().contains(uid);
}

QList<QString>
ExtensionDefinition::inputTransports() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionInputTransports).toStringList();
}

QList<QString>
ExtensionDefinition::inputFormats() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionInputFormats).toStringList();
}

QList<QString>
ExtensionDefinition::outputTransports() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionOutputTransports).toStringList();
}

QList<QString>
ExtensionDefinition::outputFormats() const
{
    return Definition::_d.constData()->get(
        DefinitionData::ExtensionOutputFormats).toStringList();
}


bool
ExtensionDefinition::isValid() const
{
    // Somewhat delispious, though still not complete validity check.
    return (Definition::isValid()
            // Extensions without a maintainer are invalid
            &&  this->maintainers().count() > 0
            // Unlicensed extensions are invalid
            && !this->licenseName().isEmpty()
            // Licensing is mostly useless without an author
            &&  this->authors().count() > 0
            &&  this->apiVersion() > VersionNumber(0, 0))
        && (! (
                // Extensions without streams must not use these lists
                this->inputTransports().count()
                || this->inputFormats().count()
                || this->outputTransports().count()
                || this->outputFormats().count())
            || (
                // Extensions with streams must use all of them
                this->inputTransports().count()
                && this->inputFormats().count()
                && this->outputTransports().count()
                && this->outputFormats().count()));
}
