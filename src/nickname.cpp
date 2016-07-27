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

#include "nickname.h"
#include <QMap>
#include <qstringlist.h>
#include <QDataStream>

using namespace KContacts;

class Q_DECL_HIDDEN NickName::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        nickname = other.nickname;
    }
    QMap<QString, QStringList> parameters;
    QString nickname;
};

NickName::NickName()
    : d(new Private)
{

}

NickName::NickName(const NickName &other)
    : d(other.d)
{

}

NickName::NickName(const QString &nickname)
    : d(new Private)
{
    d->nickname = nickname;
}

NickName::~NickName()
{

}

void NickName::setNickName(const QString &nickname)
{
    d->nickname = nickname;
}

QString NickName::nickname() const
{
    return d->nickname;
}

bool NickName::isValid() const
{
    return !d->nickname.isEmpty();
}

void NickName::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> NickName::parameters() const
{
    return d->parameters;
}

bool NickName::operator==(const NickName &other) const
{
    return (d->parameters == other.parameters()) && (d->nickname == other.nickname());
}

bool NickName::operator!=(const NickName &other) const
{
    return !(other == *this);
}

NickName &NickName::operator=(const NickName &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString NickName::toString() const
{
    QString str;
    str += QLatin1String("NickName {\n");
    str += QStringLiteral("    nickname: %1\n").arg(d->nickname);
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

QDataStream &KContacts::operator<<(QDataStream &s, const NickName &nickname)
{
    return s << nickname.d->parameters << nickname.d->nickname;
}

QDataStream &KContacts::operator>>(QDataStream &s, NickName &nickname)
{
    s >> nickname.d->parameters >> nickname.d->nickname;
    return s;
}
