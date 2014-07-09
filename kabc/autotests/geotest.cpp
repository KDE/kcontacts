/*
    This file is part of libkabc.
    Copyright (c) 2007 KDE-PIM team <kde-pim@kde.org>

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

#include "geotest.h"
#include "kabc/geo.h"
#include <qtest.h>

QTEST_MAIN( GeoTest )

void GeoTest::constructor()
{
  KABC::Geo geo( 1.2f, 3.4f );

  QVERIFY( (float)geo.latitude() == (float)1.2 );
  QVERIFY( (float)geo.longitude() == (float)3.4 );
}

void GeoTest::isValid()
{
  KABC::Geo geo;

  QVERIFY( !geo.isValid() );

  geo.setLatitude( 23 );

  QVERIFY( !geo.isValid() );

  geo.setLongitude( 45 );

  QVERIFY( geo.isValid() );
}

void GeoTest::setData()
{
  KABC::Geo geo;

  geo.setLatitude( 22.5f );
  geo.setLongitude( 45.1f );

  QVERIFY( (float)geo.latitude() == (float)22.5 );
  QVERIFY( (float)geo.longitude() == (float)45.1 );
}

void GeoTest::equals()
{
  KABC::Geo geo1( 22.5f, 33.7f );
  KABC::Geo geo2( 22.5f, 33.7f );

  QVERIFY( geo1 == geo2 );
}

void GeoTest::differs()
{
  KABC::Geo geo1( 22.5f, 33.7f );
  KABC::Geo geo2( 22.5f, 33.6f );

  QVERIFY( geo1 != geo2 );
}

void GeoTest::serialization()
{
  KABC::Geo geo1( 22.5f, 33.7f );
  QByteArray data;

  QDataStream s( &data, QIODevice::WriteOnly );
  s << geo1;

  KABC::Geo geo2;
  QDataStream t( &data, QIODevice::ReadOnly );
  t >> geo2;

  QVERIFY( geo1 == geo2 );
}

