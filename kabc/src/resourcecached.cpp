/*
    This file is part of kabc.
    Copyright (c) 2004 Tobias Koenig <tokoe@kde.org>

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

#include "resourcecached.h"

#include <kabc/vcardconverter.h>

#include <qdebug.h>
#include <klocalizedstring.h>
#include <kstandarddirs.h>

#include <QtCore/QFile>

using namespace KABC;

class ResourceCached::Private
{
  public:
    Private( ResourceCached *parent )
      : mParent( parent ), mIdMapper( QLatin1String( "kabc/uidmaps/" ) )
    {
    }

    void loadChangesCache( QMap<QString, KABC::Addressee> &map, const QString &type );
    void saveChangesCache( const QMap<QString, KABC::Addressee> &map, const QString &type );

    ResourceCached *mParent;
    KRES::IdMapper mIdMapper;

    QMap<QString, KABC::Addressee> mAddedAddressees;
    QMap<QString, KABC::Addressee> mChangedAddressees;
    QMap<QString, KABC::Addressee> mDeletedAddressees;
};

void ResourceCached::Private::saveChangesCache( const QMap<QString, KABC::Addressee> &map,
                                                const QString &type )
{
  QFile file( mParent->changesCacheFile( type ) );

  const KABC::Addressee::List list = map.values();
  if ( list.isEmpty() ) {
    file.remove();
  } else {
    if ( !file.open( QIODevice::WriteOnly ) ) {
      kError( 5700 ) << "Can't open changes cache file '" << file.fileName() << "' for saving.";
      return;
    }

    KABC::VCardConverter converter;
    const QByteArray vCards = converter.createVCards( list );
    file.write( vCards );
  }
}

void ResourceCached::Private::loadChangesCache( QMap<QString, KABC::Addressee> &map,
                                                const QString &type )
{
  QFile file( mParent->changesCacheFile( type ) );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    return;
  }

  KABC::VCardConverter converter;

  const KABC::Addressee::List list = converter.parseVCards( file.readAll() );
  KABC::Addressee::List::ConstIterator it;
  KABC::Addressee::List::ConstIterator end( list.end() );
  for ( it = list.begin(); it != end; ++it ) {
    map.insert( ( *it ).uid(), *it );
  }

  file.close();
}

ResourceCached::ResourceCached()
  : KABC::Resource(), d( new Private( this ) )
{
}

ResourceCached::ResourceCached( const KConfigGroup &group )
  : KABC::Resource( group ), d( new Private( this ) )
{
}

ResourceCached::~ResourceCached()
{
  delete d;
}

void ResourceCached::writeConfig( KConfigGroup &group )
{
  KABC::Resource::writeConfig( group );
}

void ResourceCached::insertAddressee( const Addressee &addr )
{
  if ( !mAddrMap.contains( addr.uid() ) ) { // new contact
    if ( d->mDeletedAddressees.contains( addr.uid() ) ) {
      // it was first removed, then added, so it's an update...
      d->mDeletedAddressees.remove( addr.uid() );

      mAddrMap.insert( addr.uid(), addr );
      d->mChangedAddressees.insert( addr.uid(), addr );
      return;
    }

    mAddrMap.insert( addr.uid(), addr );
    d->mAddedAddressees.insert( addr.uid(), addr );
  } else {
    KABC::Addressee oldAddressee = mAddrMap.find( addr.uid() ).value();
    if ( oldAddressee != addr ) {
      mAddrMap.remove( addr.uid() );
      mAddrMap.insert( addr.uid(), addr );
      d->mChangedAddressees.insert( addr.uid(), addr );
    }
  }
}

void ResourceCached::removeAddressee( const Addressee &addr )
{
  if ( d->mAddedAddressees.contains( addr.uid() ) ) {
    d->mAddedAddressees.remove( addr.uid() );
    return;
  }

  if ( d->mDeletedAddressees.find( addr.uid() ) == d->mDeletedAddressees.end() ) {
    d->mDeletedAddressees.insert( addr.uid(), addr );
  }

  mAddrMap.remove( addr.uid() );
}

bool ResourceCached::loadFromCache()
{
  mAddrMap.clear();

  setIdMapperIdentifier();
  d->mIdMapper.load();

  // load cache
  QFile file( cacheFile() );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    return false;
  }

  KABC::VCardConverter converter;
  KABC::Addressee::List list = converter.parseVCards( file.readAll() );
  KABC::Addressee::List::Iterator it;

  for ( it = list.begin(); it != list.end(); ++it ) {
    ( *it ).setResource( this );
    ( *it ).setChanged( false );
    mAddrMap.insert( ( *it ).uid(), *it );
  }

  file.close();
  return true;
}

void ResourceCached::saveToCache()
{
  setIdMapperIdentifier();
  d->mIdMapper.save();

  // save cache
  QFile file( cacheFile() );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    return;
  }

  KABC::Addressee::List list = mAddrMap.values();

  KABC::VCardConverter converter;
  QByteArray vCard = converter.createVCards( list );
  file.write( vCard );
  file.close();
}

void ResourceCached::cleanUpCache( const KABC::Addressee::List &addrList )
{
  // load cache
  QFile file( cacheFile() );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    return;
  }

  KABC::VCardConverter converter;
  KABC::Addressee::List list = converter.parseVCards( file.readAll() );
  KABC::Addressee::List::Iterator cacheIt;
  KABC::Addressee::List::ConstIterator it;

  for ( cacheIt = list.begin(); cacheIt != list.end(); ++cacheIt ) {
    bool found = false;
    for ( it = addrList.begin(); it != addrList.end(); ++it ) {
      if ( ( *it ).uid() == ( *cacheIt ).uid() ) {
        found = true;
      }
    }

    if ( !found ) {
      d->mIdMapper.removeRemoteId( d->mIdMapper.remoteId( ( *cacheIt ).uid() ) );
      mAddrMap.remove( ( *cacheIt ).uid() );
    }
  }

  file.close();
}

KRES::IdMapper &ResourceCached::idMapper()
{
  return d->mIdMapper;
}

bool ResourceCached::hasChanges() const
{
  return !( d->mAddedAddressees.isEmpty() &&
            d->mChangedAddressees.isEmpty() &&
            d->mDeletedAddressees.isEmpty() );
}

void ResourceCached::clearChanges()
{
  d->mAddedAddressees.clear();
  d->mChangedAddressees.clear();
  d->mDeletedAddressees.clear();
}

void ResourceCached::clearChange( const KABC::Addressee &addr )
{
  d->mAddedAddressees.remove( addr.uid() );
  d->mChangedAddressees.remove( addr.uid() );
  d->mDeletedAddressees.remove( addr.uid() );
}

void ResourceCached::clearChange( const QString &uid )
{
  d->mAddedAddressees.remove( uid );
  d->mChangedAddressees.remove( uid );
  d->mDeletedAddressees.remove( uid );
}

KABC::Addressee::List ResourceCached::addedAddressees() const
{
  return d->mAddedAddressees.values();
}

KABC::Addressee::List ResourceCached::changedAddressees() const
{
  return d->mChangedAddressees.values();
}

KABC::Addressee::List ResourceCached::deletedAddressees() const
{
  return d->mDeletedAddressees.values();
}

QString ResourceCached::cacheFile() const
{
  return KStandardDirs::locateLocal( "cache", QLatin1String( "kabc/kresources/" ) + identifier() );
}

QString ResourceCached::changesCacheFile( const QString &type ) const
{
  return KStandardDirs::locateLocal( "cache", QLatin1String( "kabc/changescache/" ) + identifier() +
                                     QLatin1Char( '_' ) + type );
}

void ResourceCached::saveChangesCache()
{
  d->saveChangesCache( d->mAddedAddressees, QLatin1String( "added" ) );
  d->saveChangesCache( d->mDeletedAddressees, QLatin1String( "deleted" ) );
  d->saveChangesCache( d->mChangedAddressees, QLatin1String( "changed" ) );
}

void ResourceCached::loadChangesCache()
{
  d->loadChangesCache( d->mAddedAddressees, QLatin1String( "added" ) );
  d->loadChangesCache( d->mDeletedAddressees, QLatin1String( "deleted" ) );
  d->loadChangesCache( d->mChangedAddressees, QLatin1String( "changed" ) );
}

void ResourceCached::setIdMapperIdentifier()
{
  d->mIdMapper.setIdentifier( type() + QLatin1Char( '_' ) + identifier() );
}

