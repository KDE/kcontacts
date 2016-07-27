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

#include "nicknametest.h"
#include "nickname.h"
#include "vcardtool.h"
#include <qtest.h>

NickNameTest::NickNameTest(QObject *parent)
    : QObject(parent)
{

}

NickNameTest::~NickNameTest()
{

}

void NickNameTest::shouldHaveDefaultValue()
{
    KContacts::NickName nickname;
    QVERIFY(!nickname.isValid());
    QVERIFY(nickname.nickname().isEmpty());
    QVERIFY(nickname.parameters().isEmpty());
}

void NickNameTest::shouldAssignValue()
{
    const QString lang(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::NickName nickname(lang);
    nickname.setParameters(params);
    QVERIFY(nickname.isValid());
    QVERIFY(!nickname.nickname().isEmpty());
    QCOMPARE(nickname.nickname(), lang);
    QVERIFY(!nickname.parameters().isEmpty());
    QCOMPARE(nickname.parameters(), params);

}

void NickNameTest::shouldAssignExternal()
{
    KContacts::NickName nickname;
    const QString lang(QStringLiteral("fr"));
    nickname.setNickName(lang);
    QVERIFY(nickname.isValid());
    QCOMPARE(nickname.nickname(), lang);
}

void NickNameTest::shouldSerialized()
{
    KContacts::NickName nickname;
    KContacts::NickName result;
    const QString lang(QStringLiteral("fr"));
    nickname.setNickName(lang);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    nickname.setParameters(params);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << nickname;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(nickname == result);

}

void NickNameTest::shouldEqualNickName()
{
    KContacts::NickName nickname;
    KContacts::NickName result;
    const QString lang(QStringLiteral("fr"));
    nickname.setNickName(lang);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    nickname.setParameters(params);

    result = nickname;
    QVERIFY(nickname == result);
}

QTEST_MAIN(NickNameTest)
