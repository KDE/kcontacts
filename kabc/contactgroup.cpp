/*
  This file is part of libkabc.
  Copyright (c) 2008 Tobias Koenig <tokoe@kde.org>

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

#include "contactgroup.h"

#include <QtCore/QMap>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtCore/QUuid>

using namespace KABC;

class ContactGroup::ContactReference::ContactReferencePrivate : public QSharedData
{
  public:
    ContactReferencePrivate()
      : QSharedData()
    {
    }

    ContactReferencePrivate( const ContactReferencePrivate &other )
      : QSharedData( other )
    {
      mUid = other.mUid;
      mPreferredEmail = other.mPreferredEmail;
      mCustoms = other.mCustoms;
    }

    QString mUid;
    QString mPreferredEmail;
    QMap<QString, QString> mCustoms;
};

ContactGroup::ContactReference::ContactReference()
  : d( new ContactReferencePrivate )
{
}

ContactGroup::ContactReference::ContactReference( const ContactReference &other )
  : d( other.d )
{
}

ContactGroup::ContactReference::ContactReference( const QString &uid )
  : d( new ContactReferencePrivate )
{
  d->mUid = uid;
}

ContactGroup::ContactReference::~ContactReference()
{
}

void ContactGroup::ContactReference::setUid( const QString &uid )
{
  d->mUid = uid;
}

QString ContactGroup::ContactReference::uid() const
{
  return d->mUid;
}

void ContactGroup::ContactReference::setPreferredEmail( const QString &email )
{
  d->mPreferredEmail = email;
}

QString ContactGroup::ContactReference::preferredEmail() const
{
  return d->mPreferredEmail;
}

void ContactGroup::ContactReference::insertCustom( const QString &key, const QString &value )
{
  d->mCustoms.insert( key, value );
}

void ContactGroup::ContactReference::removeCustom( const QString &key )
{
  d->mCustoms.remove( key );
}

QString ContactGroup::ContactReference::custom( const QString &key ) const
{
  return d->mCustoms.value( key );
}

ContactGroup::ContactReference &ContactGroup::ContactReference::operator=(
  const ContactGroup::ContactReference &other )
{
  if ( this != &other ) {
    d = other.d;
  }

  return *this;
}

bool ContactGroup::ContactReference::operator==( const ContactReference &other ) const
{
  return d->mUid == other.d->mUid &&
    d->mPreferredEmail == other.d->mPreferredEmail &&
    d->mCustoms == other.d->mCustoms;
}

class ContactGroup::ContactGroupReference::ContactGroupReferencePrivate : public QSharedData
{
  public:
    ContactGroupReferencePrivate()
      : QSharedData()
    {
    }

    ContactGroupReferencePrivate( const ContactGroupReferencePrivate &other )
      : QSharedData( other )
    {
      mUid = other.mUid;
      mCustoms = other.mCustoms;
    }

    QString mUid;
    QMap<QString, QString> mCustoms;
};

ContactGroup::ContactGroupReference::ContactGroupReference()
  : d( new ContactGroupReferencePrivate )
{
}

ContactGroup::ContactGroupReference::ContactGroupReference( const ContactGroupReference &other )
  : d( other.d )
{
}

ContactGroup::ContactGroupReference::ContactGroupReference( const QString &uid )
  : d( new ContactGroupReferencePrivate )
{
  d->mUid = uid;
}

ContactGroup::ContactGroupReference::~ContactGroupReference()
{
}

void ContactGroup::ContactGroupReference::setUid( const QString &uid )
{
  d->mUid = uid;
}

QString ContactGroup::ContactGroupReference::uid() const
{
  return d->mUid;
}

void ContactGroup::ContactGroupReference::insertCustom( const QString &key, const QString &value )
{
  d->mCustoms.insert( key, value );
}

void ContactGroup::ContactGroupReference::removeCustom( const QString &key )
{
  d->mCustoms.remove( key );
}

QString ContactGroup::ContactGroupReference::custom( const QString &key ) const
{
  return d->mCustoms.value( key );
}

ContactGroup::ContactGroupReference &ContactGroup::ContactGroupReference::operator=(
  const ContactGroup::ContactGroupReference &other )
{
  if ( this != &other ) {
    d = other.d;
  }

  return *this;
}

bool ContactGroup::ContactGroupReference::operator==( const ContactGroupReference &other ) const
{
  return d->mUid == other.d->mUid &&
    d->mCustoms == other.d->mCustoms;
}

class ContactGroup::Data::DataPrivate : public QSharedData
{
  public:
    DataPrivate()
      : QSharedData()
    {
    }

    DataPrivate( const DataPrivate &other )
      : QSharedData( other )
    {
      mName = other.mName;
      mEmail = other.mEmail;
      mCustoms = other.mCustoms;
    }

    QString mName;
    QString mEmail;
    QMap<QString, QString> mCustoms;
};

ContactGroup::Data::Data()
  : d( new DataPrivate )
{
}

ContactGroup::Data::Data( const Data &other )
  : d( other.d )
{
}

ContactGroup::Data::Data( const QString &name, const QString &email )
  : d( new DataPrivate )
{
  d->mName = name;
  d->mEmail = email;
}

ContactGroup::Data::~Data()
{
}

void ContactGroup::Data::setName( const QString &name )
{
  d->mName = name;
}

QString ContactGroup::Data::name() const
{
  return d->mName;
}

void ContactGroup::Data::setEmail( const QString &email )
{
  d->mEmail = email;
}

QString ContactGroup::Data::email() const
{
  return d->mEmail;
}

void ContactGroup::Data::insertCustom( const QString &key, const QString &value )
{
  d->mCustoms.insert( key, value );
}

void ContactGroup::Data::removeCustom( const QString &key )
{
  d->mCustoms.remove( key );
}

QString ContactGroup::Data::custom( const QString &key ) const
{
  return d->mCustoms.value( key );
}

ContactGroup::Data &ContactGroup::Data::operator=( const ContactGroup::Data &other )
{
  if ( this != &other ) {
    d = other.d;
  }

  return *this;
}

bool ContactGroup::Data::operator==( const Data &other ) const
{
  return d->mName == other.d->mName &&
    d->mEmail == other.d->mEmail &&
    d->mCustoms == other.d->mCustoms;
}

class ContactGroup::Private : public QSharedData
{
  public:
    Private()
      : QSharedData(),
        mIdentifier( QUuid::createUuid().toString() )
    {
    }

    Private( const Private &other )
      : QSharedData( other )
    {
      mIdentifier = other.mIdentifier;
      mName = other.mName;
      mContactReferences = other.mContactReferences;
      mContactGroupReferences = other.mContactGroupReferences;
      mDataObjects = other.mDataObjects;
    }

    QString mIdentifier;
    QString mName;
    ContactGroup::ContactReference::List mContactReferences;
    ContactGroup::ContactGroupReference::List mContactGroupReferences;
    ContactGroup::Data::List mDataObjects;
};

ContactGroup::ContactGroup()
  : d( new Private )
{
}

ContactGroup::ContactGroup( const ContactGroup &other )
  : d( other.d )
{
}

ContactGroup::ContactGroup( const QString &name )
  : d( new Private )
{
  d->mName = name;
}

ContactGroup::~ContactGroup()
{
}

void ContactGroup::setName( const QString &name )
{
  d->mName = name;
}

QString ContactGroup::name() const
{
  return d->mName;
}

void ContactGroup::setId( const QString &id )
{
  d->mIdentifier = id;
}

QString ContactGroup::id() const
{
  return d->mIdentifier;
}

unsigned int ContactGroup::count() const
{
  return d->mContactReferences.count() + d->mDataObjects.count();
}

unsigned int ContactGroup::contactReferenceCount() const
{
  return d->mContactReferences.count();
}

unsigned int ContactGroup::contactGroupReferenceCount() const
{
  return d->mContactGroupReferences.count();
}

unsigned int ContactGroup::dataCount() const
{
  return d->mDataObjects.count();
}

ContactGroup::ContactReference &ContactGroup::contactReference( unsigned int index )
{
  Q_ASSERT_X( index < (unsigned int)d->mContactReferences.count(),
              "contactReference()", "index out of range" );

  return d->mContactReferences[ index ];
}

const ContactGroup::ContactReference &ContactGroup::contactReference( unsigned int index ) const
{
  Q_ASSERT_X( index < (unsigned int)d->mContactReferences.count(),
              "contactReference()", "index out of range" );

  return d->mContactReferences[ index ];
}

ContactGroup::ContactGroupReference &ContactGroup::contactGroupReference( unsigned int index )
{
  Q_ASSERT_X( index < (unsigned int)d->mContactGroupReferences.count(),
              "contactGroupReference()", "index out of range" );

  return d->mContactGroupReferences[ index ];
}

const ContactGroup::ContactGroupReference &ContactGroup::contactGroupReference(
  unsigned int index ) const
{
  Q_ASSERT_X( index < (unsigned int)d->mContactGroupReferences.count(),
              "contactGroupReference()", "index out of range" );

  return d->mContactGroupReferences[ index ];
}

ContactGroup::Data &ContactGroup::data( unsigned int index )
{
  Q_ASSERT_X( index < (unsigned int)d->mDataObjects.count(), "data()", "index out of range" );

  return d->mDataObjects[ index ];
}

const ContactGroup::Data &ContactGroup::data( unsigned int index ) const
{
  Q_ASSERT_X( index < (unsigned int)d->mDataObjects.count(), "data()", "index out of range" );

  return d->mDataObjects[ index ];
}

void ContactGroup::append( const ContactReference &reference )
{
  d->mContactReferences.append( reference );
}

void ContactGroup::append( const ContactGroupReference &reference )
{
  d->mContactGroupReferences.append( reference );
}

void ContactGroup::append( const Data &data )
{
  d->mDataObjects.append( data );
}

void ContactGroup::remove( const ContactReference &reference )
{
  d->mContactReferences.removeOne( reference );
}

void ContactGroup::remove( const ContactGroupReference &reference )
{
  d->mContactGroupReferences.removeOne( reference );
}

void ContactGroup::remove( const Data &data )
{
  d->mDataObjects.removeOne( data );
}

void ContactGroup::removeAllContactReferences()
{
  d->mContactReferences.clear();
}

void ContactGroup::removeAllContactGroupReferences()
{
  d->mContactGroupReferences.clear();
}

void ContactGroup::removeAllContactData()
{
  d->mDataObjects.clear();
}

ContactGroup &ContactGroup::operator=( const ContactGroup &other )
{
  if ( this != &other ) {
    d = other.d;
  }

  return *this;
}

bool ContactGroup::operator==( const ContactGroup &other ) const
{
  return d->mIdentifier == other.d->mIdentifier &&
    d->mName == other.d->mName &&
    d->mContactReferences == other.d->mContactReferences &&
    d->mContactGroupReferences == other.d->mContactGroupReferences &&
    d->mDataObjects == other.d->mDataObjects;
}

QString ContactGroup::mimeType()
{
  return QLatin1String( "application/x-vnd.kde.contactgroup" );
}
