/*
    This file is part of the KContacts framework.
    Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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
#include "emailtest.h"
#include "email.h"
#include <QTest>
#include "vcardtool.h"

EmailTest::EmailTest(QObject *parent)
    : QObject(parent)
{
}

EmailTest::~EmailTest()
{
}

void EmailTest::shouldHaveDefaultValue()
{
    KContacts::Email email;
    QVERIFY(!email.isValid());
    QVERIFY(email.mail().isEmpty());
    QVERIFY(email.parameters().isEmpty());
}

void EmailTest::shouldAssignValue()
{
    const QString mail(QStringLiteral("foo@kde.org"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Email email(mail);
    email.setParameters(params);
    QVERIFY(email.isValid());
    QVERIFY(!email.mail().isEmpty());
    QCOMPARE(email.mail(), mail);
    QVERIFY(!email.parameters().isEmpty());
    QCOMPARE(email.parameters(), params);
}

void EmailTest::shouldAssignExternal()
{
    KContacts::Email email;
    const QString mail(QStringLiteral("foo@kde.org"));
    email.setEmail(mail);
    QVERIFY(email.isValid());
    QVERIFY(!email.mail().isEmpty());
    QCOMPARE(email.mail(), mail);
}

void EmailTest::shouldSerialized()
{
    KContacts::Email email;
    KContacts::Email result;
    const QString mail(QStringLiteral("foo@kde.org"));
    email.setEmail(mail);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    email.setParameters(params);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << email;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(email == result);
}

void EmailTest::shouldEqualEmail()
{
    KContacts::Email email;
    KContacts::Email result;
    const QString mail(QStringLiteral("foo@kde.org"));
    email.setEmail(mail);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    email.setParameters(params);

    result = email;
    QVERIFY(email == result);
}

void EmailTest::shouldParseEmailVCard()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "EMAIL;TYPE=HOME,PREF;X-EVOLUTION-UI-SLOT=2:foo@foo.com\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QVERIFY(!lst.at(0).emailList().isEmpty());
    QCOMPARE(lst.at(0).emailList().count(), 1);
    KContacts::Email email = lst.at(0).emailList().at(0);
    QCOMPARE(email.mail(), QStringLiteral("foo@foo.com"));
    QCOMPARE(email.parameters().count(), 2);
}

void EmailTest::shouldParseEmailVCardWithMultiEmails()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "EMAIL;TYPE=HOME,PREF;X-EVOLUTION-UI-SLOT=2:foo@foo.com\n"
                         "EMAIL;TYPE=HOME,PREF;X-EVOLUTION-UI-SLOT=2:bla@bla.com\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QVERIFY(!lst.at(0).emailList().isEmpty());
    QCOMPARE(lst.at(0).emailList().count(), 2);
    KContacts::Email email = lst.at(0).emailList().at(0);
    QCOMPARE(email.mail(), QStringLiteral("bla@bla.com"));
    email = lst.at(0).emailList().at(1);
    QCOMPARE(email.mail(), QStringLiteral("foo@foo.com"));

    QCOMPARE(email.parameters().count(), 2);
}

void EmailTest::shouldParseEmailVCardWithoutEmail()
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
    QVERIFY(lst.at(0).emailList().isEmpty());
}

void EmailTest::shouldExportVcard()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "EMAIL:bla@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

QTEST_MAIN(EmailTest)
