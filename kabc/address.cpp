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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "address.h"

#include <krandom.h>
#include <kdebug.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstandarddirs.h>
#include <kconfiggroup.h>

#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QTextStream>
#include <QtCore/QSharedData>

using namespace KABC;

// template tags for address formatting localization
#define KABC_FMTTAG_realname   QString("%n")
#define KABC_FMTTAG_REALNAME   QString("%N")
#define KABC_FMTTAG_company    QString("%cm")
#define KABC_FMTTAG_COMPANY    QString("%CM")
#define KABC_FMTTAG_pobox      QString("%p")
#define KABC_FMTTAG_street     QString("%s")
#define KABC_FMTTAG_STREET     QString("%S")
#define KABC_FMTTAG_zipcode    QString("%z")
#define KABC_FMTTAG_location   QString("%l")
#define KABC_FMTTAG_LOCATION   QString("%L")
#define KABC_FMTTAG_region     QString("%r")
#define KABC_FMTTAG_REGION     QString("%R")
#define KABC_FMTTAG_newline    QString("\\n")
#define KABC_FMTTAG_condcomma  QString("%,")
#define KABC_FMTTAG_condwhite  QString("%w")
#define KABC_FMTTAG_purgeempty QString("%0")

/**
  Finds the balanced closing bracket starting from the opening bracket at
  pos in tsection.
  @return  position of closing bracket, -1 for unbalanced brackets
*/
static int findBalancedBracket( const QString &tsection, int pos )
{
  int balancecounter = 0;
  for ( int i = pos + 1; i < tsection.length(); ++i ) {
    if ( ')' == tsection[i] && 0 == balancecounter ) {
      // found end of brackets
      return i;
    } else {
      if ( '(' == tsection[i] ) {
        // nested brackets
        balancecounter++;
      }
    }
  }
  return -1;
}

/**
  Parses a snippet of an address template
  @param tsection   the template string to be parsed
  @param result     QString reference in which the result will be stored
  @return           true if at least one tag evaluated positively, else false
*/
static bool parseAddressTemplateSection( const QString &tsection, QString &result,
                                         const QString &realName, const QString &orgaName,
                                         const KABC::Address &address )
{
  // This method first parses and substitutes any bracketed sections and
  // after that replaces any tags with their values. If a bracketed section
  // or a tag evaluate to zero, they are not just removed but replaced
  // with a placeholder. This is because in the last step conditionals are
  // resolved which depend on information about zero-evaluations.
  result = tsection;
  int stpos = 0;
  bool ret = false;

  // first check for brackets that have to be evaluated first
  int fpos = result.indexOf( KABC_FMTTAG_purgeempty, stpos );
  while ( -1 != fpos ) {
    int bpos1 = fpos + KABC_FMTTAG_purgeempty.length();
    int bpos2;
    // expect opening bracket and find next balanced closing bracket. If
    // next char is no opening bracket, continue parsing (no valid tag)
    if ( '(' == result[bpos1] ) {
      bpos2 = findBalancedBracket( result, bpos1 );
      if ( -1 != bpos2 ) {
        // we have balanced brackets, recursively parse:
        QString rplstr;
        bool purge = !parseAddressTemplateSection( result.mid( bpos1+1,
                                                   bpos2-bpos1-1 ), rplstr,
                                                   realName, orgaName, address );
        if ( purge ) {
          // purge -> remove all
          // replace with !_P_!, so conditional tags work later
          result.replace( fpos, bpos2 - fpos + 1, "!_P_!" );
          // leave stpos as it is
        } else {
          // no purge -> replace with recursively parsed string
          result.replace( fpos, bpos2 - fpos + 1, rplstr );
          ret = true;
          stpos = fpos + rplstr.length();
        }
      } else {
        // unbalanced brackets:  keep on parsing (should not happen
        // and will result in bad formatting)
        stpos = bpos1;
      }
    }
    fpos = result.indexOf( KABC_FMTTAG_purgeempty, stpos );
  }

  // after sorting out all purge tags, we just search'n'replace the rest,
  // keeping track of whether at least one tag evaluates to something.
  // The following macro needs QString for R_FIELD
  // It substitutes !_P_! for empty fields so conditional tags work later
#define REPLTAG(R_TAG,R_FIELD) \
  if ( result.indexOf( R_TAG, false ) != -1 ) { \
    QString rpl = R_FIELD.isEmpty() ? QString( "!_P_!" ) : R_FIELD; \
    result.replace( R_TAG, rpl ); \
    if ( !R_FIELD.isEmpty() ) { \
      ret = true; \
    } \
  }
  REPLTAG( KABC_FMTTAG_realname, realName );
  REPLTAG( KABC_FMTTAG_REALNAME, realName.toUpper() );
  REPLTAG( KABC_FMTTAG_company, orgaName );
  REPLTAG( KABC_FMTTAG_COMPANY, orgaName.toUpper() );
  REPLTAG( KABC_FMTTAG_pobox, address.postOfficeBox() );
  REPLTAG( KABC_FMTTAG_street, address.street() );
  REPLTAG( KABC_FMTTAG_STREET, address.street().toUpper() );
  REPLTAG( KABC_FMTTAG_zipcode, address.postalCode() );
  REPLTAG( KABC_FMTTAG_location, address.locality() );
  REPLTAG( KABC_FMTTAG_LOCATION, address.locality().toUpper() );
  REPLTAG( KABC_FMTTAG_region, address.region() );
  REPLTAG( KABC_FMTTAG_REGION, address.region().toUpper() );
  result.replace( KABC_FMTTAG_newline, "\n" );
#undef REPLTAG

  // conditional comma
  fpos = result.indexOf( KABC_FMTTAG_condcomma, 0 );
  while ( -1 != fpos ) {
    QString str1 = result.mid( fpos - 5, 5 );
    QString str2 = result.mid( fpos + 2, 5 );
    if ( str1 != "!_P_!" && str2 != "!_P_!" ) {
      result.replace( fpos, 2, ", " );
    } else {
      result.remove( fpos, 2 );
    }
    fpos = result.indexOf( KABC_FMTTAG_condcomma, fpos );
  }
  // conditional whitespace
  fpos = result.indexOf( KABC_FMTTAG_condwhite, 0 );
  while ( -1 != fpos ) {
    QString str1 = result.mid( fpos - 5, 5 );
    QString str2 = result.mid( fpos + 2, 5 );
    if ( str1 != "!_P_!" && str2 != "!_P_!" ) {
      result.replace( fpos, 2, " " );
    } else {
      result.remove( fpos, 2 );
    }
    fpos = result.indexOf( KABC_FMTTAG_condwhite, fpos );
  }

  // remove purged:
  result.remove( "!_P_!" );

  return ret;
}

class Address::Private : public QSharedData
{
  public:
    Private()
      : mEmpty( true ), mType( 0 )
    {
      mId = KRandom::randomString( 10 );
    }

    Private( const Private &other )
      : QSharedData( other )
    {
      mEmpty = other.mEmpty;
      mId = other.mId;
      mType = other.mType;

      mPostOfficeBox = other.mPostOfficeBox;
      mExtended = other.mExtended;
      mStreet = other.mStreet;
      mLocality = other.mLocality;
      mRegion = other.mRegion;
      mPostalCode = other.mPostalCode;
      mCountry = other.mCountry;
      mLabel = other.mLabel;
    }

    bool mEmpty;
    QString mId;
    Type mType;

    QString mPostOfficeBox;
    QString mExtended;
    QString mStreet;
    QString mLocality;
    QString mRegion;
    QString mPostalCode;
    QString mCountry;
    QString mLabel;
};

Address::Address()
  : d( new Private )
{
}

Address::Address( Type type )
  : d( new Private )
{
  d->mType = type;
}

Address::Address( const Address &other )
  : d( other.d )
{
}

Address::~Address()
{
}

Address &Address::operator=( const Address &other )
{
  if ( this != &other ) {
    d = other.d;
  }

  return *this;
}

bool Address::operator==( const Address &other ) const
{
  if ( d->mId != other.d->mId ) {
    return false;
  }
  if ( d->mType != other.d->mType ) {
    return false;
  }
  if ( d->mPostOfficeBox != other.d->mPostOfficeBox ) {
    return false;
  }
  if ( d->mExtended != other.d->mExtended ) {
    return false;
  }
  if ( d->mStreet != other.d->mStreet ) {
    return false;
  }
  if ( d->mLocality != other.d->mLocality ) {
    return false;
  }
  if ( d->mRegion != other.d->mRegion ) {
    return false;
  }
  if ( d->mPostalCode != other.d->mPostalCode ) {
    return false;
  }
  if ( d->mCountry != other.d->mCountry ) {
    return false;
  }
  if ( d->mLabel != other.d->mLabel ) {
    return false;
  }

  return true;
}

bool Address::operator!=( const Address &a ) const
{
  return !( a == *this );
}

bool Address::isEmpty() const
{
  return d->mEmpty;
}

void Address::clear()
{
  *this = Address();
}

void Address::setId( const QString &id )
{
  d->mEmpty = false;
  d->mId = id;
}

QString Address::id() const
{
  return d->mId;
}

void Address::setType( Type type )
{
  d->mEmpty = false;
  d->mType = type;
}

Address::Type Address::type() const
{
  return d->mType;
}

QString Address::typeLabel() const
{
  QString label;
  bool first = true;

  const TypeList list = typeList();

  TypeList::ConstIterator it;
  for ( it = list.begin(); it != list.end(); ++it ) {
    if ( ( type() & (*it) ) && ( (*it) != Pref ) ) {
      label.append( ( first ? "" : "/" ) + typeLabel( *it ) );
      if ( first ) {
        first = false;
      }
    }
  }

  return label;
}

void Address::setPostOfficeBox( const QString &postOfficeBox )
{
  d->mEmpty = false;
  d->mPostOfficeBox = postOfficeBox;
}

QString Address::postOfficeBox() const
{
  return d->mPostOfficeBox;
}

QString Address::postOfficeBoxLabel()
{
  return i18n( "Post Office Box" );
}

void Address::setExtended( const QString &extended )
{
  d->mEmpty = false;
  d->mExtended = extended;
}

QString Address::extended() const
{
  return d->mExtended;
}

QString Address::extendedLabel()
{
  return i18n( "Extended Address Information" );
}

void Address::setStreet( const QString &street )
{
  d->mEmpty = false;
  d->mStreet = street;
}

QString Address::street() const
{
  return d->mStreet;
}

QString Address::streetLabel()
{
  return i18n( "Street" );
}

void Address::setLocality( const QString &locality )
{
  d->mEmpty = false;
  d->mLocality = locality;
}

QString Address::locality() const
{
  return d->mLocality;
}

QString Address::localityLabel()
{
  return i18n( "Locality" );
}

void Address::setRegion( const QString &region )
{
  d->mEmpty = false;
  d->mRegion = region;
}

QString Address::region() const
{
  return d->mRegion;
}

QString Address::regionLabel()
{
  return i18n( "Region" );
}

void Address::setPostalCode( const QString &postalCode )
{
  d->mEmpty = false;
  d->mPostalCode = postalCode;
}

QString Address::postalCode() const
{
  return d->mPostalCode;
}

QString Address::postalCodeLabel()
{
  return i18n( "Postal Code" );
}

void Address::setCountry( const QString &country )
{
  d->mEmpty = false;
  d->mCountry = country;
}

QString Address::country() const
{
  return d->mCountry;
}

QString Address::countryLabel()
{
  return i18n( "Country" );
}

void Address::setLabel( const QString &label )
{
  d->mEmpty = false;
  d->mLabel = label;
}

QString Address::label() const
{
  return d->mLabel;
}

QString Address::labelLabel()
{
  return i18n( "Delivery Label" );
}

Address::TypeList Address::typeList()
{
  static TypeList list;

  if ( list.isEmpty() ) {
    list << Dom << Intl << Postal << Parcel << Home << Work << Pref;
  }

  return list;
}

QString Address::typeLabel( Type type )
{
  if ( type & Pref ) {
    return i18nc( "Preferred address", "Preferred" );
  }

  switch ( type ) {
    case Dom:
      return i18nc( "Address is in home country", "Domestic" );
      break;
    case Intl:
      return i18nc( "Address is not in home country", "International" );
      break;
    case Postal:
      return i18nc( "Address for delivering letters", "Postal" );
      break;
    case Parcel:
      return i18nc( "Address for delivering packages", "Parcel" );
      break;
    case Home:
      return i18nc( "Home Address", "Home" );
      break;
    case Work:
      return i18nc( "Work Address", "Work" );
      break;
    case Pref:
      return i18n( "Preferred Address" );
      break;
    default:
      return i18nc( "another type of address", "Other" );
      break;
  }
}

QString Address::toString() const
{
  QString str;

  str += QString( "Address {\n" );
  str += QString( "  IsEmpty: %1\n" ).arg( d->mEmpty ? "true" : "false" );
  str += QString( "  Id: %1\n" ).arg( d->mId );
  str += QString( "  Type: %1\n" ).arg( typeLabel( d->mType ) );
  str += QString( "  Post Office Box: %1\n" ).arg( d->mPostOfficeBox );
  str += QString( "  Extended: %1\n" ).arg( d->mExtended );
  str += QString( "  Street: %1\n" ).arg( d->mStreet );
  str += QString( "  Locality: %1\n" ).arg( d->mLocality );
  str += QString( "  Region: %1\n" ).arg( d->mRegion );
  str += QString( "  Postal Code: %1\n" ).arg( d->mPostalCode );
  str += QString( "  Country: %1\n" ).arg( d->mCountry );
  str += QString( "  Label: %1\n" ).arg( d->mLabel );
  str += QString( "}\n" );

  return str;
}

QString Address::formattedAddress( const QString &realName,
                                   const QString &orgaName ) const
{
  QString ciso;
  QString addrTemplate;
  QString ret;

  // FIXME: first check for iso-country-field and prefer that one
  if ( !country().isEmpty() ) {
    ciso = countryToISO( country() );
  } else {
    // fall back to our own country
    ciso = KGlobal::locale()->country();
  }
  KConfig entry( KStandardDirs::locate( "locale",
        QString( "l10n/" ) + ciso + QString( "/entry.desktop" ) ) );

  KConfigGroup group = entry.group( "KCM Locale" );
  // decide whether this needs special business address formatting
  if ( orgaName.isEmpty() ) {
    addrTemplate = group.readEntry( "AddressFormat" );
  } else {
    addrTemplate = group.readEntry( "BusinessAddressFormat" );
    if ( addrTemplate.isEmpty() ) {
      addrTemplate = group.readEntry( "AddressFormat" );
    }
  }

  // in the case there's no format found at all, default to what we've always
  // used:
  if ( addrTemplate.isEmpty() ) {
    kWarning(5700) << "address format database incomplete"
        << "(no format for locale" << ciso
        << "found). Using default address formatting.";
    addrTemplate = "%0(%n\\n)%0(%cm\\n)%0(%s\\n)%0(PO BOX %p\\n)%0(%l%w%r)%,%z";
  }

  // scan
  parseAddressTemplateSection( addrTemplate, ret, realName, orgaName, *this );

  // now add the country line if needed (formatting this time according to
  // the rules of our own system country )
  if ( !country().isEmpty() ) {
    KConfig entry( KStandardDirs::locate( "locale", QString( "l10n/" ) +
                                          KGlobal::locale()->country() +
                                          QString( "/entry.desktop" ) ) );
    KConfigGroup group = entry.group( "KCM Locale" );
    QString cpos = group.readEntry( "AddressCountryPosition" );
    if ( "BELOW" == cpos || cpos.isEmpty() ) {
      ret = ret + "\n\n" + country().toUpper();
    } else if ( "below" == cpos ) {
      ret = ret + "\n\n" + country();
    } else if ( "ABOVE" == cpos ) {
      ret = country().toUpper() + "\n\n" + ret;
    } else if ( "above" == cpos ) {
      ret = country() + "\n\n" + ret;
    }
  }

  return ret;
}

QString Address::countryToISO( const QString &cname )
{
  // we search a map file for translations from country names to
  // iso codes, storing caching things in a QMap for faster future
  // access.
  typedef QMap<QString, QString> stringMap;
  K_GLOBAL_STATIC( stringMap, sISOMap )

  QMap<QString, QString>::ConstIterator it;
  it = sISOMap->find( cname );
  if ( it != sISOMap->end() ) {
    return it.value();
  }

  QString mapfile = KGlobal::dirs()->findResource( "data",
          QLatin1String( "kabc/countrytransl.map" ) );

  QFile file( mapfile );
  if ( file.open( QIODevice::ReadOnly ) ) {
    QTextStream s( &file );
    QString strbuf = s.readLine();
    while ( !strbuf.isEmpty() ) {
      QStringList countryInfo = strbuf.split( '\t', QString::KeepEmptyParts );
      if ( countryInfo[ 0 ] == cname ) {
        file.close();
        sISOMap->insert( cname, countryInfo[ 1 ] );
        return countryInfo[ 1 ];
      }
      strbuf = s.readLine();
    }
    file.close();
  }

  // fall back to system country
  sISOMap->insert( cname, KGlobal::locale()->country() );
  return KGlobal::locale()->country();
}

QString Address::ISOtoCountry( const QString &ISOname )
{
  // get country name from ISO country code (e.g. "no" -> i18n("Norway"))
  if ( ISOname.simplified().isEmpty() ) {
    return QString();
  }

  QString mapfile = KGlobal::dirs()->findResource( "data",
          QLatin1String( "kabc/countrytransl.map" ) );

  QFile file( mapfile );
  if ( file.open( QIODevice::ReadOnly ) ) {
    QTextStream s( &file );
    QString searchStr = '\t' + ISOname.simplified().toLower();
    QString strbuf = s.readLine();
    int pos;
    while ( !strbuf.isEmpty() ) {
      if ( ( pos = strbuf.indexOf( searchStr ) ) != -1 ) {
        file.close();
        return i18n( strbuf.left( pos ).toUtf8() );
      }
      strbuf = s.readLine();
    }
    file.close();
  }

  return ISOname;
}

QDataStream &KABC::operator<<( QDataStream &s, const Address &addr )
{
  return s << addr.d->mId << (uint)addr.d->mType << addr.d->mPostOfficeBox
           << addr.d->mExtended << addr.d->mStreet << addr.d->mLocality
           << addr.d->mRegion << addr.d->mPostalCode << addr.d->mCountry
           << addr.d->mLabel << addr.d->mEmpty;
}

QDataStream &KABC::operator>>( QDataStream &s, Address &addr )
{
  uint type;
  s >> addr.d->mId >> type >> addr.d->mPostOfficeBox >> addr.d->mExtended
    >> addr.d->mStreet >> addr.d->mLocality >> addr.d->mRegion
    >> addr.d->mPostalCode >> addr.d->mCountry >> addr.d->mLabel
    >> addr.d->mEmpty;

  addr.d->mType = Address::Type( type );

  return s;
}
