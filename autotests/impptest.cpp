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

#include "impptest.h"
#include "impp.h"
#include <qtest.h>

ImppTest::ImppTest(QObject *parent)
    : QObject(parent)
{

}

ImppTest::~ImppTest()
{

}

void ImppTest::shouldHaveDefaultValue()
{
    KContacts::Impp impp;
    QVERIFY(!impp.isValid());
    QVERIFY(impp.address().isEmpty());
    QCOMPARE(impp.type(), KContacts::Impp::Unknown);
    QVERIFY(impp.parameters().isEmpty());
}

void ImppTest::shouldAssignValue()
{
    const QString address(QStringLiteral("address"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Impp impp;
    impp.setParameters(params);
    impp.setType(KContacts::Impp::Icq);
    impp.setAddress(address);
    QVERIFY(impp.isValid());
    QVERIFY(!impp.address().isEmpty());
    QCOMPARE(impp.address(), address);
    QCOMPARE(impp.type(), KContacts::Impp::Icq);
    QVERIFY(!impp.parameters().isEmpty());
    QCOMPARE(impp.parameters(), params);
}

void ImppTest::shouldSerialized()
{
    const QString address(QStringLiteral("address"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Impp impp;
    impp.setParameters(params);
    impp.setType(KContacts::Impp::Icq);
    impp.setAddress(address);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << impp;

    KContacts::Impp result;
    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(impp == result);
}

void ImppTest::shouldEqualLanguage()
{
    const QString address(QStringLiteral("address"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Impp impp;
    impp.setParameters(params);
    impp.setType(KContacts::Impp::Icq);
    impp.setAddress(address);


    KContacts::Impp result(impp);
    QVERIFY(impp == result);
}

QTEST_MAIN(ImppTest)
