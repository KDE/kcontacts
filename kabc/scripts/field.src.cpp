/*
    This file is part of libkabc.
    Copyright (c) 2002 Cornelius Schumacher <schumacher@kde.org>

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

#include <klocale.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kconfiggroup.h>

#include "field.h"

using namespace KABC;

class Field::Private
{
  public:
    Private( int fieldId, int category = 0,
             const QString &label = QString(),
             const QString &key = QString(),
             const QString &app = QString() )
      : mFieldId( fieldId ), mCategory( category ), mLabel( label ),
        mKey( key ), mApp( app ) {}

    enum FieldId
    {
      CustomField,
      --ENUMS--
    };

    int fieldId() { return mFieldId; }
    int category() { return mCategory; }

    QString label() { return mLabel; }
    QString key() { return mKey; }
    QString app() { return mApp; }

    static Field::List mAllFields;
    static Field::List mDefaultFields;
    static Field::List mCustomFields;

  private:
    int mFieldId;
    int mCategory;

    QString mLabel;
    QString mKey;
    QString mApp;
};

Field::List Field::Private::mAllFields;
Field::List Field::Private::mDefaultFields;
Field::List Field::Private::mCustomFields;

Field::Field( Private *p )
  : d( p )
{
}

Field::~Field()
{
  delete d;
}

QString Field::label()
{
  switch ( d->fieldId() ) {
    --CASELABEL--
    case Private::CustomField:
      return d->label();
    default:
      return i18n( "Unknown Field" );
  }
}

int Field::category()
{
  return d->category();
}

QString Field::categoryLabel( int category )
{
  switch ( category ) {
    case All:
      return i18n( "All" );
    case Frequent:
      return i18n( "Frequent" );
    case Address:
      return i18nc( "street/postal","Address" );
    case Email:
      return i18n( "Email" );
    case Personal:
      return i18n( "Personal" );
    case Organization:
      return i18n( "Organization" );
    case CustomCategory:
      return i18n( "Custom" );
    default:
      return i18n( "Undefined" );
  }
}

QString Field::value( const KABC::Addressee &a )
{
  switch ( d->fieldId() ) {
    --CASEVALUE--
    case Private::Email:
      return a.preferredEmail();
    case Private::Birthday:
      if ( a.birthday().isValid() )
        return a.birthday().date().toString( Qt::ISODate );
      else
        return QString();
    case Private::Url:
      return a.url().prettyUrl();
    case Private::HomePhone:
    {
      PhoneNumber::List::ConstIterator it;

      {
        // check for preferred number
        const PhoneNumber::List list = a.phoneNumbers( PhoneNumber::Home | PhoneNumber::Pref );
        for ( it = list.begin(); it != list.end(); ++it ) {
          if ( ( ( *it ).type() & ~( PhoneNumber::Pref ) ) == PhoneNumber::Home ) {
            return ( *it ).number();
          }
        }
      }

      {
        // check for normal home number
        const PhoneNumber::List list = a.phoneNumbers( PhoneNumber::Home );
        for ( it = list.begin(); it != list.end(); ++it ) {
          if ( ( ( *it ).type() & ~(PhoneNumber::Pref) ) == PhoneNumber::Home ) {
            return ( *it ).number();
          }
        }
      }

      return QString();
    }
    case Private::BusinessPhone:
    {
      PhoneNumber::List::ConstIterator it;

      {
        // check for preferred number
        const PhoneNumber::List list = a.phoneNumbers( PhoneNumber::Work | PhoneNumber::Pref );
        for ( it = list.begin(); it != list.end(); ++it ) {
          if ( ( ( *it ).type() & ~(PhoneNumber::Pref) ) == PhoneNumber::Work ) {
            return ( *it ).number();
          }
        }
      }

      {
        // check for normal work number
        const PhoneNumber::List list = a.phoneNumbers( PhoneNumber::Work );
        for ( it = list.begin(); it != list.end(); ++it ) {
          if ( ( ( *it ).type() & ~(PhoneNumber::Pref) ) == PhoneNumber::Work ) {
            return ( *it ).number();
          }
        }
      }

      return QString();
    }
    case Private::MobilePhone:
      return a.phoneNumber( PhoneNumber::Cell ).number();
    case Private::HomeFax:
      return a.phoneNumber( PhoneNumber::Home | PhoneNumber::Fax ).number();
    case Private::BusinessFax:
      return a.phoneNumber( PhoneNumber::Work | PhoneNumber::Fax ).number();
    case Private::CarPhone:
      return a.phoneNumber( PhoneNumber::Car ).number();
    case Private::Isdn:
      return a.phoneNumber( PhoneNumber::Isdn ).number();
    case Private::Pager:
      return a.phoneNumber( PhoneNumber::Pager ).number();
    case Private::HomeAddressStreet:
      return a.address( Address::Home ).street();
    case Private::HomeAddressPostOfficeBox:
      return a.address( Address::Home ).postOfficeBox();
    case Private::HomeAddressLocality:
      return a.address( Address::Home ).locality();
    case Private::HomeAddressRegion:
      return a.address( Address::Home ).region();
    case Private::HomeAddressPostalCode:
      return a.address( Address::Home ).postalCode();
    case Private::HomeAddressCountry:
      return a.address( Address::Home ).country();
    case Private::BusinessAddressStreet:
      return a.address( Address::Work ).street();
    case Private::BusinessAddressPostOfficeBox:
      return a.address( Address::Work ).postOfficeBox();
    case Private::BusinessAddressLocality:
      return a.address( Address::Work ).locality();
    case Private::BusinessAddressRegion:
      return a.address( Address::Work ).region();
    case Private::BusinessAddressPostalCode:
      return a.address( Address::Work ).postalCode();
    case Private::BusinessAddressCountry:
      return a.address( Address::Work ).country();
    case Private::CustomField:
      return a.custom( d->app(), d->key() );
    default:
      return QString();
  }
}

bool Field::setValue( KABC::Addressee &a, const QString &value )
{
  switch ( d->fieldId() ) {
    --CASESETVALUE--
    case Private::Birthday:
      a.setBirthday( QDateTime::fromString( value, Qt::ISODate ) );
      return true;
    case Private::CustomField:
      a.insertCustom( d->app(), d->key(), value );
      return true;
    default:
      return false;
  }
}

QString Field::sortKey( const KABC::Addressee &a )
{
  switch ( d->fieldId() ) {
    --CASEVALUE--
    case Private::Birthday:
      if ( a.birthday().isValid() ) {
        QDate date = a.birthday().date();
        QString key;
        key.sprintf( "%02d-%02d", date.month(), date.day() );
        return key;
      } else
        return QLatin1String( "00-00" );
    default:
      return value( a ).toLower();
  }
}

bool Field::isCustom()
{
  return d->fieldId() == Private::CustomField;
}

Field::List Field::allFields()
{
  if ( Private::mAllFields.isEmpty() ) {
    --CREATEFIELDS--
  }

  return Private::mAllFields;
}

Field::List Field::defaultFields()
{
  if ( Private::mDefaultFields.isEmpty() ) {
    createDefaultField( Private::FormattedName );
    createDefaultField( Private::Email );
  }

  return Private::mDefaultFields;
}

void Field::createField( int id, int category )
{
  Private::mAllFields.append( new Field( new Private( id, category ) ) );
}

void Field::createDefaultField( int id, int category )
{
  Private::mDefaultFields.append( new Field( new Private( id, category ) ) );
}

void Field::deleteFields()
{
  Field::List::ConstIterator it;

  for ( it = Private::mAllFields.constBegin(); it != Private::mAllFields.constEnd(); ++it ) {
    delete ( *it );
  }
  Private::mAllFields.clear();

  for ( it = Private::mDefaultFields.constBegin(); it != Private::mDefaultFields.constEnd(); ++it ) {
    delete ( *it );
  }
  Private::mDefaultFields.clear();

  for ( it = Private::mCustomFields.constBegin(); it != Private::mCustomFields.constEnd(); ++it ) {
    delete ( *it );
  }
  Private::mCustomFields.clear();
}

void Field::saveFields( const QString &identifier,
                        const Field::List &fields )
{
  KConfigGroup cg( KGlobal::config(), "KABCFields" );

  saveFields( cg, identifier, fields );
}

void Field::saveFields( KConfigGroup &cfg, const QString &identifier,
                        const Field::List &fields )
{
  QList<int> fieldIds;

  int custom = 0;
  Field::List::ConstIterator it;
  for ( it = fields.begin(); it != fields.end(); ++it ) {
    fieldIds.append( ( *it )->d->fieldId() );
    if ( ( *it )->isCustom() ) {
      QStringList customEntry;
      customEntry << ( *it )->d->label();
      customEntry << ( *it )->d->key();
      customEntry << ( *it )->d->app();
      cfg.writeEntry( QLatin1String( "KABC_CustomEntry_" ) + identifier + QLatin1Char( '_' ) +
                      QString::number( custom++ ), customEntry );
    }
  }

  cfg.writeEntry( identifier, fieldIds );
}

Field::List Field::restoreFields( const QString &identifier )
{
  KConfigGroup cg( KGlobal::config(), "KABCFields" );

  return restoreFields( cg, identifier );
}

Field::List Field::restoreFields( const KConfigGroup &cfg, const QString &identifier )
{
  const QList<int> fieldIds = cfg.readEntry( identifier,QList<int>() );

  Field::List fields;

  int custom = 0;
  QList<int>::ConstIterator it;
  for ( it = fieldIds.begin(); it != fieldIds.end(); ++it ) {
    Private *f = 0;
    if ( ( *it ) == Private::CustomField ) {
      QStringList customEntry = cfg.readEntry( QLatin1String( "KABC_CustomEntry_" ) +
                                               identifier + QLatin1Char( '_' ) +
                                               QString::number( custom++ ),QStringList() );
      f = new Private( *it, CustomCategory, customEntry[ 0 ],
                         customEntry[ 1 ], customEntry[ 2 ] );
    } else {
      f = new Private( *it );
    }
    fields.append( new Field( f ) );
  }

  return fields;
}

bool Field::equals( Field *field )
{
  bool sameId = ( d->fieldId() == field->d->fieldId() );

  if ( !sameId ) return false;

  if ( d->fieldId() != Private::CustomField ) return true;

  return d->key() == field->d->key();
}

Field *Field::createCustomField( const QString &label, int category,
                                 const QString &key, const QString &app )
{
  Field *field = new Field( new Private( Private::CustomField,
                                           category | CustomCategory,
                                           label, key, app ) );
  Private::mCustomFields.append( field );

  return field;
}
