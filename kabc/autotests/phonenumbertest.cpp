/*
    This file is part of libkabc.
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
#include "kabc/phonenumber.h"
#include <qtest.h>

QTEST_MAIN(PhoneNumberTest)

void PhoneNumberTest::emptyTest()
{
    KABC::PhoneNumber number;

    QVERIFY(number.isEmpty());
}

void PhoneNumberTest::storeTest()
{
    KABC::PhoneNumber number;

    number.setId(QLatin1String("My Id"));
    number.setType(KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell);
    number.setNumber(QLatin1String("2734826345"));

    QVERIFY(number.id() == QLatin1String("My Id"));
    QVERIFY(number.type() == (KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell));
    QVERIFY(number.number() == QLatin1String("2734826345"));
}

void PhoneNumberTest::equalsTest()
{
    KABC::PhoneNumber number1, number2;

    number1.setId(QLatin1String("My Id"));
    number1.setType(KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell);
    number1.setNumber(QLatin1String("2734826345"));

    number2.setId(QLatin1String("My Id"));
    number2.setType(KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell);
    number2.setNumber(QLatin1String("2734826345"));

    QVERIFY(number1 == number2);
}

void PhoneNumberTest::differsTest()
{
    KABC::PhoneNumber number1(QLatin1String("123"), KABC::PhoneNumber::Home);
    KABC::PhoneNumber number2(QLatin1String("123"), KABC::PhoneNumber::Work);

    QVERIFY(number1 != number2);
}

void PhoneNumberTest::assignmentTest()
{
    KABC::PhoneNumber number1, number2;

    number1.setId(QLatin1String("My Id"));
    number1.setType(KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell);
    number1.setNumber(QLatin1String("2734826345"));

    number1 = number2;

    QVERIFY(number1 == number2);
}

void PhoneNumberTest::serializeTest()
{
    KABC::PhoneNumber number1, number2;

    number1.setId(QLatin1String("My Id"));
    number1.setType(KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell);
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
    QMap<KABC::PhoneNumber::Type, QString> labels;

    const KABC::PhoneNumber::TypeList types = KABC::PhoneNumber::typeList();

    // check all types standalone
    Q_FOREACH (KABC::PhoneNumber::Type type, types) {
        const KABC::PhoneNumber phone(QLatin1String("1"), type);
        QCOMPARE(phone.type(), type);

        // Pref is special cased
        if (type != KABC::PhoneNumber::Pref) {
            QCOMPARE(phone.typeLabel(), KABC::PhoneNumber::typeFlagLabel((KABC::PhoneNumber::TypeFlag)(int)type));
            labels.insert(type, phone.typeLabel());
        } else {
            labels.insert(type, KABC::PhoneNumber::typeFlagLabel((KABC::PhoneNumber::TypeFlag)(int)type));
        }
        QCOMPARE(KABC::PhoneNumber::typeLabel(type), phone.typeLabel());
    }

    // combine all with Pref
    Q_FOREACH (KABC::PhoneNumber::Type type, types) {
        KABC::PhoneNumber::Type combinedType = type | KABC::PhoneNumber::Pref;
        const KABC::PhoneNumber phone(QLatin1String("1"), combinedType);
        QCOMPARE(phone.type(), combinedType);
        QCOMPARE(KABC::PhoneNumber::typeLabel(combinedType), phone.typeLabel());

        if (type < KABC::PhoneNumber::Pref) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[ type ]).arg(labels[ KABC::PhoneNumber::Pref ]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        } else if (type > KABC::PhoneNumber::Pref) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[ KABC::PhoneNumber::Pref ]).arg(labels[ type ]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        }
    }

    // combine all with Fax
    Q_FOREACH (KABC::PhoneNumber::Type type, types) {
        KABC::PhoneNumber::Type combinedType = type | KABC::PhoneNumber::Fax;
        const KABC::PhoneNumber phone(QLatin1String("1"), combinedType);
        QCOMPARE(phone.type(), combinedType);
        QCOMPARE(KABC::PhoneNumber::typeLabel(combinedType), phone.typeLabel());

        if (type == KABC::PhoneNumber::Home ||
                type == KABC::PhoneNumber::Work) {
            // special cased
        } else if (type < KABC::PhoneNumber::Fax) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[ type ]).arg(labels[ KABC::PhoneNumber::Fax ]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        } else if (type > KABC::PhoneNumber::Fax) {
            const QString expectedCombinedString = QString::fromLatin1("%1/%2").arg(labels[ KABC::PhoneNumber::Fax ]).arg(labels[ type ]);
            QCOMPARE(phone.typeLabel(), expectedCombinedString);
        }
    }

    // special cases
    QCOMPARE(KABC::PhoneNumber::typeLabel(KABC::PhoneNumber::Pref), QLatin1String("Preferred Number"));
    QCOMPARE(KABC::PhoneNumber::typeLabel(KABC::PhoneNumber::Home |
                                          KABC::PhoneNumber::Fax),
             QLatin1String("Home Fax"));
    QCOMPARE(KABC::PhoneNumber::typeLabel(KABC::PhoneNumber::Work |
                                          KABC::PhoneNumber::Fax),
             QLatin1String("Work Fax"));
    QCOMPARE(KABC::PhoneNumber::typeLabel(KABC::PhoneNumber::Home |
                                          KABC::PhoneNumber::Fax  |
                                          KABC::PhoneNumber::Pref),
             QLatin1String("Home Fax/Preferred"));
    QCOMPARE(KABC::PhoneNumber::typeLabel(KABC::PhoneNumber::Work |
                                          KABC::PhoneNumber::Fax |
                                          KABC::PhoneNumber::Pref),
             QLatin1String("Work Fax/Preferred"));
}

