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

#include "picturetest.h"
#include "kabc/picture.h"
#include <QPainter>
#include <qtest_kde.h>

QTEST_KDEMAIN( PictureTest, NoGUI )

static QImage testImage()
{
  static QImage image;

  if ( image.isNull() ) {
    image = QImage( 200, 200, QImage::Format_RGB32 );
    QPainter p( &image );
    p.drawRect( 10, 10, 50, 50 );
  }

  return image;
}

void PictureTest::emptyTest()
{
  KABC::Picture picture;

  QVERIFY( picture.isEmpty() );
}

void PictureTest::storeTestIntern()
{
  KABC::Picture picture;

  picture.setType( QLatin1String( "image/png" ) );
  picture.setData( testImage() );

  QVERIFY( picture.isIntern() == true );
  QVERIFY( picture.type() == QLatin1String( QLatin1String( "image/png" ) ) );
  QVERIFY( picture.data() == testImage() );
}

void PictureTest::storeTestExtern()
{
  KABC::Picture picture;

  picture.setType( QLatin1String( "image/png" ) );
  picture.setUrl( QLatin1String( "http://myhomepage.com/foto.png" ) );

  QVERIFY( picture.isIntern() == false );
  QVERIFY( picture.type() == QLatin1String( QLatin1String( "image/png" ) ) );
  QVERIFY( picture.url() == QLatin1String( QLatin1String( "http://myhomepage.com/foto.png" ) ) );
}

void PictureTest::equalsTestIntern()
{
  KABC::Picture picture1, picture2;

  picture1.setType( QLatin1String( "image/png" ) );
  picture1.setData( testImage() );

  picture2.setType( QLatin1String( "image/png" ) );
  picture2.setData( testImage() );

  QVERIFY( picture1 == picture2 );
}

void PictureTest::equalsTestExtern()
{
  KABC::Picture picture1, picture2;

  picture1.setType( QLatin1String( "image/png" ) );
  picture1.setUrl( QLatin1String( "http://myhomepage.com/foto.png" ) );

  picture2.setType( QLatin1String( "image/png" ) );
  picture2.setUrl( QLatin1String( "http://myhomepage.com/foto.png" ) );

  QVERIFY( picture1 == picture2 );
}

void PictureTest::differsTest()
{
  KABC::Picture picture1, picture2;

  picture1.setType( QLatin1String( "image/png" ) );
  picture1.setUrl( QLatin1String( "http://myhomepage.com/foto.png" ) );

  picture2.setType( QLatin1String( "image/png" ) );
  picture2.setData( testImage() );

  QVERIFY( picture1 != picture2 );
}

void PictureTest::assignmentTestIntern()
{
  KABC::Picture picture1, picture2;

  picture1.setType( QLatin1String( "image/png" ) );
  picture1.setData( testImage() );

  picture2 = picture1;

  QVERIFY( picture1 == picture2 );
}

void PictureTest::assignmentTestExtern()
{
  KABC::Picture picture1, picture2;

  picture1.setType( QLatin1String( "image/png" ) );
  picture1.setUrl( QLatin1String( "http://myhomepage.com/foto.png" ) );

  picture2 = picture1;

  QVERIFY( picture1 == picture2 );
}

void PictureTest::serializeTest()
{
  KABC::Picture picture1, picture2;

  picture1.setType( QLatin1String( "image/png" ) );
  picture1.setUrl( QLatin1String( "http://myhomepage.com/foto.png" ) );
  picture1.setData( testImage() );

  QByteArray data;
  QDataStream s( &data, QIODevice::WriteOnly );
  s << picture1;

  QDataStream t( &data, QIODevice::ReadOnly );
  t >> picture2;

  QVERIFY( picture1 == picture2 );
}

#include "picturetest.moc"
