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

#include <qtest_kde.h>
#include "addresstest.h"
#include "kabc/address.h"

QTEST_KDEMAIN(AddressTest, NoGUI)

void AddressTest::emptyTest()
{
  KABC::Address address;

  QVERIFY( address.isEmpty() );
}

void AddressTest::storeTest()
{
  KABC::Address address;

  address.setId( "My Id" );
  address.setType( KABC::Address::Home );
  address.setPostOfficeBox( "1234" );
  address.setExtended( "My Extended Label" );
  address.setStreet( "My Street" );
  address.setLocality( "My Locality" );
  address.setRegion( "My Region" );
  address.setPostalCode( "My PostalCode" );
  address.setCountry( "My Country" );
  address.setLabel( "My Label" );

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

  address1.setId( "My Id" );
  address1.setType( KABC::Address::Home );
  address1.setPostOfficeBox( "1234" );
  address1.setExtended( "My Extended Label" );
  address1.setStreet( "My Street" );
  address1.setLocality( "My Locality" );
  address1.setRegion( "My Region" );
  address1.setPostalCode( "My Postalcode" );
  address1.setCountry( "My country" );
  address1.setLabel( "My Label" );

  address2.setId( "My Id" );
  address2.setType( KABC::Address::Home );
  address2.setPostOfficeBox( "1234" );
  address2.setExtended( "My Extended Label" );
  address2.setStreet( "My Street" );
  address2.setLocality( "My Locality" );
  address2.setRegion( "My Region" );
  address2.setPostalCode( "My Postalcode" );
  address2.setCountry( "My country" );
  address2.setLabel( "My Label" );

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

  address1.setId( "My Id" );
  address1.setType( KABC::Address::Home );
  address1.setPostOfficeBox( "1234" );
  address1.setExtended( "My Extended Label" );
  address1.setStreet( "My Street" );
  address1.setLocality( "My Locality" );
  address1.setRegion( "My Region" );
  address1.setPostalCode( "My Postalcode" );
  address1.setCountry( "My country" );
  address1.setLabel( "My Label" );

  address2 = address1;

  QVERIFY( address1 == address2 );
}

void AddressTest::serializeTest()
{
  KABC::Address address1, address2;

  address1.setType( KABC::Address::Work );
  address1.setPostOfficeBox( "1234" );
  address1.setExtended( "My Extended Label" );
  address1.setStreet( "My Street" );
  address1.setLocality( "My Locality" );
  address1.setRegion( "My Region" );
  address1.setPostalCode( "My Postalcode" );
  address1.setCountry( "My country" );
  address1.setLabel( "My Label" );

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
    address.setStreet( "Lummerlandstr. 1" );
    address.setPostalCode( "12345" );
    address.setLocality( "Lummerstadt" );
    address.setCountry ( "Germany" );

    const QString result( "Jim Knopf\nLummerlandstr. 1\nLummerstadt, 12345\n\nGERMANY" );

    QVERIFY( address.formattedAddress( "Jim Knopf" ) == result );
  }

  {
    KABC::Address address;
    address.setStreet( "457 Foobar Ave" );
    address.setPostalCode( "1A2B3C" );
    address.setLocality( "Nervousbreaktown" );
    address.setRegion( "DC" );
    address.setCountry( "United States of America" );

    const QString result( "Huck Finn\n457 Foobar Ave\nNervousbreaktown DC, 1A2B3C\n\nUNITED STATES OF AMERICA" );
    QVERIFY( address.formattedAddress( "Huck Finn" ) == result );
  }

  {
    KABC::Address address;
    address.setStreet( "Lummerlandstr. 1" );
    address.setPostalCode( "12345" );
    address.setLocality( "Lummerstadt" );
    address.setCountry( "Deutschland" );

    const QString result( "Jim Knopf\nLummerlandstr. 1\nLummerstadt, 12345\n\nDEUTSCHLAND" );

    QVERIFY( address.formattedAddress( "Jim Knopf" ) == result );
  }

  {
    KABC::Address address;
    address.setStreet( "Lummerlandstr. 1" );
    address.setPostalCode( "12345" );
    address.setLocality( "Lummerstadt" );
    address.setCountry( QString() );

    const QString result( "Jim Knopf\nLummerlandstr. 1\nLummerstadt, 12345" );

    QVERIFY( address.formattedAddress( "Jim Knopf" ) == result );
  }
}

#include "addresstest.moc"
