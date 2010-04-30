/*
    This file is part of libkabc.
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
#include "kabc/address.h"
#include <qtest_kde.h>

QTEST_KDEMAIN( AddressTest, NoGUI )

void AddressTest::emptyTest()
{
  KABC::Address address;

  QVERIFY( address.isEmpty() );
}

void AddressTest::storeTest()
{
  KABC::Address address;

  address.setId( QLatin1String( "My Id" ) );
  address.setType( KABC::Address::Home );
  address.setPostOfficeBox( QLatin1String( "1234" ) );
  address.setExtended( QLatin1String( "My Extended Label" ) );
  address.setStreet( QLatin1String( "My Street" ) );
  address.setLocality( QLatin1String( "My Locality" ) );
  address.setRegion( QLatin1String( "My Region" ) );
  address.setPostalCode( QLatin1String( "My PostalCode" ) );
  address.setCountry( QLatin1String( "My Country" ) );
  address.setLabel( QLatin1String( "My Label" ) );

  QVERIFY( address.id() == QLatin1String( "My Id" ) );
  QVERIFY( address.type() == KABC::Address::Home );
  QVERIFY( address.postOfficeBox() == QLatin1String( "1234" ) );
  QVERIFY( address.extended() == QLatin1String( "My Extended Label" ) );
  QVERIFY( address.street() == QLatin1String( "My Street" ) );
  QVERIFY( address.locality() == QLatin1String( "My Locality" ) );
  QVERIFY( address.region() == QLatin1String( "My Region" ) );
  QVERIFY( address.postalCode() == QLatin1String( "My PostalCode" ) );
  QVERIFY( address.country() == QLatin1String( "My Country" ) );
  QVERIFY( address.label() == QLatin1String( "My Label" ) );
}

void AddressTest::equalsTest()
{
  KABC::Address address1, address2;

  address1.setId( QLatin1String( "My Id" ) );
  address1.setType( KABC::Address::Home );
  address1.setPostOfficeBox( QLatin1String( "1234" ) );
  address1.setExtended( QLatin1String( "My Extended Label" ) );
  address1.setStreet( QLatin1String( "My Street" ) );
  address1.setLocality( QLatin1String( "My Locality" ) );
  address1.setRegion( QLatin1String( "My Region" ) );
  address1.setPostalCode( QLatin1String( "My Postalcode" ) );
  address1.setCountry( QLatin1String( "My country" ) );
  address1.setLabel( QLatin1String( "My Label" ) );

  address2.setId( QLatin1String( "My Id" ) );
  address2.setType( KABC::Address::Home );
  address2.setPostOfficeBox( QLatin1String( "1234" ) );
  address2.setExtended( QLatin1String( "My Extended Label" ) );
  address2.setStreet( QLatin1String( "My Street" ) );
  address2.setLocality( QLatin1String( "My Locality" ) );
  address2.setRegion( QLatin1String( "My Region" ) );
  address2.setPostalCode( QLatin1String( "My Postalcode" ) );
  address2.setCountry( QLatin1String( "My country" ) );
  address2.setLabel( QLatin1String( "My Label" ) );

  QVERIFY( address1 == address2 );
}

void AddressTest::differsTest()
{
  KABC::Address address1( KABC::Address::Home );
  KABC::Address address2( KABC::Address::Work );

  QVERIFY( address1 != address2 );
}

void AddressTest::assignmentTest()
{
  KABC::Address address1, address2;

  address1.setId( QLatin1String( "My Id" ) );
  address1.setType( KABC::Address::Home );
  address1.setPostOfficeBox( QLatin1String( "1234" ) );
  address1.setExtended( QLatin1String( "My Extended Label" ) );
  address1.setStreet( QLatin1String( "My Street" ) );
  address1.setLocality( QLatin1String( "My Locality" ) );
  address1.setRegion( QLatin1String( "My Region" ) );
  address1.setPostalCode( QLatin1String( "My Postalcode" ) );
  address1.setCountry( QLatin1String( "My country" ) );
  address1.setLabel( QLatin1String( "My Label" ) );

  address2 = address1;

  QVERIFY( address1 == address2 );
}

void AddressTest::serializeTest()
{
  KABC::Address address1, address2;

  address1.setType( KABC::Address::Work );
  address1.setPostOfficeBox( QLatin1String( "1234" ) );
  address1.setExtended( QLatin1String( "My Extended Label" ) );
  address1.setStreet( QLatin1String( "My Street" ) );
  address1.setLocality( QLatin1String( "My Locality" ) );
  address1.setRegion( QLatin1String( "My Region" ) );
  address1.setPostalCode( QLatin1String( "My Postalcode" ) );
  address1.setCountry( QLatin1String( "My country" ) );
  address1.setLabel( QLatin1String( "My Label" ) );

  QByteArray data;
  QDataStream s( &data, QIODevice::WriteOnly );
  s << address1;

  QDataStream t( &data, QIODevice::ReadOnly );
  t >> address2;

  QVERIFY( address1 == address2 );
}

void AddressTest::formatTest()
{
  {
    KABC::Address address;
    address.setStreet( QLatin1String( "Lummerlandstr. 1" ) );
    address.setPostalCode( QLatin1String( "12345" ) );
    address.setLocality( QLatin1String( "Lummerstadt" ) );
    address.setCountry ( QLatin1String( "Germany" ) );

    const QString result( QLatin1String( "Jim Knopf\nLummerlandstr. 1\n"
                                         "12345 Lummerstadt\n\nGERMANY" ) );

    QVERIFY( address.formattedAddress( QLatin1String( "Jim Knopf" ) ) == result );
  }

  {
    KABC::Address address;
    address.setStreet( QLatin1String( "457 Foobar Ave" ) );
    address.setPostalCode( QLatin1String( "1A2B3C" ) );
    address.setLocality( QLatin1String( "Nervousbreaktown" ) );
    address.setRegion( QLatin1String( "DC" ) );
    address.setCountry( QLatin1String( "United States of America" ) );

    const QString result(
      QLatin1String( "Huck Finn\n457 Foobar Ave\nNervousbreaktown,"
                     "  DC 1A2B3C\n\nUNITED STATES OF AMERICA" ) );
    QVERIFY( address.formattedAddress( QLatin1String( "Huck Finn" ) ) == result );
  }

  {
    KABC::Address address;
    address.setStreet( QLatin1String( "Lummerlandstr. 1" ) );
    address.setPostalCode( QLatin1String( "12345" ) );
    address.setLocality( QLatin1String( "Lummerstadt" ) );
    address.setCountry( QLatin1String( "Deutschland" ) );

    const QString result( QLatin1String( "Jim Knopf\nLummerlandstr. 1\n"
                                         "12345 Lummerstadt\n\nDEUTSCHLAND" ) );

    QVERIFY( address.formattedAddress( QLatin1String( "Jim Knopf" ) ) == result );
  }

  {
    KABC::Address address;
    address.setStreet( QLatin1String( "Lummerlandstr. 1" ) );
    address.setPostalCode( QLatin1String( "12345" ) );
    address.setLocality( QLatin1String( "Lummerstadt" ) );
    address.setCountry( QString() );

    const QString result( QLatin1String( "Jim Knopf\nLummerlandstr. 1\nLummerstadt, 12345" ) );

    QVERIFY( address.formattedAddress( QLatin1String( "Jim Knopf" ) ) == result );
  }
}

#include "addresstest.moc"
