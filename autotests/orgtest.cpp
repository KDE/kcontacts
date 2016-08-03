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

#include "orgtest.h"
#include "org.h"
#include "vcardtool.h"
#include <qtest.h>

OrgTest::OrgTest(QObject *parent)
    : QObject(parent)
{

}

OrgTest::~OrgTest()
{

}

void OrgTest::shouldHaveDefaultValue()
{
    KContacts::Org org;
    QVERIFY(!org.isValid());
    QVERIFY(org.organization().isEmpty());
    QVERIFY(org.parameters().isEmpty());
}

void OrgTest::shouldAssignValue()
{
    const QString organization(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    KContacts::Org org(organization);
    org.setParameters(params);
    QVERIFY(org.isValid());
    QVERIFY(!org.organization().isEmpty());
    QCOMPARE(org.organization(), organization);
    QVERIFY(!org.parameters().isEmpty());
    QCOMPARE(org.parameters(), params);

}

void OrgTest::shouldAssignExternal()
{
    KContacts::Org org;
    const QString organization(QStringLiteral("fr"));
    org.setOrganization(organization);
    QVERIFY(org.isValid());
    QCOMPARE(org.organization(), organization);
}

void OrgTest::shouldSerialized()
{
    KContacts::Org org;
    KContacts::Org result;
    const QString organization(QStringLiteral("fr"));
    org.setOrganization(organization);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    org.setParameters(params);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << org;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(org == result);

}

void OrgTest::shouldEqualOrg()
{
    KContacts::Org org;
    KContacts::Org result;
    const QString organization(QStringLiteral("fr"));
    org.setOrganization(organization);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    org.setParameters(params);

    result = org;
    QVERIFY(org == result);
}

void OrgTest::shouldParseOrg()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "Org:boo\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).extraOrganizationList().count(), 1);
    QCOMPARE(lst.at(0).organization(), QStringLiteral("boo"));
}

void OrgTest::shouldParseVcardWithTwoOrg()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "Org:boo\n"
                         "Org:bla\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).extraOrganizationList().count(), 2);
    QCOMPARE(lst.at(0).extraOrganizationList().at(0).organization(), QStringLiteral("boo"));
    QCOMPARE(lst.at(0).extraOrganizationList().at(1).organization(), QStringLiteral("bla"));
}

void OrgTest::shouldParseWithoutOrg()
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
    QCOMPARE(lst.at(0).extraOrganizationList().count(), 0);
}

void OrgTest::shouldCreateVCard()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Org::List lstOrg;
    KContacts::Org org(QStringLiteral("fr"));
    lstOrg << org;
    addr.setExtraOrganizationList(lstOrg);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "ORG:fr\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

void OrgTest::shouldCreateVCardWithTwoOrg()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Org::List lstOrg;
    KContacts::Org org(QStringLiteral("fr"));
    KContacts::Org org2(QStringLiteral("fr2"));
    lstOrg << org << org2;
    addr.setExtraOrganizationList(lstOrg);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "ORG:fr\r\n"
                        "ORG:fr2\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);

}

void OrgTest::shouldCreateVCardWithParameters()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Org::List lstOrg;
    KContacts::Org org(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    org.setParameters(params);
    lstOrg << org;
    addr.setExtraOrganizationList(lstOrg);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "ORG;FOO1=bla1,blo1;FOO2=bla2,blo2:fr\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

void OrgTest::shouldGenerateOrgForVCard3()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Org::List lstOrg;
    KContacts::Org org(QStringLiteral("fr"));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    org.setParameters(params);
    lstOrg << org;
    addr.setExtraOrganizationList(lstOrg);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v3_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:3.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "ORG;FOO1=bla1,blo1;FOO2=bla2,blo2:fr\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

QTEST_MAIN(OrgTest)
