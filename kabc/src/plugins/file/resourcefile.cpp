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

#include "resourcefile.h"
#include "resourcefileconfig.h"

#include "kabc/formatfactory.h"
#include "kabc/stdaddressbook.h"
#include "kabc/lock.h"

#include <kio/scheduler.h>
#include <kconfiggroup.h>
#include <kdebug.h>
#include <klocalizedstring.h>
#include <ksavefile.h>
#include <kstandarddirs.h>
#include <ktemporaryfile.h>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

using namespace KABC;

typedef QList< QPair<QString, QString> > MissingEntryList;

class ResourceFile::ResourceFilePrivate
{
  public:
    QMap< QString, MissingEntryList > mMissingEntries;
};

ResourceFile::ResourceFile()
  : Resource(), mFormat( 0 ), mTempFile( 0 ),
    mAsynchronous( false ), d( new ResourceFilePrivate )
{
  QString fileName, formatName;

  fileName = StdAddressBook::fileName();
  formatName = QLatin1String( "vcard" );

  init( fileName, formatName );
}

ResourceFile::ResourceFile( const KConfigGroup &group )
  : Resource( group ), mFormat( 0 ), mTempFile( 0 ),
    mAsynchronous( false ), d( new ResourceFilePrivate )
{
  QString fileName, formatName;

  fileName = group.readPathEntry( "FileName", StdAddressBook::fileName() );
  formatName = group.readEntry( "FileFormat", "vcard" );

  init( fileName, formatName );
}

ResourceFile::ResourceFile( const QString &fileName,
                            const QString &formatName )
  : Resource(), mFormat( 0 ), mTempFile( 0 ),
    mAsynchronous( false ), d( new ResourceFilePrivate )
{
  init( fileName, formatName );
}

void ResourceFile::init( const QString &fileName, const QString &formatName )
{
  mFormatName = formatName;

  FormatFactory *factory = FormatFactory::self();
  mFormat = factory->format( mFormatName );

  if ( !mFormat ) {
    mFormatName = QLatin1String( "vcard" );
    mFormat = factory->format( mFormatName );
  }

  connect( &mDirWatch, SIGNAL(dirty(QString)), SLOT(fileChanged(QString)) );
  connect( &mDirWatch, SIGNAL(created(QString)), SLOT(fileChanged(QString)) );
  connect( &mDirWatch, SIGNAL(deleted(QString)), SLOT(fileChanged(QString)) );

  setFileName( fileName );

  mDirWatch.addFile( KStandardDirs::locateLocal( "data", QLatin1String( "kabc/distlists" ) ) );

  mLock = 0;
}

ResourceFile::~ResourceFile()
{
  delete d;
  d = 0;
  delete mFormat;
  mFormat = 0;
}

void ResourceFile::writeConfig( KConfigGroup &group )
{
  Resource::writeConfig( group );

  if ( mFileName == StdAddressBook::fileName() ) {
    group.deleteEntry( "FileName" );
  } else {
    group.writePathEntry( "FileName", mFileName );
  }

  group.writeEntry( "FileFormat", mFormatName );
}

Ticket *ResourceFile::requestSaveTicket()
{
  qDebug();

  if ( !addressBook() ) {
    return 0;
  }

  delete mLock;
  mLock = new Lock( mFileName );

  if ( mLock->lock() ) {
    addressBook()->emitAddressBookLocked();
  } else {
    addressBook()->error( mLock->error() );
    qDebug() << "Unable to lock file '" << mFileName
             << "':" << mLock->error();
    return 0;
  }

  return createTicket( this );
}

void ResourceFile::releaseSaveTicket( Ticket *ticket )
{
  delete ticket;

  delete mLock;
  mLock = 0;

  addressBook()->emitAddressBookUnlocked();
}

bool ResourceFile::doOpen()
{
  QFile file( mFileName );

  if ( !file.exists() ) {
    // try to create the file
    bool ok = file.open( QIODevice::WriteOnly );
    if ( ok ) {
      file.close();
    }
    return ok;
  } else {
    QFileInfo fileInfo( mFileName );
    if ( readOnly() || !fileInfo.isWritable() ) {
      if ( !file.open( QIODevice::ReadOnly ) ) {
        return false;
      }
    } else {
      if ( !file.open( QIODevice::ReadWrite ) ) {
        return false;
      }
    }

    if ( file.size() == 0 ) {
      file.close();
      return true;
    }

    bool ok = mFormat->checkFormat( &file );
    file.close();

    return ok;
  }
}

void ResourceFile::doClose()
{
}

bool ResourceFile::load()
{
  qDebug() << mFileName << "'";

  mAsynchronous = false;

  QFile file( mFileName );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    addressBook()->error( i18n( "Unable to open file '%1'.", mFileName ) );
    return false;
  }

  if ( !clearAndLoad( &file ) ) {
      addressBook()->error( i18n( "Problems parsing file '%1'.", mFileName ) );
    return false;
  }

  return true;
}

bool ResourceFile::clearAndLoad( QFile *file )
{
  clear();

  bool addresseesOk = mFormat->loadAll( addressBook(), this, file );

  bool listsOk = loadDistributionLists();

  return addresseesOk && listsOk;
}

bool ResourceFile::asyncLoad()
{
  mAsynchronous = true;

  load();

  QTimer::singleShot( 0, this, SLOT(emitLoadingFinished()) );

  return true;
}

bool ResourceFile::save( Ticket *ticket )
{
  Q_UNUSED( ticket );
  qDebug();

  // create backup file
  QString extension = QLatin1Char( '_' ) + QString::number( QDate::currentDate().dayOfWeek() );
  (void) KSaveFile::simpleBackupFile( mFileName, QString(), extension );

  mDirWatch.stopScan();

  KSaveFile saveFile( mFileName );
  bool ok = false;

  if ( saveFile.open() ) {
    saveToFile( &saveFile );
    ok = saveFile.finalize();
  }

  if ( !ok ) {
    addressBook()->error( i18n( "Unable to save file '%1'.", mFileName ) );
  }

  mDirWatch.startScan();

  return ok;
}

bool ResourceFile::asyncSave( Ticket *ticket )
{
  qDebug();

  save( ticket );

  QTimer::singleShot( 0, this, SLOT(emitSavingFinished()) );

  return true;
}

void ResourceFile::emitLoadingFinished()
{
  emit loadingFinished( this );
}

void ResourceFile::emitSavingFinished()
{
  emit savingFinished( this );
}

bool ResourceFile::loadDistributionLists()
{
  KConfig cfg( KStandardDirs::locateLocal( "data", QLatin1String( "kabc/distlists" ) ) );

  KConfigGroup cg( &cfg, "DistributionLists" );
  KConfigGroup cgId( &cfg, "DistributionLists-Identifiers" );
  const QStringList entryList = cg.keyList();

  d->mMissingEntries.clear();

  QStringList::ConstIterator it;
  for ( it = entryList.constBegin(); it != entryList.constEnd(); ++it ) {
    const QString name = *it;
    const QStringList value = cg.readEntry( name, QStringList() );

    qDebug() << name << QLatin1Char( ':' ) << value.join( QLatin1String( "," ) );

    DistributionList *list = 0;
    if ( cgId.isValid() ) {
      const QString identifier = cgId.readEntry( name, QString() );
      if ( !identifier.isEmpty() ) {
        list = new DistributionList( this, identifier, name );
      }
    }

    if ( list == 0 ) {
      list = new DistributionList( this, name );
    }

    MissingEntryList missingEntries;
    QStringList::ConstIterator entryIt = value.constBegin();
    while ( entryIt != value.constEnd() ) {
      QString id = *entryIt++;
      QString email = entryIt != value.constEnd() ? *entryIt : QString();
      if ( email.isEmpty() && !email.isNull() ) {
        email = QString(); //krazy:exclude=nullstrassign
      }

      qDebug() << "----- Entry" << id;

      Addressee a = addressBook()->findByUid( id );
      if ( !a.isEmpty() ) {
        list->insertEntry( a, email );
      } else {
        missingEntries.append( qMakePair( id, email ) );
      }

      if ( entryIt == value.constEnd() ) {
        break;
      }
      ++entryIt;
    }

    d->mMissingEntries.insert( name, missingEntries );
  }

  return true;
}

void ResourceFile::saveDistributionLists()
{
  qDebug();

  KConfig cfg( KStandardDirs::locateLocal( "data", QLatin1String( "kabc/distlists" ) ) );
  KConfigGroup cg( &cfg, "DistributionLists" );
  cg.deleteGroup();
  KConfigGroup cgId( &cfg, "DistributionLists-Identifiers" );
  cgId.deleteGroup();

  QMapIterator<QString, DistributionList*> it( mDistListMap );
  while ( it.hasNext() ) {
    DistributionList *list = it.next().value();
    qDebug() << "  Saving '" << list->name() << "'";

    QStringList value;
    const DistributionList::Entry::List entries = list->entries();
    DistributionList::Entry::List::ConstIterator it;
    for ( it = entries.begin(); it != entries.end(); ++it ) {
      value.append( ( *it ).addressee().uid() );
      value.append( ( *it ).email() );
    }

    if ( d->mMissingEntries.find( list->name() ) != d->mMissingEntries.end() ) {
      const MissingEntryList missList = d->mMissingEntries[ list->name() ];
      MissingEntryList::ConstIterator missIt;
      for ( missIt = missList.begin(); missIt != missList.end(); ++missIt ) {
        value.append( ( *missIt ).first );
        value.append( ( *missIt ).second );
      }
    }

    cg.writeEntry( list->name(), value );
    cgId.writeEntry( list->name(), list->identifier() );
  }

  cfg.sync();
}

void ResourceFile::saveToFile( QFile *file )
{
  mFormat->saveAll( addressBook(), this, file );

  saveDistributionLists();
}

void ResourceFile::setFileName( const QString &fileName )
{
  mDirWatch.stopScan();
  if ( mDirWatch.contains( mFileName ) ) {
    mDirWatch.removeFile( mFileName );
  }

  mFileName = fileName;

  mDirWatch.addFile( mFileName );
  mDirWatch.startScan();
}

QString ResourceFile::fileName() const
{
  return mFileName;
}

void ResourceFile::setFormat( const QString &format )
{
  mFormatName = format;
  delete mFormat;

  FormatFactory *factory = FormatFactory::self();
  mFormat = factory->format( mFormatName );
}

QString ResourceFile::format() const
{
  return mFormatName;
}

void ResourceFile::fileChanged( const QString &path )
{
  qDebug() << path;

  if ( !addressBook() ) {
    return;
  }

  if ( path == KStandardDirs::locateLocal( "data", QLatin1String( "kabc/distlists" ) ) ) {
    // clear old distribution lists
    // take a copy of mDistListMap, then clear it and finally qDeleteAll
    // the copy to avoid problems with removeDistributionList() called by
    // ~DistributionList().
    DistributionListMap tempDistListMap( mDistListMap );
    mDistListMap.clear();
    qDeleteAll( tempDistListMap );

    loadDistributionLists();

    qDebug() << "addressBookChanged()";
    addressBook()->emitAddressBookChanged();

    return;
  }

//  clear(); // moved to clearAndLoad()
  if ( mAsynchronous ) {
    asyncLoad();
  } else {
    load();
    qDebug() << "addressBookChanged()";
    addressBook()->emitAddressBookChanged();
  }
}

void ResourceFile::removeAddressee( const Addressee &addr )
{
  QFile::remove( KStandardDirs::locateLocal(
                   "data", QLatin1String( "kabc/photos/" ) ) + addr.uid() );
  QFile::remove( KStandardDirs::locateLocal(
                   "data", QLatin1String( "kabc/logos/" ) ) + addr.uid() );
  QFile::remove( KStandardDirs::locateLocal(
                   "data", QLatin1String( "kabc/sounds/" ) ) + addr.uid() );

  mAddrMap.remove( addr.uid() );
}

