/*
    This file is part of the KContacts framework.
    Copyright (c) 2016 Laurent Montel <montel@kde.org>

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

#include "clientpidmap.h"
#include <QMap>
#include <qstringlist.h>
#include <QDataStream>

using namespace KContacts;

class Q_DECL_HIDDEN ClientPidMap::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        clientpidmap = other.clientpidmap;
    }
    QMap<QString, QStringList> parameters;
    QString clientpidmap;
};

ClientPidMap::ClientPidMap()
    : d(new Private)
{

}

ClientPidMap::ClientPidMap(const ClientPidMap &other)
    : d(other.d)
{

}

ClientPidMap::ClientPidMap(const QString &clientpidmap)
    : d(new Private)
{
    d->clientpidmap = clientpidmap;
}

ClientPidMap::~ClientPidMap()
{

}

void ClientPidMap::setClientPidMap(const QString &clientpidmap)
{
    d->clientpidmap = clientpidmap;
}

QString ClientPidMap::clientPidMap() const
{
    return d->clientpidmap;
}

bool ClientPidMap::isValid() const
{
    return !d->clientpidmap.isEmpty();
}

void ClientPidMap::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> ClientPidMap::parameters() const
{
    return d->parameters;
}

bool ClientPidMap::operator==(const ClientPidMap &other) const
{
    return (d->parameters == other.parameters()) && (d->clientpidmap == other.clientPidMap());
}

bool ClientPidMap::operator!=(const ClientPidMap &other) const
{
    return !(other == *this);
}

ClientPidMap &ClientPidMap::operator=(const ClientPidMap &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString ClientPidMap::toString() const
{
    QString str;
    str += QLatin1String("ClientPidMap {\n");
    str += QStringLiteral("    clientpidmap: %1\n").arg(d->clientpidmap);
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

QDataStream &KContacts::operator<<(QDataStream &s, const ClientPidMap &clientpidmap)
{
    return s << clientpidmap.d->parameters << clientpidmap.d->clientpidmap;
}

QDataStream &KContacts::operator>>(QDataStream &s, ClientPidMap &clientpidmap)
{
    s >> clientpidmap.d->parameters >> clientpidmap.d->clientpidmap;
    return s;
}
