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

#include "stdaddressbook.h"
#include "resource.h"

#include "kresources/manager.h"

#include <kdebug.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstandarddirs.h>
#include <k3staticdeleter.h>
#include <kconfiggroup.h>

#include <stdlib.h>

using namespace KABC;

static K3StaticDeleter<StdAddressBook> addressBookDeleter;

class StdAddressBook::Private
{
  public:
    Private( StdAddressBook *parent )
      : mParent( parent )
    {
    }

    void init( bool asynchronous );
    bool saveAll();

    StdAddressBook *mParent;
    static StdAddressBook *mSelf;
    static bool mAutomaticSave;
};

StdAddressBook *StdAddressBook::Private::mSelf = 0;
bool StdAddressBook::Private::mAutomaticSave = true;

QString StdAddressBook::fileName()
{
  return KStandardDirs::locateLocal( "data", "kabc/std.vcf" );
}

QString StdAddressBook::directoryName()
{
  return KStandardDirs::locateLocal( "data", "kabc/stdvcf" );
}

StdAddressBook *StdAddressBook::self()
{
  kDebug(5700);

  if ( !Private::mSelf ) {
    addressBookDeleter.setObject( Private::mSelf, new StdAddressBook );
  }

  return Private::mSelf;
}

StdAddressBook *StdAddressBook::self( bool asynchronous )
{
  kDebug(5700);

  if ( !Private::mSelf ) {
    addressBookDeleter.setObject( Private::mSelf, new StdAddressBook( asynchronous ) );
  }

  return Private::mSelf;
}

StdAddressBook::StdAddressBook()
  : AddressBook( "" ), d( new Private( this ) )
{
  kDebug(5700);

  d->init( false );
}

StdAddressBook::StdAddressBook( bool asynchronous )
  : AddressBook( "" ), d( new Private( this ) )
{
  kDebug(5700);

  d->init( asynchronous );
}

StdAddressBook::~StdAddressBook()
{
  if ( Private::mAutomaticSave ) {
    d->saveAll();
  }

  delete d;
}

void StdAddressBook::Private::init( bool asynchronous )
{
  KRES::Manager<Resource> *manager = mParent->resourceManager();

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = manager->activeBegin(); it != manager->activeEnd(); ++it ) {
    (*it)->setAddressBook( mParent );
    if ( !(*it)->open() ) {
      mParent->error( QString( "Unable to open resource '%1'!" ).arg( (*it)->resourceName() ) );
      continue;
    }
    mParent->connect( *it, SIGNAL( loadingFinished( Resource* ) ),
                      mParent, SLOT( resourceLoadingFinished( Resource* ) ) );
    mParent->connect( *it, SIGNAL( savingFinished( Resource* ) ),
                      mParent, SLOT( resourceSavingFinished( Resource* ) ) );

    mParent->connect( *it, SIGNAL( loadingError( Resource*, const QString& ) ),
                      mParent, SLOT( resourceLoadingError( Resource*, const QString& ) ) );
    mParent->connect( *it, SIGNAL( savingError( Resource*, const QString& ) ),
                      mParent, SLOT( resourceSavingError( Resource*, const QString& ) ) );
  }

  Resource *res = mParent->standardResource();
  if ( !res ) {
    res = manager->createResource( "file" );
    if ( res ) {
      mParent->addResource( res );
    } else {
      kDebug(5700) << "No resource available!!!";
    }
  }

  mParent->setStandardResource( res );
  manager->writeConfig();

  if ( asynchronous ) {
    mParent->asyncLoad();
  } else {
    mParent->load();
  }
}

bool StdAddressBook::Private::saveAll()
{
  kDebug(5700);
  bool ok = true;

  KRES::Manager<Resource>::ActiveIterator it;
  KRES::Manager<Resource> *manager = mParent->resourceManager();
  for ( it = manager->activeBegin(); it != manager->activeEnd(); ++it ) {
    if ( !(*it)->readOnly() && (*it)->isOpen() ) {
      Ticket *ticket = mParent->requestSaveTicket( *it );
      if ( !ticket ) {
        mParent->error( i18n( "Unable to save to resource '%1'. It is locked.",
                              (*it)->resourceName() ) );
        return false;
      }

      if ( !mParent->AddressBook::save( ticket ) ) {
        ok = false;
        mParent->releaseSaveTicket( ticket );
      }
    }
  }

  return ok;
}

bool StdAddressBook::save()
{
  kDebug(5700);

  if ( Private::mSelf ) {
    return Private::mSelf->d->saveAll();
  } else {
    return true;
  }
}

void StdAddressBook::close()
{
  addressBookDeleter.destructObject();
}

void StdAddressBook::setAutomaticSave( bool enable )
{
  Private::mAutomaticSave = enable;
}

bool StdAddressBook::automaticSave()
{
  return Private::mAutomaticSave;
}

Addressee StdAddressBook::whoAmI() const
{
  KConfig _config( "kabcrc" );
  KConfigGroup config(&_config, "General" );

  return findByUid( config.readEntry( "WhoAmI" ) );
}

void StdAddressBook::setWhoAmI( const Addressee &addr )
{
  KConfig _config( "kabcrc" );
  KConfigGroup config(&_config, "General" );

  config.writeEntry( "WhoAmI", addr.uid() );
}
