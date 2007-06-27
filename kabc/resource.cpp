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

#include "resource.h"

#include <kdebug.h>
#include <klocale.h>

using namespace KABC;

class Ticket::Private
{
  public:
    Private( Resource *resource )
      : mResource( resource )
    {
    }

    Resource *mResource;
};

Ticket::Ticket( Resource *resource )
  : d( new Private( resource ) )
{
}

Ticket::~Ticket()
{
  delete d;
}

Resource *Ticket::resource()
{
  return d->mResource;
}

class Resource::Iterator::Private
{
  public:
    Addressee::Map::Iterator mIt;
};

class Resource::ConstIterator::Private
{
  public:
    Addressee::Map::ConstIterator mIt;
};

Resource::Iterator::Iterator()
  : d( new Private )
{
}

Resource::Iterator::Iterator( const Resource::Iterator &other )
  : d( new Private )
{
  d->mIt = other.d->mIt;
}

Resource::Iterator &Resource::Iterator::operator=( const Resource::Iterator &other )
{
  if ( this != &other ) {
    d->mIt = other.d->mIt;
  }

  return *this;
}

Resource::Iterator::~Iterator()
{
  delete d;
}

const Addressee &Resource::Iterator::operator*() const
{
  return d->mIt.value();
}

Addressee &Resource::Iterator::operator*()
{
  return d->mIt.value();
}

Resource::Iterator &Resource::Iterator::operator++()
{
  (d->mIt)++;
  return *this;
}

Resource::Iterator &Resource::Iterator::operator++( int )
{
  (d->mIt)++;
  return *this;
}

Resource::Iterator &Resource::Iterator::operator--()
{
  (d->mIt)--;
  return *this;
}

Resource::Iterator &Resource::Iterator::operator--( int )
{
  (d->mIt)--;
  return *this;
}

bool Resource::Iterator::operator==( const Iterator &it ) const
{
  return d->mIt == it.d->mIt;
}

bool Resource::Iterator::operator!=( const Iterator &it ) const
{
  return d->mIt != it.d->mIt;
}

Resource::ConstIterator::ConstIterator()
  : d( new Private )
{
}

Resource::ConstIterator::ConstIterator( const Resource::ConstIterator &other )
  : d( new Private )
{
  d->mIt = other.d->mIt;
}

Resource::ConstIterator::ConstIterator( const Resource::Iterator &other )
  : d( new Private )
{
  d->mIt = other.d->mIt;
}

Resource::ConstIterator &Resource::ConstIterator::operator=( const Resource::ConstIterator &other )
{
  if ( this != &other ) {
    d->mIt = other.d->mIt;
  }

  return *this;
}

Resource::ConstIterator::~ConstIterator()
{
  delete d;
}

const Addressee &Resource::ConstIterator::operator*() const
{
  return *(d->mIt);
}

Resource::ConstIterator &Resource::ConstIterator::operator++()
{
  (d->mIt)++;
  return *this;
}

Resource::ConstIterator &Resource::ConstIterator::operator++( int )
{
  (d->mIt)++;
  return *this;
}

Resource::ConstIterator &Resource::ConstIterator::operator--()
{
  --(d->mIt);
  return *this;
}

Resource::ConstIterator &Resource::ConstIterator::operator--( int )
{
  --(d->mIt);
  return *this;
}

bool Resource::ConstIterator::operator==( const ConstIterator &it ) const
{
  return d->mIt == it.d->mIt;
}

bool Resource::ConstIterator::operator!=( const ConstIterator &it ) const
{
  return d->mIt != it.d->mIt;
}

class Resource::Private
{
  public:
    Private()
      : mAddressBook( 0 )
    {
    }

    AddressBook *mAddressBook;
};

Resource::Resource()
  : KRES::Resource(), d( new Private )
{
}

Resource::Resource( const KConfigGroup &group )
  : KRES::Resource( group ), d( new Private )
{
}

Resource::~Resource()
{
  delete d;
}

Resource::Iterator Resource::begin()
{
  Iterator it;
  it.d->mIt = mAddrMap.begin();

  return it;
}

Resource::ConstIterator Resource::begin() const
{
  ConstIterator it;
  it.d->mIt = mAddrMap.constBegin();
  return it;
}

Resource::Iterator Resource::end()
{
  Iterator it;
  it.d->mIt = mAddrMap.end();

  return it;
}

Resource::ConstIterator Resource::end() const
{
  ConstIterator it;
  it.d->mIt = mAddrMap.constEnd();
  return it;
}

void Resource::writeConfig( KConfigGroup &group )
{
  KRES::Resource::writeConfig( group );
}

void Resource::setAddressBook( AddressBook *ab )
{
  d->mAddressBook = ab;
}

AddressBook *Resource::addressBook()
{
  return d->mAddressBook;
}

Ticket *Resource::createTicket( Resource *resource )
{
  return new Ticket( resource );
}

void Resource::insertAddressee( const Addressee &addr )
{
  mAddrMap.insert( addr.uid(), addr );
}

void Resource::removeAddressee( const Addressee &addr )
{
  mAddrMap.remove( addr.uid() );
}

Addressee Resource::findByUid( const QString &uid )
{
  Addressee::Map::ConstIterator it = mAddrMap.find( uid );

  if ( it != mAddrMap.end() ) {
    return it.value();
  }

  return Addressee();
}

Addressee::List Resource::findByName( const QString &name )
{
  Addressee::List results;

  ConstIterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( name == (*it).name() ) {
      results.append( *it );
    }
  }

  return results;
}

Addressee::List Resource::findByEmail( const QString &email )
{
  Addressee::List results;
  const QString lowerEmail = email.toLower();

  ConstIterator it;
  for ( it = begin(); it != end(); ++it ) {
    const QStringList mailList = (*it).emails();
    for ( QStringList::ConstIterator ite = mailList.begin(); ite != mailList.end(); ++ite ) {
      if ( lowerEmail == (*ite).toLower() ) {
        results.append( *it );
      }
    }
  }

  return results;
}

Addressee::List Resource::findByCategory( const QString &category )
{
  Addressee::List results;

  ConstIterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( (*it).hasCategory( category ) ) {
      results.append( *it );
    }
  }

  return results;
}

void Resource::clear()
{
  mAddrMap.clear();
}

bool Resource::asyncLoad()
{
  bool ok = load();
  if ( !ok ) {
    emit loadingError( this, i18n( "Loading resource '%1' failed.", resourceName() ) );
  } else {
    emit loadingFinished( this );
  }

  return ok;
}

bool Resource::asyncSave( Ticket *ticket )
{
  bool ok = save( ticket );
  if ( !ok ) {
    emit savingError( this, i18n( "Saving resource '%1' failed.", resourceName() ) );
  } else {
    emit savingFinished( this );
  }

  return ok;
}

#include "resource.moc"
