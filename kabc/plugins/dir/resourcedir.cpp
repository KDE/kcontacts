/*
    This file is part of libkabc.
    Copyright (c) 2002 - 2003 Tobias Koenig <tokoe@kde.org>

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

#include "resourcedir.h"
#include "resourcedirconfig.h"

#include "kabc/addressbook.h"
#include "kabc/formatfactory.h"
#include "kabc/stdaddressbook.h"
#include "kabc/lock.h"

#include <kconfiggroup.h>
#include <kdebug.h>
#include <kgenericfactory.h>
#include <kglobal.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kurlrequester.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

using namespace KABC;

class ResourceDir::Private
{
  public:
    Private( ResourceDir *parent )
      : mParent( parent ), mFormat( 0 ), mAsynchronous( false )
    {
    }

    ~Private()
    {
      delete mFormat;
      mFormat = 0;
    }

    void pathChanged();
    void init( const QString &path, const QString &format );

    ResourceDir *mParent;
    Format *mFormat;
    KDirWatch mDirWatch;

    QString mPath;
    QString mFormatName;

    Lock *mLock;

    bool mAsynchronous;
};

void ResourceDir::Private::init( const QString &path, const QString &format )
{
  mFormatName = format;

  FormatFactory *factory = FormatFactory::self();
  mFormat = factory->format( mFormatName );

  if ( !mFormat ) {
    mFormatName = "vcard";
    mFormat = factory->format( mFormatName );
  }

  mLock = 0;

  mParent->connect( &mDirWatch, SIGNAL( dirty(const QString&) ), SLOT( pathChanged() ) );
  mParent->connect( &mDirWatch, SIGNAL( created(const QString&) ), SLOT( pathChanged() ) );
  mParent->connect( &mDirWatch, SIGNAL( deleted(const QString&) ), SLOT( pathChanged() ) );

  mParent->setPath( path );
}

void ResourceDir::Private::pathChanged()
{
  if ( !mParent->addressBook() ) {
    return;
  }

  mParent->clear();
  if ( mAsynchronous ) {
    mParent->asyncLoad();
  } else {
    mParent->load();
    mParent->addressBook()->emitAddressBookChanged();
  }
}

ResourceDir::ResourceDir()
  : Resource(), d( new Private( this ) )
{
  d->init( StdAddressBook::directoryName(), "vcard" );
}

ResourceDir::ResourceDir( const KConfigGroup &group )
  : Resource( group ), d( new Private( this ) )
{
  d->init( group.readPathEntry( "FilePath", StdAddressBook::directoryName() ),
           group.readEntry( "FileFormat", "vcard" ) );
}

ResourceDir::ResourceDir( const QString &path, const QString &format )
  : Resource(), d( new Private( this ) )
{
  d->init( path, format );
}

ResourceDir::~ResourceDir()
{
  delete d;
}

void ResourceDir::writeConfig( KConfigGroup &group )
{
  Resource::writeConfig( group );

  if ( d->mPath == StdAddressBook::directoryName() ) {
    group.deleteEntry( "FilePath" );
  } else {
    group.writePathEntry( "FilePath", d->mPath );
  }

  group.writeEntry( "FileFormat", d->mFormatName );
}

Ticket *ResourceDir::requestSaveTicket()
{
  kDebug(5700) << "ResourceDir::requestSaveTicket()" << endl;

  if ( !addressBook() ) {
    return 0;
  }

  delete d->mLock;
  d->mLock = new Lock( d->mPath );

  if ( d->mLock->lock() ) {
    addressBook()->emitAddressBookLocked();
  } else {
    addressBook()->error( d->mLock->error() );
    kDebug(5700) << "ResourceFile::requestSaveTicket(): Unable to lock path '"
                  << d->mPath << "': " << d->mLock->error() << endl;
    return 0;
  }

  return createTicket( this );
}

void ResourceDir::releaseSaveTicket( Ticket *ticket )
{
  delete ticket;

  delete d->mLock;
  d->mLock = 0;
}

bool ResourceDir::doOpen()
{
  QDir dir( d->mPath );
  if ( !dir.exists() ) { // no directory available
    return dir.mkdir( dir.path() );
  } else {
    const QStringList lst = dir.entryList( QDir::Files );
    if ( lst.isEmpty() ) { //path doesn't exist or list of file empty
      return true;
    }
    QString testName = lst.first();
    QFile file( d->mPath + '/' + testName );
    if ( file.open( QIODevice::ReadOnly ) ) {
      return true;
    }
    if ( file.size() == 0 ) {
      return true;
    }

    bool ok = d->mFormat->checkFormat( &file );
    file.close();
    return ok;
  }
}

void ResourceDir::doClose()
{
}

bool ResourceDir::load()
{
  kDebug(5700) << "ResourceDir::load(): '" << d->mPath << "'" << endl;

  d->mAsynchronous = false;

  QDir dir( d->mPath );
  QStringList files = dir.entryList( QDir::Files );

  QStringList::Iterator it;
  bool ok = true;
  for ( it = files.begin(); it != files.end(); ++it ) {
    QFile file( d->mPath + '/' + (*it) );

    if ( !file.open( QIODevice::ReadOnly ) ) {
      addressBook()->error( i18n( "Unable to open file '%1' for reading", file.fileName() ) );
      ok = false;
      continue;
    }

    if ( !d->mFormat->loadAll( addressBook(), this, &file ) ) {
      ok = false;
    }

    file.close();
  }

  return ok;
}

bool ResourceDir::asyncLoad()
{
  d->mAsynchronous = true;

  bool ok = load();
  if ( !ok ) {
    emit loadingError( this, i18n( "Loading resource '%1' failed!", resourceName() ) );
  } else {
    emit loadingFinished( this );
  }

  return ok;
}

bool ResourceDir::save( Ticket * )
{
  kDebug(5700) << "ResourceDir::save(): '" << d->mPath << "'" << endl;

  Addressee::Map::Iterator it;
  bool ok = true;

  d->mDirWatch.stopScan();

  for ( it = mAddrMap.begin(); it != mAddrMap.end(); ++it ) {
    if ( !it.value().changed() ) {
      continue;
    }

    QFile file( d->mPath + '/' + (*it).uid() );
    if ( !file.open( QIODevice::WriteOnly ) ) {
      addressBook()->error( i18n( "Unable to open file '%1' for writing", file.fileName() ) );
      continue;
    }

    d->mFormat->save( *it, &file );

    // mark as unchanged
    (*it).setChanged( false );

    file.close();
  }

  d->mDirWatch.startScan();

  return ok;
}

bool ResourceDir::asyncSave( Ticket *ticket )
{
  bool ok = save( ticket );
  if ( !ok ) {
    emit savingError( this, i18n( "Saving resource '%1' failed!", resourceName() ) );
  } else {
    emit savingFinished( this );
  }
  return ok;
}

void ResourceDir::setPath( const QString &path )
{
  d->mDirWatch.stopScan();
  if ( d->mDirWatch.contains( d->mPath ) ) {
    d->mDirWatch.removeDir( d->mPath );
  }

  d->mPath = path;
  d->mDirWatch.addDir( d->mPath, true );
  d->mDirWatch.startScan();
}

QString ResourceDir::path() const
{
  return d->mPath;
}

void ResourceDir::setFormat( const QString &format )
{
  d->mFormatName = format;

  delete d->mFormat;

  FormatFactory *factory = FormatFactory::self();
  d->mFormat = factory->format( d->mFormatName );
}

QString ResourceDir::format() const
{
  return d->mFormatName;
}

void ResourceDir::removeAddressee( const Addressee &addr )
{
  QFile::remove( d->mPath + '/' + addr.uid() );
  mAddrMap.remove( addr.uid() );
}

#include "resourcedir.moc"
