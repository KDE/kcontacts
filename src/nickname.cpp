/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "nickname.h"
#include <QDataStream>
#include <QStringList>

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

    QHash<QString, QStringList> parameters;
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

void NickName::setParameters(const QHash<QString, QStringList> &params)
{
    d->parameters = params;
}

QHash<QString, QStringList> NickName::parameters() const
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
    QString str = QLatin1String("NickName {\n");
    str += QStringLiteral("    nickname: %1\n").arg(d->nickname);
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

QDataStream &KContacts::operator<<(QDataStream &s, const NickName &nickname)
{
    return s << nickname.d->parameters << nickname.d->nickname;
}

QDataStream &KContacts::operator>>(QDataStream &s, NickName &nickname)
{
    s >> nickname.d->parameters >> nickname.d->nickname;
    return s;
}
