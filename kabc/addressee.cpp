/*** Warning! This file has been generated by the script makeaddressee ***/
/*
    This file is part of libkabc.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#include <ksharedptr.h>
#include <kdebug.h>
#include <kapplication.h>
#include <klocale.h>

#include "resource.h"
#include "addressee.h"

using namespace KABC;

struct Addressee::AddresseeData : public KShared
{
  QString uid;
  QString name;
  QString formattedName;
  QString familyName;
  QString givenName;
  QString additionalName;
  QString prefix;
  QString suffix;
  QString nickName;
  QDateTime birthday;
  QString mailer;
  TimeZone timeZone;
  Geo geo;
  QString title;
  QString role;
  QString organization;
  QString note;
  QString productId;
  QDateTime revision;
  QString sortString;
  KURL url;

  PhoneNumber::List phoneNumbers;
  Address::List addresses;
  QStringList emails;
  QStringList categories;
  QStringList custom;

  Resource *resource;

  bool empty;
  bool changed;
};

Addressee::Addressee()
{
  mData = new AddresseeData;
  mData->uid = KApplication::randomString( 10 );
  mData->empty = true;
  mData->changed = false;
  mData->resource = 0;
}

Addressee::~Addressee()
{
}

Addressee::Addressee( const Addressee &a )
{
  mData = a.mData;
}

Addressee &Addressee::operator=( const Addressee &a )
{
  mData = a.mData;
  return (*this);
}

Addressee Addressee::copy()
{
  Addressee a;
  *(a.mData) = *mData;
  return a;
}

void Addressee::detach()
{
  if ( mData.count() == 1 ) return;
  *this = copy();
}

bool Addressee::operator==( const Addressee &a ) const
{
  if ( mData->uid != a.mData->uid ) return false;
  if ( mData->name != a.mData->name ) return false;
  if ( mData->formattedName != a.mData->formattedName ) return false;
  if ( mData->familyName != a.mData->familyName ) return false;
  if ( mData->givenName != a.mData->givenName ) return false;
  if ( mData->additionalName != a.mData->additionalName ) return false;
  if ( mData->prefix != a.mData->prefix ) return false;
  if ( mData->suffix != a.mData->suffix ) return false;
  if ( mData->nickName != a.mData->nickName ) return false;
  if ( mData->birthday != a.mData->birthday ) return false;
  if ( mData->mailer != a.mData->mailer ) return false;
  if ( mData->timeZone != a.mData->timeZone ) return false;
  if ( mData->geo != a.mData->geo ) return false;
  if ( mData->title != a.mData->title ) return false;
  if ( mData->role != a.mData->role ) return false;
  if ( mData->organization != a.mData->organization ) return false;
  if ( mData->note != a.mData->note ) return false;
  if ( mData->productId != a.mData->productId ) return false;
  if ( mData->revision != a.mData->revision ) return false;
  if ( mData->sortString != a.mData->sortString ) return false;
  if ( ( mData->url.isValid() || a.mData->url.isValid() ) &&
       ( mData->url != a.mData->url ) ) return false;
  if ( mData->phoneNumbers != a.mData->phoneNumbers ) return false;
  if ( mData->addresses != a.mData->addresses ) return false;
  if ( mData->emails != a.mData->emails ) return false;
  if ( mData->categories != a.mData->categories ) return false;
  if ( mData->custom != a.mData->custom ) return false;

  return true;
}

bool Addressee::operator!=( const Addressee &a ) const
{
  return !( a == *this );
}

bool Addressee::isEmpty() const
{
  return mData->empty;
}

void Addressee::setUid( const QString &uid )
{
  if ( uid == mData->uid ) return;
  detach();
  mData->empty = false;
  mData->uid = uid;
}

QString Addressee::uid() const
{
  return mData->uid;
}

QString Addressee::uidLabel()
{
  return i18n("Unique Identifier");
}


void Addressee::setName( const QString &name )
{
  if ( name == mData->name ) return;
  detach();
  mData->empty = false;
  mData->name = name;
}

QString Addressee::name() const
{
  return mData->name;
}

QString Addressee::nameLabel()
{
  return i18n("Name");
}


void Addressee::setFormattedName( const QString &formattedName )
{
  if ( formattedName == mData->formattedName ) return;
  detach();
  mData->empty = false;
  mData->formattedName = formattedName;
}

QString Addressee::formattedName() const
{
  return mData->formattedName;
}

QString Addressee::formattedNameLabel()
{
  return i18n("Formatted Name");
}


void Addressee::setFamilyName( const QString &familyName )
{
  if ( familyName == mData->familyName ) return;
  detach();
  mData->empty = false;
  mData->familyName = familyName;
}

QString Addressee::familyName() const
{
  return mData->familyName;
}

QString Addressee::familyNameLabel()
{
  return i18n("Family Name");
}


void Addressee::setGivenName( const QString &givenName )
{
  if ( givenName == mData->givenName ) return;
  detach();
  mData->empty = false;
  mData->givenName = givenName;
}

QString Addressee::givenName() const
{
  return mData->givenName;
}

QString Addressee::givenNameLabel()
{
  return i18n("Given Name");
}


void Addressee::setAdditionalName( const QString &additionalName )
{
  if ( additionalName == mData->additionalName ) return;
  detach();
  mData->empty = false;
  mData->additionalName = additionalName;
}

QString Addressee::additionalName() const
{
  return mData->additionalName;
}

QString Addressee::additionalNameLabel()
{
  return i18n("Additional Names");
}


void Addressee::setPrefix( const QString &prefix )
{
  if ( prefix == mData->prefix ) return;
  detach();
  mData->empty = false;
  mData->prefix = prefix;
}

QString Addressee::prefix() const
{
  return mData->prefix;
}

QString Addressee::prefixLabel()
{
  return i18n("Honorific Prefixes");
}


void Addressee::setSuffix( const QString &suffix )
{
  if ( suffix == mData->suffix ) return;
  detach();
  mData->empty = false;
  mData->suffix = suffix;
}

QString Addressee::suffix() const
{
  return mData->suffix;
}

QString Addressee::suffixLabel()
{
  return i18n("Honorific Suffixes");
}


void Addressee::setNickName( const QString &nickName )
{
  if ( nickName == mData->nickName ) return;
  detach();
  mData->empty = false;
  mData->nickName = nickName;
}

QString Addressee::nickName() const
{
  return mData->nickName;
}

QString Addressee::nickNameLabel()
{
  return i18n("Nick Name");
}


void Addressee::setBirthday( const QDateTime &birthday )
{
  if ( birthday == mData->birthday ) return;
  detach();
  mData->empty = false;
  mData->birthday = birthday;
}

QDateTime Addressee::birthday() const
{
  return mData->birthday;
}

QString Addressee::birthdayLabel()
{
  return i18n("Birthday");
}


QString Addressee::homeAddressStreetLabel()
{
  return i18n("Home Address Street");
}


QString Addressee::homeAddressLocalityLabel()
{
  return i18n("Home Address Locality");
}


QString Addressee::homeAddressRegionLabel()
{
  return i18n("Home Address Region");
}


QString Addressee::homeAddressPostalCodeLabel()
{
  return i18n("Home Address Postal Code");
}


QString Addressee::homeAddressCountryLabel()
{
  return i18n("Home Address Country");
}


QString Addressee::businessAddressStreetLabel()
{
  return i18n("Business Address Street");
}


QString Addressee::businessAddressLocalityLabel()
{
  return i18n("Business Address Locality");
}


QString Addressee::businessAddressRegionLabel()
{
  return i18n("Business Address Region");
}


QString Addressee::businessAddressPostalCodeLabel()
{
  return i18n("Business Address Postal Code");
}


QString Addressee::businessAddressCountryLabel()
{
  return i18n("Business Address Country");
}


QString Addressee::homePhoneLabel()
{
  return i18n("Home Phone");
}


QString Addressee::businessPhoneLabel()
{
  return i18n("Business Phone");
}


QString Addressee::mobilePhoneLabel()
{
  return i18n("Mobile Phone");
}


QString Addressee::homeFaxLabel()
{
  return i18n("Home Fax");
}


QString Addressee::businessFaxLabel()
{
  return i18n("Business Fax");
}


QString Addressee::emailLabel()
{
  return i18n("Email Address");
}


void Addressee::setMailer( const QString &mailer )
{
  if ( mailer == mData->mailer ) return;
  detach();
  mData->empty = false;
  mData->mailer = mailer;
}

QString Addressee::mailer() const
{
  return mData->mailer;
}

QString Addressee::mailerLabel()
{
  return i18n("Mail Client");
}


void Addressee::setTimeZone( const TimeZone &timeZone )
{
  if ( timeZone == mData->timeZone ) return;
  detach();
  mData->empty = false;
  mData->timeZone = timeZone;
}

TimeZone Addressee::timeZone() const
{
  return mData->timeZone;
}

QString Addressee::timeZoneLabel()
{
  return i18n("Time Zone");
}


void Addressee::setGeo( const Geo &geo )
{
  if ( geo == mData->geo ) return;
  detach();
  mData->empty = false;
  mData->geo = geo;
}

Geo Addressee::geo() const
{
  return mData->geo;
}

QString Addressee::geoLabel()
{
  return i18n("Geographic Position");
}


void Addressee::setTitle( const QString &title )
{
  if ( title == mData->title ) return;
  detach();
  mData->empty = false;
  mData->title = title;
}

QString Addressee::title() const
{
  return mData->title;
}

QString Addressee::titleLabel()
{
  return i18n("Title");
}


void Addressee::setRole( const QString &role )
{
  if ( role == mData->role ) return;
  detach();
  mData->empty = false;
  mData->role = role;
}

QString Addressee::role() const
{
  return mData->role;
}

QString Addressee::roleLabel()
{
  return i18n("Role");
}


void Addressee::setOrganization( const QString &organization )
{
  if ( organization == mData->organization ) return;
  detach();
  mData->empty = false;
  mData->organization = organization;
}

QString Addressee::organization() const
{
  return mData->organization;
}

QString Addressee::organizationLabel()
{
  return i18n("Organization");
}


void Addressee::setNote( const QString &note )
{
  if ( note == mData->note ) return;
  detach();
  mData->empty = false;
  mData->note = note;
}

QString Addressee::note() const
{
  return mData->note;
}

QString Addressee::noteLabel()
{
  return i18n("Note");
}


void Addressee::setProductId( const QString &productId )
{
  if ( productId == mData->productId ) return;
  detach();
  mData->empty = false;
  mData->productId = productId;
}

QString Addressee::productId() const
{
  return mData->productId;
}

QString Addressee::productIdLabel()
{
  return i18n("Product Identifier");
}


void Addressee::setRevision( const QDateTime &revision )
{
  if ( revision == mData->revision ) return;
  detach();
  mData->empty = false;
  mData->revision = revision;
}

QDateTime Addressee::revision() const
{
  return mData->revision;
}

QString Addressee::revisionLabel()
{
  return i18n("Revision Date");
}


void Addressee::setSortString( const QString &sortString )
{
  if ( sortString == mData->sortString ) return;
  detach();
  mData->empty = false;
  mData->sortString = sortString;
}

QString Addressee::sortString() const
{
  return mData->sortString;
}

QString Addressee::sortStringLabel()
{
  return i18n("Sort String");
}


void Addressee::setUrl( const KURL &url )
{
  if ( url == mData->url ) return;
  detach();
  mData->empty = false;
  mData->url = url;
}

KURL Addressee::url() const
{
  return mData->url;
}

QString Addressee::urlLabel()
{
  return i18n("URL");
}



void Addressee::setNameFromString( const QString &str )
{
  setFormattedName( str );
  setName( str );

  int i = str.find(',');
  if( i < 0 ) {
    // "Given [Additional] Family"
    QStringList nameParts = QStringList::split( " ", str );
    switch( nameParts.count() ) {
      case 1:
       setFamilyName( nameParts[ 0 ] );
       break;
      case 2:
       setGivenName( nameParts[ 0 ] );
       setFamilyName( nameParts[ 1 ] );
       break;
      case 3:
       setGivenName( nameParts[ 0 ] );
       setAdditionalName( nameParts[ 1 ] );
       setFamilyName( nameParts[ 2 ] );
       break;
      default:
       setFamilyName( str );
       break;
    }
  } else {
    // "Family, Given [Additional]"
    setFamilyName( str.left( i ) );
    QString part2 = str.mid( i + 1 );
    part2.stripWhiteSpace();

    QStringList nameParts = QStringList::split( " ", part2 );
    switch( nameParts.count() ) {
      case 1:
       setGivenName( nameParts[ 0 ] );
       break;
      case 2:
       setGivenName( nameParts[ 0 ] );
       setAdditionalName( nameParts[ 1 ] );
       break;
      default:
       setGivenName( part2 );
       break;
    }
  }
}

QString Addressee::realName() const
{
  if ( !formattedName().isEmpty() ) return formattedName();

  QString n = prefix() + " " + givenName() + " " + additionalName() + " " +
              familyName() + " " + suffix();
  n = n.simplifyWhiteSpace();             

  if ( n.isEmpty() ) n = name();
  
  return n;
}

QString Addressee::fullEmail( const QString &email ) const
{
  QString e;
  if ( email.isNull() ) {
    e = preferredEmail();
  } else {
    e = email;
  }
  if ( e.isEmpty() ) return QString::null;
  
  QString text;
  if ( !realName().isEmpty() ) text = realName() + " ";
  text.append( "<" + e + ">" );

  return text;
}

void Addressee::insertEmail( const QString &email, bool preferred )
{
  detach();

  QStringList::Iterator it = mData->emails.find( email );

  if ( it != mData->emails.end() ) {
    if ( !preferred || it == mData->emails.begin() ) return;
    mData->emails.remove( it );
    mData->emails.prepend( email );
  } else {
    if ( preferred ) {
      mData->emails.prepend( email );
    } else {
      mData->emails.append( email );
    }
  }
}

void Addressee::removeEmail( const QString &email )
{
  detach();

  QStringList::Iterator it = mData->emails.find( email );
  if ( it == mData->emails.end() ) return;

  mData->emails.remove( it );
}

QString Addressee::preferredEmail() const
{
  if ( mData->emails.count() == 0 ) return QString::null;
  else return mData->emails.first();
}

QStringList Addressee::emails() const
{
  return mData->emails;
}

void Addressee::insertPhoneNumber( const PhoneNumber &phoneNumber )
{
  detach();
  mData->empty = false;

  PhoneNumber::List::Iterator it;
  for( it = mData->phoneNumbers.begin(); it != mData->phoneNumbers.end(); ++it ) {
    if ( (*it).id() == phoneNumber.id() ) {
      *it = phoneNumber;
      return;
    }
  }
  mData->phoneNumbers.append( phoneNumber );
}

void Addressee::removePhoneNumber( const PhoneNumber &phoneNumber )
{
  detach();

  PhoneNumber::List::Iterator it;
  for( it = mData->phoneNumbers.begin(); it != mData->phoneNumbers.end(); ++it ) {
    if ( (*it).id() == phoneNumber.id() ) {
      mData->phoneNumbers.remove( it );
      return;
    }
  }
}

PhoneNumber Addressee::phoneNumber( int type ) const
{
  PhoneNumber::List::ConstIterator it;
  for( it = mData->phoneNumbers.begin(); it != mData->phoneNumbers.end(); ++it ) {
    if ( (*it).type() == type ) {
      return *it;
    }
  }
  return PhoneNumber( QString(), type );
}

PhoneNumber::List Addressee::phoneNumbers() const
{
  return mData->phoneNumbers;
}

PhoneNumber::List Addressee::phoneNumbers( int type ) const
{
  PhoneNumber::List list;

  PhoneNumber::List::ConstIterator it;
  for( it = mData->phoneNumbers.begin(); it != mData->phoneNumbers.end(); ++it ) {
    if ( (*it).type() == type ) {
      list.append( *it );
    }
  }
  return list;
}

PhoneNumber Addressee::findPhoneNumber( const QString &id ) const
{
  PhoneNumber::List::ConstIterator it;
  for( it = mData->phoneNumbers.begin(); it != mData->phoneNumbers.end(); ++it ) {
    if ( (*it).id() == id ) {
      return *it;
    }
  }
  return PhoneNumber();
}


void Addressee::dump() const
{
  kdDebug(5700) << "Addressee {" << endl;

  kdDebug(5700) << "  Uid: '" << uid() << "'" << endl;
  kdDebug(5700) << "  Name: '" << name() << "'" << endl;
  kdDebug(5700) << "  FormattedName: '" << formattedName() << "'" << endl;
  kdDebug(5700) << "  FamilyName: '" << familyName() << "'" << endl;
  kdDebug(5700) << "  GivenName: '" << givenName() << "'" << endl;
  kdDebug(5700) << "  AdditionalName: '" << additionalName() << "'" << endl;
  kdDebug(5700) << "  Prefix: '" << prefix() << "'" << endl;
  kdDebug(5700) << "  Suffix: '" << suffix() << "'" << endl;
  kdDebug(5700) << "  NickName: '" << nickName() << "'" << endl;
  kdDebug(5700) << "  Birthday: '" << birthday().toString() << "'" << endl;
  kdDebug(5700) << "  Mailer: '" << mailer() << "'" << endl;
  kdDebug(5700) << "  TimeZone: '" << timeZone().asString() << "'" << endl;
  kdDebug(5700) << "  Geo: '" << geo().asString() << "'" << endl;
  kdDebug(5700) << "  Title: '" << title() << "'" << endl;
  kdDebug(5700) << "  Role: '" << role() << "'" << endl;
  kdDebug(5700) << "  Organization: '" << organization() << "'" << endl;
  kdDebug(5700) << "  Note: '" << note() << "'" << endl;
  kdDebug(5700) << "  ProductId: '" << productId() << "'" << endl;
  kdDebug(5700) << "  Revision: '" << revision().toString() << "'" << endl;
  kdDebug(5700) << "  SortString: '" << sortString() << "'" << endl;
  kdDebug(5700) << "  Url: '" << url().url() << "'" << endl;
  
  kdDebug(5700) << "  Emails {" << endl;
  QStringList e = emails();
  QStringList::ConstIterator it;
  for( it = e.begin(); it != e.end(); ++it ) {
    kdDebug(5700) << "    " << (*it) << endl;
  }
  kdDebug(5700) << "  }" << endl;

  kdDebug(5700) << "  PhoneNumbers {" << endl;
  PhoneNumber::List p = phoneNumbers();
  PhoneNumber::List::ConstIterator it2;
  for( it2 = p.begin(); it2 != p.end(); ++it2 ) {
    kdDebug(5700) << "    Type: " << int((*it2).type()) << " Number: " << (*it2).number() << endl;
  }
  kdDebug(5700) << "  }" << endl;

  Address::List a = addresses();
  Address::List::ConstIterator it3;
  for( it3 = a.begin(); it3 != a.end(); ++it3 ) {
    (*it3).dump();
  }

  kdDebug(5700) << "}" << endl;
}


void Addressee::insertAddress( const Address &address )
{
  detach();
  mData->empty = false;

  Address::List::Iterator it;
  for( it = mData->addresses.begin(); it != mData->addresses.end(); ++it ) {
    if ( (*it).id() == address.id() ) {
      *it = address;
      return;
    }
  }
  mData->addresses.append( address );
}

void Addressee::removeAddress( const Address &address )
{
  detach();

  Address::List::Iterator it;
  for( it = mData->addresses.begin(); it != mData->addresses.end(); ++it ) {
    if ( (*it).id() == address.id() ) {
      mData->addresses.remove( it );
      return;
    }
  }
}

Address Addressee::address( int type ) const
{
  Address::List::ConstIterator it;
  for( it = mData->addresses.begin(); it != mData->addresses.end(); ++it ) {
    if ( (*it).type() == type ) {
      return *it;
    }
  }
  return Address( type );
}

Address::List Addressee::addresses() const
{
  return mData->addresses;
}

Address::List Addressee::addresses( int type ) const
{
  Address::List list;

  Address::List::ConstIterator it;
  for( it = mData->addresses.begin(); it != mData->addresses.end(); ++it ) {
    if ( (*it).type() == type ) {
      list.append( *it );
    }
  }

  return list;
}

Address Addressee::findAddress( const QString &id ) const
{
  Address::List::ConstIterator it;
  for( it = mData->addresses.begin(); it != mData->addresses.end(); ++it ) {
    if ( (*it).id() == id ) {
      return *it;
    }
  }
  return Address();
}

void Addressee::insertCategory( const QString &c )
{
  detach();
  mData->empty = false;

  if ( mData->categories.contains( c ) ) return;

  mData->categories.append( c );
}

void Addressee::removeCategory( const QString &c )
{
  detach();

  QStringList::Iterator it = mData->categories.find( c );
  if ( it == mData->categories.end() ) return;

  mData->categories.remove( it );
}

bool Addressee::hasCategory( const QString &c ) const
{
  return ( mData->categories.find( c ) != mData->categories.end() );
}

void Addressee::setCategories( const QStringList &c )
{
  detach();
  mData->empty = false;

  mData->categories = c;
}

QStringList Addressee::categories() const
{
  return mData->categories;
}

void Addressee::insertCustom( const QString &app, const QString &name,
                              const QString &value )
{
  if ( value.isNull() || name.isEmpty() || app.isEmpty() ) return;

  detach();
  mData->empty = false;
  
  QString qualifiedName = app + "-" + name + ":";
  
  QStringList::Iterator it;
  for( it = mData->custom.begin(); it != mData->custom.end(); ++it ) {
    if ( (*it).startsWith( qualifiedName ) ) {
      (*it) = qualifiedName + value;
      return;
    }
  }
  
  mData->custom.append( qualifiedName + value );
}

void Addressee::removeCustom( const QString &app, const QString &name)
{
  detach();
  
  QString qualifiedName = app + "-" + name + ":";
  
  QStringList::Iterator it;
  for( it = mData->custom.begin(); it != mData->custom.end(); ++it ) {
    if ( (*it).startsWith( qualifiedName ) ) {
      mData->custom.remove( it );
      return;
    }
  }
}

QString Addressee::custom( const QString &app, const QString &name ) const
{
  QString qualifiedName = app + "-" + name + ":";
  QString value;
  
  QStringList::ConstIterator it;
  for( it = mData->custom.begin(); it != mData->custom.end(); ++it ) {
    if ( (*it).startsWith( qualifiedName ) ) {
      value = (*it).mid( (*it).find( ":" ) + 1 );
      break;
    }
  }
  
  return value;
}

void Addressee::setCustoms( const QStringList &l )
{
  detach();
  mData->empty = false;
  
  mData->custom = l;
}

QStringList Addressee::customs() const
{
  return mData->custom;
}

void Addressee::parseEmailAddress( const QString &rawEmail, QString &fullName, 
                                   QString &email)
{
  int startPos, endPos, len;
  QString partA, partB, result;
  char endCh = '>';
  
  startPos = rawEmail.find('<');
  if (startPos < 0)
  {
    startPos = rawEmail.find('(');
    endCh = ')';
  }
  if (startPos < 0)
  {
    // We couldn't find any separators, so we assume the whole string
    // is the email address
    email = rawEmail;
    fullName = "";
  }
  else 
  {
    // We have a start position, try to find an end
    endPos = rawEmail.find(endCh, startPos+1);
    
    if (endPos < 0) 
    {
      // We couldn't find the end of the email address. We can only
      // assume the entire string is the email address.
      email = rawEmail;
      fullName = "";
    }
    else
    {
      // We have a start and end to the email address
      
      // Grab the name part
      fullName = rawEmail.left(startPos).stripWhiteSpace();
      
      // grab the email part
      email = rawEmail.mid(startPos+1, endPos-startPos-1).stripWhiteSpace();

      // Check that we do not have any extra characters on the end of the
      // strings
      len = fullName.length();
      if (fullName[0]=='"' && fullName[len-1]=='"')
        fullName = fullName.mid(1, len-2);
      else if (fullName[0]=='<' && fullName[len-1]=='>')
        fullName = fullName.mid(1, len-2);
      else if (fullName[0]=='(' && fullName[len-1]==')')
        fullName = fullName.mid(1, len-2);
    }
  }
}

void Addressee::setResource( Resource *resource )
{
    detach();
    mData->resource = resource;
} 

Resource *Addressee::resource() const
{
    return mData->resource;
}

void Addressee::setChanged( bool value )
{
    detach();
    mData->changed = value;
}

bool Addressee::changed()
{
    return mData->changed;
}

QDataStream &KABC::operator<<( QDataStream &s, const Addressee &a )
{
  if (!a.mData) return s;

  s << a.mData->uid;
  s << a.mData->name;
  s << a.mData->formattedName;
  s << a.mData->familyName;
  s << a.mData->givenName;
  s << a.mData->additionalName;
  s << a.mData->prefix;
  s << a.mData->suffix;
  s << a.mData->nickName;
  s << a.mData->birthday;
  s << a.mData->mailer;
  s << a.mData->timeZone;
  s << a.mData->geo;
  s << a.mData->title;
  s << a.mData->role;
  s << a.mData->organization;
  s << a.mData->note;
  s << a.mData->productId;
  s << a.mData->revision;
  s << a.mData->sortString;
  s << a.mData->url;
  s << a.mData->phoneNumbers;
  s << a.mData->addresses;
  s << a.mData->emails;
  s << a.mData->categories;
  s << a.mData->custom;
  return s;
}

QDataStream &KABC::operator>>( QDataStream &s, Addressee &a )
{
  if (!a.mData) return s;

  s >> a.mData->uid;
  s >> a.mData->name;
  s >> a.mData->formattedName;
  s >> a.mData->familyName;
  s >> a.mData->givenName;
  s >> a.mData->additionalName;
  s >> a.mData->prefix;
  s >> a.mData->suffix;
  s >> a.mData->nickName;
  s >> a.mData->birthday;
  s >> a.mData->mailer;
  s >> a.mData->timeZone;
  s >> a.mData->geo;
  s >> a.mData->title;
  s >> a.mData->role;
  s >> a.mData->organization;
  s >> a.mData->note;
  s >> a.mData->productId;
  s >> a.mData->revision;
  s >> a.mData->sortString;
  s >> a.mData->url;
  s >> a.mData->phoneNumbers;
  s >> a.mData->addresses;
  s >> a.mData->emails;
  s >> a.mData->categories;
  s >> a.mData->custom;

  a.mData->empty = false;

  return s;
}
