/*
    This file is part of the KContacts framework.
    Copyright (c) 2007 Tobias Koenig <tokoe@kde.org>

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

#include "addresstest.h"
#include "addresseelist.h"
#include "vcardtool.h"
#include "kcontacts/address.h"
#include <kconfig.h>
#include <kconfiggroup.h>
#include <qtest.h>
#include <QStandardPaths>
#include <QFileInfo>

QTEST_MAIN(AddressTest)

void AddressTest::initTestCase()
{
    setenv("LANG", "C", 1);
}

void AddressTest::emptyTest()
{
    KContacts::Address address;

    QVERIFY(address.isEmpty());
}

void AddressTest::storeTest()
{
    KContacts::Address address;

    address.setId(QStringLiteral("My Id"));
    address.setType(KContacts::Address::Home);
    address.setPostOfficeBox(QStringLiteral("1234"));
    address.setExtended(QStringLiteral("My Extended Label"));
    address.setStreet(QStringLiteral("My Street"));
    address.setLocality(QStringLiteral("My Locality"));
    address.setRegion(QStringLiteral("My Region"));
    address.setPostalCode(QStringLiteral("My PostalCode"));
    address.setCountry(QStringLiteral("My Country"));
    address.setLabel(QStringLiteral("My Label"));

    QCOMPARE(address.id(), QStringLiteral("My Id"));
    QCOMPARE(address.type(), KContacts::Address::Home);
    QCOMPARE(address.postOfficeBox(), QStringLiteral("1234"));
    QCOMPARE(address.extended(), QStringLiteral("My Extended Label"));
    QCOMPARE(address.street(), QStringLiteral("My Street"));
    QCOMPARE(address.locality(), QStringLiteral("My Locality"));
    QCOMPARE(address.region(), QStringLiteral("My Region"));
    QCOMPARE(address.postalCode(), QStringLiteral("My PostalCode"));
    QCOMPARE(address.country(), QStringLiteral("My Country"));
    QCOMPARE(address.label(), QStringLiteral("My Label"));
}

void AddressTest::equalsTest()
{
    KContacts::Address address1, address2;

    address1.setId(QStringLiteral("My Id"));
    address1.setType(KContacts::Address::Home);
    address1.setPostOfficeBox(QStringLiteral("1234"));
    address1.setExtended(QStringLiteral("My Extended Label"));
    address1.setStreet(QStringLiteral("My Street"));
    address1.setLocality(QStringLiteral("My Locality"));
    address1.setRegion(QStringLiteral("My Region"));
    address1.setPostalCode(QStringLiteral("My Postalcode"));
    address1.setCountry(QStringLiteral("My country"));
    address1.setLabel(QStringLiteral("My Label"));

    address2.setId(QStringLiteral("My Id"));
    address2.setType(KContacts::Address::Home);
    address2.setPostOfficeBox(QStringLiteral("1234"));
    address2.setExtended(QStringLiteral("My Extended Label"));
    address2.setStreet(QStringLiteral("My Street"));
    address2.setLocality(QStringLiteral("My Locality"));
    address2.setRegion(QStringLiteral("My Region"));
    address2.setPostalCode(QStringLiteral("My Postalcode"));
    address2.setCountry(QStringLiteral("My country"));
    address2.setLabel(QStringLiteral("My Label"));

    QVERIFY(address1 == address2);
}

void AddressTest::differsTest()
{
    KContacts::Address address1(KContacts::Address::Home);
    KContacts::Address address2(KContacts::Address::Work);

    QVERIFY(address1 != address2);
}

void AddressTest::assignmentTest()
{
    KContacts::Address address1, address2;

    address1.setId(QStringLiteral("My Id"));
    address1.setType(KContacts::Address::Home);
    address1.setPostOfficeBox(QStringLiteral("1234"));
    address1.setExtended(QStringLiteral("My Extended Label"));
    address1.setStreet(QStringLiteral("My Street"));
    address1.setLocality(QStringLiteral("My Locality"));
    address1.setRegion(QStringLiteral("My Region"));
    address1.setPostalCode(QStringLiteral("My Postalcode"));
    address1.setCountry(QStringLiteral("My country"));
    address1.setLabel(QStringLiteral("My Label"));

    address2 = address1;

    QVERIFY(address1 == address2);
}

void AddressTest::serializeTest()
{
    KContacts::Address address1, address2;

    address1.setType(KContacts::Address::Work);
    address1.setPostOfficeBox(QStringLiteral("1234"));
    address1.setExtended(QStringLiteral("My Extended Label"));
    address1.setStreet(QStringLiteral("My Street"));
    address1.setLocality(QStringLiteral("My Locality"));
    address1.setRegion(QStringLiteral("My Region"));
    address1.setPostalCode(QStringLiteral("My Postalcode"));
    address1.setCountry(QStringLiteral("My country"));
    address1.setLabel(QStringLiteral("My Label"));

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << address1;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> address2;

    QVERIFY(address1 == address2);
}

void AddressTest::formatTest()
{
    {
        // check availability of country to ISO code mapping data file
        const QString mapfile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kf5/kcontacts/countrytransl.map"));
        QVERIFY2(!mapfile.isEmpty(), "Country to ISO code mapping data file does not exist");

        QFileInfo fileInfo(mapfile);
        QVERIFY2(fileInfo.exists(), "Country to ISO code mapping data file does not exist");
        QVERIFY2(fileInfo.isReadable(), "Country to ISO code mapping data file is not readable");
    }

    {
        const QString templateFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kf5/locale/countries/de/country.desktop"));
        QVERIFY(!templateFile.isEmpty());
        const KConfig templateConfig(templateFile);
        QVERIFY2(templateConfig.hasGroup("KCM Locale"),
                 "Address format config for 'de' is missing expected 'KCM Locale' group");
        const KConfigGroup templateGroup = templateConfig.group("KCM Locale");
        QVERIFY2(templateGroup.hasKey("AddressFormat"),
                 "Address format config is missing expected key 'AddressFormat'");
        const QString addressTemplate = templateGroup.readEntry("AddressFormat");
        QVERIFY2(!addressTemplate.isEmpty(),
                 "Address format template for 'de' is empty");

        KContacts::Address address;
        address.setStreet(QStringLiteral("Lummerlandstr. 1"));
        address.setPostalCode(QStringLiteral("12345"));
        address.setLocality(QStringLiteral("Lummerstadt"));
        address.setCountry(QStringLiteral("Germany"));

        const QString result(QStringLiteral("Jim Knopf\nLummerlandstr. 1\n"
                                            "12345 Lummerstadt\n\nGERMANY"));

        QCOMPARE(address.formattedAddress(QStringLiteral("Jim Knopf")), result);
    }

    {
        KContacts::Address address;
        address.setStreet(QStringLiteral("457 Foobar Ave"));
        address.setPostalCode(QStringLiteral("1A2B3C"));
        address.setLocality(QStringLiteral("Nervousbreaktown"));
        address.setRegion(QStringLiteral("DC"));
        address.setCountry(QStringLiteral("United States of America"));

        const QString result(
            QStringLiteral("Huck Finn\n457 Foobar Ave\nNervousbreaktown,"
                           "  DC 1A2B3C\n\nUNITED STATES OF AMERICA"));
        QCOMPARE(address.formattedAddress(QStringLiteral("Huck Finn")), result);
    }

    {
        KContacts::Address address;
        address.setStreet(QStringLiteral("Lummerlandstr. 1"));
        address.setPostalCode(QStringLiteral("12345"));
        address.setLocality(QStringLiteral("Lummerstadt"));
        address.setCountry(QStringLiteral("Deutschland"));

        const QString result(QStringLiteral("Jim Knopf\nLummerlandstr. 1\n"
                                            "12345 Lummerstadt\n\nDEUTSCHLAND"));

        QCOMPARE(address.formattedAddress(QStringLiteral("Jim Knopf")), result);
    }

    {
        KContacts::Address address;
        address.setStreet(QStringLiteral("Lummerlandstr. 1"));
        address.setPostalCode(QStringLiteral("12345"));
        address.setLocality(QStringLiteral("Lummerstadt"));
        address.setCountry(QString());

        const QString result(QStringLiteral("Jim Knopf\nLummerlandstr. 1\nLummerstadt, 12345"));

        QCOMPARE(address.formattedAddress(QStringLiteral("Jim Knopf")), result);
    }
}


void AddressTest::shouldExportVcard3()
{
    KContacts::Address address;

    address.setId(QStringLiteral("My Id"));
    address.setType(KContacts::Address::Home);
    address.setPostOfficeBox(QStringLiteral("1234"));
    address.setExtended(QStringLiteral("My Extended Label"));
    address.setStreet(QStringLiteral("My Street"));
    address.setLocality(QStringLiteral("My Locality"));
    address.setRegion(QStringLiteral("My Region"));
    address.setPostalCode(QStringLiteral("My Postalcode"));
    address.setCountry(QStringLiteral("My country"));
    address.setLabel(QStringLiteral("My Label"));

    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    addr.insertAddress(address);
    lst << addr;

    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v3_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:3.0\r\n"
                        "ADR;TYPE=home:1234;My Extended Label;My Street;My Locality;My Region;My Pos\r\n"
                        " talcode;My country\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "LABEL;TYPE=home:My Label\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

void AddressTest::shouldParseAddress()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "ADR;TYPE=home:1234;My Extended Label;My Street;My Locality;My Region;My Pos\r\n"
                         " talcode;My country\r\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "LABEL;TYPE=home:My Label\r\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).addresses().count(), 1);
    KContacts::Address address = lst.at(0).addresses().at(0);
    QCOMPARE(address.type(), KContacts::Address::Home);
    QCOMPARE(address.postOfficeBox(), QStringLiteral("1234"));
    QCOMPARE(address.extended(), QStringLiteral("My Extended Label"));
    QCOMPARE(address.region(), QStringLiteral("My Region"));
    QCOMPARE(address.street(), QStringLiteral("My Street"));
    QCOMPARE(address.country(), QStringLiteral("My country"));
    QCOMPARE(address.postalCode(), QStringLiteral("My Postalcode"));
    QCOMPARE(address.label(), QStringLiteral("My Label"));
}

void AddressTest::shouldExportVcard4()
{
    KContacts::Address address;

    address.setId(QStringLiteral("My Id"));
    address.setType(KContacts::Address::Home);
    address.setPostOfficeBox(QStringLiteral("1234"));
    address.setExtended(QStringLiteral("My Extended Label"));
    address.setStreet(QStringLiteral("My Street"));
    address.setLocality(QStringLiteral("My Locality"));
    address.setRegion(QStringLiteral("My Region"));
    address.setPostalCode(QStringLiteral("My Postalcode"));
    address.setCountry(QStringLiteral("My country"));
    address.setLabel(QStringLiteral("My Label"));

    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    addr.insertAddress(address);
    lst << addr;

    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "ADR;LABEL=\"My Label\";TYPE=home:1234;My Extended Label;My Street;My Locality\r\n"
                        " ;My Region;My Postalcode;My country\r\nEMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}
