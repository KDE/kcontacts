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
#include <klocale.h>
#include <ksavefile.h>
#include <kstandarddirs.h>
#include <ktemporaryfile.h>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

static const char *s_customFieldName = "DistributionList";

using namespace KABC;

typedef QList< QPair<QString, QString> > MissingEntryList;

static bool isDistributionList( const Addressee &addr )
{
  const QString str = addr.custom( "KADDRESSBOOK", s_customFieldName );
  return !str.isEmpty();
}

// Helper function, to parse the contents of the custom field
// Returns a list of { uid, email }
typedef QList<QPair<QString, QString> > ParseList;
static ParseList parseCustom( const Addressee &addr )
{
  ParseList res;
  const QStringList lst = addr.custom( "KADDRESSBOOK", s_customFieldName ).split( ';', QString::SkipEmptyParts );
  for ( QStringList::ConstIterator it = lst.constBegin(); it != lst.constEnd(); ++it ) {
    if ( (*it).isEmpty() ) {
      continue;
    }

    // parse "uid,email"
    QStringList helpList = (*it).split( ',', QString::SkipEmptyParts );
    Q_ASSERT( !helpList.isEmpty() );
    if ( helpList.isEmpty() ) {
      continue;
    }
    const QString uid = helpList.first();
    QString email;
    Q_ASSERT( helpList.count() < 3 ); // 1 or 2 items, but not more
    if ( helpList.count() == 2 ) {
      email = helpList.last();
    }
    res.append( qMakePair( uid, email ) );
  }
  return res;
}

static KABC::Addressee::List findByFormattedName( KABC::AddressBook *book,
                                                  const QString &name,
                                                  bool caseSensitive = true )
{
  KABC::Addressee::List res;
  KABC::AddressBook::Iterator abIt;
  for ( abIt = book->begin(); abIt != book->end(); ++abIt ) {
    if ( caseSensitive && (*abIt).formattedName() == name ) {
      res.append( *abIt );
    }
    if ( !caseSensitive && (*abIt).formattedName().toLower() == name.toLower() ) {
      res.append( *abIt );
    }
  }
  return res;
}

static KABC::Addressee findByUidOrName( KABC::AddressBook *book,
                                        const QString &uidOrName,
                                        const QString &email )
{
  KABC::Addressee a = book->findByUid( uidOrName );
  if ( a.isEmpty() ) {
    // UID not found, maybe it is a name instead.
    // If we have an email, let's use that for the lookup.
    // [This is used by e.g. the Kolab resource]
    if ( !email.isEmpty() ) {
      KABC::Addressee::List lst = book->findByEmail( email );
      KABC::Addressee::List::ConstIterator listit = lst.constBegin();
      for ( ; listit != lst.constEnd(); ++listit ) {
        if ( (*listit).formattedName() == uidOrName ) {
          a = *listit;
          break;
        }
      }
      if ( !lst.isEmpty() && a.isEmpty() ) { // found that email, but no match on the fullname
        a = lst.first(); // probably the last name changed
      }
    }

    // If we don't have an email, or if we didn't find any match for it, look up by full name
    if ( a.isEmpty() ) {
      // (But this has to be done here, since when loading we might not have the entries yet)
      KABC::Addressee::List lst = findByFormattedName( book, uidOrName );
      if ( !lst.isEmpty() ) {
        a = lst.first();
      }
    }
  }
  return a;
}

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
  formatName = "vcard";

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
    mFormatName = "vcard";
    mFormat = factory->format( mFormatName );
  }

  connect( &mDirWatch, SIGNAL( dirty(const QString&) ), SLOT( fileChanged(const QString&) ) );
  connect( &mDirWatch, SIGNAL( created(const QString&) ), SLOT( fileChanged(const QString&) ) );
  connect( &mDirWatch, SIGNAL( deleted(const QString&) ), SLOT( fileChanged(const QString&) ) );

  setFileName( fileName );

  mDirWatch.addFile( KStandardDirs::locateLocal( "data", "kabc/distlists" ) );

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
  kDebug();

  if ( !addressBook() ) {
    return 0;
  }

  delete mLock;
  mLock = new Lock( mFileName );

  if ( mLock->lock() ) {
    addressBook()->emitAddressBookLocked();
  } else {
    addressBook()->error( mLock->error() );
    kDebug() << "Unable to lock file '" << mFileName
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
  kDebug() << mFileName << "'";

  mAsynchronous = false;

  QFile file( mFileName );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    addressBook()->error( i18n( "Unable to open file '%1'.", mFileName ) );
    return false;
  }

  if ( !clearAndLoad( &file ) ) {
      addressBook()->error( i18n( "Problems during parsing file '%1'.", mFileName ) );
    return false;
  }

  return true;
}

bool ResourceFile::clearAndLoad( QFile *file )
{
  clear();

  bool addresseesOk = mFormat->loadAll( addressBook(), this, file );

  bool listsOk = loadDistributionLists();

  if ( !addresseesOk || !listsOk )
    return false;

  bool importedVCardDistLists = false;

  // check if any of the loaded addressees is an old enterprise branch
  // workaround distlists
  Addressee::Map::Iterator addrIt    = mAddrMap.begin();
  Addressee::Map::Iterator addrEndIt = mAddrMap.end();
  while ( addrIt != addrEndIt) {
    Addressee addr = addrIt.value();
    if ( !isDistributionList( addr ) ) {
      ++addrIt;
      continue;
    }

    importedVCardDistLists = true;

    kWarning(5700) << "Addressee uid=" << addr.uid() << "contains the distlist"
                   << addr.formattedName();

    // remove the addressee from the map
    addrIt = mAddrMap.erase( addrIt );

    // parse the distlist entries
    const ParseList parseList = parseCustom( addr );
    if ( parseList.isEmpty() )
      continue;

    DistributionList *list = mDistListMap.value( addr.uid(), 0 );
    if ( list == 0 ) {
        list = new DistributionList( this, addr.uid(), addr.formattedName() );
        kDebug(5700) << "Created new distlist instance";
    } else {
        kDebug(5700) << "Adding entries to existing distlist instance";
    }

    MissingEntryList missingEntries;
    ParseList::ConstIterator it    = parseList.constBegin();
    ParseList::ConstIterator endIt = parseList.constEnd();
    for ( ; it != endIt; ++it ) {
      const QString uid = (*it).first;
      const QString email = (*it).second;
      // look up contact
      KABC::Addressee a = findByUidOrName( addressBook(), uid, email );
      if ( a.isEmpty() ) {
        missingEntries.append( qMakePair( uid, email ) );
      } else {
        list->insertEntry( a, email );
      }
    }
    d->mMissingEntries.insert( addr.formattedName(), missingEntries );
  }

  if ( importedVCardDistLists ) {
    mDirWatch.stopScan();

    KSaveFile saveFile( mFileName );
    if ( saveFile.open() ) {
        saveToFile( &saveFile );
        saveFile.finalize();
    }

    mDirWatch.startScan();
  }

  return true;
}

bool ResourceFile::asyncLoad()
{
  mAsynchronous = true;

  load();

  QTimer::singleShot( 0, this, SLOT( emitLoadingFinished() ) );

  return true;
}

bool ResourceFile::save( Ticket *ticket )
{
  Q_UNUSED( ticket );
  kDebug();

  // create backup file
  QString extension = '_' + QString::number( QDate::currentDate().dayOfWeek() );
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
  kDebug();

  save( ticket );

  QTimer::singleShot( 0, this, SLOT( emitSavingFinished() ) );

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
  KConfig cfg( KStandardDirs::locateLocal( "data", "kabc/distlists" ) );

  KConfigGroup cg( &cfg, "DistributionLists" );
  KConfigGroup cgId( &cfg, "DistributionLists-Identifiers" );
  const QStringList entryList = cg.keyList();

  d->mMissingEntries.clear();

  QStringList::ConstIterator it;
  for ( it = entryList.constBegin(); it != entryList.constEnd(); ++it ) {
    const QString name = *it;
    const QStringList value = cg.readEntry( name, QStringList() );

    kDebug() << name << ":" << value.join( "," );

    DistributionList *list = 0;
    if ( cgId.isValid() ) {
      const QString identifier = cgId.readEntry( name, QString() );
      if ( !identifier.isEmpty() )
        list = new DistributionList( this, identifier, name );
    }

    if ( list == 0 )
        list = new DistributionList( this, name );

    MissingEntryList missingEntries;
    QStringList::ConstIterator entryIt = value.constBegin();
    while ( entryIt != value.constEnd() ) {
      QString id = *entryIt++;
      QString email = entryIt != value.constEnd() ? *entryIt : QString();
      if ( email.isEmpty() && !email.isNull() ) {
        email = QString();
      }

      kDebug() << "----- Entry" << id;

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
  kDebug();

  KConfig cfg( KStandardDirs::locateLocal( "data", "kabc/distlists" ) );
  KConfigGroup cg( &cfg, "DistributionLists" );
  cg.deleteGroup();
  KConfigGroup cgId( &cfg, "DistributionLists-Identifiers" );
  cgId.deleteGroup();

  QMapIterator<QString, DistributionList*> it( mDistListMap );
  while ( it.hasNext() ) {
    DistributionList *list = it.next().value();
    kDebug() << "  Saving '" << list->name() << "'";

    QStringList value;
    const DistributionList::Entry::List entries = list->entries();
    DistributionList::Entry::List::ConstIterator it;
    for ( it = entries.begin(); it != entries.end(); ++it ) {
      value.append( (*it).addressee().uid() );
      value.append( (*it).email() );
    }

    if ( d->mMissingEntries.find( list->name() ) != d->mMissingEntries.end() ) {
      const MissingEntryList missList = d->mMissingEntries[ list->name() ];
      MissingEntryList::ConstIterator missIt;
      for ( missIt = missList.begin(); missIt != missList.end(); ++missIt ) {
        value.append( (*missIt).first );
        value.append( (*missIt).second );
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
  kDebug() << path;

  if ( !addressBook() ) {
    return;
  }

  if ( path == KStandardDirs::locateLocal( "data", "kabc/distlists" ) ) {
    // clear old distribution lists
    // take a copy of mDistListMap, then clear it and finally qDeleteAll
    // the copy to avoid problems with removeDistributionList() called by
    // ~DistributionList().
    DistributionListMap tempDistListMap( mDistListMap );
    mDistListMap.clear();
    qDeleteAll( tempDistListMap );

    loadDistributionLists();

    kDebug() << "addressBookChanged()";
    addressBook()->emitAddressBookChanged();

    return;
  }

//  clear(); // moved to clearAndLoad()
  if ( mAsynchronous ) {
    asyncLoad();
  } else {
    load();
    kDebug() << "addressBookChanged()";
    addressBook()->emitAddressBookChanged();
  }
}

void ResourceFile::removeAddressee( const Addressee &addr )
{
  QFile::remove(
    QFile::encodeName( KStandardDirs::locateLocal( "data", "kabc/photos/" ) + addr.uid() ) );

  QFile::remove(
    QFile::encodeName( KStandardDirs::locateLocal( "data", "kabc/logos/" ) + addr.uid() ) );

  QFile::remove(
    QFile::encodeName( KStandardDirs::locateLocal( "data", "kabc/sounds/" ) + addr.uid() ) );

  mAddrMap.remove( addr.uid() );
}

#include "resourcefile.moc"
