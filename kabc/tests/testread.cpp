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

#include <iostream>
#include <stdlib.h>

#include <QtCore/QFile>

#include <qdebug.h>
#include <kcomponentdata.h>
#include <kcmdlineargs.h>
#include <klocalizedstring.h>
#include <kaboutdata.h>

#include "kabc/vcardconverter.h"
#include "vcard.h"

int main( int argc, char **argv )
{
  KAboutData aboutData( "testread", 0, ki18n( "vCard test reader" ), "0.1" );
  aboutData.addAuthor( ki18n( "Cornelius Schumacher" ), KLocalizedString(), "schumacher@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );

  KCmdLineOptions options;
  options.add( "vcard21", ki18n( "vCard 2.1" ) );
  options.add( "+inputfile", ki18n( "Input file" ) );
  KCmdLineArgs::addCmdLineOptions( options );

  KComponentData componentData( &aboutData );
  // QCoreApp not needed

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
