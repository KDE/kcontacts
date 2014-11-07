/*
    This file is part of the KContacts framework..
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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

#include "phonenumber.h"

#include <klocalizedstring.h>
#include <krandom.h>

#include <QtCore/QDataStream>
#include <QtCore/QSharedData>

using namespace KContacts;

static QString cleanupNumber(const QString &input)
{
    return input.simplified();
}

class PhoneNumber::Private : public QSharedData
{
public:
    Private(Type type)
        : mId(KRandom::randomString(8)), mType(type)
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mId = other.mId;
        mType = other.mType;
        mNumber = other.mNumber;
    }

    QString mId;
    Type mType;
    QString mNumber;
};

PhoneNumber::PhoneNumber()
    : d(new Private(Home))
{
}

PhoneNumber::PhoneNumber(const QString &number, Type type)
    : d(new Private(type))
{
    d->mNumber = cleanupNumber(number);
}

PhoneNumber::PhoneNumber(const PhoneNumber &other)
    : d(other.d)
{
}

PhoneNumber::~PhoneNumber()
{
}

bool PhoneNumber::operator==(const PhoneNumber &other) const
{
    if (d->mId != other.d->mId) {
        return false;
    }

    if (d->mNumber != other.d->mNumber) {
        return false;
    }

    if (d->mType != other.d->mType) {
        return false;
    }

    return true;
}

bool PhoneNumber::operator!=(const PhoneNumber &other) const
{
    return !(other == *this);
}

PhoneNumber &PhoneNumber::operator=(const PhoneNumber &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

bool PhoneNumber::isEmpty() const
{
    return d->mNumber.isEmpty();
}

void PhoneNumber::setId(const QString &id)
{
    d->mId = id;
}

QString PhoneNumber::id() const
{
    return d->mId;
}

void PhoneNumber::setNumber(const QString &number)
{
    d->mNumber = cleanupNumber(number);
}

QString PhoneNumber::number() const
{
    return d->mNumber;
}

void PhoneNumber::setType(Type type)
{
    d->mType = type;
}

PhoneNumber::Type PhoneNumber::type() const
{
    return d->mType;
}

QString PhoneNumber::typeLabel() const
{
    return typeLabel(type());
}

PhoneNumber::TypeList PhoneNumber::typeList()
{
    static TypeList list;

    if (list.isEmpty()) {
        list << Home << Work << Msg << Pref << Voice << Fax << Cell << Video
             << Bbs << Modem << Car << Isdn << Pcs << Pager;
    }

    return list;
}

QString PhoneNumber::typeFlagLabel(TypeFlag type)
{
    switch (type) {
    case Home:
        return i18nc("Home phone", "Home");
        break;
    case Work:
        return i18nc("Work phone", "Work");
        break;
    case Msg:
        return i18n("Messenger");
        break;
    case Pref:
        return i18nc("Preferred phone", "Preferred");
        break;
    case Voice:
        return i18n("Voice");
        break;
    case Fax:
        return i18n("Fax");
        break;
    case Cell:
        return i18nc("Mobile Phone", "Mobile");
        break;
    case Video:
        return i18nc("Video phone", "Video");
        break;
    case Bbs:
        return i18n("Mailbox");
        break;
    case Modem:
        return i18n("Modem");
        break;
    case Car:
        return i18nc("Car Phone", "Car");
        break;
    case Isdn:
        return i18n("ISDN");
        break;
    case Pcs:
        return i18n("PCS");
        break;
    case Pager:
        return i18n("Pager");
        break;
    default:
        return i18nc("another type of phone", "Other");
    }
}

QString PhoneNumber::typeLabel(Type type)
{
    QString label;
    bool first = true;

    // special cases
    // Pref stand alone -> Preferred Number
    // Home+Fax or Work+Fax -> combine as initial string
    if (type == Pref) {
        return i18n("Preferred Number");
    }

    if (type & Fax) {
        if (type & Home) {
            label = i18n("Home Fax");
            first = false;
            type &= ~Fax;
            type &= ~Home;
        } else if (type & Work) {
            label = i18n("Work Fax");
            first = false;
            type &= ~Fax;
            type &= ~Work;
        }
    }

    const TypeList list = typeList();

    TypeList::ConstIterator it;
    TypeList::ConstIterator end(list.end());
    for (it = list.begin(); it != end; ++it) {
        // these are actually flags
        const TypeFlag flag = static_cast<TypeFlag>(static_cast<int>(*it));
        if (type & flag) {
            if (!first) {
                label.append(QLatin1Char('/'));
            }

            label.append(typeFlagLabel(flag));

            if (first) {
                first = false;
            }
        }
    }

    return label;
}

QString PhoneNumber::toString() const
{
    QString str;

    str += QString::fromLatin1("PhoneNumber {\n");
    str += QString::fromLatin1("    Id: %1\n").arg(d->mId);
    str += QString::fromLatin1("    Type: %1\n").arg(typeLabel(d->mType));
    str += QString::fromLatin1("    Number: %1\n").arg(d->mNumber);
    str += QString::fromLatin1("}\n");

    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const PhoneNumber &phone)
{
    return s << phone.d->mId << (uint)phone.d->mType << phone.d->mNumber;
}

QDataStream &KContacts::operator>>(QDataStream &s, PhoneNumber &phone)
{
    uint type;
    s >> phone.d->mId >> type >> phone.d->mNumber;
    phone.d->mType = PhoneNumber::Type(type);

    return s;
}
