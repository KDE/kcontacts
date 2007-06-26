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

#include "phonenumbertest.h"
#include "kabc/phonenumber.h"
#include <qtest_kde.h>

QTEST_KDEMAIN( PhoneNumberTest, NoGUI )

void PhoneNumberTest::emptyTest()
{
  KABC::PhoneNumber number;

  QVERIFY( number.isEmpty() );
}

void PhoneNumberTest::storeTest()
{
  KABC::PhoneNumber number;

  number.setId( "My Id" );
  number.setType( KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell );
  number.setNumber( "2734826345" );

  QVERIFY( number.id() == QLatin1String( "My Id" ) );
  QVERIFY( number.type() == KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell );
  QVERIFY( number.number() == QLatin1String( "2734826345" ) );
}

void PhoneNumberTest::equalsTest()
{
  KABC::PhoneNumber number1, number2;

  number1.setId( "My Id" );
  number1.setType( KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell );
  number1.setNumber( "2734826345" );

  number2.setId( "My Id" );
  number2.setType( KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell );
  number2.setNumber( "2734826345" );

  QVERIFY( number1 == number2 );
}

void PhoneNumberTest::differsTest()
{
  KABC::PhoneNumber number1( "123", KABC::PhoneNumber::Home );
  KABC::PhoneNumber number2( "123", KABC::PhoneNumber::Work );

  QVERIFY( number1 != number2 );
}

void PhoneNumberTest::assignmentTest()
{
  KABC::PhoneNumber number1, number2;

  number1.setId( "My Id" );
  number1.setType( KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell );
  number1.setNumber( "2734826345" );

  number1 = number2;

  QVERIFY( number1 == number2 );
}

void PhoneNumberTest::serializeTest()
{
  KABC::PhoneNumber number1, number2;

  number1.setId( "My Id" );
  number1.setType( KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell );
  number1.setNumber( "2734826345" );

  QByteArray data;
  QDataStream s( &data, QIODevice::WriteOnly );
  s << number1;

  QDataStream t( &data, QIODevice::ReadOnly );
  t >> number2;

  QVERIFY( number1 == number2 );
}

#include "phonenumbertest.moc"
