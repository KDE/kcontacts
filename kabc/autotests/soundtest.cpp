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

#include "soundtest.h"
#include "kabc/sound.h"
#include <qtest.h>

QTEST_MAIN( SoundTest )

static QByteArray testData()
{
  static QByteArray data;

  if ( data.isNull() ) {
    for ( int i = 0; i < 20; ++i ) {
      data.append( char( i ) );
    }
  }

  return data;
}

void SoundTest::emptyTest()
{
  KABC::Sound sound;

  QVERIFY( sound.isEmpty() );
}

void SoundTest::storeTestIntern()
{
  KABC::Sound sound;

  sound.setData( testData() );

  QVERIFY( sound.isIntern() == true );
  QVERIFY( sound.data() == testData() );
}

void SoundTest::storeTestExtern()
{
  KABC::Sound sound;

  sound.setUrl( QLatin1String( "http://myhomepage.com/sound.wav" ) );

  QVERIFY( sound.isIntern() == false );
  QVERIFY( sound.url() == QLatin1String( QLatin1String( "http://myhomepage.com/sound.wav" ) ) );
}

void SoundTest::equalsTestIntern()
{
  KABC::Sound sound1, sound2;

  sound1.setData( testData() );
  sound2.setData( testData() );

  QVERIFY( sound1 == sound2 );
}

void SoundTest::equalsTestExtern()
{
  KABC::Sound sound1, sound2;

  sound1.setUrl( QLatin1String( "http://myhomepage.com/sound.wav" ) );
  sound2.setUrl( QLatin1String( "http://myhomepage.com/sound.wav" ) );

  QVERIFY( sound1 == sound2 );
}

void SoundTest::differsTest()
{
  KABC::Sound sound1, sound2;

  sound1.setUrl( QLatin1String( "http://myhomepage.com/sound.wav" ) );
  sound2.setData( testData() );

  QVERIFY( sound1 != sound2 );
}

void SoundTest::assignmentTestIntern()
{
  KABC::Sound sound1, sound2;

  sound1.setData( testData() );

  sound2 = sound1;

  QVERIFY( sound1 == sound2 );
}

void SoundTest::assignmentTestExtern()
{
  KABC::Sound sound1, sound2;

  sound1.setUrl( QLatin1String( "http://myhomepage.com/sound.wav" ) );

  sound2 = sound1;

  QVERIFY( sound1 == sound2 );
}

void SoundTest::serializeTest()
{
  KABC::Sound sound1, sound2;

  sound1.setUrl( QLatin1String( "http://myhomepage.com/sound.wav" ) );
  sound1.setData( testData() );

  QByteArray data;
  QDataStream s( &data, QIODevice::WriteOnly );
  s << sound1;

  QDataStream t( &data, QIODevice::ReadOnly );
  t >> sound2;

  QVERIFY( sound1 == sound2 );
}

