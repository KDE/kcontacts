/*
    This file is part of the KContacts framework.
    Copyright (c) 2007 Tobias Koenig <tokoe@kde.org>

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

#include "phonenumbertest.h"
#include "kcontacts/phonenumber.h"
#include <qtest.h>

QTEST_MAIN(PhoneNumberTest)

void PhoneNumberTest::emptyTest()
{
    KContacts::PhoneNumber number;

    QVERIFY(number.isEmpty());
}

void PhoneNumberTest::storeTest()
{
    KContacts::PhoneNumber number;

    number.setId(QLatin1String("My Id"));
    number.setType(KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell);
    number.setNumber(QLatin1String("2734826345"));

    QVERIFY(number.id() == QLatin1String("My Id"));
    QVERIFY(number.type() == (KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell));
    QVERIFY(number.number() == QLatin1String("2734826345"));
}

void PhoneNumberTest::equalsTest()
{
    KContacts::PhoneNumber number1, number2;

    number1.setId(QLatin1String("My Id"));
    number1.setType(KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell);
    number1.setNumber(QLatin1String("2734826345"));

    number2.setId(QLatin1String("My Id"));
    number2.setType(KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell);
    number2.setNumber(QLatin1String("2734826345"));

    QVERIFY(number1 == number2);
}

void PhoneNumberTest::differsTest()
{
    KContacts::PhoneNumber number1(QLatin1String("123"), KContacts::PhoneNumber::Home);
    KContacts::PhoneNumber number2(QLatin1String("123"), KContacts::PhoneNumber::Work);

    QVERIFY(number1 != number2);
}

void PhoneNumberTest::assignmentTest()
{
    KContacts::PhoneNumber number1, number2;

    number1.setId(QLatin1String("My Id"));
    number1.setType(KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell);
    number1.setNumber(QLatin1String("2734826345"));

    number1 = number2;

    QVERIFY(number1 == number2);
}

void PhoneNumberTest::serializeTest()
{
    KContacts::PhoneNumber number1, number2;

    number1.setId(QLatin1String("My Id"));
    number1.setType(KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Cell);
    number1.setNumber(QLatin1String("2734826345"));

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << number1;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> number2;

    QVERIFY(number1 == number2);
}

void PhoneNumberTest::labelTest()
{
    QMap<KContacts::PhoneNumber::Type, QString> labels;

    const KContacts::PhoneNumber::TypeList types = KContacts::PhoneNumber::typeList();

    // check all types standalone
    Q_FOREACH (KContacts::PhoneNumber::Type type, types) {
        const KContacts::PhoneNumber phone(QLatin1String("1"), type);
        QCOMPARE(phone.type(), type);

        // Pref is special cased
        if (type != KContacts::PhoneNumber::Pref) {
            QCOMPARE(phone.typeLabel(), KContacts::PhoneNumber::typeFlagLabel((KContacts::PhoneNumber::TypeFlag)(int)type));
            labels.insert(type, phone.typeLabel());
        } else {
            labels.insert(type, KContacts::PhoneNumber::typeFlagLabel((KContacts::PhoneNumber::TypeFlag)(int)type));
        }
        QCOMPARE(KContacts::PhoneNumber::typeLabel(type), phone.typeLabel());
    }

    // combine all with Pref
    Q_FOREACH (KContacts::PhoneNumber::Type type, types) {
        KContacts::PhoneNumber::Type combinedType = type | KContacts::PhoneNumber::Pref;
        const KContacts::PhoneNumber phone(QLatin1String("1"), combinedType);
        QCOMPARE(phone.type(), combinedType);
        QCOMPARE(KContacts::PhoneNumber::typeLabel(combinedType), phone.typeLabel());

        if (type < KContacts::PhoneNumber::Pref) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[type]).arg(labels[KContacts::PhoneNumber::Pref]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        } else if (type > KContacts::PhoneNumber::Pref) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[KContacts::PhoneNumber::Pref]).arg(labels[type]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        }
    }

    // combine all with Fax
    Q_FOREACH (KContacts::PhoneNumber::Type type, types) {
        KContacts::PhoneNumber::Type combinedType = type | KContacts::PhoneNumber::Fax;
        const KContacts::PhoneNumber phone(QLatin1String("1"), combinedType);
        QCOMPARE(phone.type(), combinedType);
        QCOMPARE(KContacts::PhoneNumber::typeLabel(combinedType), phone.typeLabel());

        if (type == KContacts::PhoneNumber::Home ||
            type == KContacts::PhoneNumber::Work) {
            // special cased
        } else if (type < KContacts::PhoneNumber::Fax) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[type]).arg(labels[KContacts::PhoneNumber::Fax]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        } else if (type > KContacts::PhoneNumber::Fax) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[KContacts::PhoneNumber::Fax]).arg(labels[type]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        }
    }

    // special cases
    QCOMPARE(KContacts::PhoneNumber::typeLabel(KContacts::PhoneNumber::Pref), QLatin1String("Preferred Number"));
    QCOMPARE(KContacts::PhoneNumber::typeLabel(KContacts::PhoneNumber::Home |
                                          KContacts::PhoneNumber::Fax),
             QLatin1String("Home Fax"));
    QCOMPARE(KContacts::PhoneNumber::typeLabel(KContacts::PhoneNumber::Work |
                                          KContacts::PhoneNumber::Fax),
             QLatin1String("Work Fax"));
    QCOMPARE(KContacts::PhoneNumber::typeLabel(KContacts::PhoneNumber::Home |
                                          KContacts::PhoneNumber::Fax  |
                                          KContacts::PhoneNumber::Pref),
             QLatin1String("Home Fax/Preferred"));
    QCOMPARE(KContacts::PhoneNumber::typeLabel(KContacts::PhoneNumber::Work |
                                          KContacts::PhoneNumber::Fax |
                                          KContacts::PhoneNumber::Pref),
             QLatin1String("Work Fax/Preferred"));
}
