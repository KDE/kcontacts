/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "org.h"
#include <QDataStream>
#include <QStringList>

using namespace KContacts;

class Q_DECL_HIDDEN Org::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        organization = other.organization;
    }

    QHash<QString, QStringList> parameters;
    QString organization;
};

Org::Org()
    : d(new Private)
{
}

Org::Org(const Org &other)
    : d(other.d)
{
}

Org::Org(const QString &org)
    : d(new Private)
{
    d->organization = org;
}

Org::~Org()
{
}

void Org::setOrganization(const QString &org)
{
    d->organization = org;
}

QString Org::organization() const
{
    return d->organization;
}

bool Org::isValid() const
{
    return !d->organization.isEmpty();
}

void Org::setParameters(const QHash<QString, QStringList> &params)
{
    d->parameters = params;
}

QHash<QString, QStringList> Org::parameters() const
{
    return d->parameters;
}

bool Org::operator==(const Org &other) const
{
    return (d->parameters == other.parameters()) && (d->organization == other.organization());
}

bool Org::operator!=(const Org &other) const
{
    return !(other == *this);
}

Org &Org::operator=(const Org &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Org::toString() const
{
    QString str = QLatin1String("Org {\n");
    str += QStringLiteral("    organization: %1\n").arg(d->organization);
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

QDataStream &KContacts::operator<<(QDataStream &s, const Org &org)
{
    return s << org.d->parameters << org.d->organization;
}

QDataStream &KContacts::operator>>(QDataStream &s, Org &org)
{
    s >> org.d->parameters >> org.d->organization;
    return s;
}
