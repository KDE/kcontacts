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
//krazy:excludeall=contractions

#include "addresseetest.h"
#include "kabc/addressee.h"
#include <qtest.h>

QTEST_MAIN( AddresseeTest )

void AddresseeTest::emptyTest()
{
  KABC::Addressee addressee;
  QVERIFY( addressee.isEmpty() );

  KABC::Addressee addresseeWithMail;
  addresseeWithMail.insertEmail( QLatin1String( "foo@bar.org" ) );
  QVERIFY( !addresseeWithMail.isEmpty() );
}

void AddresseeTest::storeTest()
{
  KABC::Addressee addressee;

  KABC::Picture logo( QLatin1String( "http://scottlandyard.info/pics/logo.png" ) );
  KABC::Picture photo( QLatin1String( "http://scottlandyard.info/~sinclair/photo.png" ) );
  KABC::Sound sound( QLatin1String( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << QLatin1String( "john@sinclair.com" ) << QLatin1String( "ghosthunter@sinclair.com" );

  KABC::Key::List keys;
  keys << KABC::Key( QLatin1String( "SecretKey" ) );

  QStringList categories;
  categories << QLatin1String( "Helper" ) << QLatin1String( "Friend" );

  QStringList customs;
  customs << QLatin1String( "X-Danger: high" );

  addressee.setUid( QLatin1String( "My uid" ) );
  addressee.setName( QLatin1String( "John Sinclair" ) );
  addressee.setFormattedName( QLatin1String( "Sinclair, John" ) );
  addressee.setFamilyName( QLatin1String( "Sinclair" ) );
  addressee.setGivenName( QLatin1String( "John" ) );
  addressee.setAdditionalName( QLatin1String( "Bob" ) );
  addressee.setPrefix( QLatin1String( "Sir" ) );
  addressee.setSuffix( QLatin1String( "II" ) );
  addressee.setNickName( QLatin1String( "ghosthunter" ) );
  addressee.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee.setMailer( QLatin1String( "mutt" ) );
  addressee.setTimeZone( KABC::TimeZone( 2 ) );
  addressee.setGeo( KABC::Geo( 42, 23 ) );
  addressee.setTitle( QLatin1String( "Ghost Hunter" ) );
  addressee.setRole( QLatin1String( "Leader" ) );
  addressee.setOrganization( QLatin1String( "Scottland Yard" ) );
  addressee.setNote( QLatin1String( "Don't cross black deads way..." ) );
  addressee.setProductId( QLatin1String( "ProductId45" ) );
  addressee.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee.setSortString( QLatin1String( "Name" ) );
  addressee.setUrl( KUrl( QLatin1String( "www.scottlandyard.info" ) ) );
  addressee.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee.setLogo( logo );
  addressee.setPhoto( photo );
  addressee.setSound( sound );
  addressee.setEmails( emails );
  addressee.setKeys( keys );
  addressee.setCategories( categories );
  addressee.setCustoms( customs );
  addressee.setChanged( false );

  QVERIFY( addressee.uid() == QLatin1String( "My uid" ) );
  QVERIFY( addressee.name() == QLatin1String( "John Sinclair" ) );
  QVERIFY( addressee.formattedName() == QLatin1String( "Sinclair, John" ) );
  QVERIFY( addressee.familyName() == QLatin1String( "Sinclair" ) );
  QVERIFY( addressee.givenName() == QLatin1String( "John" ) );
  QVERIFY( addressee.additionalName() == QLatin1String( "Bob" ) );
  QVERIFY( addressee.prefix() == QLatin1String( "Sir" ) );
  QVERIFY( addressee.suffix() == QLatin1String( "II" ) );
  QVERIFY( addressee.nickName() == QLatin1String( "ghosthunter" ) );
  QVERIFY( addressee.birthday() == QDateTime( QDate( 1982, 7, 19 ) ) );
  QVERIFY( addressee.mailer() == QLatin1String( "mutt" ) );
  QVERIFY( addressee.timeZone() == KABC::TimeZone( 2 ) );
  QVERIFY( addressee.geo() == KABC::Geo( 42, 23 ) );
  QVERIFY( addressee.title() == QLatin1String( "Ghost Hunter" ) );
  QVERIFY( addressee.role() == QLatin1String( "Leader" ) );
  QVERIFY( addressee.organization() == QLatin1String( "Scottland Yard" ) );
  QVERIFY( addressee.note() == QLatin1String( "Don't cross black deads way..." ) );
  QVERIFY( addressee.productId() == QLatin1String( "ProductId45" ) );
  QVERIFY( addressee.revision() == QDateTime( QDate( 1982, 9, 15 ) ) );
  QVERIFY( addressee.sortString() == QLatin1String( "Name" ) );
  QVERIFY( addressee.url() == KUrl( "www.scottlandyard.info" ) );
  QVERIFY( addressee.secrecy() == KABC::Secrecy( KABC::Secrecy::Public ) );
  QVERIFY( addressee.logo() == logo );
  QVERIFY( addressee.photo() == photo );
  QVERIFY( addressee.sound() == sound );
  QVERIFY( addressee.emails() == emails );
  QVERIFY( addressee.keys() == keys );
  QVERIFY( addressee.categories() == categories );
  QVERIFY( addressee.customs() == customs );
  QVERIFY( addressee.changed() == false );
}

void AddresseeTest::equalsTest()
{
  KABC::Addressee addressee1, addressee2;

  KABC::Picture logo( QLatin1String( "http://scottlandyard.info/pics/logo.png" ) );
  KABC::Picture photo( QLatin1String( "http://scottlandyard.info/~sinclair/photo.png" ) );
  KABC::Sound sound( QLatin1String( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << QLatin1String( "john@sinclair.com" ) << QLatin1String( "ghosthunter@sinclair.com" );

  KABC::Key::List keys;
  keys << KABC::Key( QLatin1String( "SecretKey" ) );

  QStringList categories;
  categories << QLatin1String( "Helper" ) << QLatin1String( "Friend" );

  QStringList customs;
  customs << QLatin1String( "X-Danger: high" );

  addressee1.setUid( QLatin1String( "My uid" ) );
  addressee1.setName( QLatin1String( "John Sinclair" ) );
  addressee1.setFormattedName( QLatin1String( "Sinclair, John" ) );
  addressee1.setFamilyName( QLatin1String( "Sinclair" ) );
  addressee1.setGivenName( QLatin1String( "John" ) );
  addressee1.setAdditionalName( QLatin1String( "Bob" ) );
  addressee1.setPrefix( QLatin1String( "Sir" ) );
  addressee1.setSuffix( QLatin1String( "II" ) );
  addressee1.setNickName( QLatin1String( "ghosthunter" ) );
  addressee1.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee1.setMailer( QLatin1String( "mutt" ) );
  addressee1.setTimeZone( KABC::TimeZone( 2 ) );
  addressee1.setGeo( KABC::Geo( 42, 23 ) );
  addressee1.setTitle( QLatin1String( "Ghost Hunter" ) );
  addressee1.setRole( QLatin1String( "Leader" ) );
  addressee1.setOrganization( QLatin1String( "Scottland Yard" ) );
  addressee1.setNote( QLatin1String( "Don't cross black deads way..." ) );
  addressee1.setProductId( QLatin1String( "ProductId45" ) );
  addressee1.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee1.setSortString( QLatin1String( "Name" ) );
  addressee1.setUrl( KUrl( QLatin1String( "www.scottlandyard.info" ) ) );
  addressee1.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee1.setLogo( logo );
  addressee1.setPhoto( photo );
  addressee1.setSound( sound );
  addressee1.setEmails( emails );
  addressee1.setKeys( keys );
  addressee1.setCategories( categories );
  addressee1.setCustoms( customs );
  addressee1.setChanged( false );

  addressee2.setUid( QLatin1String( "My uid" ) );
  addressee2.setName( QLatin1String( "John Sinclair" ) );
  addressee2.setFormattedName( QLatin1String( "Sinclair, John" ) );
  addressee2.setFamilyName( QLatin1String( "Sinclair" ) );
  addressee2.setGivenName( QLatin1String( "John" ) );
  addressee2.setAdditionalName( QLatin1String( "Bob" ) );
  addressee2.setPrefix( QLatin1String( "Sir" ) );
  addressee2.setSuffix( QLatin1String( "II" ) );
  addressee2.setNickName( QLatin1String( "ghosthunter" ) );
  addressee2.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee2.setMailer( QLatin1String( "mutt" ) );
  addressee2.setTimeZone( KABC::TimeZone( 2 ) );
  addressee2.setGeo( KABC::Geo( 42, 23 ) );
  addressee2.setTitle( QLatin1String( "Ghost Hunter" ) );
  addressee2.setRole( QLatin1String( "Leader" ) );
  addressee2.setOrganization( QLatin1String( "Scottland Yard" ) );
  addressee2.setNote( QLatin1String( "Don't cross black deads way..." ) );
  addressee2.setProductId( QLatin1String( "ProductId45" ) );
  addressee2.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee2.setSortString( QLatin1String( "Name" ) );
  addressee2.setUrl( KUrl( QLatin1String( "www.scottlandyard.info" ) ) );
  addressee2.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee2.setLogo( logo );
  addressee2.setPhoto( photo );
  addressee2.setSound( sound );
  addressee2.setEmails( emails );
  addressee2.setKeys( keys );
  addressee2.setCategories( categories );
  addressee2.setCustoms( customs );
  addressee2.setChanged( false );

  QVERIFY( addressee1 == addressee2 );
}

void AddresseeTest::differsTest()
{
  KABC::Addressee addressee1, addressee2;

  addressee1.setNameFromString( QLatin1String( "John Sinclair" ) );
  addressee2.setNameFromString( QLatin1String( "Suko" ) );

  QVERIFY( addressee1 != addressee2 );
}

void AddresseeTest::assignmentTest()
{
  KABC::Addressee addressee1, addressee2;

  KABC::Picture logo( QLatin1String( "http://scottlandyard.info/pics/logo.png" ) );
  KABC::Picture photo( QLatin1String( "http://scottlandyard.info/~sinclair/photo.png" ) );
  KABC::Sound sound( QLatin1String( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << QLatin1String( "john@sinclair.com" ) << QLatin1String( "ghosthunter@sinclair.com" );

  KABC::Key::List keys;
  keys << KABC::Key( QLatin1String( "SecretKey" ) );

  QStringList categories;
  categories << QLatin1String( "Helper" ) << QLatin1String( "Friend" );

  QStringList customs;
  customs << QLatin1String( "X-Danger: high" );

  addressee1.setUid( QLatin1String( "My uid" ) );
  addressee1.setName( QLatin1String( "John Sinclair" ) );
  addressee1.setFormattedName( QLatin1String( "Sinclair, John" ) );
  addressee1.setFamilyName( QLatin1String( "Sinclair" ) );
  addressee1.setGivenName( QLatin1String( "John" ) );
  addressee1.setAdditionalName( QLatin1String( "Bob" ) );
  addressee1.setPrefix( QLatin1String( "Sir" ) );
  addressee1.setSuffix( QLatin1String( "II" ) );
  addressee1.setNickName( QLatin1String( "ghosthunter" ) );
  addressee1.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee1.setMailer( QLatin1String( "mutt" ) );
  addressee1.setTimeZone( KABC::TimeZone( 2 ) );
  addressee1.setGeo( KABC::Geo( 42, 23 ) );
  addressee1.setTitle( QLatin1String( "Ghost Hunter" ) );
  addressee1.setRole( QLatin1String( "Leader" ) );
  addressee1.setOrganization( QLatin1String( "Scottland Yard" ) );
  addressee1.setNote( QLatin1String( "Don't cross black deads way..." ) );
  addressee1.setProductId( QLatin1String( "ProductId45" ) );
  addressee1.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee1.setSortString( QLatin1String( "Name" ) );
  addressee1.setUrl( KUrl( QLatin1String( "www.scottlandyard.info" ) ) );
  addressee1.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee1.setLogo( logo );
  addressee1.setPhoto( photo );
  addressee1.setSound( sound );
  addressee1.setEmails( emails );
  addressee1.setKeys( keys );
  addressee1.setCategories( categories );
  addressee1.setCustoms( customs );
  addressee1.setChanged( false );

  addressee2 = addressee1;

  QVERIFY( addressee1 == addressee2 );
}

void AddresseeTest::serializeTest()
{
  KABC::Addressee addressee1, addressee2;

  KABC::Picture logo( QLatin1String( "http://scottlandyard.info/pics/logo.png" ) );
  KABC::Picture photo( QLatin1String( "http://scottlandyard.info/~sinclair/photo.png" ) );
  KABC::Sound sound( QLatin1String( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << QLatin1String( "john@sinclair.com" ) << QLatin1String( "ghosthunter@sinclair.com" );

  KABC::Key::List keys;
  keys << KABC::Key( QLatin1String( "SecretKey" ) );

  QStringList categories;
  categories << QLatin1String( "Helper" ) << QLatin1String( "Friend" );

  QStringList customs;
  customs << QLatin1String( "FirstApp-FirstKey:FirstValue" )
          << QLatin1String( "SecondApp-SecondKey:SecondValue" )
          << QLatin1String( "ThirdApp-ThirdKey:ThirdValue" );

  addressee1.setUid( QLatin1String( "My uid" ) );
  addressee1.setName( QLatin1String( "John Sinclair" ) );
  addressee1.setFormattedName( QLatin1String( "Sinclair, John" ) );
  addressee1.setFamilyName( QLatin1String( "Sinclair" ) );
  addressee1.setGivenName( QLatin1String( "John" ) );
  addressee1.setAdditionalName( QLatin1String( "Bob" ) );
  addressee1.setPrefix( QLatin1String( "Sir" ) );
  addressee1.setSuffix( QLatin1String( "II" ) );
  addressee1.setNickName( QLatin1String( "ghosthunter" ) );
  addressee1.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee1.setMailer( QLatin1String( "mutt" ) );
  addressee1.setTimeZone( KABC::TimeZone( 2 ) );
  addressee1.setGeo( KABC::Geo( 42, 23 ) );
  addressee1.setTitle( QLatin1String( "Ghost Hunter" ) );
  addressee1.setRole( QLatin1String( "Leader" ) );
  addressee1.setOrganization( QLatin1String( "Scottland Yard" ) );
  addressee1.setNote( QLatin1String( "Don't cross black deads way..." ) );
  addressee1.setProductId( QLatin1String( "ProductId45" ) );
  addressee1.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee1.setSortString( QLatin1String( "Name" ) );
  addressee1.setUrl( KUrl( QLatin1String( "www.scottlandyard.info" ) ) );
  addressee1.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee1.setLogo( logo );
  addressee1.setPhoto( photo );
  addressee1.setSound( sound );
  addressee1.setEmails( emails );
  addressee1.setKeys( keys );
  addressee1.setCategories( categories );
  addressee1.setCustoms( customs );
  addressee1.setChanged( false );

  QByteArray data;
  QDataStream s( &data, QIODevice::WriteOnly );
  s << addressee1;

  QDataStream t( &data, QIODevice::ReadOnly );
  t >> addressee2;

  QVERIFY( addressee1 == addressee2 );
}

void AddresseeTest::fullEmailTest()
{
  KABC::Addressee a;
  QStringList emails;
  emails << QLatin1String( "foo@bar.com" );
  a.setEmails( emails );
  a.setFormattedName( QLatin1String( "firstname \"nickname\" lastname" ) );
  QCOMPARE( a.fullEmail(),
            QLatin1String( "\"firstname \\\"nickname\\\" lastname\" <foo@bar.com>" ) );
}

void AddresseeTest::nameFromStringTest()
{
  KABC::Addressee a;
  a.setNameFromString( QLatin1String( "Firstname Lastname" ) );
  QCOMPARE( a.givenName(), QLatin1String( "Firstname" ) );
  QCOMPARE( a.familyName(), QLatin1String( "Lastname" ) );
  QCOMPARE( a.formattedName(), QLatin1String( "Firstname Lastname" ) );
}

void AddresseeTest::customFieldsTest()
{
  KABC::Addressee a;

  // test for empty
  QVERIFY( a.customs().isEmpty() );

  // test insert
  a.insertCustom( QLatin1String( "MyApp" ), QLatin1String( "MyKey" ), QLatin1String( "MyValue" ) );
  QCOMPARE( a.customs().count(), 1 );
  QCOMPARE( a.custom( QLatin1String( "MyApp" ), QLatin1String( "MyKey" ) ), QLatin1String( "MyValue" ) );

  a.insertCustom( QLatin1String( "MyApp" ), QLatin1String( "MyKey" ), QLatin1String( "YourValue" ) );
  QCOMPARE( a.customs().count(), 1 ); // still one, we overwrite...
  QCOMPARE( a.custom( QLatin1String( "MyApp" ), QLatin1String( "MyKey" ) ), QLatin1String( "YourValue" ) );

  // test query non-existing app/key
  QCOMPARE( a.custom( QLatin1String( "MyApp" ), QLatin1String( "UnknownKey" ) ), QString() );
  QCOMPARE( a.custom( QLatin1String( "UnknownApp" ), QLatin1String( "MyKey" ) ), QString() );

  // test insert with different key
  a.insertCustom( QLatin1String( "MyApp" ), QLatin1String( "AnotherKey" ), QLatin1String( "OtherValue" ) );
  QCOMPARE( a.customs().count(), 2 );
  QCOMPARE( a.custom( QLatin1String( "MyApp" ), QLatin1String( "AnotherKey" ) ), QLatin1String( "OtherValue" ) );
  QCOMPARE( a.custom( QLatin1String( "MyApp" ), QLatin1String( "MyKey" ) ), QLatin1String( "YourValue" ) );

  // test insert with different app
  a.insertCustom( QLatin1String( "OtherApp" ), QLatin1String( "OtherKey" ), QLatin1String( "OurValue" ) );
  QCOMPARE( a.customs().count(), 3 );
  QCOMPARE( a.custom( QLatin1String( "OtherApp" ), QLatin1String( "OtherKey" ) ), QLatin1String( "OurValue" ) );
  QCOMPARE( a.custom( QLatin1String( "MyApp" ), QLatin1String( "AnotherKey" ) ), QLatin1String( "OtherValue" ) );
  QCOMPARE( a.custom( QLatin1String( "MyApp" ), QLatin1String( "MyKey" ) ), QLatin1String( "YourValue" ) );

  // test customs
  QCOMPARE( a.customs().at( 0 ), QLatin1String( "MyApp-AnotherKey:OtherValue" ) );
  QCOMPARE( a.customs().at( 1 ), QLatin1String( "MyApp-MyKey:YourValue" ) );
  QCOMPARE( a.customs().at( 2 ), QLatin1String( "OtherApp-OtherKey:OurValue" ) );

  // test equal operator
  KABC::Addressee b;
  b.setUid( a.uid() );
  b.insertCustom( QLatin1String( "OtherApp" ), QLatin1String( "OtherKey" ), QLatin1String( "OurValue" ) );
  b.insertCustom( QLatin1String( "MyApp" ), QLatin1String( "MyKey" ), QLatin1String( "YourValue" ) );
  b.insertCustom( QLatin1String( "MyApp" ), QLatin1String( "AnotherKey" ), QLatin1String( "OtherValue" ) );

  QCOMPARE( a, b );

  b.insertCustom( QLatin1String( "MyApp" ), QLatin1String( "AnotherKey" ), QLatin1String( "WrongValue" ) );
  QVERIFY( a != b );

  // test setCustoms
  KABC::Addressee c;
  c.insertCustom( QLatin1String( "ThisApp" ), QLatin1String( "ShouldNotBe" ), QLatin1String( "There" ) );
  QCOMPARE( c.customs().count(), 1 );

  const QStringList testData = QStringList() << QLatin1String( "FirstApp-FirstKey:FirstValue" )
                                             << QLatin1String( "SecondApp-SecondKey:SecondValue" )
                                             << QLatin1String( "ThirdApp-ThirdKey:ThirdValue" );

  c.setCustoms( testData );
  QCOMPARE( c.customs().count(), 3 );

  QCOMPARE( c.custom( QLatin1String( "FirstApp" ), QLatin1String( "FirstKey" ) ), QLatin1String( "FirstValue" ) );
  QCOMPARE( c.custom( QLatin1String( "SecondApp" ), QLatin1String( "SecondKey" ) ), QLatin1String( "SecondValue" ) );
  QCOMPARE( c.custom( QLatin1String( "ThirdApp" ), QLatin1String( "ThirdKey" ) ), QLatin1String( "ThirdValue" ) );

  // test remove
  QCOMPARE( c.customs().count(), 3 );
  c.removeCustom( QLatin1String( "UnknownApp" ), QLatin1String( "FirstKey" ) );
  QCOMPARE( c.customs().count(), 3 );
  c.removeCustom( QLatin1String( "FirstApp" ), QLatin1String( "UnknownKey" ) );
  QCOMPARE( c.customs().count(), 3 );
  c.removeCustom( QLatin1String( "FirstApp" ), QLatin1String( "FirstKey" ) );
  QCOMPARE( c.customs().count(), 2 );
}
