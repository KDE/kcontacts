/*
    This file is part of the KContacts framework.
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

#include "relatedtest.h"
#include "related.h"
#include "vcardtool.h"
#include <qtest.h>

RelatedTest::RelatedTest(QObject *parent)
    : QObject(parent)
{

}

RelatedTest::~RelatedTest()
{

}

void RelatedTest::shouldHaveDefaultValue()
{
    KContacts::Related related;
    QVERIFY(!related.isValid());
    QVERIFY(related.related().isEmpty());
    QVERIFY(related.parameters().isEmpty());
}

void RelatedTest::shouldAssignValue()
{
    const QString relatedTo(QStringLiteral("friend"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Related related(relatedTo);
    related.setParameters(params);
    QVERIFY(related.isValid());
    QVERIFY(!related.related().isEmpty());
    QCOMPARE(related.related(), relatedTo);
    QVERIFY(!related.parameters().isEmpty());
    QCOMPARE(related.parameters(), params);

}

void RelatedTest::shouldAssignExternal()
{
    KContacts::Related related;
    const QString relatedTo(QStringLiteral("friend"));
    related.setRelated(relatedTo);
    QVERIFY(related.isValid());
    QCOMPARE(related.related(), relatedTo);
}

void RelatedTest::shouldSerialized()
{
    KContacts::Related related;
    KContacts::Related result;
    const QString relatedTo(QStringLiteral("friend"));
    related.setRelated(relatedTo);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    related.setParameters(params);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << related;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(related == result);
}

void RelatedTest::shouldEqualRelated()
{
    KContacts::Related related;
    KContacts::Related result;
    const QString relatedTo(QStringLiteral("friend"));
    related.setRelated(relatedTo);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    related.setParameters(params);

    result = related;
    QVERIFY(related == result);
}

void RelatedTest::shouldParseRelated()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:4.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "RELATED:friend"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).relationShips().count(), 1);
}

void RelatedTest::shouldParseWithoutRelated()
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
    QCOMPARE(lst.at(0).relationShips().count(), 0);
}

void RelatedTest::shouldCreateVCard4()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Related::List lstRelated;
    KContacts::Related related(QStringLiteral("friend"));
    lstRelated << related;
    addr.setRelationShips(lstRelated);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "RELATED:friend\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

void RelatedTest::shouldCreateVCardWithTwoRelated()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Related::List lstRelated;
    KContacts::Related related(QStringLiteral("friend"));
    KContacts::Related related2(QStringLiteral("kde"));
    lstRelated << related << related2;
    addr.setRelationShips(lstRelated);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "RELATED:friend\r\n"
                        "RELATED:kde\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);

}

void RelatedTest::shouldCreateVCardWithParameters()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Related::List lstRelated;
    KContacts::Related related(QStringLiteral("friend"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    related.setParameters(params);
    lstRelated << related;
    addr.setRelationShips(lstRelated);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "RELATED;FOO1=bla1,blo1;FOO2=bla2,blo2:friend\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

void RelatedTest::shouldNotExportInVcard3()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Related::List lstRelated;
    KContacts::Related related(QStringLiteral("friend"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    related.setParameters(params);
    lstRelated << related;
    addr.setRelationShips(lstRelated);
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

QTEST_MAIN(RelatedTest)
