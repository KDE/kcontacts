/*
    This file is part of libkabc.
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

#include "impptest.h"
#include "impp.h"
#include <QTest>
#include "vcardtool.h"
Q_DECLARE_METATYPE(KContacts::Impp::ImppType)

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

void ImppTest::shouldEqualImpp()
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

void ImppTest::shouldParseWithoutImpp()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "LANG:fr"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).imppList().count(), 0);
}

void ImppTest::shouldParseImpp()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "IMPP;X-SERVICE-TYPE=skype:skype:xxxxxxxx\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).imppList().count(), 1);
    KContacts::Impp impp = lst.at(0).imppList().at(0);
    QCOMPARE(impp.address(), QStringLiteral("skype:xxxxxxxx"));
    QCOMPARE(impp.type(), KContacts::Impp::Skype);
}

void ImppTest::shouldParseImppVcard4()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:4.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "IMPP:skype:xxxxxxxx\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).imppList().count(), 1);
    KContacts::Impp impp = lst.at(0).imppList().at(0);
    QCOMPARE(impp.address(), QStringLiteral("xxxxxxxx"));
    QCOMPARE(impp.type(), KContacts::Impp::Skype);
}

QByteArray createCard(KContacts::Impp::ImppType type)
{
    QByteArray expected("BEGIN:VCARD\n"
                        "VERSION:3.0\n"
                        "EMAIL:foo@kde.org\n");
    if (type != KContacts::Impp::Unknown) {
        const QByteArray baType = KContacts::Impp::typeToString(type).toLatin1();
        expected += "IMPP;X-SERVICE-TYPE=" + baType + ":" + baType + ":address\n";
    }
    expected += QByteArray(
        "N:;;;;\n"
        "UID:testuid\n"
        "END:VCARD\n\n");
    return expected;
}

void ImppTest::shouldParseServiceType_data()
{
    QTest::addColumn<KContacts::Impp::ImppType>("type");
    QTest::addColumn<bool>("hasImpp");
    QTest::newRow("withoutimpp") << KContacts::Impp::Unknown << false;
    QTest::newRow("skype") << KContacts::Impp::Skype << true;
    for (int i = KContacts::Impp::Unknown + 1; i < KContacts::Impp::EndList; ++i) {
        KContacts::Impp::ImppType type = static_cast<KContacts::Impp::ImppType>(i);
        QTest::newRow(KContacts::Impp::typeToString(type).toLatin1().constData()) << type << true;
    }
}

void ImppTest::shouldParseServiceType()
{
    QFETCH(KContacts::Impp::ImppType, type);
    QFETCH(bool, hasImpp);

    QByteArray vcarddata = createCard(type);

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);

    QCOMPARE(!lst.at(0).imppList().isEmpty(), hasImpp);
    if (hasImpp) {
        KContacts::Impp impp = lst.at(0).imppList().at(0);
        QCOMPARE(impp.address(), QStringLiteral("%1:address").arg(KContacts::Impp::typeToString(type)));
        QCOMPARE(impp.type(), type);
    }
}

QByteArray expectedVcard(KContacts::Impp::ImppType type)
{
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n");
    if (type != KContacts::Impp::Unknown) {
        const QByteArray baType = KContacts::Impp::typeToString(type).toLatin1();
        expected += "IMPP;X-SERVICE-TYPE=" + baType + ":address\r\n";
    }

    expected += ("N:;;;;\r\n"
                 "UID:testuid\r\n"
                 "END:VCARD\r\n\r\n");
    return expected;
}

void ImppTest::shouldExportEmptyType()
{
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QCOMPARE(ba, expected);
}

void ImppTest::shouldExportType_data()
{
    QTest::addColumn<KContacts::Impp::ImppType>("type");
    for (int i = KContacts::Impp::Unknown + 1; i < KContacts::Impp::EndList; ++i) {
        KContacts::Impp::ImppType type = static_cast<KContacts::Impp::ImppType>(i);
        QTest::newRow(KContacts::Impp::typeToString(type).toLatin1().constData()) << type;
    }
}

void ImppTest::shouldExportType()
{
    QFETCH(KContacts::Impp::ImppType, type);

    QByteArray expected = expectedVcard(type);
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Impp impp;
    impp.setAddress(QStringLiteral("address"));
    impp.setType(type);
    addr.insertImpp(impp);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QCOMPARE(ba, expected);
}

void ImppTest::shouldExportWithParameters()
{
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "IMPP;FOO1=bla1,blo1;FOO2=bla2,blo2;X-SERVICE-TYPE=skype:address\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Impp impp;
    impp.setAddress(QStringLiteral("address"));
    impp.setType(KContacts::Impp::Skype);
    impp.setParameters(params);
    addr.insertImpp(impp);
    lst << addr;

    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QCOMPARE(ba, expected);
}

void ImppTest::shouldShouldNotExportTwiceServiceType()
{
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "IMPP;FOO1=bla1,blo1;FOO2=bla2,blo2;X-SERVICE-TYPE=skype:address\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    params.insert(QStringLiteral("X-SERVICE-TYPE"), QStringList() << QStringLiteral("aim"));
    KContacts::Impp impp;
    impp.setAddress(QStringLiteral("address"));
    impp.setType(KContacts::Impp::Skype);
    impp.setParameters(params);
    addr.insertImpp(impp);
    lst << addr;

    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QCOMPARE(ba, expected);
}

QTEST_MAIN(ImppTest)
