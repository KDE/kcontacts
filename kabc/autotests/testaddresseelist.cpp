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

#include <kaboutdata.h>
#include <qdebug.h>
#include <klocalizedstring.h>
#include <kcmdlineargs.h>


#include "addressbook.h"
#include "addresseelist.h"

using namespace KABC;

int main( int, char** )
{
/*  KAboutData aboutData("testaddresseelist", 0,ki18n("TestAddresseeList"),"0.1");
  KCmdLineArgs::init(argc, argv, &aboutData);

  KCmdLineOptions options;
  options.add( "save" );
  options.add( "number" );
  KCmdLineArgs::addCmdLineOptions( options );

  KApplication app;
  KCmdLineArgs* args = KCmdLineArgs::parsedArgs(); */

  qDebug() << "Creating addressees";
  Addressee a, b, c, d, e, f;
  a.setGivenName( "Peter" );
  a.setFamilyName( "Pan" );
  a.setFormattedName( "Pan, Peter" );
  a.setUid( "Asdf" );
  b.setGivenName( "Phileas" );
  b.setFamilyName( "Fogg" );
  b.setFormattedName( "Fogg, Phileas" );
  b.setUid( "Rsdf" );
  c.setGivenName( "Jim" );
  c.setFamilyName( "Hawkins" );
  c.setFormattedName( "Hawkins, Jim" );
  c.setUid( "Fhwn" );
  d.setGivenName( "John" );
  d.setFamilyName( "Silver" );
  d.setPrefix( "Long" );
  d.setFormattedName( "Long John Silver" );
  d.setUid( "Z2hk" );
  e.setGivenName( "Alice" );
  e.setFamilyName( "Liddel" );
  e.setFormattedName( "Liddel, Alice" );
  e.setUid( "kk45" );
  f.setGivenName( "Edmond" );
  f.setFamilyName( "Dantes" );
  f.setFormattedName( "Dantes, Edmond" );
  f.setUid( "78ze" );

  qDebug() << "Adding to list";
  AddresseeList list;
  list.append( a );
  list.append( b );
  list.append( c );
  list.append( d );
  list.append( e );
  list.append( f );

  list.sortBy( FamilyName );
  if ( !( ( *list.at( 0 ) ).uid() == "78ze" &&
          ( *list.at( 1 ) ).uid() == "Rsdf" &&
          ( *list.at( 2 ) ).uid() == "Fhwn" &&
          ( *list.at( 3 ) ).uid() == "kk45" &&
          ( *list.at( 4 ) ).uid() == "Asdf" &&
          ( *list.at( 5 ) ).uid() == "Z2hk" ) ) {
      qCritical() << "SORTING BY FAMILY NAME NOT CORRECT!";
      qDebug() << "list sorted by family name:";
      list.dump();
  } else {
      qDebug() << "Sorting by family name correct.";
  }
  list.setReverseSorting( true );
  list.sort();
  if ( !( ( *list.at( 5 ) ).uid() == "78ze" &&
          ( *list.at( 4 ) ).uid() == "Rsdf" &&
          ( *list.at( 3 ) ).uid() == "Fhwn" &&
          ( *list.at( 2 ) ).uid() == "kk45" &&
          ( *list.at( 1 ) ).uid() == "Asdf" &&
          ( *list.at( 0 ) ).uid() == "Z2hk" ) ) {
      qCritical() << "REVERSE SORTING BY FAMILY NAME NOT CORRECT!";
      qDebug() << "list reverse sorted by family name:";
      list.dump();
  } else {
      qDebug() << "Reverse sorting by family name correct.";
  }

  list.setReverseSorting( false );
  list.sortBy( FormattedName );
  if ( !( ( *list.at( 0 ) ).uid() == "78ze" &&
          ( *list.at( 1 ) ).uid() == "Rsdf" &&
          ( *list.at( 2 ) ).uid() == "Fhwn" &&
          ( *list.at( 3 ) ).uid() == "kk45" &&
          ( *list.at( 4 ) ).uid() == "Z2hk" &&
          ( *list.at( 5 ) ).uid() == "Asdf" ) ) {
      qCritical() << "SORTING BY FORMATTED NAME NOT CORRECT!";
      qDebug() << "list sorted by formatted name:";
      list.dump();
  } else {
      qDebug() << "Sorting by formatted name correct.";
  }
  list.setReverseSorting( true );
  list.sort();
  if ( !( ( *list.at( 5 ) ).uid() == "78ze" &&
          ( *list.at( 4 ) ).uid() == "Rsdf" &&
          ( *list.at( 3 ) ).uid() == "Fhwn" &&
          ( *list.at( 2 ) ).uid() == "kk45" &&
          ( *list.at( 1 ) ).uid() == "Z2hk" &&
          ( *list.at( 0 ) ).uid() == "Asdf" ) ) {
      qCritical() << "REVERSE SORTING BY FORMATTED NAME NOT CORRECT!";
      qDebug() << "list reverse sorted by formatted name:";
      list.dump();
  } else {
      qDebug() << "Reverse sorting by formatted name correct.";
  }

  list.setReverseSorting( false );
  list.sortBy( Uid );
  if ( !( ( *list.at( 0 ) ).uid() == "78ze" &&
          ( *list.at( 1 ) ).uid() == "Asdf" &&
          ( *list.at( 2 ) ).uid() == "Fhwn" &&
          ( *list.at( 3 ) ).uid() == "Rsdf" &&
          ( *list.at( 4 ) ).uid() == "Z2hk" &&
          ( *list.at( 5 ) ).uid() == "kk45" ) ) {
      qCritical() << "SORTING BY UID NOT CORRECT!";
      qDebug() << "list sorted by Uid:";
      list.dump();
  } else {
      qDebug() << "Sorting by Uid correct.";
  }
  list.setReverseSorting( true );
  list.sortBy( Uid );
  if ( !( ( *list.at( 5 ) ).uid() == "78ze" &&
          ( *list.at( 4 ) ).uid() == "Asdf" &&
          ( *list.at( 3 ) ).uid() == "Fhwn" &&
          ( *list.at( 2 ) ).uid() == "Rsdf" &&
          ( *list.at( 1 ) ).uid() == "Z2hk" &&
          ( *list.at( 0 ) ).uid() == "kk45" ) ) {
      qCritical() << "REVERSE SORTING BY UID NOT CORRECT!";
      qDebug() << "list sorted by Uid:";
      list.dump();
  } else {
      qDebug() << "Reverse sorting by Uid correct.";
  }

  // zero, one or two entries might give errors in a poor sorting
  // implementation
  qDebug() << "sorting empty list";
  AddresseeList list2;
  list2.sort();

  qDebug() << "sorting one entry list";
  list2.append( a );
  list2.sort();

  qDebug() << "sorting two entry list";
  list2.append( f );
  list2.setReverseSorting( false );
  list2.sort();
  if ( !( ( *list2.at( 0 ) ).uid() == "78ze" &&
          ( *list2.at( 1 ) ).uid() == "Asdf" ) ) {
      qCritical() << "SORTING BY FORMATTED NAME IN A TWO ENTRY LIST NOT CORRECT!";
      qDebug() << "list sorted by formatted name, two entries:";
      list2.dump();
  } else {
      qDebug() << "Sorting by FormattedName in a two entry list correct.";
  }
  list2.setReverseSorting( true );
  list2.sort();
  if ( !( ( *list2.at( 1 ) ).uid() == "78ze" &&
          ( *list2.at( 0 ) ).uid() == "Asdf" ) ) {
      qCritical() << "REVERSE SORTING BY FORMATTED NAME IN A TWO ENTRY LIST NOT CORRECT!";
      qDebug() << "list reverse sorted by formatted name, two entries:";
      list2.dump();
  } else {
      qDebug() << "Reverse sorting by FormattedName in a two entry list correct.";
  }

}

