/*
    This file is part of the KContacts framework.
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

class Q_DECL_HIDDEN PhoneNumber::Private : public QSharedData
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
    QString mNumber;
    Type mType;
    QMap<QString, QStringList> mParameters;
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

    if (d->mParameters != other.d->mParameters) {
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
             << Bbs << Modem << Car << Isdn << Pcs << Pager << Undefined;
    }

    return list;
}

QString PhoneNumber::typeFlagLabel(TypeFlag type)
{
    switch (type) {
    case Undefined:
        return i18nc("Undefined phone type", "Telephone number");
    case Home:
        return i18nc("Home phone", "Home");
    case Work:
        return i18nc("Work phone", "Work");
    case Msg:
        return i18n("Messenger");
    case Pref:
        return i18nc("Preferred phone", "Preferred");
    case Voice:
        return i18n("Voice");
    case Fax:
        return i18n("Fax");
    case Cell:
        return i18nc("Mobile Phone", "Mobile");
    case Video:
        return i18nc("Video phone", "Video");
    case Bbs:
        return i18n("Mailbox");
    case Modem:
        return i18n("Modem");
    case Car:
        return i18nc("Car Phone", "Car");
    case Isdn:
        return i18n("ISDN");
    case Pcs:
        return i18n("PCS");
    case Pager:
        return i18n("Pager");
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
    TypeList::ConstIterator end(list.constEnd());
    for (it = list.constBegin(); it != end; ++it) {
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
    QString str = QLatin1String("PhoneNumber {\n");
    str += QStringLiteral("    Id: %1\n").arg(d->mId);
    str += QStringLiteral("    Type: %1\n").arg(typeLabel(d->mType));
    if (!d->mParameters.isEmpty()) {
        QString param;
        QMap<QString, QStringList>::const_iterator it = d->mParameters.constBegin();
        const QMap<QString, QStringList>::const_iterator end = d->mParameters.constEnd();
        while (it != end) {
            param += QStringLiteral("%1 %2").arg(it.key(), it.value().join(QLatin1Char(',')));
            it++;
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QStringLiteral("    Number: %1\n").arg(d->mNumber);
    str += QLatin1String("}\n");

    return str;
}

void PhoneNumber::setParameters(const QMap<QString, QStringList> &params)
{
    d->mParameters = params;
}

QMap<QString, QStringList> PhoneNumber::parameters() const
{
    return d->mParameters;
}

QDataStream &KContacts::operator<<(QDataStream &s, const PhoneNumber &phone)
{
    return s << phone.d->mId << (uint)phone.d->mType << phone.d->mNumber << phone.d->mParameters;
}

QDataStream &KContacts::operator>>(QDataStream &s, PhoneNumber &phone)
{
    uint type;
    s >> phone.d->mId >> type >> phone.d->mNumber >> phone.d->mParameters;
    phone.d->mType = PhoneNumber::Type(type);

    return s;
}
