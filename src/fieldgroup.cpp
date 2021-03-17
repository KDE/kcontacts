/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fieldgroup.h"

#include <QDataStream>
#include <QMap>
#include <QStringList>

using namespace KContacts;

class Q_DECL_HIDDEN FieldGroup::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        fieldGroupName = other.fieldGroupName;
        value = other.value;
    }

    QMap<QString, QStringList> parameters;
    QString fieldGroupName;
    QString value;
};

FieldGroup::FieldGroup()
    : d(new Private)
{
}

FieldGroup::FieldGroup(const FieldGroup &other)
    : d(other.d)
{
}

FieldGroup::FieldGroup(const QString &FieldGroupName)
    : d(new Private)
{
    d->fieldGroupName = FieldGroupName;
}

FieldGroup::~FieldGroup()
{
}

void FieldGroup::setFieldGroupName(const QString &fieldGroup)
{
    d->fieldGroupName = fieldGroup;
}

QString FieldGroup::fieldGroupName() const
{
    return d->fieldGroupName;
}

bool FieldGroup::isValid() const
{
    return !d->fieldGroupName.isEmpty();
}

void FieldGroup::setValue(const QString &value)
{
    d->value = value;
}

QString FieldGroup::value() const
{
    return d->value;
}

void FieldGroup::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> FieldGroup::parameters() const
{
    return d->parameters;
}

bool FieldGroup::operator==(const FieldGroup &other) const
{
    return (d->parameters == other.parameters()) && (d->fieldGroupName == other.fieldGroupName()) && (d->value == other.value());
}

bool FieldGroup::operator!=(const FieldGroup &other) const
{
    return !(other == *this);
}

FieldGroup &FieldGroup::operator=(const FieldGroup &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString FieldGroup::toString() const
{
    QString str = QLatin1String("FieldGroup {\n");
    str += QStringLiteral("    FieldGroupName: %1 Value %2\n").arg(d->fieldGroupName).arg(d->value);
    if (!d->parameters.isEmpty()) {
        QString param;
        QMap<QString, QStringList>::const_iterator it = d->parameters.constBegin();
        const QMap<QString, QStringList>::const_iterator end = d->parameters.constEnd();
        while (it != end) {
            param += QStringLiteral("%1 %2").arg(it.key(), it.value().join(QLatin1Char(',')));
            ++it;
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const FieldGroup &fieldGroup)
{
    return s << fieldGroup.d->parameters << fieldGroup.d->fieldGroupName << fieldGroup.d->value;
}

QDataStream &KContacts::operator>>(QDataStream &s, FieldGroup &fieldGroup)
{
    s >> fieldGroup.d->parameters >> fieldGroup.d->fieldGroupName >> fieldGroup.d->value;
    return s;
}
