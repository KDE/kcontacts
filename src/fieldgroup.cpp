/*
    This file is part of the KContacts framework.
    Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "fieldgroup.h"

#include <QMap>
#include <qstringlist.h>
#include <QDataStream>

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
