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

#include "addressbook.h"
#include "vcardformat.h"
#include "plugins/file/resourcefile.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <qdebug.h>
#include <klocalizedstring.h>
#include <kcmdlineargs.h>

#include <sys/times.h>

using namespace KABC;

int main( int argc, char **argv )
{
  KAboutData aboutData( "bigwrite", 0, ki18n( "BigWriteKabc" ), "0.1" );
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app( false );

  AddressBook ab;
  ResourceFile r( QLatin1String( "my.kabc" ), QLatin1String( "vcard" ) );
  ab.addResource( &r );

  for ( int i = 0; i < 5000; ++i ) {
    Addressee a;
    a.setGivenName( QLatin1String( "number" ) + QString::number( i ) );
    a.setFamilyName( QLatin1String( "Name" ) );
    a.insertEmail( QString::number( i ) + QLatin1String( "@domain" ) );

    ab.insertAddressee( a );
  }
  printf( "\n" );

  Ticket *t = ab.requestSaveTicket( &r );
  if ( t ) {
    struct tms start;

    times( &start );

#if 0
    qDebug() << "utime :" << int( start.tms_utime );
    qDebug() << "stime :" << int( start.tms_stime );
    qDebug() << "cutime:" << int( start.tms_cutime );
    qDebug() << "cstime:" << int( start.tms_cstime );
#endif

    if ( !ab.save( t ) ) {
      qDebug() << "Can't save.";
    }

    struct tms end;

    times( &end );

#if 0
    qDebug() << "utime :" << int( end.tms_utime );
    qDebug() << "stime :" << int( end.tms_stime );
    qDebug() << "cutime:" << int( end.tms_cutime );
    qDebug() << "cstime:" << int( end.tms_cstime );
#endif

    qDebug() << "UTime:" << int( end.tms_utime ) - int( start.tms_utime );
    qDebug() << "STime:" << int( end.tms_stime ) - int( start.tms_stime );

  } else {
    qDebug() << "No ticket for save.";
  }
}
