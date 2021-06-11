/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clientpidmap.h"
#include <QDataStream>
#include <QStringList>

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
    QString str = QLatin1String("ClientPidMap {\n");
    str += QStringLiteral("    clientpidmap: %1\n").arg(d->clientpidmap);
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

QDataStream &KContacts::operator<<(QDataStream &s, const ClientPidMap &clientpidmap)
{
    return s << clientpidmap.d->parameters << clientpidmap.d->clientpidmap;
}

QDataStream &KContacts::operator>>(QDataStream &s, ClientPidMap &clientpidmap)
{
    s >> clientpidmap.d->parameters >> clientpidmap.d->clientpidmap;
    return s;
}
