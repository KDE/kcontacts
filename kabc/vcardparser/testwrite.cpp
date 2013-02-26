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

#include <QtCore/QFile>

#include <kaboutdata.h>
#include <klocalizedstring.h>
#include <klocale.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "kabc/addressee.h"
#include "kabc/phonenumber.h"
#include "kabc/address.h"
#include "kabc/key.h"
#include "kabc/picture.h"
#include "kabc/sound.h"
#include "kabc/secrecy.h"
#include "kabc/vcardconverter.h"

int main( int argc, char **argv )
{
  KAboutData aboutData( "testwrite", 0, ki18n( "vCard test writer" ), "0.1" );

  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app( false );

  KABC::Addressee addressee;

  addressee.setNameFromString( QLatin1String( "Mr. Tobias Koenig Jr." ) );
  addressee.setNickName( QLatin1String( "tokoe" ) );
  addressee.setBirthday( QDateTime( QDate( 1982, 7, 19 ) ) );
  addressee.setMailer( QLatin1String( "mutt1.2" ) );
  addressee.setTimeZone( KABC::TimeZone( +2 ) );

  KABC::Geo geo;
  geo.setLatitude( 30 );
  geo.setLongitude( 51 );
  addressee.setGeo( geo );

  addressee.setTitle( QLatin1String( "nerd" ) );
  addressee.setRole( QLatin1String( "Maintainer" ) );
  addressee.setOrganization( QLatin1String( "KDE" ) );
  addressee.setNote( QLatin1String( "nerver\ntouch a running system" ) );
  addressee.setProductId( QLatin1String( "testId" ) );
  addressee.setRevision( QDateTime::currentDateTime() );
  addressee.setSortString( QLatin1String( "koenig" ) );
  addressee.setUrl( KUrl( QLatin1String( "http://wgess16.dyndns.org" ) ) );
  addressee.setSecrecy( KABC::Secrecy( KABC::Secrecy::Confidential ) );

  addressee.insertEmail( QLatin1String( "tokoe@kde.org" ), true );
  addressee.insertEmail( QLatin1String( "tokoe82@yahoo.de" ), true );

  KABC::PhoneNumber phone1( QLatin1String( "3541523475" ),
                            KABC::PhoneNumber::Pref | KABC::PhoneNumber::Home );
  KABC::PhoneNumber phone2( QLatin1String( "+46745673475" ),
                            KABC::PhoneNumber::Work );
  addressee.insertPhoneNumber( phone1 );
  addressee.insertPhoneNumber( phone2 );

  KABC::Key key( QLatin1String( "secret key" ), KABC::Key::X509 );
  addressee.insertKey( key );

  QStringList categories;
  categories << QLatin1String( "Friends" ) << QLatin1String( "School" ) << QLatin1String( "KDE" );
  addressee.setCategories( categories );

  KABC::Address a( KABC::Address::Work | KABC::Address::Postal | KABC::Address::Parcel );
  a.setStreet( QLatin1String( "6544 Battleford Drive" ) );
  a.setLocality( QLatin1String( "Raleigh" ) );
  a.setRegion( QLatin1String( "NC" ) );
  a.setPostalCode( QLatin1String( "27613-3502" ) );
  a.setCountry( QLatin1String( "U.S.A." ) );
  addressee.insertAddress( a );

  addressee.insertCustom( QLatin1String( "1hsdf" ), QLatin1String( "ertuer" ),
                          QLatin1String( "iurt" ) );
  addressee.insertCustom( QLatin1String( "2hsdf" ), QLatin1String( "ertuer" ),
                          QLatin1String( "iurt" ) );
  addressee.insertCustom( QLatin1String( "3hsdf" ), QLatin1String( "ertuer" ),
                          QLatin1String( "iurt" ) );

  KABC::Addressee::List list;
  for ( int i = 0; i < 1000; ++i ) {
    KABC::Addressee addr = addressee;
    addr.setUid( QString::number( i ) );
    list.append( addr );
  }

  KABC::VCardConverter converter;
  QByteArray txt = converter.createVCards( list );

  QFile file( QLatin1String( "out.vcf" ) );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    qDebug( "Can't open file '%s' fro writing", qPrintable( file.fileName() ) );
    return 1;
  }

  file.write( txt );
  file.close();

  return 0;
}
