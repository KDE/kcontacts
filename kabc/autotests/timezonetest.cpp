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

#include "timezonetest.h"
#include "kabc/timezone.h"
#include <qtest.h>

QTEST_MAIN( TimeZoneTest )

void TimeZoneTest::emptyTest()
{
  KABC::TimeZone timezone;

  QVERIFY( timezone.isValid() == false );
}

void TimeZoneTest::storeTest()
{
  KABC::TimeZone timezone;

  timezone.setOffset( 2 );

  QVERIFY( timezone.offset() == 2 );
}

void TimeZoneTest::equalsTest()
{
  KABC::TimeZone timezone1, timezone2;

  timezone1.setOffset( 2 );
  timezone2.setOffset( 2 );

  QVERIFY( timezone1 == timezone2 );
}

void TimeZoneTest::differsTest()
{
  KABC::TimeZone timezone1( 2 );
  KABC::TimeZone timezone2( 3 );

  QVERIFY( timezone1 != timezone2 );
  QVERIFY( timezone1 != KABC::TimeZone() );
}

void TimeZoneTest::assignmentTest()
{
  KABC::TimeZone timezone1, timezone2;

  timezone1.setOffset( 2 );
  timezone1 = timezone2;

  QVERIFY( timezone1 == timezone2 );
}

void TimeZoneTest::serializeTest()
{
  KABC::TimeZone timezone1, timezone2;

  timezone1.setOffset( 2 );

  QByteArray data;
  QDataStream s( &data, QIODevice::WriteOnly );
  s << timezone1;

  QDataStream t( &data, QIODevice::ReadOnly );
  t >> timezone2;

  QVERIFY( timezone1 == timezone2 );
}

