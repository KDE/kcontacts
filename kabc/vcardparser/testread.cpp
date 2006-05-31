/*
    This file is part of libkabc.

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

#include <iostream>
#include <stdlib.h>

#include <QFile>

#include <kprocess.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kaboutdata.h>

#include "vcardconverter.h"
#include "vcard.h"

static const KCmdLineOptions options[] =
{
  {"vcard21", I18N_NOOP("vCard 2.1"), 0},
  {"+inputfile", I18N_NOOP("Input file"), 0},
  KCmdLineLastOption
};

int main( int argc, char **argv )
{
  //KApplication::disableAutoDcopRegistration(); // nice thought, but KUrl -> ksycoca -> DCOP attach() anyway

  KAboutData aboutData( "testread", "vCard test reader", "0.1" );
  aboutData.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );

  KApplication app( false );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() != 1 ) {
    std::cerr << "Missing argument" << std::endl;
    return 1;
  }

  QString inputFile( args->arg( 0 ) );

  QFile file( inputFile );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qDebug( "Unable to open file '%s' for reading!", qPrintable( file.fileName() ) );
    return 1;
  }

  QByteArray text = file.readAll();
  file.close();

  KABC::VCardConverter converter;
  KABC::Addressee::List list = converter.parseVCards( text );

  if ( args->isSet( "vcard21" ) ) {
    text = converter.createVCards( list, KABC::VCardConverter::v2_1 ); // uses version 2.1
  } else {
    text = converter.createVCards( list ); // uses version 3.0
  }

  std::cout << text.data();

  return 0;
}
