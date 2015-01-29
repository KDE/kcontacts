/*
    This file is part of libkabc.
    Copyright (c) 2015 Laurent Montel <montel@kde.org>

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

#include "gendertest.h"
#include "gender.h"
#include <qtest.h>

GenderTest::GenderTest(QObject *parent)
    : QObject(parent)
{

}

GenderTest::~GenderTest()
{

}

void GenderTest::shouldHaveDefaultValue()
{
    KContacts::Gender gender;
    QVERIFY(!gender.isValid());
    QVERIFY(gender.gender().isEmpty());
    QVERIFY(gender.comment().isEmpty());
}

void GenderTest::shouldAssignValue()
{
    const QString genderStr(QLatin1String("F"));
    KContacts::Gender gender(genderStr);
    const QString commentStr(QLatin1String("foo"));
    gender.setComment(commentStr);
    QVERIFY(gender.isValid());
    QVERIFY(!gender.gender().isEmpty());
    QCOMPARE(gender.gender(), genderStr);
    QVERIFY(!gender.comment().isEmpty());
    QCOMPARE(gender.comment(), commentStr);
}

void GenderTest::shouldAssignExternal()
{
    KContacts::Gender gender;
    const QString genderStr(QLatin1String("H"));
    gender.setGender(genderStr);
    QVERIFY(gender.isValid());
    QVERIFY(!gender.gender().isEmpty());
    QCOMPARE(gender.gender(), genderStr);
}

void GenderTest::shouldSerialized()
{
    KContacts::Gender gender;
    KContacts::Gender result;
    const QString genderStr(QLatin1String("H"));
    gender.setGender(genderStr);
    gender.setComment(QLatin1String("foo"));


    QByteArray data;
    QDataStream s( &data, QIODevice::WriteOnly );
    s << gender;

    QDataStream t( &data, QIODevice::ReadOnly );
    t >> result;

    QVERIFY( gender == result );
}

void GenderTest::shouldEqualGender()
{
    KContacts::Gender gender;
    KContacts::Gender result;
    const QString genderStr(QLatin1String("H"));
    gender.setGender(genderStr);
    gender.setComment(QLatin1String("foo"));

    result = gender;
    QVERIFY( gender == result );
}

QTEST_MAIN(GenderTest)
