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

#include "kabc/addressbook.h"
#include "kabc/vcardformat.h"
#include "kabc/plugins/file/resourcefile.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>

#include <sys/times.h>

using namespace KABC;

int main( int argc, char **argv )
{
  KAboutData aboutData( "bigwrite", 0, ki18n( "BigWriteKabc" ), "0.1" );
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app( false );

  AddressBook ab;
  ResourceFile r( "my.kabc", "vcard" );
  ab.addResource( &r );

  for ( int i = 0; i < 5000; ++i ) {
    Addressee a;
    a.setGivenName( "number" + QString::number( i ) );
    a.setFamilyName( "Name" );
    a.insertEmail( QString::number( i ) + "@domain" );

    ab.insertAddressee( a );
  }
  printf( "\n" );

  Ticket *t = ab.requestSaveTicket( &r );
  if ( t ) {
    struct tms start;

    times( &start );

#if 0
    kDebug() << "utime : " << int( start.tms_utime ) << endl;
    kDebug() << "stime : " << int( start.tms_stime ) << endl;
    kDebug() << "cutime: " << int( start.tms_cutime ) << endl;
    kDebug() << "cstime: " << int( start.tms_cstime ) << endl;
#endif

    if ( !ab.save( t ) ) {
      kDebug() << "Can't save." << endl;
    }

    struct tms end;

    times( &end );

#if 0
    kDebug() << "utime : " << int( end.tms_utime ) << endl;
    kDebug() << "stime : " << int( end.tms_stime ) << endl;
    kDebug() << "cutime: " << int( end.tms_cutime ) << endl;
    kDebug() << "cstime: " << int( end.tms_cstime ) << endl;
#endif

    kDebug() << "UTime: " << int( end.tms_utime ) - int( start.tms_utime ) << endl;
    kDebug() << "STime: " << int( end.tms_stime ) - int( start.tms_stime ) << endl;

  } else {
    kDebug() << "No ticket for save." << endl;
  }
}
