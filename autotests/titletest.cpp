/*
    This file is part of the KContacts framework.
    Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

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

#include "titletest.h"
#include "title.h"
#include "vcardtool_p.h"
#include <QTest>

TitleTest::TitleTest(QObject *parent)
    : QObject(parent)
{
}

TitleTest::~TitleTest()
{
}

void TitleTest::shouldHaveDefaultValue()
{
    KContacts::Title title;
    QVERIFY(!title.isValid());
    QVERIFY(title.title().isEmpty());
    QVERIFY(title.parameters().isEmpty());
}

void TitleTest::shouldAssignValue()
{
    const QString lang(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Title title(lang);
    title.setParameters(params);
    QVERIFY(title.isValid());
    QVERIFY(!title.title().isEmpty());
    QCOMPARE(title.title(), lang);
    QVERIFY(!title.parameters().isEmpty());
    QCOMPARE(title.parameters(), params);
}

void TitleTest::shouldAssignExternal()
{
    KContacts::Title title;
    const QString lang(QStringLiteral("fr"));
    title.setTitle(lang);
    QVERIFY(title.isValid());
    QCOMPARE(title.title(), lang);
}

void TitleTest::shouldSerialized()
{
    KContacts::Title title;
    KContacts::Title result;
    const QString lang(QStringLiteral("fr"));
    title.setTitle(lang);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    title.setParameters(params);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << title;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(title == result);
}

void TitleTest::shouldEqualTitle()
{
    KContacts::Title title;
    KContacts::Title result;
    const QString lang(QStringLiteral("fr"));
    title.setTitle(lang);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    title.setParameters(params);

    result = title;
    QVERIFY(title == result);
}

void TitleTest::shouldParseTwoTitles()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "Title:boo\n"
                         "Title:bla\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).extraTitleList().count(), 2);
    QCOMPARE(lst.at(0).title(), QStringLiteral("boo"));
    QCOMPARE(lst.at(0).extraTitleList().at(0).title(), QStringLiteral("boo"));
    QCOMPARE(lst.at(0).extraTitleList().at(1).title(), QStringLiteral("bla"));
}

void TitleTest::shouldParseTitle()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "Title:boo\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).extraTitleList().count(), 1);
    QCOMPARE(lst.at(0).title(), QStringLiteral("boo"));
    QCOMPARE(lst.at(0).extraTitleList().at(0).title(), QStringLiteral("boo"));
}

void TitleTest::shouldParseWithoutTitle()
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
    QCOMPARE(lst.at(0).extraTitleList().count(), 0);
    QCOMPARE(lst.at(0).title(), QString());
}

void TitleTest::shouldCreateVCard()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Title::List lstTitle;
    KContacts::Title title(QStringLiteral("fr"));
    lstTitle << title;
    addr.setExtraTitleList(lstTitle);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "TITLE:fr\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

void TitleTest::shouldCreateVCardWithTwoTitle()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Title::List lstTitle;
    KContacts::Title title(QStringLiteral("fr"));
    KContacts::Title title2(QStringLiteral("fr2"));
    lstTitle << title << title2;
    addr.setExtraTitleList(lstTitle);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "TITLE:fr\r\n"
                        "TITLE:fr2\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

void TitleTest::shouldCreateVCardWithParameters()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Title::List lstTitle;
    KContacts::Title title(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    title.setParameters(params);
    lstTitle << title;
    addr.setExtraTitleList(lstTitle);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "TITLE;FOO1=bla1,blo1;FOO2=bla2,blo2:fr\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

void TitleTest::shouldGenerateTitleForVCard3()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Title::List lstTitle;
    KContacts::Title title(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    title.setParameters(params);
    lstTitle << title;
    addr.setExtraTitleList(lstTitle);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v3_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:3.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "TITLE;FOO1=bla1,blo1;FOO2=bla2,blo2:fr\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

QTEST_MAIN(TitleTest)
