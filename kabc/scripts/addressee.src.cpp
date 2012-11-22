/*
    This file is part of libkabc.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2003 Carsten Pfeiffer <pfeiffer@kde.org>
    Copyright (c) 2005 Ingo Kloecker <kloecker@kde.org>

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

#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QSharedData>

#include <kdebug.h>
#include <klocale.h>
#include <krandom.h>

#include "addresseehelper.h"
#include "field.h"
#ifndef KDEPIM_NO_KRESOURCES
#include "resource.h"
#endif
#include "sortmode.h"

#include "addressee.h"

using namespace KABC;

static bool matchBinaryPattern( int value, int pattern );

template <class L>
static bool listEquals( const QList<L>&, const QList<L>& );
static bool listEquals( const QStringList&, const QStringList& );
static bool emailsEquals( const QStringList&, const QStringList& );

class Addressee::Private : public QSharedData
{
  public:
    Private()
      : mUid( KRandom::randomString( 10 ) ),
#ifndef KDEPIM_NO_KRESOURCES
      mResource( 0 ),
#endif
        mEmpty( true ), mChanged( false )
    {
    }

    Private( const Private &other )
      : QSharedData( other )
#ifndef KDEPIM_NO_KRESOURCES
      ,mResource( 0 )
#endif
    {
      mUid = other.mUid;
      --VARIABLES_ASSIGNMENT--

      mPhoneNumbers = other.mPhoneNumbers;
      mAddresses = other.mAddresses;
      mKeys = other.mKeys;
      mEmails = other.mEmails;
      mCategories = other.mCategories;
      mCustom = other.mCustom;

#ifndef KDEPIM_NO_KRESOURCES
      mResource = other.mResource;
#endif
      mEmpty = other.mEmpty;
      mChanged = other.mChanged;
    }

    ~Private()
    {
    }

    QString mUid;
    --VARIABLES--

    PhoneNumber::List mPhoneNumbers;
    Address::List mAddresses;
    Key::List mKeys;
    QStringList mEmails;
    QStringList mCategories;
    QStringList mCustom;

#ifndef KDEPIM_NO_KRESOURCES
    Resource *mResource;
#endif

    bool mEmpty    :1;
    bool mChanged  :1;

    static KABC::SortMode *mSortMode;
};

KABC::SortMode *Addressee::Private::mSortMode = 0;

Addressee::Addressee()
  : d( new Private )
{
}

Addressee::~Addressee()
{
}

Addressee::Addressee( const Addressee &other )
  : d( other.d )
{
}

Addressee& Addressee::operator=( const Addressee &other )
{
  if ( this != &other )
    d = other.d;

  return *this;
}

bool Addressee::operator==( const Addressee &a ) const
{
  if ( d->mUid != a.d->mUid ) {
    kDebug() << "uid differs";
    return false;
  }
  --EQUALSTEST--
  if ( ( d->mUrl.isValid() || a.d->mUrl.isValid() ) &&
       ( d->mUrl != a.d->mUrl ) ) {
    kDebug() << "url differs";
    return false;
  }
  if ( !listEquals( d->mPhoneNumbers, a.d->mPhoneNumbers ) ) {
    kDebug() << "phoneNumbers differs";
    return false;
  }
  if ( !listEquals( d->mAddresses, a.d->mAddresses ) ) {
    kDebug() << "addresses differs";
    return false;
  }
  if ( !listEquals( d->mKeys, a.d->mKeys ) ) {
    kDebug() << "keys differs";
    return false;
  }
  if ( !emailsEquals( d->mEmails, a.d->mEmails ) ) {
    kDebug() << "emails differs";
    return false;
  }
  if ( !listEquals( d->mCategories, a.d->mCategories ) ) {
    kDebug() << "categories differs";
    return false;
  }
  if ( !listEquals( d->mCustom, a.d->mCustom ) ) {
    kDebug() << "custom differs";
    return false;
  }

  return true;
}

bool Addressee::operator!=( const Addressee &a ) const
{
  return !( a == *this );
}

bool Addressee::isEmpty() const
{
  return d->mEmpty;
}

void Addressee::setUid( const QString &id )
{
  if ( id == d->mUid ) return;
  d->mEmpty = false;
  d->mUid = id;
}

QString Addressee::uid() const
{
  return d->mUid;
}

QString Addressee::uidLabel()
{
  return i18n("Unique Identifier");
}

--DEFINITIONS--

void Addressee::setNameFromString( const QString &s )
{
  QString str = s;
  //remove enclosing quotes from string
  if ( str.length() > 1  && s[ 0 ] == QLatin1Char( '"' ) && s[ s.length() - 1 ] == QLatin1Char( '"' ) )
    str = s.mid( 1, s.length() - 2 );

  setFormattedName( str );
  setName( str );

  // clear all name parts
  setPrefix( QString() );
  setGivenName( QString() );
  setAdditionalName( QString() );
  setFamilyName( QString() );
  setSuffix( QString() );

  if ( str.isEmpty() )
    return;

  static QString spaceStr = QString::fromLatin1( " " );
  static QString emptyStr = QString::fromLatin1( "" );
  AddresseeHelper *helper = AddresseeHelper::self();

  int i = str.indexOf( QLatin1Char( ',' ) );
  if ( i < 0 ) {
    QStringList parts = str.split( spaceStr );
    int leftOffset = 0;
    int rightOffset = parts.count() - 1;

    QString suffix;
    while ( rightOffset >= 0 ) {
      if ( helper->containsSuffix( parts[ rightOffset ] ) ) {
        suffix.prepend(parts[ rightOffset ] + (suffix.isEmpty() ? emptyStr : spaceStr));
        rightOffset--;
      } else
        break;
    }
    setSuffix( suffix );

    if ( rightOffset < 0 )
      return;

    if ( rightOffset - 1 >= 0 && helper->containsPrefix( parts[ rightOffset - 1 ].toLower() ) ) {
      setFamilyName( parts[ rightOffset - 1 ] + spaceStr + parts[ rightOffset ] );
      rightOffset--;
    } else {
      if ( helper->tradeAsFamilyName() )
        setFamilyName( parts[ rightOffset ] );
      else
        setGivenName( parts[ rightOffset ] );
    }

    QString prefix;
    while ( leftOffset < rightOffset ) {
      if ( helper->containsTitle( parts[ leftOffset ] ) ) {
        prefix.append( (prefix.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ] );
        leftOffset++;
      } else
        break;
    }
    setPrefix( prefix );

    if ( leftOffset < rightOffset ) {
      setGivenName( parts[ leftOffset ] );
      leftOffset++;
    }

    QString additionalName;
    while ( leftOffset < rightOffset ) {
      additionalName.append( (additionalName.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ] );
      leftOffset++;
    }
    setAdditionalName( additionalName );
  } else {
    QString part1 = str.left( i );
    QString part2 = str.mid( i + 1 );

    QStringList parts = part1.split( spaceStr );
    int leftOffset = 0;
    int rightOffset = parts.count() - 1;

    if ( parts.count() > 0 ) {

      QString suffix;
      while ( rightOffset >= 0 ) {
        if ( helper->containsSuffix( parts[ rightOffset ] ) ) {
          suffix.prepend( parts[ rightOffset ] + (suffix.isEmpty() ? emptyStr : spaceStr) );
          rightOffset--;
        } else
          break;
      }
      setSuffix( suffix );

      if ( rightOffset - 1 >= 0 && helper->containsPrefix( parts[ rightOffset - 1 ].toLower() ) ) {
        setFamilyName( parts[ rightOffset - 1 ] + spaceStr + parts[ rightOffset ] );
        rightOffset--;
      } else
        setFamilyName( parts[ rightOffset ] );

      QString prefix;
      while ( leftOffset < rightOffset ) {
        if ( helper->containsTitle( parts[ leftOffset ] ) ) {
          prefix.append( (prefix.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ] );
          leftOffset++;
        } else
          break;
      }
    } else {
      setPrefix( QString() );
      setFamilyName( QString() );
      setSuffix( QString() );
    }

    parts = part2.split( spaceStr );

    leftOffset = 0;
    rightOffset = parts.count();

    if ( parts.count() > 0 ) {

      QString prefix;
      while ( leftOffset < rightOffset ) {
        if ( helper->containsTitle( parts[ leftOffset ] ) ) {
          prefix.append( (prefix.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ] );
          leftOffset++;
        } else
          break;
      }
      setPrefix( prefix );

      if ( leftOffset < rightOffset ) {
        setGivenName( parts[ leftOffset ] );
        leftOffset++;
      }

      QString additionalName;
      while ( leftOffset < rightOffset ) {
        additionalName.append( (additionalName.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ] );
        leftOffset++;
      }
      setAdditionalName( additionalName );
    } else {
      setGivenName( QString() );
      setAdditionalName( QString() );
    }
  }
}

QString Addressee::realName() const
{
  QString n( formattedName() );
  if ( !n.isEmpty() )
    return n;

  n = assembledName();
  if ( !n.isEmpty() )
    return n;

  n = name();
  if ( !n.isEmpty() )
    return n;

  return organization();
}

QString Addressee::assembledName() const
{
  const QString name = prefix() + QLatin1Char( ' ' ) +
                       givenName() + QLatin1Char( ' ' ) +
                       additionalName() + QLatin1Char( ' ' ) +
                       familyName() + QLatin1Char( ' ' ) +
                       suffix();

  return name.simplified();
}

QString Addressee::fullEmail( const QString &email ) const
{
  QString e;
  if ( email.isNull() ) {
    e = preferredEmail();
  } else {
    e = email;
  }
  if ( e.isEmpty() ) return QString();

  QString text;
  if ( realName().isEmpty() )
    text = e;
  else {
    QRegExp needQuotes( QLatin1String( "[^ 0-9A-Za-z\\x0080-\\xFFFF]" ) );
    if ( realName().indexOf( needQuotes ) != -1 ) {
      QString name = realName();
      name.replace( QLatin1String( "\"" ), QLatin1String( "\\\"" ) );
      text = QLatin1String( "\"" ) + name + QLatin1String( "\" <" ) + e + QLatin1Char( '>' );
    } else
      text = realName() + QLatin1String( " <" ) + e + QLatin1Char( '>' );
  }

  return text;
}

void Addressee::insertEmail( const QString &email, bool preferred )
{
  if ( email.simplified().isEmpty() )
    return;

  if ( d->mEmails.contains( email ) ) {
    if ( !preferred || d->mEmails.first() == email )
      return;

    d->mEmails.removeAll( email );
    d->mEmails.prepend( email );
  } else {
    d->mEmpty = false;
    if ( preferred ) {
      d->mEmails.prepend( email );
    } else {
      d->mEmails.append( email );
    }
  }
}

void Addressee::removeEmail( const QString &email )
{
  if ( d->mEmails.contains( email ) ) {
    d->mEmails.removeAll( email );
  }
}

QString Addressee::preferredEmail() const
{
  if ( d->mEmails.count() == 0 )
    return QString();
  else
    return d->mEmails.first();
}

QStringList Addressee::emails() const
{
  return d->mEmails;
}

void Addressee::setEmails( const QStringList& emails )
{
  d->mEmails = emails;
}
void Addressee::insertPhoneNumber( const PhoneNumber &phoneNumber )
{
  d->mEmpty = false;

  PhoneNumber::List::Iterator it;
  for ( it = d->mPhoneNumbers.begin(); it != d->mPhoneNumbers.end(); ++it ) {
    if ( (*it).id() == phoneNumber.id() ) {
      *it = phoneNumber;
      return;
    }
  }
  if ( !phoneNumber.number().simplified().isEmpty() )
    d->mPhoneNumbers.append( phoneNumber );
}

void Addressee::removePhoneNumber( const PhoneNumber &phoneNumber )
{
  PhoneNumber::List::Iterator it;
  for ( it = d->mPhoneNumbers.begin(); it != d->mPhoneNumbers.end(); ++it ) {
    if ( (*it).id() == phoneNumber.id() ) {
      d->mPhoneNumbers.erase( it );
      return;
    }
  }
}

PhoneNumber Addressee::phoneNumber( PhoneNumber::Type type ) const
{
  PhoneNumber phoneNumber( QString(), type );
  PhoneNumber::List::ConstIterator it;
  for ( it = d->mPhoneNumbers.constBegin(); it != d->mPhoneNumbers.constEnd(); ++it ) {
    if ( matchBinaryPattern( (*it).type(), type ) ) {
      if ( (*it).type() & PhoneNumber::Pref )
        return (*it);
      else if ( phoneNumber.number().isEmpty() )
        phoneNumber = (*it);
    }
  }

  return phoneNumber;
}

PhoneNumber::List Addressee::phoneNumbers() const
{
  return d->mPhoneNumbers;
}

PhoneNumber::List Addressee::phoneNumbers( PhoneNumber::Type type ) const
{
  PhoneNumber::List list;

  PhoneNumber::List::ConstIterator it;
  PhoneNumber::List::ConstIterator end(d->mPhoneNumbers.constEnd());
  for ( it = d->mPhoneNumbers.constBegin(); it != end; ++it ) {
    if ( matchBinaryPattern( (*it).type(), type ) ) {
      list.append( *it );
    }
  }
  return list;
}

PhoneNumber Addressee::findPhoneNumber( const QString &id ) const
{
  PhoneNumber::List::ConstIterator it;
  PhoneNumber::List::ConstIterator end(d->mPhoneNumbers.constEnd());
  for ( it = d->mPhoneNumbers.constBegin(); it != end; ++it ) {
    if ( (*it).id() == id ) {
      return *it;
    }
  }
  return PhoneNumber();
}

void Addressee::insertKey( const Key &key )
{
  d->mEmpty = false;

  Key::List::Iterator it;
  for ( it = d->mKeys.begin(); it != d->mKeys.end(); ++it ) {
    if ( (*it).id() == key.id() ) {
      *it = key;
      return;
    }
  }
  d->mKeys.append( key );
}

void Addressee::removeKey( const Key &key )
{
  Key::List::Iterator it;
  for ( it = d->mKeys.begin(); it != d->mKeys.end(); ++it ) {
    if ( (*it).id() == key.id() ) {
      d->mKeys.removeAll( key );
      return;
    }
  }
}

Key Addressee::key( Key::Type type, QString customTypeString ) const
{
  Key::List::ConstIterator it;
  Key::List::ConstIterator end(d->mKeys.constEnd());
  for ( it = d->mKeys.constBegin(); it != end; ++it ) {
    if ( (*it).type() == type ) {
      if ( type == Key::Custom ) {
        if ( customTypeString.isEmpty() ) {
          return *it;
        } else {
          if ( (*it).customTypeString() == customTypeString )
            return (*it);
        }
      } else {
        return *it;
      }
    }
  }
  return Key( QString(), type );
}

void Addressee::setKeys( const Key::List& list )
{
  d->mKeys = list;
}

Key::List Addressee::keys() const
{
  return d->mKeys;
}

Key::List Addressee::keys( Key::Type type, QString customTypeString ) const
{
  Key::List list;

  Key::List::ConstIterator it;
  Key::List::ConstIterator end(d->mKeys.constEnd());
  for ( it = d->mKeys.constBegin(); it != end; ++it ) {
    if ( (*it).type() == type ) {
      if ( type == Key::Custom ) {
        if ( customTypeString.isEmpty() ) {
          list.append( *it );
        } else {
          if ( (*it).customTypeString() == customTypeString )
            list.append( *it );
        }
      } else {
        list.append( *it );
      }
    }
  }
  return list;
}

Key Addressee::findKey( const QString &id ) const
{
  Key::List::ConstIterator it;
  Key::List::ConstIterator end(d->mKeys.constEnd());
  for ( it = d->mKeys.constBegin(); it != end; ++it ) {
    if ( (*it).id() == id ) {
      return *it;
    }
  }
  return Key();
}

QString Addressee::toString() const
{
  QString str;

  str += QLatin1String( "Addressee {\n" );
  str += QString::fromLatin1( "  Uid: %1\n" ).arg( uid() );

  --DEBUG--

  str += QLatin1String( "  Emails {\n" );
  const QStringList e = emails();
  QStringList::ConstIterator it;
  for ( it = e.begin(); it != e.end(); ++it ) {
    str += QString::fromLatin1( "    %1\n" ).arg( *it );
  }
  str += QLatin1String( "  }\n" );

  str += QLatin1String( "  PhoneNumbers {\n" );
  const PhoneNumber::List p = phoneNumbers();
  PhoneNumber::List::ConstIterator it2;
  for ( it2 = p.begin(); it2 != p.end(); ++it2 ) {
    str += (*it2).toString();
  }
  str += QLatin1String( "  }\n" );

  str += QLatin1String( "  Addresses {\n" );
  const Address::List a = addresses();
  Address::List::ConstIterator it3;
  for ( it3 = a.begin(); it3 != a.end(); ++it3 ) {
    str += (*it3).toString();
  }
  str += QLatin1String( "  }\n" );

  str += QLatin1String( "  Keys {\n" );
  const Key::List k = keys();
  Key::List::ConstIterator it4;
  for ( it4 = k.begin(); it4 != k.end(); ++it4 ) {
    str += (*it4).toString();
  }
  str += QLatin1String( "  }\n" );

  str += QLatin1String( "}\n" );

  return str;
}


void Addressee::insertAddress( const Address &address )
{
  if ( address.isEmpty() )
    return;

  d->mEmpty = false;

  Address::List::Iterator it;
  for ( it = d->mAddresses.begin(); it != d->mAddresses.end(); ++it ) {
    if ( (*it).id() == address.id() ) {
      *it = address;
      return;
    }
  }

  d->mAddresses.append( address );
}

void Addressee::removeAddress( const Address &address )
{
  Address::List::Iterator it;
  for ( it = d->mAddresses.begin(); it != d->mAddresses.end(); ++it ) {
    if ( (*it).id() == address.id() ) {
      d->mAddresses.erase( it );
      return;
    }
  }
}

Address Addressee::address( Address::Type type ) const
{
  Address address( type );
  Address::List::ConstIterator it;
  Address::List::ConstIterator end(d->mAddresses.constEnd());
  for ( it = d->mAddresses.constBegin(); it != end; ++it ) {
    if ( matchBinaryPattern( (*it).type(), type ) ) {
      if ( (*it).type() & Address::Pref )
        return (*it);
      else if ( address.isEmpty() )
        address = (*it);
    }
  }

  return address;
}

Address::List Addressee::addresses() const
{
  return d->mAddresses;
}

Address::List Addressee::addresses( Address::Type type ) const
{
  Address::List list;

  Address::List::ConstIterator it;
  Address::List::ConstIterator end(d->mAddresses.constEnd());
  for ( it = d->mAddresses.constBegin(); it != end; ++it ) {
    if ( matchBinaryPattern( (*it).type(), type ) ) {
      list.append( *it );
    }
  }

  return list;
}

Address Addressee::findAddress( const QString &id ) const
{
  Address::List::ConstIterator it;
  Address::List::ConstIterator end(d->mAddresses.constEnd());
  for ( it = d->mAddresses.constBegin(); it != end; ++it ) {
    if ( (*it).id() == id ) {
      return *it;
    }
  }
  return Address();
}

void Addressee::insertCategory( const QString &c )
{
  d->mEmpty = false;

  if ( d->mCategories.contains( c ) )
    return;

  d->mCategories.append( c );
}

void Addressee::removeCategory( const QString &category )
{
  if ( d->mCategories.contains( category ) ) {
    d->mCategories.removeAll( category );
  }
}

bool Addressee::hasCategory( const QString &category ) const
{
  return d->mCategories.contains( category );
}

void Addressee::setCategories( const QStringList &c )
{
  d->mEmpty = false;

  d->mCategories = c;
}

QStringList Addressee::categories() const
{
  return d->mCategories;
}

void Addressee::insertCustom( const QString &app, const QString &name,
                              const QString &value )
{
  if ( value.isEmpty() || name.isEmpty() || app.isEmpty() ) return;

  d->mEmpty = false;

  QString qualifiedName = app + QLatin1Char( '-' ) + name + QLatin1Char( ':' );

  QStringList::Iterator it;
  QStringList::Iterator end(d->mCustom.end());
  for ( it = d->mCustom.begin(); it != end; ++it ) {
    if ( (*it).startsWith( qualifiedName ) ) {
      (*it) = qualifiedName + value;
      return;
    }
  }

  d->mCustom.append( qualifiedName + value );
}

void Addressee::removeCustom( const QString &app, const QString &name )
{
  const QString qualifiedName = app + QLatin1Char( '-' ) + name + QLatin1Char( ':' );

  QStringList::Iterator it;
  for ( it = d->mCustom.begin(); it != d->mCustom.end(); ++it ) {
    if ( (*it).startsWith( qualifiedName ) ) {
      d->mCustom.erase( it );
      return;
    }
  }
}

QString Addressee::custom( const QString &app, const QString &name ) const
{
  QString qualifiedName = app + QLatin1Char( '-' ) + name + QLatin1Char( ':' );
  QString value;

  QStringList::ConstIterator it;
  QStringList::ConstIterator end(d->mCustom.constEnd());
  for ( it = d->mCustom.constBegin(); it != end; ++it ) {
    if ( (*it).startsWith( qualifiedName ) ) {
      value = (*it).mid( (*it).indexOf( QLatin1Char( ':' ) ) + 1 );
      break;
    }
  }

  return value;
}

void Addressee::setCustoms( const QStringList &l )
{
  d->mEmpty = false;
  d->mCustom = l;
}

QStringList Addressee::customs() const
{
  return d->mCustom;
}

void Addressee::parseEmailAddress( const QString &rawEmail, QString &fullName,
                                   QString &email )
{
  // This is a simplified version of KPIM::splitAddress().

  fullName.clear();
  email.clear();
  if ( rawEmail.isEmpty() )
    return; // KPIM::AddressEmpty;

  // The code works on 8-bit strings, so convert the input to UTF-8.
  QByteArray address = rawEmail.toUtf8();

  QByteArray displayName;
  QByteArray addrSpec;
  QByteArray comment;

  // The following is a primitive parser for a mailbox-list (cf. RFC 2822).
  // The purpose is to extract a displayable string from the mailboxes.
  // Comments in the addr-spec are not handled. No error checking is done.

  enum { TopLevel, InComment, InAngleAddress } context = TopLevel;
  bool inQuotedString = false;
  int commentLevel = 0;
  bool stop = false;

  for ( char* p = address.data(); *p && !stop; ++p ) {
    switch ( context ) {
    case TopLevel : {
      switch ( *p ) {
      case '"' : inQuotedString = !inQuotedString;
                 displayName += *p;
                 break;
      case '(' : if ( !inQuotedString ) {
                   context = InComment;
                   commentLevel = 1;
                 }
                 else
                   displayName += *p;
                 break;
      case '<' : if ( !inQuotedString ) {
                   context = InAngleAddress;
                 }
                 else
                   displayName += *p;
                 break;
      case '\\' : // quoted character
                 displayName += *p;
                 ++p; // skip the '\'
                 if ( *p )
                   displayName += *p;
                 else
                   //return KPIM::UnexpectedEnd;
                   goto ABORT_PARSING;
                 break;
      case ',' : if ( !inQuotedString ) {
                   //if ( allowMultipleAddresses )
                   //  stop = true;
                   //else
                   //  return KPIM::UnexpectedComma;
                   goto ABORT_PARSING;
                 }
                 else
                   displayName += *p;
                 break;
      default :  displayName += *p;
      }
      break;
    }
    case InComment : {
      switch ( *p ) {
      case '(' : ++commentLevel;
                 comment += *p;
                 break;
      case ')' : --commentLevel;
                 if ( commentLevel == 0 ) {
                   context = TopLevel;
                   comment += ' '; // separate the text of several comments
                 }
                 else
                   comment += *p;
                 break;
      case '\\' : // quoted character
                 comment += *p;
                 ++p; // skip the '\'
                 if ( *p )
                   comment += *p;
                 else
                   //return KPIM::UnexpectedEnd;
                   goto ABORT_PARSING;
                 break;
      default :  comment += *p;
      }
      break;
    }
    case InAngleAddress : {
      switch ( *p ) {
      case '"' : inQuotedString = !inQuotedString;
                 addrSpec += *p;
                 break;
      case '>' : if ( !inQuotedString ) {
                   context = TopLevel;
                 }
                 else
                   addrSpec += *p;
                 break;
      case '\\' : // quoted character
                 addrSpec += *p;
                 ++p; // skip the '\'
                 if ( *p )
                   addrSpec += *p;
                 else
                   //return KPIM::UnexpectedEnd;
                   goto ABORT_PARSING;
                 break;
      default :  addrSpec += *p;
      }
      break;
    }
    } // switch ( context )
  }

ABORT_PARSING:
  displayName = displayName.trimmed();
  comment = comment.trimmed();
  addrSpec = addrSpec.trimmed();

  fullName = QString::fromUtf8( displayName );
  email = QString::fromUtf8( addrSpec );

  // check for errors
  if ( inQuotedString )
    return; // KPIM::UnbalancedQuote;
  if ( context == InComment )
    return; // KPIM::UnbalancedParens;
  if ( context == InAngleAddress )
    return; // KPIM::UnclosedAngleAddr;

  if ( addrSpec.isEmpty() ) {
    if ( displayName.isEmpty() )
      return; // KPIM::NoAddressSpec;
    else {
      //addrSpec = displayName;
      //displayName.truncate( 0 );
      // Address of the form "foo@bar" or "foo@bar (Name)".
      email = fullName;
      fullName = QString::fromUtf8( comment );
    }
  }

  // Check that we do not have any extra characters on the end of the
  // strings
  unsigned int len = fullName.length();
  if ( fullName[ 0 ] == QLatin1Char( '"' ) && fullName[ len - 1 ] == QLatin1Char( '"' ) )
    fullName = fullName.mid( 1, len - 2 );
}

#ifndef KDEPIM_NO_KRESOURCES
void Addressee::setResource( Resource *resource )
{
  d->mResource = resource;
}

Resource *Addressee::resource() const
{
  return d->mResource;
}
#endif

void Addressee::setChanged( bool value )
{
  d->mChanged = value;
}

bool Addressee::changed() const
{
  return d->mChanged;
}

void Addressee::setSortMode( KABC::SortMode *mode )
{
  Private::mSortMode = mode;
}

bool Addressee::operator< ( const Addressee &addr ) const
{
  if ( !Private::mSortMode )
    return false;
  else
    return Private::mSortMode->lesser( *this, addr );
}

QString Addressee::mimeType()
{
  return QLatin1String( "text/directory" );
}

QDataStream &KABC::operator<<( QDataStream &s, const Addressee &a )
{
  s << a.d->mUid;

  --STREAMOUT--
  s << a.d->mPhoneNumbers;
  s << a.d->mAddresses;
  s << a.d->mEmails;
  s << a.d->mCategories;
  s << a.d->mCustom;
  s << a.d->mKeys;
  return s;
}

QDataStream &KABC::operator>>( QDataStream &s, Addressee &a )
{
  s >> a.d->mUid;

  --STREAMIN--
  s >> a.d->mPhoneNumbers;
  s >> a.d->mAddresses;
  s >> a.d->mEmails;
  s >> a.d->mCategories;
  s >> a.d->mCustom;
  s >> a.d->mKeys;

  a.d->mEmpty = false;

  return s;
}

bool matchBinaryPattern( int value, int pattern )
{
  /**
    We want to match all telephonnumbers/addresses which have the bits in the
    pattern set. More are allowed.
    if pattern == 0 we have a special handling, then we want only those with
    exactly no bit set.
   */
  if ( pattern == 0 )
    return ( value == 0 );
  else
    return ( pattern == ( pattern & value ) );
}

template <class L>
bool listEquals( const QList<L> &list, const QList<L> &pattern )
{
  if ( list.count() != pattern.count() )
    return false;
  const int numberOfElement(list.count());
  for ( int i = 0; i < numberOfElement; ++i )
    if ( !pattern.contains( list[ i ] ) )
      return false;

  return true;
}

bool listEquals( const QStringList &list, const QStringList &pattern )
{
  if ( list.count() != pattern.count() )
    return false;

  const int numberOfElement(list.count());
  for ( int i = 0; i < numberOfElement; ++i )
    if ( !pattern.contains( list[ i ] ) )
      return false;

  return true;
}

bool emailsEquals( const QStringList &list, const QStringList &pattern )
{
  if ( list.count() != pattern.count() )
    return false;

  if ( list.isEmpty() )
    return true;

  if ( list.first() != pattern.first() )
    return false;

  QStringList::ConstIterator it;
  QStringList::ConstIterator end(list.constEnd());
  for ( it = list.constBegin(); it != end; ++it )
    if ( !pattern.contains( *it ) )
      return false;

  return true;
}
