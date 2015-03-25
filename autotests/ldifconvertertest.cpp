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

#include "ldifconvertertest.h"
#include "converter/ldifconverter.h"
#include <qtest.h>
using namespace KContacts;

LDifConverterTest::LDifConverterTest(QObject *parent)
    : QObject(parent)
{

}

LDifConverterTest::~LDifConverterTest()
{

}

void LDifConverterTest::shouldImportEmail()
{
    QString str = QLatin1String("dn: cn=laurent,mail=foo@kde.org\n"
                                "sn: laurent\n"
                                "cn: laurent\n"
                                "uid: d1d5cdd4-7d5d-484b-828d-58864d8efe74\n"
                                "mail: foo@kde.org\n"
                                "objectclass: top_n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson");
    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).emails().count(), 1);
    QCOMPARE(lst.at(0).emails().at(0), QLatin1String("foo@kde.org"));
    QCOMPARE(contactGroup.count(), 0);
}

void LDifConverterTest::shouldImportStandardBirthday()
{
    QString str = QLatin1String("dn: cn=laurent,mail=foo@kde.org\n"
                                "sn: laurent\n"
                                "cn: laurent\n"
                                "uid: d1d5cdd4-7d5d-484b-828d-58864d8efe74\n"
                                "birthyear: 2015\n"
                                "birthmonth: 3\n"
                                "birthday: 19\n"
                                "mail: foo@kde.org\n"
                                "objectclass: top_n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson");
    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QVERIFY(lst.at(0).birthday().date().isValid());
    QCOMPARE(lst.at(0).birthday().date(), QDate(2015, 3, 19));
    QCOMPARE(contactGroup.count(), 0);
}

void LDifConverterTest::shouldImportTheBatsBirthday()
{
    QString str = QLatin1String("dn: cn=laurent,mail=foo@kde.org\n"
                                "sn: laurent\n"
                                "cn: laurent\n"
                                "uid: d1d5cdd4-7d5d-484b-828d-58864d8efe74\n"
                                "xbatBirthday: 20150319\n"
                                "mail: foo@kde.org\n"
                                "objectclass: top_n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson");
    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QVERIFY(lst.at(0).birthday().date().isValid());
    QCOMPARE(lst.at(0).birthday().date(), QDate(2015, 3, 19));
    QCOMPARE(contactGroup.count(), 0);
}

void LDifConverterTest::shouldImportTitle()
{
    QString str = QLatin1String("dn: cn=laurent,mail=foo@kde.org\n"
                                "sn: laurent\n"
                                "cn: laurent\n"
                                "uid: d1d5cdd4-7d5d-484b-828d-58864d8efe74\n"
                                "title: foo\n"
                                "mail: foo@kde.org\n"
                                "objectclass: top_n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson");
    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).title(), QLatin1String("foo"));
    QCOMPARE(contactGroup.count(), 0);
}

void LDifConverterTest::shouldImportWorkStreet()
{
    QString str = QLatin1String("dn: cn=laurent,mail=foo@kde.org\n"
                                "sn: laurent\n"
                                "cn: laurent\n"
                                "uid: d1d5cdd4-7d5d-484b-828d-58864d8efe74\n"
                                "title: foo\n"
                                "mail: foo@kde.org\n"
                                "street: work address\n"
                                "mozillaWorkStreet2: work address next\n"
                                "objectclass: top_n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson");
    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).address(Address::Work).street(), QLatin1String("work address\nwork address next"));
    QCOMPARE(contactGroup.count(), 0);
}

void LDifConverterTest::shouldImportContactGroup()
{
    QString str = QLatin1String("dn: cn=test,mail=\n"
                                "cn: test\n"
                                "modifyTimeStamp: 20080526T234914Z\n"
                                "display-name: Test\n"
                                "objectclass: top\n"
                                "objectclass: groupOfNames\n"
                                "member: cn=Jim Doe,mail=jim.doe@foobar.com\n"
                                "member: cn=Jane Doe,mail=jane.doe@foobar.com\n"
                                "member: cn=John Doe,mail=john.doe@foobar.com\n");
    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 0);
    QCOMPARE(contactGroup.count(), 1);
    ContactGroup first = contactGroup.first();
    QCOMPARE(first.name(), QLatin1String("Test"));
    QCOMPARE((int)first.count(), 3);
}

void LDifConverterTest::shouldImportMultiEntries()
{
    QString str = QLatin1String("dn: cn=test1,mail=test1@test.test\n"
                                "sn: test1\n"
                                "cn: test1\n"
                                "uid: jpgdf2NrLQ\n"
                                "mail: test1@test.test\n"
                                "modifytimestamp: 20121219T140848Z\n"
                                "objectclass: top\n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson\n"
                                "\n"
                                "dn: cn=test2,mail=test2@test.test\n"
                                "sn: test2\n"
                                "cn: test2\n"
                                "uid: ow2mwdUb6A\n"
                                "mail: test2@test.test\n"
                                "objectclass: top\n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson\n");
    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 2);
    QCOMPARE(contactGroup.count(), 0);
    QCOMPARE(lst.at(0).emails().at(0), QLatin1String("test1@test.test"));
    QCOMPARE(lst.at(1).emails().at(0), QLatin1String("test2@test.test"));
}

void LDifConverterTest::shouldImportGroupAndAddress()
{
    QString str = QLatin1String("dn: cn=laurent,mail=foo@kde.org\n"
                                "sn: laurent\n"
                                "cn: laurent\n"
                                "uid: d1d5cdd4-7d5d-484b-828d-58864d8efe74\n"
                                "title: foo\n"
                                "mail: foo@kde.org\n"
                                "street: work address\n"
                                "mozillaWorkStreet2: work address next\n"
                                "objectclass: top_n"
                                "objectclass: person\n"
                                "objectclass: organizationalPerson\n"
                                "\n"
                                "dn: cn=test,mail=\n"
                                "cn: test\n"
                                "modifyTimeStamp: 20080526T234914Z\n"
                                "display-name: Test\n"
                                "objectclass: top\n"
                                "objectclass: groupOfNames\n"
                                "member: cn=Jim Doe,mail=jim.doe@foobar.com\n"
                                "member: cn=Jane Doe,mail=jane.doe@foobar.com\n"
                                "member: cn=John Doe,mail=john.doe@foobar.com\n");

    AddresseeList lst;
    ContactGroup::List contactGroup;
    bool result = LDIFConverter::LDIFToAddressee(str, lst, contactGroup);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(contactGroup.count(), 1);
}

QTEST_MAIN(LDifConverterTest)
