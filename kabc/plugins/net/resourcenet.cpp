/*
    This file is part of libkabc.
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>

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

#include "resourcenet.h"
#include "resourcenetconfig.h"

#include "kabc/addressbook.h"
#include "kabc/formatfactory.h"
#include "kabc/stdaddressbook.h"

#include <kio/netaccess.h>
#include <kio/scheduler.h>
#include <kdebug.h>
#include <klocale.h>
#include <ksavefile.h>
#include <ktemporaryfile.h>
#include <kurlrequester.h>

#include <QtCore/QFile>

using namespace KABC;

class ResourceNet::ResourceNetPrivate
{
  public:
    KIO::Job *mLoadJob;
    bool mIsLoading;

    KIO::Job *mSaveJob;
    bool mIsSaving;
};

ResourceNet::ResourceNet()
  : Resource(), mFormat( 0 ),
    mTempFile( 0 ),
    d( new ResourceNetPrivate )
{
  init( KUrl(), "vcard" );
}

ResourceNet::ResourceNet( const KConfigGroup &group )
  : Resource( group ), mFormat( 0 ),
    mTempFile( 0 ),
    d( new ResourceNetPrivate )
{
  init( KUrl( group.readPathEntry( "NetUrl" ) ), group.readEntry( "NetFormat" ) );
}

ResourceNet::ResourceNet( const KUrl &url, const QString &format )
  : Resource(), mFormat( 0 ),
    mTempFile( 0 ),
    d( new ResourceNetPrivate )
{
  init( url, format );
}

void ResourceNet::init( const KUrl &url, const QString &format )
{
  d->mLoadJob = 0;
  d->mIsLoading = false;
  d->mSaveJob = 0;
  d->mIsSaving = false;

  mFormatName = format;

  FormatFactory *factory = FormatFactory::self();
  mFormat = factory->format( mFormatName );
  if ( !mFormat ) {
    mFormatName = "vcard";
    mFormat = factory->format( mFormatName );
  }

  setUrl( url );
}

ResourceNet::~ResourceNet()
{
  if ( d->mIsLoading ) {
    d->mLoadJob->kill();
  }
  if ( d->mIsSaving ) {
    d->mSaveJob->kill();
  }

  delete d;
  d = 0;

  delete mFormat;
  mFormat = 0;

  deleteLocalTempFile();
}

void ResourceNet::writeConfig( KConfigGroup &group )
{
  Resource::writeConfig( group );

  group.writePathEntry( "NetUrl", mUrl.url() );
  group.writeEntry( "NetFormat", mFormatName );
}

Ticket *ResourceNet::requestSaveTicket()
{
  kDebug(5700) << "ResourceNet::requestSaveTicket()";

  return createTicket( this );
}

void ResourceNet::releaseSaveTicket( Ticket *ticket )
{
  delete ticket;
}

bool ResourceNet::doOpen()
{
  return true;
}

void ResourceNet::doClose()
{
}

bool ResourceNet::load()
{
  QString tempFile;

  if ( !KIO::NetAccess::download( mUrl, tempFile, 0 ) ) {
    addressBook()->error( i18n( "Unable to download file '%1'.", mUrl.prettyUrl() ) );
    return false;
  }

  QFile file( tempFile );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    addressBook()->error( i18n( "Unable to open file '%1'.", tempFile ) );
    KIO::NetAccess::removeTempFile( tempFile );
    return false;
  }

  bool result = clearAndLoad( &file );
  if ( !result ) {
    addressBook()->error( i18n( "Problems during parsing file '%1'.", tempFile ) );
  }

  KIO::NetAccess::removeTempFile( tempFile );

  return result;
}

bool ResourceNet::clearAndLoad( QFile *file )
{
  clear();
  return mFormat->loadAll( addressBook(), this, file );
}

bool ResourceNet::asyncLoad()
{
  if ( d->mIsLoading ) {
    abortAsyncLoading();
  }

  if ( d->mIsSaving ) {
    kWarning(5700) << "Aborted asyncLoad() because we're still asyncSave()ing!" << endl;
    return false;
  }

  bool ok = createLocalTempFile();

  if ( !ok ) {
    emit loadingError( this, i18n( "Unable to open file '%1'.", mTempFile->fileName() ) );
    deleteLocalTempFile();
    return false;
  }

  KUrl dest;
  dest.setPath( mTempFile->fileName() );

  KIO::Scheduler::checkSlaveOnHold( true );
  d->mLoadJob = KIO::file_copy( mUrl, dest, -1, true, false, false );
  d->mIsLoading = true;
  connect( d->mLoadJob, SIGNAL( result( KJob* ) ),
           this, SLOT( downloadFinished( KJob* ) ) );

  return true;
}

void ResourceNet::abortAsyncLoading()
{
  kDebug(5700) << "ResourceNet::abortAsyncLoading()";

  if ( d->mLoadJob ) {
    d->mLoadJob->kill(); // result not emitted
    d->mLoadJob = 0;
  }

  deleteLocalTempFile();
  d->mIsLoading = false;
}

void ResourceNet::abortAsyncSaving()
{
  kDebug(5700) << "ResourceNet::abortAsyncSaving()";

  if ( d->mSaveJob ) {
    d->mSaveJob->kill(); // result not emitted
    d->mSaveJob = 0;
  }

  deleteLocalTempFile();
  d->mIsSaving = false;
}

bool ResourceNet::save( Ticket *ticket )
{
  Q_UNUSED( ticket );
  kDebug(5700) << "ResourceNet::save()";

  if ( d->mIsSaving ) {
    abortAsyncSaving();
  }

  KTemporaryFile tempFile;
  bool ok = tempFile.open();

  if ( ok ) {
    saveToFile( &tempFile );
  }

  if ( !ok ) {
    addressBook()->error( i18n( "Unable to save file '%1'.", tempFile.fileName() ) );
    return false;
  }

  ok = KIO::NetAccess::upload( tempFile.fileName(), mUrl, 0 );
  if ( !ok ) {
    addressBook()->error( i18n( "Unable to upload to '%1'.", mUrl.prettyUrl() ) );
  }

  return ok;
}

bool ResourceNet::asyncSave( Ticket *ticket )
{
  Q_UNUSED( ticket );
  kDebug(5700) << "ResourceNet::asyncSave()";

  if ( d->mIsSaving ) {
    abortAsyncSaving();
  }

  if ( d->mIsLoading ) {
    kWarning(5700) << "Aborted asyncSave() because we're still asyncLoad()ing!" << endl;
    return false;
  }

  bool ok = createLocalTempFile();
  if ( ok ) {
    saveToFile( mTempFile );
  }

  if ( !ok ) {
    emit savingError( this, i18n( "Unable to save file '%1'.", mTempFile->fileName() ) );
    deleteLocalTempFile();
    return false;
  }

  KUrl src;
  src.setPath( mTempFile->fileName() );

  KIO::Scheduler::checkSlaveOnHold( true );
  d->mIsSaving = true;
  d->mSaveJob = KIO::file_copy( src, mUrl, -1, true, false, false );
  connect( d->mSaveJob, SIGNAL( result( KJob* ) ),
           this, SLOT( uploadFinished( KJob* ) ) );

  return true;
}

bool ResourceNet::createLocalTempFile()
{
  deleteStaleTempFile();
  mTempFile = new KTemporaryFile();
  return mTempFile->open();
}

void ResourceNet::deleteStaleTempFile()
{
  if ( hasTempFile() ) {
    kDebug(5700) << "stale temp file detected" << mTempFile->fileName();
    deleteLocalTempFile();
  }
}

void ResourceNet::deleteLocalTempFile()
{
  delete mTempFile;
  mTempFile = 0;
}

void ResourceNet::saveToFile( QFile *file )
{
  mFormat->saveAll( addressBook(), this, file );
}

void ResourceNet::setUrl( const KUrl &url )
{
  mUrl = url;
}

KUrl ResourceNet::url() const
{
  return mUrl;
}

void ResourceNet::setFormat( const QString &name )
{
  mFormatName = name;
  if ( mFormat ) {
    delete mFormat;
  }

  FormatFactory *factory = FormatFactory::self();
  mFormat = factory->format( mFormatName );
}

QString ResourceNet::format() const
{
  return mFormatName;
}

void ResourceNet::downloadFinished( KJob *job )
{
  Q_UNUSED( job );
  kDebug(5700) << "ResourceNet::downloadFinished()";

  d->mIsLoading = false;

  if ( !hasTempFile() ) {
    emit loadingError( this, i18n( "Download failed, could not create temporary file" ) );
    return;
  }

  QFile file( mTempFile->fileName() );
  if ( file.open( QIODevice::ReadOnly ) ) {
    if ( clearAndLoad( &file ) ) {
      emit loadingFinished( this );
    } else {
      emit loadingError( this, i18n( "Problems during parsing file '%1'.",
                                     mTempFile->fileName() ) );
    }
  } else {
    emit loadingError( this, i18n( "Unable to open file '%1'.",
                                   mTempFile->fileName() ) );
  }

  deleteLocalTempFile();
}

void ResourceNet::uploadFinished( KJob *job )
{
  kDebug(5700) << "ResourceFile::uploadFinished()";

  d->mIsSaving = false;

  if ( job->error() ) {
    emit savingError( this, job->errorString() );
  } else {
    emit savingFinished( this );
  }

  deleteLocalTempFile();
}

#include "resourcenet.moc"
