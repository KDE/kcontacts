/*
    This file is part of libkabc.
    Copyright (c) 2003  Helge Deller <deller@kde.org>

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

/*
    Useful links:
        - http://tldp.org/HOWTO/LDAP-Implementation-HOWTO/schemas.html
        - http://www.faqs.org/rfcs/rfc2849.html

    Not yet handled items:
        - objectclass microsoftaddressbook
                - info,
                - initials,
                - otherfacsimiletelephonenumber,
                - otherpager,
                - physicaldeliveryofficename,
*/

#include "ldifconverter.h"
#include "vcardconverter.h"
#include "address.h"
#include "addressee.h"
#include "kabc_debug.h"

#include "ldif_p.h"

#include <klocalizedstring.h>

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

using namespace KABC;

/* generate LDIF stream */

bool LDIFConverter::addresseeToLDIF( const AddresseeList &addrList, QString &str )
{
  AddresseeList::ConstIterator it;
  AddresseeList::ConstIterator end( addrList.constEnd() );
  for ( it = addrList.constBegin(); it != end; ++it ) {
    addresseeToLDIF( *it, str );
  }
  return true;
}

static void ldif_out( QTextStream &t, const QString &formatStr,
                      const QString &value )
{
  if ( value.isEmpty() ) {
    return;
  }

  const QByteArray txt = Ldif::assembleLine( formatStr, value, 72 );

  // write the string
  t << QString::fromUtf8( txt ) << "\n";
}

bool LDIFConverter::addresseeToLDIF( const Addressee &addr, QString &str )
{
  if ( addr.isEmpty() ) {
    return false;
  }

  QTextStream t( &str, QIODevice::WriteOnly|QIODevice::Append );
  t.setCodec( QTextCodec::codecForName( "UTF-8" ) );

  const Address homeAddr = addr.address( Address::Home );
  const Address workAddr = addr.address( Address::Work );

  ldif_out( t, QLatin1String( "dn" ), QString::fromLatin1( "cn=%1,mail=%2" ).
            arg( addr.formattedName().simplified() ).
            arg( addr.preferredEmail() ) );
  ldif_out( t, QLatin1String( "givenname" ), addr.givenName() );
  ldif_out( t, QLatin1String( "sn" ), addr.familyName() );
  ldif_out( t, QLatin1String( "cn" ), addr.formattedName().simplified() );
  ldif_out( t, QLatin1String( "uid" ), addr.uid() );
  ldif_out( t, QLatin1String( "nickname" ), addr.nickName() );
  ldif_out( t, QLatin1String( "xmozillanickname" ), addr.nickName() );
  ldif_out( t, QLatin1String( "mozillanickname" ), addr.nickName() );

  ldif_out( t, QLatin1String( "mail" ), addr.preferredEmail() );
  if ( addr.emails().count() > 1 ) {
    ldif_out( t, QLatin1String( "mozillasecondemail" ), addr.emails()[ 1 ] );
  }
//ldif_out( t, "mozilla_AIMScreenName: %1\n", "screen_name" );

  ldif_out( t, QLatin1String( "telephonenumber" ),
            addr.phoneNumber( PhoneNumber::Work ).number() );
  ldif_out( t, QLatin1String( "facsimiletelephonenumber" ),
            addr.phoneNumber( PhoneNumber::Fax ).number() );
  ldif_out( t, QLatin1String( "homephone" ),
            addr.phoneNumber( PhoneNumber::Home ).number() );
  ldif_out( t, QLatin1String( "mobile" ),
            addr.phoneNumber( PhoneNumber::Cell ).number() ); // Netscape 7
  ldif_out( t, QLatin1String( "cellphone" ),
            addr.phoneNumber( PhoneNumber::Cell ).number() ); // Netscape 4.x
  ldif_out( t, QLatin1String( "pager" ),
            addr.phoneNumber( PhoneNumber::Pager ).number() );
  ldif_out( t, QLatin1String( "pagerphone" ),
            addr.phoneNumber( PhoneNumber::Pager ).number() );

  ldif_out( t, QLatin1String( "streethomeaddress" ), homeAddr.street() );
  ldif_out( t, QLatin1String( "postalcode" ), workAddr.postalCode() );
  ldif_out( t, QLatin1String( "postofficebox" ), workAddr.postOfficeBox() );

  QStringList streets = homeAddr.street().split( QLatin1Char( '\n' ) );
  const int numberOfStreets( streets.count() );
  if ( numberOfStreets > 0 ) {
    ldif_out( t, QLatin1String( "homepostaladdress" ), streets[ 0 ] ); // Netscape 7
  }
  if ( numberOfStreets > 1 ) {
    ldif_out( t, QLatin1String( "mozillahomepostaladdress2" ), streets[ 1 ] ); // Netscape 7
  }
  ldif_out( t, QLatin1String( "mozillahomelocalityname" ), homeAddr.locality() ); // Netscape 7
  ldif_out( t, QLatin1String( "mozillahomestate" ), homeAddr.region() );
  ldif_out( t, QLatin1String( "mozillahomepostalcode" ), homeAddr.postalCode() );
  ldif_out( t, QLatin1String( "mozillahomecountryname" ),
            Address::ISOtoCountry( homeAddr.country() ) );
  ldif_out( t, QLatin1String( "locality" ), workAddr.locality() );
  ldif_out( t, QLatin1String( "streetaddress" ), workAddr.street() ); // Netscape 4.x

  streets = workAddr.street().split( QLatin1Char( '\n' ) );
  if ( streets.count() > 0 ) {
    ldif_out( t, QLatin1String( "postaladdress" ), streets[ 0 ] );
  }
  if ( streets.count() > 1 ) {
    ldif_out( t, QLatin1String( "mozillapostaladdress2" ), streets[ 1 ] );
  }
  ldif_out( t, QLatin1String( "countryname" ), Address::ISOtoCountry( workAddr.country() ) );
  ldif_out( t, QLatin1String( "l" ), workAddr.locality() );
  ldif_out( t, QLatin1String( "c" ), Address::ISOtoCountry( workAddr.country() ) );
  ldif_out( t, QLatin1String( "st" ), workAddr.region() );

  ldif_out( t, QLatin1String( "title" ), addr.title() );
  ldif_out( t, QLatin1String( "vocation" ), addr.prefix() );
  ldif_out( t, QLatin1String( "ou" ), addr.role() );
  ldif_out( t, QLatin1String( "o" ), addr.organization() );
  ldif_out( t, QLatin1String( "organization" ), addr.organization() );
  ldif_out( t, QLatin1String( "organizationname" ), addr.organization() );

  // Compatibility with older kabc versions.
  if ( !addr.department().isEmpty() ) {
    ldif_out( t, QLatin1String( "department" ), addr.department() );
  } else {
    ldif_out( t, QLatin1String( "department" ), addr.custom( QLatin1String( "KADDRESSBOOK" ),
                                                             QLatin1String( "X-Department" ) ) );
  }

  ldif_out( t, QLatin1String( "workurl" ), addr.url().toDisplayString() );
  ldif_out( t, QLatin1String( "homeurl" ), addr.url().toDisplayString() );
  ldif_out( t, QLatin1String( "mozillahomeurl" ), addr.url().toDisplayString() );

  ldif_out( t, QLatin1String( "description" ), addr.note() );
  if ( addr.revision().isValid() ) {
    ldif_out( t, QLatin1String( "modifytimestamp" ), dateToVCardString( addr.revision() ) );
  }

  const QDateTime birthday = addr.birthday();
  if ( birthday.isValid() ) {
    const QDate date = birthday.date();
    ldif_out( t, QLatin1String( "birthyear" ), QString::number( date.year() ) );
    ldif_out( t, QLatin1String( "birthmonth" ), QString::number( date.month() ) );
    ldif_out( t, QLatin1String( "birthday" ), QString::number( date.day() ) );
  }

  t << "objectclass: top\n";
  t << "objectclass: person\n";
  t << "objectclass: organizationalPerson\n";

  t << "\n";

  return true;
}

/* convert from LDIF stream */

bool LDIFConverter::LDIFToAddressee( const QString &str, AddresseeList &addrList,
                                     const QDateTime &dt )
{
  if ( str.isEmpty() ) {
    return true;
  }

  bool endldif = false, end = false;
  Ldif ldif;
  Ldif::ParseValue ret;
  Addressee a;
  Address homeAddr, workAddr;
  int birthday = -1;
  int birthmonth = -1;
  int birthyear = -1;

  ldif.setLdif( str.toLatin1() );
  QDateTime qdt = dt;
  if ( !qdt.isValid() ) {
    qdt = QDateTime::currentDateTime();
  }
  a.setRevision( qdt );
  homeAddr = Address( Address::Home );
  workAddr = Address( Address::Work );

  do {
    ret = ldif.nextItem();
    switch ( ret ) {
      case Ldif::Item:
      {
        QString fieldname = ldif.attr().toLower();
        QString value = QString::fromUtf8( ldif.value() );
        evaluatePair( a, homeAddr, workAddr, fieldname, value, birthday, birthmonth, birthyear );
        break;
      }
      case Ldif::EndEntry:
      {
        // if the new address is not empty, append it
        QDateTime birthDate( QDate( birthyear, birthmonth, birthday ) );
        if ( birthDate.isValid() ) {
          a.setBirthday( birthDate );
        }

        if ( !a.formattedName().isEmpty() || !a.name().isEmpty() ||
             !a.familyName().isEmpty() ) {
          if ( !homeAddr.isEmpty() ) {
            a.insertAddress( homeAddr );
          }
          if ( !workAddr.isEmpty() ) {
            a.insertAddress( workAddr );
          }
          addrList.append( a );
        }
        a = Addressee();
        a.setRevision( qdt );
        homeAddr = Address( Address::Home );
        workAddr = Address( Address::Work );
      }
      break;
      case Ldif::MoreData:
      {
        if ( endldif ) {
          end = true;
        } else {
          ldif.endLdif();
          endldif = true;
          break;
        }
      }
      default:
        break;
    }
  } while ( !end );

  return true;
}

bool LDIFConverter::evaluatePair( Addressee &a, Address &homeAddr,
                                  Address &workAddr,
                                  QString &fieldname, QString &value,
                                  int &birthday, int &birthmonth, int &birthyear )
{
  if ( fieldname == QLatin1String( "dn" ) ) { // ignore & return false!
    return false;
  }

  if ( fieldname.startsWith( QLatin1Char( '#' ) ) ) {
    return true;
  }

  if ( fieldname.isEmpty() && !a.note().isEmpty() ) {
    // some LDIF export filters are borken and add additional
    // comments on stand-alone lines. Just add them to the notes for now.
    a.setNote( a.note() + QLatin1Char( '\n' ) + value );
    return true;
  }

  if ( fieldname == QLatin1String( "givenname" ) ) {
    a.setGivenName( value );
    return true;
  }

  if ( fieldname == QLatin1String( "xmozillanickname" ) ||
       fieldname == QLatin1String( "nickname" ) ||
       fieldname == QLatin1String( "mozillanickname" ) ) {
    a.setNickName( value );
    return true;
  }

  if ( fieldname == QLatin1String( "sn" ) ) {
    a.setFamilyName( value );
    return true;
  }

  if ( fieldname == QLatin1String( "uid" ) ) {
    a.setUid( value );
    return true;
  }
  if ( fieldname == QLatin1String( "mail" ) ||
       fieldname == QLatin1String( "mozillasecondemail" ) ) { // mozilla
    if ( a.emails().indexOf( value ) == -1 ) {
      a.insertEmail( value );
    }
    return true;
  }

  if ( fieldname == QLatin1String( "title" ) ) {
    a.setTitle( value );
    return true;
  }

  if ( fieldname == QLatin1String( "vocation" ) ) {
    a.setPrefix( value );
    return true;
  }

  if ( fieldname == QLatin1String( "cn" ) ) {
    a.setFormattedName( value );
    return true;
  }

  if ( fieldname == QLatin1String( "o" ) ||
       fieldname == QLatin1String( "organization" ) ||      // Exchange
       fieldname == QLatin1String( "organizationname" ) ) { // Exchange
    a.setOrganization( value );
    return true;
  }

  if ( fieldname == QLatin1String( "description" ) ) {
addComment:
    if ( !a.note().isEmpty() ) {
      a.setNote( a.note() + QLatin1Char( '\n' ) );
    }
    a.setNote( a.note() + value );
    return true;
  }

  if ( fieldname == QLatin1String( "custom1" ) ||
       fieldname == QLatin1String( "custom2" ) ||
       fieldname == QLatin1String( "custom3" ) ||
       fieldname == QLatin1String( "custom4" ) ) {
    goto addComment;
  }

  if ( fieldname == QLatin1String( "homeurl" ) ||
       fieldname == QLatin1String( "workurl" ) ||
       fieldname == QLatin1String( "mozillahomeurl" ) ) {
    if ( a.url().isEmpty() ) {
      a.setUrl( QUrl( value ) );
      return true;
    }
    if ( a.url().toDisplayString() == QUrl( value ).toDisplayString() ) {
      return true;
    }
    // TODO: current version of kabc only supports one URL.
    // TODO: change this with KDE 4
  }

  if ( fieldname == QLatin1String( "homephone" ) ) {
    a.insertPhoneNumber( PhoneNumber( value, PhoneNumber::Home ) );
    return true;
  }

  if ( fieldname == QLatin1String( "telephonenumber" ) ) {
    a.insertPhoneNumber( PhoneNumber( value, PhoneNumber::Work ) );
    return true;
  }

  if ( fieldname == QLatin1String( "mobile" ) ) {         // mozilla/Netscape 7
    a.insertPhoneNumber( PhoneNumber( value, PhoneNumber::Cell ) );
    return true;
  }

  if ( fieldname == QLatin1String( "cellphone" ) ) {
    a.insertPhoneNumber( PhoneNumber( value, PhoneNumber::Cell ) );
    return true;
  }

  if ( fieldname == QLatin1String( "pager" )  ||       // mozilla
       fieldname == QLatin1String( "pagerphone" ) ) {  // mozilla
    a.insertPhoneNumber( PhoneNumber( value, PhoneNumber::Pager ) );
    return true;
  }

  if ( fieldname == QLatin1String( "facsimiletelephonenumber" ) ) {
    a.insertPhoneNumber( PhoneNumber( value, PhoneNumber::Fax ) );
    return true;
  }

  if ( fieldname == QLatin1String( "xmozillaanyphone" ) ) {  // mozilla
    a.insertPhoneNumber( PhoneNumber( value, PhoneNumber::Work ) );
    return true;
  }

  if ( fieldname == QLatin1String( "streethomeaddress" ) ||
       fieldname == QLatin1String( "mozillahomestreet" ) ) {  // thunderbird
    homeAddr.setStreet( value );
    return true;
  }

  if ( fieldname == QLatin1String( "street" ) ||
       fieldname == QLatin1String( "postaladdress" ) ) {  // mozilla
    workAddr.setStreet( value );
    return true;
  }

  if ( fieldname == QLatin1String( "mozillapostaladdress2" ) ) {  // mozilla
    workAddr.setStreet( workAddr.street() + QLatin1String( "\n" ) + value );
    return true;
  }

  if ( fieldname == QLatin1String( "postalcode" ) ) {
    workAddr.setPostalCode( value );
    return true;
  }

  if ( fieldname == QLatin1String( "postofficebox" ) ) {
    workAddr.setPostOfficeBox( value );
    return true;
  }

  if ( fieldname == QLatin1String( "homepostaladdress" ) ) {  // Netscape 7
    homeAddr.setStreet( value );
    return true;
  }

  if ( fieldname == QLatin1String( "mozillahomepostaladdress2" ) ) {  // mozilla
    homeAddr.setStreet( homeAddr.street() + QLatin1String( "\n" ) + value );
    return true;
  }

  if ( fieldname == QLatin1String( "mozillahomelocalityname" ) ) {  // mozilla
    homeAddr.setLocality( value );
    return true;
  }

  if ( fieldname == QLatin1String( "mozillahomestate" ) ) { // mozilla
    homeAddr.setRegion( value );
    return true;
  }

  if ( fieldname == QLatin1String( "mozillahomepostalcode" ) ) {  // mozilla
    homeAddr.setPostalCode( value );
    return true;
  }

  if ( fieldname == QLatin1String( "mozillahomecountryname" ) ) { // mozilla
    if ( value.length() <= 2 ) {
      value = Address::ISOtoCountry( value );
    }
    homeAddr.setCountry( value );
    return true;
  }

  if ( fieldname == QLatin1String( "locality" ) ) {
    workAddr.setLocality( value );
    return true;
  }

  if ( fieldname == QLatin1String( "streetaddress" ) ) { // Netscape 4.x
    workAddr.setStreet( value );
    return true;
  }

  if ( fieldname == QLatin1String( "countryname" ) ||
       fieldname == QLatin1String( "c" ) ) {  // mozilla
    if ( value.length() <= 2 ) {
      value = Address::ISOtoCountry( value );
    }
    workAddr.setCountry( value );
    return true;
  }

  if ( fieldname == QLatin1String( "l" ) ) {  // mozilla
    workAddr.setLocality( value );
    return true;
  }

  if ( fieldname == QLatin1String( "st" ) ) {
    workAddr.setRegion( value );
    return true;
  }

  if ( fieldname == QLatin1String( "ou" ) ) {
    a.setRole( value );
    return true;
  }

  if ( fieldname == QLatin1String( "department" ) ) {
    a.setDepartment( value );
    return true;
  }

  if ( fieldname == QLatin1String( "member" ) ) {
    // this is a mozilla list member (cn=xxx, mail=yyy)
    QStringList list = value.split( QLatin1Char( ',' ) );
    QString name, email;

    QStringList::Iterator it;
    for ( it = list.begin(); it != list.end(); ++it ) {
      if ( ( *it ).startsWith( QLatin1String( "cn=" ) ) ) {
        name = ( *it ).mid( 3 ).trimmed();
      }
      if ( ( *it ).startsWith( QLatin1String( "mail=" ) ) ) {
        email = ( *it ).mid( 5 ).trimmed();
      }
    }
    if ( !name.isEmpty() && !email.isEmpty() ) {
      email = QLatin1String( " <" ) + email + QLatin1Char( '>' );
    }
    a.insertEmail( name + email );
    a.insertCategory( i18n( "List of Emails" ) );
    return true;
  }

  if ( fieldname == QLatin1String( "modifytimestamp" ) ) {
    if ( value == QLatin1String( "0Z" ) ) { // ignore
      return true;
    }
    QDateTime dt = VCardStringToDate( value );
    if ( dt.isValid() ) {
      a.setRevision( dt );
      return true;
    }
  }

  if ( fieldname == QLatin1String( "objectclass" ) ) { // ignore
    return true;
  }

  if ( fieldname == QLatin1String( "birthyear" ) ) {
    birthyear = value.toInt();
    return true;
  }
  if ( fieldname == QLatin1String( "birthmonth" ) ) {
    birthmonth = value.toInt();
    return true;
  }
  if ( fieldname == QLatin1String( "birthday" ) ) {
    birthday = value.toInt();
    return true;
  }

  qCWarning( KABC_LOG ) << QString::fromLatin1( "LDIFConverter: Unknown field for '%1': '%2=%3'\n" ).
    arg( a.formattedName() ).arg( fieldname ).arg( value );

  return true;
}
