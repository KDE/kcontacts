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

#include "keytest.h"
#include "kabc/key.h"
#include <qtest.h>

QTEST_MAIN( KeyTest )

void KeyTest::storeTest()
{
  KABC::Key key;

  key.setId( QLatin1String( "My Id" ) );
  key.setType( KABC::Key::Custom );
  key.setCustomTypeString( QLatin1String( "GnuPG" ) );
  key.setTextData( QLatin1String( "That's my super secret key" ) );

  QVERIFY( key.id() == QLatin1String( "My Id" ) );
  QVERIFY( key.type() == KABC::Key::Custom );
  QVERIFY( key.customTypeString() == QLatin1String( "GnuPG" ) );
  QVERIFY( key.textData() == QLatin1String( "That's my super secret key" ) );
  QVERIFY( key.isBinary() == false );
}

void KeyTest::equalsTest()
{
  KABC::Key key1, key2;

  key1.setId( QLatin1String( "My Id" ) );
  key1.setType( KABC::Key::Custom );
  key1.setCustomTypeString( QLatin1String( "GnuPG" ) );
  key1.setTextData( QLatin1String( "That's my super secret key" ) );

  key2.setId( QLatin1String( "My Id" ) );
  key2.setType( KABC::Key::Custom );
  key2.setCustomTypeString( QLatin1String( "GnuPG" ) );
  key2.setTextData( QLatin1String( "That's my super secret key" ) );

  QVERIFY( key1 == key2 );
}

void KeyTest::differsTest()
{
  KABC::Key key1( QLatin1String( "TextKey" ), KABC::Key::PGP );
  KABC::Key key2( QLatin1String( "TextKey" ), KABC::Key::Custom );

  QVERIFY( key1 != key2 );
}

void KeyTest::assignmentTest()
{
  KABC::Key key1, key2;

  key1.setId( QLatin1String( "My Id" ) );
  key1.setType( KABC::Key::Custom );
  key1.setCustomTypeString( QLatin1String( "GnuPG" ) );
  key1.setTextData( QLatin1String( "That's my super secret key" ) );

  key2 = key1;

  QVERIFY( key1 == key2 );
}

void KeyTest::serializeTest()
{
  KABC::Key key1, key2;

  key1.setId( QLatin1String( "My Id" ) );
  key1.setType( KABC::Key::Custom );
  key1.setCustomTypeString( QLatin1String( "GnuPG" ) );
  key1.setTextData( QLatin1String( "That's my super secret key" ) );

  QByteArray data;
  QDataStream s( &data, QIODevice::WriteOnly );
  s << key1;

  QDataStream t( &data, QIODevice::ReadOnly );
  t >> key2;

  QVERIFY( key1 == key2 );
}

