/*
    This file is part of the KContacts framework..
    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>

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

#include "secrecy.h"

#include <klocalizedstring.h>

#include <QtCore/QDataStream>
#include <QtCore/QSharedData>

using namespace KContacts;

class Secrecy::PrivateData : public QSharedData
{
public:
    PrivateData()
        : mType(Secrecy::Invalid)
    {
    }

    PrivateData(const PrivateData &other)
        : QSharedData(other)
    {
        mType = other.mType;
    }

    Type mType;
};

Secrecy::Secrecy(Type type)
    : d(new PrivateData)
{
    d->mType = type;
}

Secrecy::Secrecy(const Secrecy &other)
    : d(other.d)
{
}

Secrecy::~Secrecy()
{
}

Secrecy &Secrecy::operator=(const Secrecy &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

bool Secrecy::operator==(const Secrecy &other) const
{
    return d->mType == other.d->mType;
}

bool Secrecy::operator!=(const Secrecy &other) const
{
    return !(*this == other);
}

bool Secrecy::isValid() const
{
    return d->mType != Invalid;
}

void Secrecy::setType(Type type)
{
    d->mType = type;
}

Secrecy::Type Secrecy::type() const
{
    return d->mType;
}

Secrecy::TypeList Secrecy::typeList()
{
    static TypeList list;

    if (list.isEmpty()) {
        list << Public << Private << Confidential;
    }

    return list;
}

QString Secrecy::typeLabel(Type type)
{
    switch (type) {
    case Public:
        return i18nc("access is for everyone", "Public");
        break;
    case Private:
        return i18nc("access is by owner only", "Private");
        break;
    case Confidential:
        return i18nc("access is by owner and a controlled group", "Confidential");
        break;
    default:
        return i18nc("unknown secrecy type", "Unknown type");
        break;
    }
}

QString Secrecy::toString() const
{
    QString str;

    str += QString::fromLatin1("Secrecy {\n");
    str += QString::fromLatin1("  Type: %1\n").arg(typeLabel(d->mType));
    str += QString::fromLatin1("}\n");

    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Secrecy &secrecy)
{
    return s << (uint)secrecy.d->mType;
}

QDataStream &KContacts::operator>>(QDataStream &s, Secrecy &secrecy)
{
    uint type;
    s >> type;

    switch (type) {
    case 0:
        secrecy.d->mType = Secrecy::Public;
        break;
    case 1:
        secrecy.d->mType = Secrecy::Private;
        break;
    case 2:
        secrecy.d->mType = Secrecy::Confidential;
        break;
    default:
        secrecy.d->mType = Secrecy::Invalid;
        break;
    }

    return s;
}
