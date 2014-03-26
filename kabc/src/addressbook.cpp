/*
    This file is part of libkabc.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General  Public
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

#include "addressbook.h"
#include "distributionlist.h"
#include "errorhandler.h"
#include "resource.h"

#include <kdebug.h>
#include <kglobal.h>
#include <kcomponentdata.h>
#include <klocalizedstring.h>
#include <kstandarddirs.h>


using namespace KABC;

class AddressBook::Private
{
  public:
    Field::List mAllFields;
    ErrorHandler *mErrorHandler;
    KConfig *mConfig;
    KRES::Manager<Resource> *mManager;
    QList<Resource*> mPendingLoadResources;
    QList<Resource*> mPendingSaveResources;
    Iterator end;
    ConstIterator constEnd;
};

struct AddressBook::Iterator::IteratorData
{
  Resource::Iterator mIt;
  QList<Resource*> mResources;
  int mCurrRes;
};

struct AddressBook::ConstIterator::ConstIteratorData
{
  Resource::ConstIterator mIt;
  QList<Resource*> mResources;
  int mCurrRes;
};

AddressBook::Iterator::Iterator()
  : d( new IteratorData )
{
}

AddressBook::Iterator::Iterator( const AddressBook::Iterator &i )
  : d( new IteratorData )
{
  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;
}

AddressBook::Iterator &AddressBook::Iterator::operator=
  ( const AddressBook::Iterator &i )
{
  if ( this == &i ) {
    return *this; // guard against self assignment
  }

  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;

  return *this;
}

AddressBook::Iterator::~Iterator()
{
  delete d;
}

const Addressee &AddressBook::Iterator::operator*() const
{
  return *( d->mIt );
}

Addressee &AddressBook::Iterator::operator*()
{
  return *( d->mIt );
}

Addressee *AddressBook::Iterator::operator->()
{
  return &( *( d->mIt ) );
}

AddressBook::Iterator &AddressBook::Iterator::operator++()
{
  do {
    bool jumped = false;
    while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->end() ) {
      // at end of addressee list of resource
      if ( d->mCurrRes == d->mResources.count() - 1 ) {
        return *this;
      }

      d->mCurrRes++; // jump to next resource

      jumped = true;
      d->mIt = ( d->mResources[ d->mCurrRes ] )->begin();
    }

    if ( !jumped ) {
      ( d->mIt )++;
    }

  } while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->end() );

  return *this;
}

AddressBook::Iterator &AddressBook::Iterator::operator++( int )
{
  do {
    bool jumped = false;
    while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->end() ) {
      // at end of addressee list of resource
      if ( d->mCurrRes == d->mResources.count() - 1 ) {
        return *this;
      }

      d->mCurrRes++; // jump to next resource

      jumped = true;
      d->mIt = ( d->mResources[ d->mCurrRes ] )->begin();
    }

    if ( !jumped ) {
      ( d->mIt )++;
    }

  } while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->end() );

  return *this;
}

AddressBook::Iterator &AddressBook::Iterator::operator--()
{
  ( d->mIt )--;

  return *this;
}

AddressBook::Iterator &AddressBook::Iterator::operator--( int )
{
  ( d->mIt )--;

  return *this;
}

bool AddressBook::Iterator::operator==( const Iterator &it ) const
{
  return d->mIt == it.d->mIt;
}

bool AddressBook::Iterator::operator!=( const Iterator &it ) const
{
  return d->mIt != it.d->mIt;
}

AddressBook::ConstIterator::ConstIterator()
  : d( new ConstIteratorData )
{
}

AddressBook::ConstIterator::ConstIterator( const AddressBook::ConstIterator &i )
  : d( new ConstIteratorData )
{
  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;
}

#ifndef QT_STRICT_ITERATORS
AddressBook::ConstIterator::ConstIterator( const AddressBook::Iterator &i )
  :d( new ConstIteratorData )
{
  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;
}
#endif

AddressBook::ConstIterator &AddressBook::ConstIterator::operator=
  ( const AddressBook::ConstIterator &i )
{
  if ( this  == &i ) {
    return *this; // guard for self assignment
  }

  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;

  return *this;
}

AddressBook::ConstIterator::~ConstIterator()
{
  delete d;
}

const Addressee &AddressBook::ConstIterator::operator*() const
{
  return *( d->mIt );
}

const Addressee *AddressBook::ConstIterator::operator->() const
{
  return &( *( d->mIt ) );
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator++()
{
  do {
    bool jumped = false;
    while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->constEnd() ) {
      // at end of addressee list of resource
      if ( d->mCurrRes == d->mResources.count() - 1 ) {
        return *this;
      }

      d->mCurrRes++; // jump to next resource

      jumped = true;
      d->mIt = ( d->mResources[ d->mCurrRes ] )->constBegin();
    }

    if ( !jumped ) {
      ( d->mIt )++;
    }

  } while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->constEnd() );

  return *this;
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator++(int)
{
  do {
    bool jumped = false;
    while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->constEnd() ) {
      // at end of addressee list of resource
      if ( d->mCurrRes == d->mResources.count() - 1 ) {
        return *this;
      }

      d->mCurrRes++; // jump to next resource

      jumped = true;
      d->mIt = ( d->mResources[ d->mCurrRes ] )->constBegin();
    }

    if ( !jumped ) {
      ( d->mIt )++;
    }

  } while ( d->mIt == ( d->mResources[ d->mCurrRes ] )->constEnd() );

  return *this;
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator--()
{
  ( d->mIt )--;
  return *this;
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator--(int)
{
  ( d->mIt )--;
  return *this;
}

bool AddressBook::ConstIterator::operator==( const ConstIterator &it ) const
{
  return d->mIt == it.d->mIt;
}

bool AddressBook::ConstIterator::operator!=( const ConstIterator &it ) const
{
  return d->mIt != it.d->mIt;
}

AddressBook::AddressBook()
  : d( new Private )
{
  d->mErrorHandler = 0;
  d->mConfig = 0;
  d->mManager = new KRES::Manager<Resource>( QLatin1String( "contact" ) );
  d->end.d->mResources = QList<Resource*>();
  d->end.d->mCurrRes = -1;
  d->constEnd.d->mResources = QList<Resource*>();
  d->constEnd.d->mCurrRes = -1;
}

AddressBook::AddressBook( const QString &config )
  : d( new Private )
{
  d->mErrorHandler = 0;
  if ( config.isEmpty() ) {
    d->mConfig = 0;
  } else {
    d->mConfig = new KConfig( config );
  }
  d->mManager = new KRES::Manager<Resource>( QLatin1String( "contact" ) );
  d->mManager->readConfig( d->mConfig );
  d->end.d->mResources = QList<Resource*>();
  d->end.d->mCurrRes = -1;
  d->constEnd.d->mResources = QList<Resource*>();
  d->constEnd.d->mCurrRes = -1;
}

AddressBook::~AddressBook()
{
  delete d->mManager;
  d->mManager = 0;
  delete d->mConfig;
  d->mConfig = 0;
  delete d->mErrorHandler;
  d->mErrorHandler = 0;
  delete d;
}

bool AddressBook::load()
{
  kDebug();

  clear();

  KRES::Manager<Resource>::ActiveIterator it;
  bool ok = true;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    if ( !( *it )->load() ) {
      error( i18n( "Unable to load resource '%1'", ( *it )->resourceName() ) );
      ok = false;
    }
  }

  return ok;
}

bool AddressBook::asyncLoad()
{
  kDebug();

  clear();

  KRES::Manager<Resource>::ActiveIterator it;
  bool ok = true;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    d->mPendingLoadResources.append( *it );
    if ( !( *it )->asyncLoad() ) {
      error( i18n( "Unable to load resource '%1'", ( *it )->resourceName() ) );
      ok = false;
    }
  }

  return ok;
}

bool AddressBook::save( Ticket *ticket )
{
  kDebug();

  if ( ticket->resource() ) {
    bool ok = ticket->resource()->save( ticket );
    if ( ok ) {
      ticket->resource()->releaseSaveTicket( ticket );
    }
    return ok;
  }

  return false;
}

bool AddressBook::asyncSave( Ticket *ticket )
{
  kDebug();

  if ( ticket->resource() ) {
    d->mPendingSaveResources.append( ticket->resource() );
    bool ok = ticket->resource()->asyncSave( ticket );
    if ( ok ) {
      ticket->resource()->releaseSaveTicket( ticket );
    }
    return ok;
  }

  return false;
}

AddressBook::Iterator AddressBook::begin()
{
  QList<Resource*> list;
  KRES::Manager<Resource>::ActiveIterator resIt;
  for ( resIt = d->mManager->activeBegin();
        resIt != d->mManager->activeEnd(); ++resIt ) {
    list.append( *resIt );
  }

  if ( list.count() == 0 ) {
    return end();
  }

  Iterator it = Iterator();
  it.d->mResources = list;
  it.d->mCurrRes = 0;
  it.d->mIt = ( it.d->mResources[ it.d->mCurrRes ] )->begin();

  while ( it.d->mIt == ( it.d->mResources[ it.d->mCurrRes ] )->end() ) {
    if ( it.d->mCurrRes == it.d->mResources.count() - 1 ) {
      return end();
    }

    it.d->mCurrRes++;

    it.d->mIt = ( it.d->mResources[ it.d->mCurrRes ] )->begin();
  }

  return it;
}

AddressBook::ConstIterator AddressBook::begin() const
{
  QList<Resource*> list;
  KRES::Manager<Resource>::ActiveIterator resIt;
  for ( resIt = d->mManager->activeBegin();
        resIt != d->mManager->activeEnd(); ++resIt ) {
    list.append( *resIt );
  }

  if ( list.count() == 0 ) {
    return end();
  }

  ConstIterator it = ConstIterator();
  it.d->mResources = list;
  it.d->mCurrRes = 0;
  it.d->mIt = ( it.d->mResources[ it.d->mCurrRes ] )->constBegin();

  while ( it.d->mIt == ( it.d->mResources[ it.d->mCurrRes ] )->constEnd() ) {
    if ( it.d->mCurrRes == it.d->mResources.count() - 1 ) {
      return end();
    }

    it.d->mCurrRes++;

    it.d->mIt = ( it.d->mResources[ it.d->mCurrRes ] )->constBegin();
  }

  return it;
}

AddressBook::Iterator AddressBook::end()
{
  KRES::Manager<Resource>::ActiveIterator resIt = d->mManager->activeEnd();

  if ( resIt == d->mManager->activeBegin() || ! *( --resIt ) ) {
    // no resource available
    d->end.d->mIt = Resource::Iterator();
  } else {
    d->end.d->mIt = ( *resIt )->end();
  }

  return d->end;
}

AddressBook::ConstIterator AddressBook::end() const
{
  KRES::Manager<Resource>::ActiveIterator resIt = d->mManager->activeEnd();

  if ( resIt == d->mManager->activeBegin() || ! *( --resIt ) ) {
    // no resource available
    d->constEnd.d->mIt = Resource::ConstIterator();
  } else {
    d->constEnd.d->mIt = ( *resIt )->constEnd();
  }

  return d->constEnd;
}

void AddressBook::clear()
{
  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    ( *it )->clear();
  }
}

Ticket *AddressBook::requestSaveTicket( Resource *resource )
{
  kDebug();

  if ( !resource ) {
    resource = standardResource();
  }

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    if ( ( *it ) == resource ) {
      if ( ( *it )->readOnly() || !( *it )->isOpen() ) {
        return 0;
      } else {
        return ( *it )->requestSaveTicket();
      }
    }
  }

  return 0;
}

void AddressBook::releaseSaveTicket( Ticket *ticket )
{
  if ( !ticket ) {
    return;
  }

  if ( ticket->resource() ) {
    ticket->resource()->releaseSaveTicket( ticket );
  }
}

void AddressBook::insertAddressee( const Addressee &a )
{
  Resource *resource = a.resource();
  if ( resource == 0 ) {
    resource = standardResource();
  }

  Resource::Iterator it;
  Addressee fAddr = resource->findByUid( a.uid() );

  Addressee addr( a );
  if ( !fAddr.isEmpty() ) {
    if ( fAddr != a ) {
      addr.setRevision( QDateTime::currentDateTime() );
    } else {
      if ( fAddr.resource() == 0 ) {
        fAddr.setResource( resource );
        //NOTE: Should we have setChanged( true ) here?
        resource->insertAddressee( fAddr );
      }
      return;
    }
  }

  addr.setResource( resource );
  addr.setChanged( true );
  resource->insertAddressee( addr );
}

void AddressBook::removeAddressee( const Addressee &a )
{
  if ( a.resource() ) {
    a.resource()->removeAddressee( a );
  }
}

void AddressBook::removeAddressee( const Iterator &it )
{
  if ( ( *it ).resource() ) {
    ( *it ).resource()->removeAddressee( *it );
  }
}

AddressBook::Iterator AddressBook::find( const Addressee &a )
{
  Iterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( a.uid() == ( *it ).uid() ) {
      return it;
    }
  }

  return end();
}

AddressBook::ConstIterator AddressBook::find( const Addressee &a ) const
{
  ConstIterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( a.uid() == ( *it ).uid() ) {
      return it;
    }
  }

  return end();
}

Addressee AddressBook::findByUid( const QString &uid ) const
{
  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    Addressee addr = ( *it )->findByUid( uid );
    if ( !addr.isEmpty() ) {
      return addr;
    }
  }

  return Addressee();
}

Addressee::List AddressBook::allAddressees() const
{
  Addressee::List list;

  ConstIterator it;
  for ( it = begin(); it != end(); ++it ) {
    list.append( *it );
  }

  return list;
}

Addressee::List AddressBook::findByName( const QString &name ) const
{
  Addressee::List results;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    results += ( *it )->findByName( name );
  }

  return results;
}

Addressee::List AddressBook::findByEmail( const QString &email ) const
{
  Addressee::List results;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    results += ( *it )->findByEmail( email );
  }

  return results;
}

Addressee::List AddressBook::findByCategory( const QString &category ) const
{
  Addressee::List results;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    results += ( *it )->findByCategory( category );
  }

  return results;
}

DistributionList *AddressBook::createDistributionList( const QString &name, Resource *resource )
{
  if ( resource == 0 ) {
    resource = standardResource();
  }

  return new DistributionList( resource, name );
}

void AddressBook::removeDistributionList( DistributionList *list )
{
  if ( !list || !list->resource() ) {
    return;
  }

  list->resource()->removeDistributionList( list );
}

DistributionList *AddressBook::findDistributionListByIdentifier( const QString &identifier )
{
  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    DistributionList *list = ( *it )->findDistributionListByIdentifier( identifier );
    if ( list ) {
      return list;
    }
  }

  return 0;
}

DistributionList *AddressBook::findDistributionListByName( const QString &name,
                                                           Qt::CaseSensitivity caseSensitivity )
{
  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    DistributionList *list = ( *it )->findDistributionListByName( name, caseSensitivity );
    if ( list ) {
      return list;
    }
  }

  return 0;
}

QList<DistributionList*> AddressBook::allDistributionLists()
{
  QList<DistributionList*> results;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    results += ( *it )->allDistributionLists();
  }

  return results;
}

QStringList AddressBook::allDistributionListNames() const
{
  QStringList results;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    results += ( *it )->allDistributionListNames();
  }

  return results;
}

void AddressBook::dump() const
{
  kDebug() << "--- begin ---";

  ConstIterator it;
  for ( it = begin(); it != end(); ++it ) {
    kDebug() << ( *it ).toString();
  }

  kDebug() << "---  end  ---";
}

QString AddressBook::identifier() const
{
  QStringList identifier;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    if ( !( *it )->identifier().isEmpty() ) {
      identifier.append( ( *it )->identifier() );
    }
  }

  return identifier.join( QLatin1String( ":" ) );
}

Field::List AddressBook::fields( int category ) const
{
  if ( d->mAllFields.isEmpty() ) {
    d->mAllFields = Field::allFields();
  }

  if ( category == Field::All ) {
    return d->mAllFields;
  }

  Field::List result;
  Field::List::ConstIterator it;
  for ( it = d->mAllFields.constBegin(); it != d->mAllFields.constEnd(); ++it ) {
    if ( ( *it )->category() & category ) {
      result.append( *it );
    }
  }

  return result;
}

bool AddressBook::addCustomField( const QString &label,
                                  int category,
                                  const QString &key,
                                  const QString &app ) const
{
  if ( d->mAllFields.isEmpty() ) {
    d->mAllFields = Field::allFields();
  }

  QString a = app.isNull() ? KGlobal::mainComponent().componentName() : app;
  QString k = key.isNull() ? label : key;

  Field *field = Field::createCustomField( label, category, k, a );

  if ( !field ) {
    return false;
  }

  d->mAllFields.append( field );

  return true;
}

QDataStream &KABC::operator<<( QDataStream &s, const AddressBook &ab )
{
  if ( !ab.d ) {
    return s;
  }

  return s;// << ab.d->mAddressees;
}

QDataStream &KABC::operator>>( QDataStream &s, AddressBook &ab )
{
  if ( !ab.d ) {
    return s;
  }

  return s;//  s >> ab.d->mAddressees;
}

bool AddressBook::addResource( Resource *resource )
{
  if ( !resource->open() ) {
    kDebug() << "can't add resource";
    return false;
  }

  d->mManager->add( resource );
  resource->setAddressBook( this );

  connect( resource, SIGNAL(loadingFinished(Resource*)),
           this, SLOT(resourceLoadingFinished(Resource*)) );
  connect( resource, SIGNAL(savingFinished(Resource*)),
           this, SLOT(resourceSavingFinished(Resource*)) );

  connect( resource, SIGNAL(loadingError(Resource*,QString)),
           this, SLOT(resourceLoadingError(Resource*,QString)) );
  connect( resource, SIGNAL(savingError(Resource*,QString)),
           this, SLOT(resourceSavingError(Resource*,QString)) );

  return true;
}

bool AddressBook::removeResource( Resource *resource )
{
  resource->close();

  if ( resource == standardResource() ) {
    d->mManager->setStandardResource( 0 );
  }

  resource->setAddressBook( 0 );

  disconnect( resource, SIGNAL(loadingFinished(Resource*)),
              this, SLOT(resourceLoadingFinished(Resource*)) );
  disconnect( resource, SIGNAL(savingFinished(Resource*)),
              this, SLOT(resourceSavingFinished(Resource*)) );

  disconnect( resource, SIGNAL(loadingError(Resource*,QString)),
              this, SLOT(resourceLoadingError(Resource*,QString)) );
  disconnect( resource, SIGNAL(savingError(Resource*,QString)),
              this, SLOT(resourceLoadingError(Resource*,QString)) );

  d->mManager->remove( resource );

  return true;
}

QList<Resource*> AddressBook::resources() const
{
  QList<Resource*> list;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    if ( d->mManager->standardResource() == ( *it ) ) {
      list.prepend( *it );
    } else {
      list.append( *it );
    }
  }

  return list;
}

void AddressBook::setErrorHandler( ErrorHandler *handler )
{
  delete d->mErrorHandler;
  d->mErrorHandler = handler;
}

void AddressBook::error( const QString &msg )
{
  if ( !d->mErrorHandler ) {
    // create default error handler
    d->mErrorHandler = new ConsoleErrorHandler();
  }

  if ( d->mErrorHandler ) {
    d->mErrorHandler->error( msg );
  } else {
    kError() << "no error handler defined";
  }
}

void AddressBook::setStandardResource( Resource *resource )
{
  d->mManager->setStandardResource( resource );
}

Resource *AddressBook::standardResource()
{
  return d->mManager->standardResource();
}

KRES::Manager<Resource> *AddressBook::resourceManager()
{
  return d->mManager;
}

bool AddressBook::loadingHasFinished() const
{
  return d->mPendingLoadResources.isEmpty();
}

void AddressBook::resourceLoadingFinished( Resource *resource )
{
  d->mPendingLoadResources.removeAll( resource );
  emit loadingFinished( resource );

  if ( d->mPendingLoadResources.count() == 0 ) {
    emit addressBookChanged( this );
  }
}

void AddressBook::resourceSavingFinished( Resource *resource )
{
  d->mPendingSaveResources.removeAll( resource );

  emit savingFinished( resource );
}

void AddressBook::resourceLoadingError( Resource *resource,
                                        const QString &errMsg )
{
  error( errMsg );

  d->mPendingLoadResources.removeAll( resource );
  if ( d->mPendingLoadResources.count() == 0 ) {
    emit addressBookChanged( this );
  }
}

void AddressBook::resourceSavingError( Resource *resource,
                                       const QString &errMsg )
{
  error( errMsg );

  d->mPendingSaveResources.removeAll( resource );
}
