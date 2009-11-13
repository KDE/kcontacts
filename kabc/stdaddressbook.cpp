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
#include <kconfiggroup.h>

 #include <QCoreApplication>

#include <stdlib.h>

using namespace KABC;

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
    static bool mAutomaticSave;
};

static StdAddressBook *s_gStdAddressBook = 0;
bool StdAddressBook::Private::mAutomaticSave = true;

static void deleteGlobalStdAddressBook()
{
  if ( s_gStdAddressBook ) {
    delete s_gStdAddressBook;
    s_gStdAddressBook = 0;
  }
}

QString StdAddressBook::fileName()
{
  return KStandardDirs::locateLocal( "data", QLatin1String( "kabc/std.vcf" ) );
}

QString StdAddressBook::directoryName()
{
  return KStandardDirs::locateLocal( "data", QLatin1String( "kabc/stdvcf" ) );
}

StdAddressBook *StdAddressBook::self()
{
  kDebug();

  // delegate to other self() method since the only difference
  // was the constructor being used and their only difference is
  // what they pass to Private::init()
  return self( false );
}

StdAddressBook *StdAddressBook::self( bool asynchronous )
{
  kDebug() << "asynchronous=" << asynchronous;

  if ( !s_gStdAddressBook ) {
    s_gStdAddressBook = new StdAddressBook( asynchronous, false );

    kDebug() << "calling init after instance creation";
    s_gStdAddressBook->d->init( asynchronous );

    // We don't use a global static here for this reason:
    //
    // There are problems with the destruction order: The destructor of
    // StdAddressBook calls save(), which for LDAP address books, needs KIO
    // (more specific: KProtocolInfo) to be still alive. However, with a global
    // static, KProtocolInfo is already deleted, and the app will crash.
    //
    // qAddPostRoutine deletes the objects when the QApplication is destroyed,
    // which is earlier than the global statics, so this will work.
    qAddPostRoutine( deleteGlobalStdAddressBook );
  }

  return s_gStdAddressBook;
}

StdAddressBook::StdAddressBook()
  : AddressBook( QString() ), d( new Private( this ) )
{
  kDebug();

  d->init( false );
}

StdAddressBook::StdAddressBook( bool asynchronous )
  : AddressBook( QString() ), d( new Private( this ) )
{
  kDebug();

  d->init( asynchronous );
}

StdAddressBook::StdAddressBook( bool asynchronous, bool doInit )
  : AddressBook( QString() ), d( new Private( this ) )
{
  kDebug();

  if ( doInit ) {
    d->init( asynchronous );
  }
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
      mParent->error( i18n( "Unable to open resource '%1'." ,(*it)->resourceName() ) );
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
    res = manager->createResource( QLatin1String( "file" ) );
    if ( res ) {
      res->setResourceName( i18n( "Default Address Book" ) );
      mParent->addResource( res );
    } else {
      kDebug() << "No resource available!!!";
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
  kDebug();
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
  kDebug();

  if ( s_gStdAddressBook ) {
    return s_gStdAddressBook->d->saveAll();
  } else {
    return true;
  }
}

void StdAddressBook::close()
{
  delete s_gStdAddressBook;
  s_gStdAddressBook = 0;
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
  KConfig _config( QLatin1String( "kabcrc" ) );
  KConfigGroup config(&_config, "General" );

  return findByUid( config.readEntry( "WhoAmI" ) );
}

void StdAddressBook::setWhoAmI( const Addressee &addr )
{
  KConfig _config( QLatin1String( "kabcrc" ) );
  KConfigGroup config(&_config, "General" );

  config.writeEntry( "WhoAmI", addr.uid() );
}
