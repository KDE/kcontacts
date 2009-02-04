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
  KAboutData aboutData( "bigread", 0, ki18n( "BigReadKabc" ), "0.1" );
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app( false );

  AddressBook ab;

  ResourceFile r( QLatin1String( "my.kabc" ), QLatin1String( "vcard2" ) );
  ab.addResource( &r );

#if 0
  ResourceSql rsql( &ab, "root", "kde4ever", "localhost" );
  ab.addResource( &rsql );
#endif

  struct tms start;

  times( &start );

#if 0
  kDebug() << "utime :" << int( start.tms_utime );
  kDebug() << "stime :" << int( start.tms_stime );
  kDebug() << "cutime:" << int( start.tms_cutime );
  kDebug() << "cstime:" << int( start.tms_cstime );
#endif

  kDebug() << "Start load";
  ab.load();
  kDebug() << "Finished load";

  struct tms end;

  times( &end );

#if 0
  kDebug() << "utime :" << int( end.tms_utime );
  kDebug() << "stime :" << int( end.tms_stime );
  kDebug() << "cutime:" << int( end.tms_cutime );
  kDebug() << "cstime:" << int( end.tms_cstime );
#endif

  kDebug() << "UTime:" << int( end.tms_utime ) - int( start.tms_utime );
  kDebug() << "STime:" << int( end.tms_stime ) - int( start.tms_stime );

//  ab.dump();
}
