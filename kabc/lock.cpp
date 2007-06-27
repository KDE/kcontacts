/*
    This file is part of libkabc.
    Copyright (c) 2001,2003 Cornelius Schumacher <schumacher@kde.org>

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

#include "lock.h"

#include <krandom.h>
#include <kcomponentdata.h>
#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace KABC;

class Lock::Private
{
  public:
    Private( const QString &identifier )
      : mIdentifier( identifier )
    {
      mIdentifier.replace( "/", "_" );
    }

    QString mIdentifier;
    QString mLockUniqueName;
    QString mError;
};

Lock::Lock( const QString &identifier )
  : d( new Private( identifier ) )
{
}

Lock::~Lock()
{
  unlock();

  delete d;
}

QString Lock::locksDir()
{
  return KStandardDirs::locateLocal( "data", "kabc/lock/" );
}

bool Lock::readLockFile( const QString &filename, int &pid, QString &app )
{
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    return false;
  }

  QTextStream t( &file );
  t >> pid >> ws >> app;

  return true;
}

bool Lock::writeLockFile( const QString &filename )
{
  QFile file( filename );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    return false;
  }

  QTextStream t( &file );
  t << ::getpid() << endl << QString( KGlobal::mainComponent().componentName() );

  return true;
}

QString Lock::lockFileName() const
{
  return locksDir() + d->mIdentifier + ".lock";
}

bool Lock::lock()
{
  kDebug(5700) << "Lock::lock()" << endl;

  QString lockName = lockFileName();
  kDebug(5700) << "-- lock name: " << lockName << endl;

  if ( QFile::exists( lockName ) ) {  // check if it is a stale lock file
    int pid;
    QString app;

    if ( !readLockFile( lockFileName(), pid, app ) ) {
      d->mError = i18n( "Unable to open lock file." );
      return false;
    }

    int retval = ::kill( pid, 0 );
    if ( retval == -1 && errno == ESRCH ) { // process doesn't exists anymore
      QFile::remove( lockName );
      kWarning(5700) << "Removed stale lock file from process '" << app << "'"
                      << endl;
    } else {
      QString identifier( d->mIdentifier );
      identifier.replace( '_', '/' );

      d->mError = i18n( "The resource '%1' is locked by application '%2'.", identifier, app );
      return false;
    }
  }

  QString lockUniqueName;
  lockUniqueName = d->mIdentifier + KRandom::randomString( 8 );
  d->mLockUniqueName = KStandardDirs::locateLocal( "data", "kabc/lock/" + lockUniqueName );
  kDebug(5700) << "-- lock unique name: " << d->mLockUniqueName << endl;

  // Create unique file
  writeLockFile( d->mLockUniqueName );

  // Create lock file
  int result = ::link( QFile::encodeName( d->mLockUniqueName ),
                       QFile::encodeName( lockName ) );

  if ( result == 0 ) {
    d->mError.clear();
    emit locked();
    return true;
  }

  // TODO: check stat

  d->mError = i18n( "Error" );
  return false;
}

bool Lock::unlock()
{
  int pid;
  QString app;
  if ( readLockFile( lockFileName(), pid, app ) ) {
    if ( pid == getpid() ) {
      QFile::remove( lockFileName() );
      QFile::remove( d->mLockUniqueName );
      emit unlocked();
    } else {
      d->mError = i18n( "Unlock failed. Lock file is owned by other process: %1 (%2)", app, pid );
      kDebug(5700) << "Lock::unlock(): " << d->mError << endl;
      return false;
    }
  }

  d->mError.clear();

  return true;
}

QString Lock::error() const
{
  return d->mError;
}

#include "lock.moc"
