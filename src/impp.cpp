/*
    This file is part of libkabc.
    Copyright (c) 2015-2016 Laurent Montel <montel@kde.org>

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

#include "impp.h"

#include <QMap>
#include <qstringlist.h>
#include <QDataStream>
#include "kcontacts_debug.h"

using namespace KContacts;

class Q_DECL_HIDDEN Impp::Private : public QSharedData
{
public:
    Private()
        : type(KContacts::Impp::Unknown)
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        type = other.type;
    }
    QMap<QString, QStringList> parameters;
    QString address;
    ImppType type;
};

Impp::Impp()
    : d(new Private)
{

}

Impp::Impp(const Impp &other)
    : d(other.d)
{
}

Impp::Impp(const QString &address)
    : d(new Private)
{
    d->address = address;
}

Impp::~Impp()
{

}

bool Impp::isValid() const
{
    return !d->address.isEmpty() && (d->type != Unknown);
}

Impp::ImppType Impp::type() const
{
    return d->type;
}

void Impp::setType(Impp::ImppType type)
{
    d->type = type;
}

void Impp::setAddress(const QString &address)
{
    d->address = address;
}

QString Impp::address() const
{
    return d->address;
}

void Impp::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> Impp::parameters() const
{
    return d->parameters;
}

bool Impp::operator==(const Impp &other) const
{
    return (d->parameters == other.parameters()) && (d->address == other.address()) && (d->type == other.type());
}

bool Impp::operator!=(const Impp &other) const
{
    return !(other == *this);
}

Impp &Impp::operator=(const Impp &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Impp::toString() const
{
    QString str;
    str += QLatin1String("Impp {\n");
    str += QStringLiteral("    type: %1\n").arg(typeToString(d->type));
    str += QStringLiteral("    address: %1\n").arg(d->address);
    if (!d->parameters.isEmpty()) {
        QMapIterator<QString, QStringList> i(d->parameters);
        QString param;
        while (i.hasNext()) {
            i.next();
            param += QStringLiteral("%1 %2").arg(i.key(), i.value().join(QStringLiteral(",")));
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

QString Impp::typeToString(ImppType type)
{
    switch (type) {
    case Unknown:
    case EndList:
        qCWarning(KCONTACTS_LOG) << "Invalid type requested";
        break;
    case Skype:
        return QStringLiteral("skype");
    case Xmpp:
        return QStringLiteral("xmpp");
    case Jabber:
        return QStringLiteral("jabber");
    case Sip:
        return QStringLiteral("sip");
    case Aim:
        return QStringLiteral("aim");
    case Msn:
        return QStringLiteral("msn");
    case Twitter:
        return QStringLiteral("twitter");
    case GoogleTalk:
        return QStringLiteral("googletalk");
    case Yahoo:
        return QStringLiteral("yahoo");
    case Qq:
        return QStringLiteral("qq");
    case GaduGadu:
        return QStringLiteral("gadugadu");
    case Ownclound:
        return QStringLiteral("owncloud-handle");
    case Facebook:
        return QStringLiteral("facebook");
    case Icq:
        return QStringLiteral("icq");
    }
    return QString();
}

QDataStream &KContacts::operator<<(QDataStream &s, const Impp &impp)
{
    return s << impp.d->parameters << impp.d->address << impp.d->type;
}

QDataStream &KContacts::operator>>(QDataStream &s, Impp &impp)
{
    int i;
    s >> impp.d->parameters >> impp.d->address >> i;
    impp.d->type = static_cast<Impp::ImppType>(i);
    return s;
}

