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
#include <qtest_kde.h>

QTEST_KDEMAIN( AddresseeTest, NoGUI )

void AddresseeTest::emptyTest()
{
  KABC::Addressee addressee;

  QVERIFY( addressee.isEmpty() );
}

void AddresseeTest::storeTest()
{
  KABC::Addressee addressee;

  KABC::Picture logo( "http://scottlandyard.info/pics/logo.png" );
  KABC::Picture photo( "http://scottlandyard.info/~sinclair/photo.png" );
  KABC::Sound sound( QString( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << "john@sinclair.com" << "ghosthunter@sinclair.com";

  KABC::Key::List keys;
  keys << KABC::Key( "SecretKey" );

  QStringList categories;
  categories << "Helper" << "Friend";

  QStringList customs;
  customs << "X-Danger: high";

  addressee.setUid( "My uid" );
  addressee.setName( "John Sinclair" );
  addressee.setFormattedName( "Sinclair, John" );
  addressee.setFamilyName( "Sinclair" );
  addressee.setGivenName( "John" );
  addressee.setAdditionalName( "Bob" );
  addressee.setPrefix( "Sir" );
  addressee.setSuffix( "II" );
  addressee.setNickName( "ghosthunter" );
  addressee.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee.setMailer( "mutt" );
  addressee.setTimeZone( KABC::TimeZone( 2 ) );
  addressee.setGeo( KABC::Geo( 42, 23 ) );
  addressee.setTitle( "Ghost Hunter" );
  addressee.setRole( "Leader" );
  addressee.setOrganization( "Scottland Yard" );
  addressee.setNote( "Don't cross black deads way..." );
  addressee.setProductId( "ProductId45" );
  addressee.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee.setSortString( "Name" );
  addressee.setUrl( KUrl( "www.scottlandyard.info" ) );
  addressee.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee.setLogo( logo );
  addressee.setPhoto( photo );
  addressee.setSound( sound );
  addressee.setEmails( emails );
  addressee.setKeys( keys );
  addressee.setCategories( categories );
  addressee.setCustoms( customs );
  addressee.setResource( 0 );
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
  QVERIFY( addressee.resource() == 0 );
  QVERIFY( addressee.changed() == false );
}

void AddresseeTest::equalsTest()
{
  KABC::Addressee addressee1, addressee2;

  KABC::Picture logo( "http://scottlandyard.info/pics/logo.png" );
  KABC::Picture photo( "http://scottlandyard.info/~sinclair/photo.png" );
  KABC::Sound sound( QString( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << "john@sinclair.com" << "ghosthunter@sinclair.com";

  KABC::Key::List keys;
  keys << KABC::Key( "SecretKey" );

  QStringList categories;
  categories << "Helper" << "Friend";

  QStringList customs;
  customs << "X-Danger: high";

  addressee1.setUid( "My uid" );
  addressee1.setName( "John Sinclair" );
  addressee1.setFormattedName( "Sinclair, John" );
  addressee1.setFamilyName( "Sinclair" );
  addressee1.setGivenName( "John" );
  addressee1.setAdditionalName( "Bob" );
  addressee1.setPrefix( "Sir" );
  addressee1.setSuffix( "II" );
  addressee1.setNickName( "ghosthunter" );
  addressee1.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee1.setMailer( "mutt" );
  addressee1.setTimeZone( KABC::TimeZone( 2 ) );
  addressee1.setGeo( KABC::Geo( 42, 23 ) );
  addressee1.setTitle( "Ghost Hunter" );
  addressee1.setRole( "Leader" );
  addressee1.setOrganization( "Scottland Yard" );
  addressee1.setNote( "Don't cross black deads way..." );
  addressee1.setProductId( "ProductId45" );
  addressee1.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee1.setSortString( "Name" );
  addressee1.setUrl( KUrl( "www.scottlandyard.info" ) );
  addressee1.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee1.setLogo( logo );
  addressee1.setPhoto( photo );
  addressee1.setSound( sound );
  addressee1.setEmails( emails );
  addressee1.setKeys( keys );
  addressee1.setCategories( categories );
  addressee1.setCustoms( customs );
  addressee1.setResource( 0 );
  addressee1.setChanged( false );

  addressee2.setUid( "My uid" );
  addressee2.setName( "John Sinclair" );
  addressee2.setFormattedName( "Sinclair, John" );
  addressee2.setFamilyName( "Sinclair" );
  addressee2.setGivenName( "John" );
  addressee2.setAdditionalName( "Bob" );
  addressee2.setPrefix( "Sir" );
  addressee2.setSuffix( "II" );
  addressee2.setNickName( "ghosthunter" );
  addressee2.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee2.setMailer( "mutt" );
  addressee2.setTimeZone( KABC::TimeZone( 2 ) );
  addressee2.setGeo( KABC::Geo( 42, 23 ) );
  addressee2.setTitle( "Ghost Hunter" );
  addressee2.setRole( "Leader" );
  addressee2.setOrganization( "Scottland Yard" );
  addressee2.setNote( "Don't cross black deads way..." );
  addressee2.setProductId( "ProductId45" );
  addressee2.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee2.setSortString( "Name" );
  addressee2.setUrl( KUrl( "www.scottlandyard.info" ) );
  addressee2.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee2.setLogo( logo );
  addressee2.setPhoto( photo );
  addressee2.setSound( sound );
  addressee2.setEmails( emails );
  addressee2.setKeys( keys );
  addressee2.setCategories( categories );
  addressee2.setCustoms( customs );
  addressee2.setResource( 0 );
  addressee2.setChanged( false );

  QVERIFY( addressee1 == addressee2 );
}

void AddresseeTest::differsTest()
{
  KABC::Addressee addressee1, addressee2;

  addressee1.setNameFromString( "John Sinclair" );
  addressee2.setNameFromString( "Suko" );

  QVERIFY( addressee1 != addressee2 );
}

void AddresseeTest::assignmentTest()
{
  KABC::Addressee addressee1, addressee2;

  KABC::Picture logo( "http://scottlandyard.info/pics/logo.png" );
  KABC::Picture photo( "http://scottlandyard.info/~sinclair/photo.png" );
  KABC::Sound sound( QString( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << "john@sinclair.com" << "ghosthunter@sinclair.com";

  KABC::Key::List keys;
  keys << KABC::Key( "SecretKey" );

  QStringList categories;
  categories << "Helper" << "Friend";

  QStringList customs;
  customs << "X-Danger: high";

  addressee1.setUid( "My uid" );
  addressee1.setName( "John Sinclair" );
  addressee1.setFormattedName( "Sinclair, John" );
  addressee1.setFamilyName( "Sinclair" );
  addressee1.setGivenName( "John" );
  addressee1.setAdditionalName( "Bob" );
  addressee1.setPrefix( "Sir" );
  addressee1.setSuffix( "II" );
  addressee1.setNickName( "ghosthunter" );
  addressee1.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee1.setMailer( "mutt" );
  addressee1.setTimeZone( KABC::TimeZone( 2 ) );
  addressee1.setGeo( KABC::Geo( 42, 23 ) );
  addressee1.setTitle( "Ghost Hunter" );
  addressee1.setRole( "Leader" );
  addressee1.setOrganization( "Scottland Yard" );
  addressee1.setNote( "Don't cross black deads way..." );
  addressee1.setProductId( "ProductId45" );
  addressee1.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee1.setSortString( "Name" );
  addressee1.setUrl( KUrl( "www.scottlandyard.info" ) );
  addressee1.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee1.setLogo( logo );
  addressee1.setPhoto( photo );
  addressee1.setSound( sound );
  addressee1.setEmails( emails );
  addressee1.setKeys( keys );
  addressee1.setCategories( categories );
  addressee1.setCustoms( customs );
  addressee1.setResource( 0 );
  addressee1.setChanged( false );

  addressee2 = addressee1;

  QVERIFY( addressee1 == addressee2 );
}

void AddresseeTest::serializeTest()
{
  KABC::Addressee addressee1, addressee2;

  KABC::Picture logo( "http://scottlandyard.info/pics/logo.png" );
  KABC::Picture photo( "http://scottlandyard.info/~sinclair/photo.png" );
  KABC::Sound sound( QString( "http://scottlandyard.info/~sinclair/sound.wav" ) );

  QStringList emails;
  emails << "john@sinclair.com" << "ghosthunter@sinclair.com";

  KABC::Key::List keys;
  keys << KABC::Key( "SecretKey" );

  QStringList categories;
  categories << "Helper" << "Friend";

  QStringList customs;
  customs << "X-Danger: high";

  addressee1.setUid( "My uid" );
  addressee1.setName( "John Sinclair" );
  addressee1.setFormattedName( "Sinclair, John" );
  addressee1.setFamilyName( "Sinclair" );
  addressee1.setGivenName( "John" );
  addressee1.setAdditionalName( "Bob" );
  addressee1.setPrefix( "Sir" );
  addressee1.setSuffix( "II" );
  addressee1.setNickName( "ghosthunter" );
  addressee1.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee1.setMailer( "mutt" );
  addressee1.setTimeZone( KABC::TimeZone( 2 ) );
  addressee1.setGeo( KABC::Geo( 42, 23 ) );
  addressee1.setTitle( "Ghost Hunter" );
  addressee1.setRole( "Leader" );
  addressee1.setOrganization( "Scottland Yard" );
  addressee1.setNote( "Don't cross black deads way..." );
  addressee1.setProductId( "ProductId45" );
  addressee1.setRevision( QDateTime( QDate( 1982, 9, 15 ) ) );
  addressee1.setSortString( "Name" );
  addressee1.setUrl( KUrl( "www.scottlandyard.info" ) );
  addressee1.setSecrecy( KABC::Secrecy( KABC::Secrecy::Public ) );
  addressee1.setLogo( logo );
  addressee1.setPhoto( photo );
  addressee1.setSound( sound );
  addressee1.setEmails( emails );
  addressee1.setKeys( keys );
  addressee1.setCategories( categories );
  addressee1.setCustoms( customs );
  addressee1.setResource( 0 );
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
  emails << "foo@bar.com";
  a.setEmails( emails );
  a.setFormattedName( "firstname \"nickname\" lastname" );
  QCOMPARE( a.fullEmail(), QString( "\"firstname \\\"nickname\\\" lastname\" <foo@bar.com>" ) );
}

#include "addresseetest.moc"
