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

#include "clientpidmaptest.h"
#include "clientpidmap.h"
#include "vcardtool.h"
#include <qtest.h>

ClientPidMapTest::ClientPidMapTest(QObject *parent)
    : QObject(parent)
{

}

ClientPidMapTest::~ClientPidMapTest()
{

}

void ClientPidMapTest::shouldHaveDefaultValue()
{
    KContacts::ClientPidMap role;
    QVERIFY(!role.isValid());
    QVERIFY(role.clientPidMap().isEmpty());
    QVERIFY(role.parameters().isEmpty());
}

void ClientPidMapTest::shouldAssignValue()
{
    const QString lang(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::ClientPidMap role(lang);
    role.setParameters(params);
    QVERIFY(role.isValid());
    QVERIFY(!role.clientPidMap().isEmpty());
    QCOMPARE(role.clientPidMap(), lang);
    QVERIFY(!role.parameters().isEmpty());
    QCOMPARE(role.parameters(), params);

}

void ClientPidMapTest::shouldAssignExternal()
{
    KContacts::ClientPidMap role;
    const QString lang(QStringLiteral("fr"));
    role.setClientPidMap(lang);
    QVERIFY(role.isValid());
    QCOMPARE(role.clientPidMap(), lang);
}

void ClientPidMapTest::shouldSerialized()
{
    KContacts::ClientPidMap role;
    KContacts::ClientPidMap result;
    const QString lang(QStringLiteral("fr"));
    role.setClientPidMap(lang);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    role.setParameters(params);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << role;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(role == result);

}

void ClientPidMapTest::shouldEqualClientPidMap()
{
    KContacts::ClientPidMap role;
    KContacts::ClientPidMap result;
    const QString lang(QStringLiteral("fr"));
    role.setClientPidMap(lang);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    role.setParameters(params);

    result = role;
    QVERIFY(role == result);
}

void ClientPidMapTest::shouldParseClientPidMap()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "CLIENTPIDMAP:boo\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).clientPidMapList().count(), 1);
    QCOMPARE(lst.at(0).clientPidMapList().at(0).clientPidMap(), QStringLiteral("boo"));
}

void ClientPidMapTest::shouldParseWithoutClientPidMap()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).clientPidMapList().count(), 0);
}

void ClientPidMapTest::shouldCreateVCard()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::ClientPidMap::List lstClientPidMap;
    KContacts::ClientPidMap clientpidmap(QStringLiteral("fr"));
    lstClientPidMap << clientpidmap;
    addr.setClientPidMapList(lstClientPidMap);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "CLIENTPIDMAP:fr\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

void ClientPidMapTest::shouldCreateVCardWithTwoClientPidMap()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::ClientPidMap::List lstClientPidMap;
    KContacts::ClientPidMap clientpidmap(QStringLiteral("fr"));
    KContacts::ClientPidMap clientpidmap2(QStringLiteral("fr2"));
    lstClientPidMap << clientpidmap << clientpidmap2;
    addr.setClientPidMapList(lstClientPidMap);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "CLIENTPIDMAP:fr\r\n"
                        "CLIENTPIDMAP:fr2\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);

}
void ClientPidMapTest::shouldCreateVCardWithParameters()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::ClientPidMap::List lstClientPidMap;
    KContacts::ClientPidMap clientpidmap(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    clientpidmap.setParameters(params);
    lstClientPidMap << clientpidmap;
    addr.setClientPidMapList(lstClientPidMap);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "CLIENTPIDMAP;FOO1=bla1,blo1;FOO2=bla2,blo2:fr\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

void ClientPidMapTest::shouldGenerateClientPidMapForVCard3()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::ClientPidMap::List lstClientPidMap;
    KContacts::ClientPidMap clientpidmap(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    clientpidmap.setParameters(params);
    lstClientPidMap << clientpidmap;
    addr.setClientPidMapList(lstClientPidMap);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v3_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:3.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

QTEST_MAIN(ClientPidMapTest)
