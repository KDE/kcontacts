/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "role.h"
#include <QDataStream>
#include <QStringList>

using namespace KContacts;

class Q_DECL_HIDDEN Role::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        role = other.role;
    }

    QHash<QString, QStringList> parameters;
    QString role;
};

Role::Role()
    : d(new Private)
{
}

Role::Role(const Role &other)
    : d(other.d)
{
}

Role::Role(const QString &role)
    : d(new Private)
{
    d->role = role;
}

Role::~Role()
{
}

void Role::setRole(const QString &role)
{
    d->role = role;
}

QString Role::role() const
{
    return d->role;
}

bool Role::isValid() const
{
    return !d->role.isEmpty();
}

void Role::setParameters(const QHash<QString, QStringList> &params)
{
    d->parameters = params;
}

QHash<QString, QStringList> Role::parameters() const
{
    return d->parameters;
}

bool Role::operator==(const Role &other) const
{
    return (d->parameters == other.parameters()) && (d->role == other.role());
}

bool Role::operator!=(const Role &other) const
{
    return !(other == *this);
}

Role &Role::operator=(const Role &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Role::toString() const
{
    QString str = QLatin1String("Role {\n");
    str += QStringLiteral("    role: %1\n").arg(d->role);
    if (!d->parameters.isEmpty()) {
        QString param;
        for (auto it = d->parameters.cbegin(); it != d->parameters.cend(); ++it) {
            param += QStringLiteral("%1 %2").arg(it.key(), it.value().join(QLatin1Char(',')));
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Role &role)
{
    return s << role.d->parameters << role.d->role;
}

QDataStream &KContacts::operator>>(QDataStream &s, Role &role)
{
    s >> role.d->parameters >> role.d->role;
    return s;
}
