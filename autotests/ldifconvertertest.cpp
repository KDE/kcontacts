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

void LDifConverterTest::shouldConvertEmail()
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
    bool result = LDIFConverter::LDIFToAddressee(str, lst);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).emails().count(), 1);
    QCOMPARE(lst.at(0).emails().at(0), QLatin1String("foo@kde.org"));
}

void LDifConverterTest::shouldConvertStandardBirthday()
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
    bool result = LDIFConverter::LDIFToAddressee(str, lst);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QVERIFY(lst.at(0).birthday().date().isValid());
    QCOMPARE(lst.at(0).birthday().date(), QDate(2015,3,19));
}

void LDifConverterTest::shouldConvertTheBatsBirthday()
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
    bool result = LDIFConverter::LDIFToAddressee(str, lst);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QVERIFY(lst.at(0).birthday().date().isValid());
    QCOMPARE(lst.at(0).birthday().date(), QDate(2015,3,19));
}

void LDifConverterTest::shouldConvertTitle()
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
    bool result = LDIFConverter::LDIFToAddressee(str, lst);
    QVERIFY(result);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).title(), QLatin1String("foo"));
}


QTEST_MAIN(LDifConverterTest)
