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

#include <kabc/addressee.h>

#include "vcardparser.h"

using namespace KABC;

Addressee vcard1()
{
  Addressee addr;

  addr.setName( "Frank Dawson" );
  addr.setOrganization( "Lotus Development Corporation" );
  addr.setUrl( KUrl( "http://home.earthlink.net/~fdawson" ) );
  addr.insertEmail( "fdawson@earthlink.net" );
  addr.insertEmail( "Frank_Dawson@Lotus.com", true );
  addr.insertPhoneNumber( PhoneNumber( "+1-919-676-9515",
                                       PhoneNumber::Voice|PhoneNumber::Msg|PhoneNumber::Work ) );
  addr.insertPhoneNumber( PhoneNumber( "+1-919-676-9564",
                                       PhoneNumber::Fax |PhoneNumber::Work ) );
  Address a( Address::Work | Address::Postal | Address::Parcel );
  a.setStreet( "6544 Battleford Drive" );
  a.setLocality( "Raleigh" );
  a.setRegion( "NC" );
  a.setPostalCode( "27613-3502" );
  a.setCountry( "U.S.A." );
  addr.insertAddress( a );
  return addr;
}

Addressee vcard2()
{
  Addressee addr;

  addr.setName( "Tim Howes" );
  addr.setOrganization( "Netscape Communications Corp." );
  addr.insertEmail( "howes@netscape.com" );
  addr.insertPhoneNumber( PhoneNumber( "+1-415-937-3419",
                                       PhoneNumber::Voice|PhoneNumber::Msg|PhoneNumber::Work ) );
  addr.insertPhoneNumber( PhoneNumber( "+1-415-528-4164",
                                       PhoneNumber::Fax|PhoneNumber::Work ) );
  Address a( Address::Work );
  a.setStreet( "501 E. Middlefield Rd." );
  a.setLocality( "Mountain View" );
  a.setRegion( "CA" );
  a.setPostalCode( "94043" );
  a.setCountry( "U.S.A." );
  addr.insertAddress( a );
  return addr;
}

Addressee vcard3()
{
  Addressee addr;

  addr.setName( "ian geiser" );
  addr.setOrganization( "Source eXtreme" );
  addr.insertEmail( "geiseri@yahoo.com" );
  addr.setTitle( "VP of Engineering" );
  return addr;
}

QByteArray vcardAsText( const QString &location )
{
  QByteArray text;

  QFile file( location );
  if ( file.open( QIODevice::ReadOnly ) ) {
    text = file.readAll();
    file.close();
  }

  return text;
}

Addressee::List vCardsAsAddresseeList()
{
  Addressee::List l;

  l.append( vcard1() );
  l.append( vcard2() );
  l.append( vcard3() );

  return l;
}

QByteArray vCardsAsText()
{
  QByteArray vcards = vcardAsText( "tests/vcard1.vcf" );
  vcards += vcardAsText( "tests/vcard2.vcf" );
  vcards += vcardAsText( "tests/vcard3.vcf" );

  return vcards;
}
