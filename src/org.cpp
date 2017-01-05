/*
    This file is part of the KContacts framework.
    Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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

#include "org.h"
#include <QMap>
#include <qstringlist.h>
#include <QDataStream>

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
    QMap<QString, QStringList> parameters;
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

void Org::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> Org::parameters() const
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
    QString str;
    str += QLatin1String("Org {\n");
    str += QStringLiteral("    organization: %1\n").arg(d->organization);
    if (!d->parameters.isEmpty()) {
        QMapIterator<QString, QStringList> i(d->parameters);
        QString param;
        while (i.hasNext()) {
            i.next();
            param += QStringLiteral("%1 %2").arg(i.key(), i.value().join(QLatin1Char(',')));
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
