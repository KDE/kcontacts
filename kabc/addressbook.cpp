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

#include <qfile.h>
#include <qregexp.h>
#include <qtimer.h>

#include <kapplication.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kinstance.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include "errorhandler.h"
#include "resource.h"

#include "addressbook.h"
#include "addressbook.moc"

using namespace KABC;

struct AddressBook::AddressBookData
{
  Field::List mAllFields;
  ErrorHandler *mErrorHandler;
  KConfig *mConfig;
  KRES::Manager<Resource> *mManager;
};

struct AddressBook::Iterator::IteratorData
{
  Resource::Iterator mIt;
  QValueList<Resource*> mResources;
  int mCurrRes;
};

struct AddressBook::ConstIterator::ConstIteratorData
{
  Resource::ConstIterator mIt;
  QValueList<Resource*> mResources;
  int mCurrRes;
};

AddressBook::Iterator::Iterator()
{
  d = new IteratorData;
}

AddressBook::Iterator::Iterator( const AddressBook::Iterator &i )
{
  d = new IteratorData;
  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;
}

AddressBook::Iterator &AddressBook::Iterator::operator=( const AddressBook::Iterator &i )
{
  if( this == &i ) return *this; // guard against self assignment
  delete d; // delete the old data the Iterator was completely constructed before
  d = new IteratorData;
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
  return *(d->mIt);
}

Addressee &AddressBook::Iterator::operator*()
{
  return *(d->mIt);
}

Addressee *AddressBook::Iterator::operator->()
{
  return &(*(d->mIt));
}

AddressBook::Iterator &AddressBook::Iterator::operator++()
{
start:
  bool jumped = false;
  while ( d->mIt == (d->mResources[ d->mCurrRes ])->end() ) { // at end of addressee list of resource
    if ( (uint)d->mCurrRes == d->mResources.count() - 1 ) {
      return *this;
    }

    d->mCurrRes++; // jump to next resource

    jumped = true;
    d->mIt = (d->mResources[ d->mCurrRes ])->begin();
  }

  if ( !jumped )
    (d->mIt)++;

  if ( d->mIt == (d->mResources[ d->mCurrRes ])->end() )
    goto start;

  return *this;
}

AddressBook::Iterator &AddressBook::Iterator::operator++( int )
{
start:
  bool jumped = false;
  while ( d->mIt == (d->mResources[ d->mCurrRes ])->end() ) { // at end of addressee list of resource
    if ( (uint)d->mCurrRes == d->mResources.count() - 1 ) {
      return *this;
    }

    d->mCurrRes++; // jump to next resource

    jumped = true;
    d->mIt = (d->mResources[ d->mCurrRes ])->begin();
  }

  if ( !jumped )
    (d->mIt)++;

  if ( d->mIt == (d->mResources[ d->mCurrRes ])->end() )
    goto start;

  return *this;
}

AddressBook::Iterator &AddressBook::Iterator::operator--()
{
  (d->mIt)--;

  return *this;
}

AddressBook::Iterator &AddressBook::Iterator::operator--( int )
{
  (d->mIt)--;

  return *this;
}

bool AddressBook::Iterator::operator==( const Iterator &it )
{
  return ( d->mIt == it.d->mIt );
}

bool AddressBook::Iterator::operator!=( const Iterator &it )
{
  return ( d->mIt != it.d->mIt );
}


AddressBook::ConstIterator::ConstIterator()
{
  d = new ConstIteratorData;
}

AddressBook::ConstIterator::ConstIterator( const AddressBook::ConstIterator &i )
{
  d = new ConstIteratorData;
  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;
}

AddressBook::ConstIterator::ConstIterator( const AddressBook::Iterator &i )
{
  d = new ConstIteratorData;
  d->mIt = i.d->mIt;
  d->mResources = i.d->mResources;
  d->mCurrRes = i.d->mCurrRes;
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator=( const AddressBook::ConstIterator &i )
{
  if( this  == &i ) return *this; // guard for self assignment
  delete d; // delete the old data because the Iterator was really constructed before
  d = new ConstIteratorData;
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
  return *(d->mIt);
}

const Addressee* AddressBook::ConstIterator::operator->() const
{
  return &(*(d->mIt));
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator++()
{
start:
  bool jumped = false;
  while ( d->mIt == (d->mResources[ d->mCurrRes ])->end() ) { // at end of addressee list of resource
    if ( (uint)d->mCurrRes == d->mResources.count() - 1 ) {
      return *this;
    }

    d->mCurrRes++; // jump to next resource

    jumped = true;
    d->mIt = (d->mResources[ d->mCurrRes ])->begin();
  }

  if ( !jumped )
    (d->mIt)++;

  if ( d->mIt == (d->mResources[ d->mCurrRes ])->end() )
    goto start;

  return *this;
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator++(int)
{
start:
  bool jumped = false;
  while ( d->mIt == (d->mResources[ d->mCurrRes ])->end() ) { // at end of addressee list of resource
    if ( (uint)d->mCurrRes == d->mResources.count() - 1 ) {
      return *this;
    }

    d->mCurrRes++; // jump to next resource

    jumped = true;
    d->mIt = (d->mResources[ d->mCurrRes ])->begin();
  }

  if ( !jumped )
    (d->mIt)++;

  if ( d->mIt == (d->mResources[ d->mCurrRes ])->end() )
    goto start;

  return *this;
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator--()
{
  (d->mIt)--;
  return *this;
}

AddressBook::ConstIterator &AddressBook::ConstIterator::operator--(int)
{
  (d->mIt)--;
  return *this;
}

bool AddressBook::ConstIterator::operator==( const ConstIterator &it )
{
  return ( d->mIt == it.d->mIt );
}

bool AddressBook::ConstIterator::operator!=( const ConstIterator &it )
{
  return ( d->mIt != it.d->mIt );
}


AddressBook::AddressBook()
{
  d = new AddressBookData;
  d->mErrorHandler = 0;
  d->mConfig = 0;
  d->mManager = new KRES::Manager<Resource>( "contact" );
}

AddressBook::AddressBook( const QString &config )
{
  d = new AddressBookData;
  d->mErrorHandler = 0;
  d->mConfig = new KConfig( config );
  d->mManager = new KRES::Manager<Resource>( "contact" );
  d->mManager->readConfig( d->mConfig );
}

AddressBook::~AddressBook()
{
  delete d->mManager; d->mManager = 0;
  delete d->mConfig; d->mConfig = 0;
  delete d->mErrorHandler; d->mErrorHandler = 0;
  delete d; d = 0;
}

bool AddressBook::load()
{
  kdDebug(5700) << "AddressBook::load()" << endl;

  clear();

  KRES::Manager<Resource>::ActiveIterator it;
  bool ok = true;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    (*it)->setAddressBook( this );
    if ( !(*it)->load() ) {
      error( i18n("Unable to load resource '%1'").arg( (*it)->resourceName() ) );
      ok = false;
    }
  }

  return ok;
}

bool AddressBook::loadAsynchronous()
{
  kdDebug(5700) << "AddressBook::loadAsynchronous()" << endl;

  clear();

  KRES::Manager<Resource>::ActiveIterator it;
  bool ok = true;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    (*it)->setAddressBook( this );
    if ( !(*it)->loadAsynchronous() ) {
      error( i18n("Unable to load resource '%1'").arg( (*it)->resourceName() ) );
      ok = false;
    }
  }

  return ok;
}

bool AddressBook::save( Ticket *ticket )
{
  kdDebug(5700) << "AddressBook::save()"<< endl;

  if ( ticket->resource() ) {
    deleteRemovedAddressees();
    return ticket->resource()->save( ticket );
  }

  return false;
}

AddressBook::Iterator AddressBook::begin()
{
  QValueList<Resource*> list;
  KRES::Manager<Resource>::ActiveIterator resIt;
  for ( resIt = d->mManager->activeBegin(); resIt != d->mManager->activeEnd(); ++resIt )
    list.append( *resIt );

  if ( list.count() == 0 )
    return end();

  Iterator it = Iterator();
  it.d->mResources = list;
  it.d->mCurrRes = 0;
  it.d->mIt = (it.d->mResources[ it.d->mCurrRes ])->begin();

  while ( it.d->mIt == (it.d->mResources[ it.d->mCurrRes ])->end() ) {
    if ( (uint)it.d->mCurrRes == it.d->mResources.count() - 1 )
      return end();

    it.d->mCurrRes++;

    it.d->mIt = (it.d->mResources[ it.d->mCurrRes ])->begin();
  }

  return it;
}

AddressBook::ConstIterator AddressBook::begin() const
{
  QValueList<Resource*> list;
  KRES::Manager<Resource>::ActiveIterator resIt;
  for ( resIt = d->mManager->activeBegin(); resIt != d->mManager->activeEnd(); ++resIt ) 
    list.append( *resIt );

  if ( list.count() == 0 )
    return end();

  Iterator it = Iterator();
  it.d->mResources = list;
  it.d->mCurrRes = 0;
  it.d->mIt = (it.d->mResources[ it.d->mCurrRes ])->begin();

  while ( it.d->mIt == (it.d->mResources[ it.d->mCurrRes ])->end() ) {
    if ( (uint)it.d->mCurrRes == it.d->mResources.count() - 1 )
      return end();

    it.d->mCurrRes++;

    it.d->mIt = (it.d->mResources[ it.d->mCurrRes ])->begin();
  }

  return it;
}

AddressBook::Iterator AddressBook::end()
{
  Resource *res = 0;
  KRES::Manager<Resource>::ActiveIterator resIt;
  for ( resIt = d->mManager->activeBegin(); resIt != d->mManager->activeEnd(); ++resIt ) 
    res = (*resIt);

  Iterator it = Iterator();
  if ( !res ) { // no resource available
    it.d->mResources = QValueList<Resource*>();
    it.d->mCurrRes = -1;
    it.d->mIt = Resource::Iterator();
  } else {
    it.d->mResources = QValueList<Resource*>();
    it.d->mCurrRes = -1;
    it.d->mIt = res->end();
  }

  return it;
}

AddressBook::ConstIterator AddressBook::end() const
{
  Resource *res = 0;
  KRES::Manager<Resource>::ActiveIterator resIt;
  for ( resIt = d->mManager->activeBegin(); resIt != d->mManager->activeEnd(); ++resIt ) 
    res = (*resIt);

  Iterator it = Iterator();
  if ( !res ) { // no resource available
    it.d->mResources = QValueList<Resource*>();
    it.d->mCurrRes = -1;
    it.d->mIt = Resource::Iterator();
  } else {
    it.d->mResources = QValueList<Resource*>();
    it.d->mCurrRes = -1;
    it.d->mIt = res->end();
  }

  return it;
}

void AddressBook::clear()
{
  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) 
    (*it)->clear();
}

Ticket *AddressBook::requestSaveTicket( Resource *resource )
{
  kdDebug(5700) << "AddressBook::requestSaveTicket()" << endl;

  if ( !resource )
    resource = standardResource();

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    if ( (*it) == resource ) {
      if ( (*it)->readOnly() || !(*it)->isOpen() )
        return 0;
      else
        return (*it)->requestSaveTicket();
    }
  }

  return 0;
}

void AddressBook::insertAddressee( const Addressee &a )
{
  Resource *resource = a.resource();
  if ( resource == 0 )
    resource = standardResource();

  Resource::Iterator it;
  for ( it = resource->begin(); it != resource->end(); ++it ) {
    if ( a.uid() == (*it).uid() ) {
      bool changed = false;
      Addressee addr = a;
      if ( addr != (*it) )
        changed = true;

      (*it) = a;
      (*it).setResource( resource );

      if ( changed ) {
        (*it).setRevision( QDateTime::currentDateTime() );
        (*it).setChanged( true ); 
      }

      return;
    }
  }

  Addressee addr( a );
  addr.setResource( resource );
  addr.setChanged( true );
  resource->insertAddressee( addr );
}

void AddressBook::removeAddressee( const Addressee &a )
{
  if ( a.resource() )
    a.resource()->removeAddressee( a );
}

void AddressBook::removeAddressee( const Iterator &it )
{
  if ( (*it).resource() )
    (*it).resource()->removeAddressee( *it );
}

AddressBook::Iterator AddressBook::find( const Addressee &a )
{
  Iterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( a.uid() == (*it).uid() ) {
      return it;
    }
  }

  return end();
}

Addressee AddressBook::findByUid( const QString &uid )
{
  Iterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( uid == (*it).uid() ) {
      return *it;
    }
  }

  return Addressee();
}

Addressee::List AddressBook::allAddressees()
{
  Addressee::List list;

  Iterator it;
  for ( it = begin(); it != end(); ++it )
    list.append( *it );

  return list;
}

Addressee::List AddressBook::findByName( const QString &name )
{
  Addressee::List results;

  Iterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( name == (*it).name() ) {
      results.append( *it );
    }
  }

  return results;
}

Addressee::List AddressBook::findByEmail( const QString &email )
{
  Addressee::List results;
  QStringList mailList;

  Iterator it;
  for ( it = begin(); it != end(); ++it ) {
    mailList = (*it).emails();    
    for ( QStringList::Iterator ite = mailList.begin(); ite != mailList.end(); ++ite ) {
      if ( email == (*ite) ) {
        results.append( *it );
      }
    }
  }

  return results;
}

Addressee::List AddressBook::findByCategory( const QString &category )
{
  Addressee::List results;

  Iterator it;
  for ( it = begin(); it != end(); ++it ) {
    if ( (*it).hasCategory( category) ) {
      results.append( *it );
    }
  }

  return results;
}

void AddressBook::dump() const
{
  kdDebug(5700) << "AddressBook::dump() --- begin ---" << endl;

  ConstIterator it;
  for( it = begin(); it != end(); ++it ) {
    (*it).dump();
  }

  kdDebug(5700) << "AddressBook::dump() ---  end  ---" << endl;
}

QString AddressBook::identifier()
{
  QStringList identifier;


  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    if ( !(*it)->identifier().isEmpty() )
      identifier.append( (*it)->identifier() );
  }

  return identifier.join( ":" );
}

Field::List AddressBook::fields( int category )
{
  if ( d->mAllFields.isEmpty() ) {
    d->mAllFields = Field::allFields();
  }
  
  if ( category == Field::All ) return d->mAllFields;
  
  Field::List result;
  Field::List::ConstIterator it;
  for( it = d->mAllFields.begin(); it != d->mAllFields.end(); ++it ) {
    if ( (*it)->category() & category ) result.append( *it );
  }
  
  return result;
}

bool AddressBook::addCustomField( const QString &label, int category,
                                  const QString &key, const QString &app )
{
  if ( d->mAllFields.isEmpty() ) {
    d->mAllFields = Field::allFields();
  }
  
  QString a = app.isNull() ? KGlobal::instance()->instanceName() : app;
  QString k = key.isNull() ? label : key;

  Field *field = Field::createCustomField( label, category, k, a );

  if ( !field ) return false;

  d->mAllFields.append( field );

  return true;
}

QDataStream &KABC::operator<<( QDataStream &s, const AddressBook &ab )
{
  if (!ab.d) return s;

  return s;// << ab.d->mAddressees;
}

QDataStream &KABC::operator>>( QDataStream &s, AddressBook &ab )
{
  if (!ab.d) return s;

//  s >> ab.d->mAddressees;

  return s;
}

bool AddressBook::addResource( Resource *resource )
{
  if ( !resource->open() ) {
    kdDebug(5700) << "AddressBook::addResource(): can't add resource" << endl;
    return false;
  }

  resource->setAddressBook( this );

  d->mManager->add( resource );
  return true;
}

bool AddressBook::removeResource( Resource *resource )
{
  resource->close();

  if ( resource == standardResource() )
    d->mManager->setStandardResource( 0 );

  resource->setAddressBook( 0 );

  d->mManager->remove( resource );
  return true;
}

QPtrList<Resource> AddressBook::resources()
{
  QPtrList<Resource> list;

  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it )
    list.append( *it );

  return list;
}

void AddressBook::setErrorHandler( ErrorHandler *handler )
{
  delete d->mErrorHandler;
  d->mErrorHandler = handler;
}

void AddressBook::error( const QString& msg )
{
  if ( !d->mErrorHandler ) // create default error handler
    d->mErrorHandler = new ConsoleErrorHandler;

  if ( d->mErrorHandler )
    d->mErrorHandler->error( msg );
  else
    kdError(5700) << "no error handler defined" << endl;
}

void AddressBook::deleteRemovedAddressees()
{
  // no any longer needed
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

void AddressBook::cleanUp()
{
  KRES::Manager<Resource>::ActiveIterator it;
  for ( it = d->mManager->activeBegin(); it != d->mManager->activeEnd(); ++it ) {
    if ( !(*it)->readOnly() && (*it)->isOpen() )
      (*it)->cleanUp();
  }
}
