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

#include "distributionlist.h"
#include "resource.h"

#include <kconfig.h>
#include <krandom.h>
#include <kstandarddirs.h>
#include <kdebug.h>

#include <QtGui/QApplication>
#include <QtCore/QPair>

using namespace KABC;

class DistributionList::Entry::Private {
  public:
    Private() {}
    Private( Addressee _addressee, const QString &_email )
      : addressee( _addressee ), email( _email ) {}

    Addressee addressee;
    QString email;
};

DistributionList::Entry::Entry()
  : d( new Private() )
{
}

DistributionList::Entry::Entry( const DistributionList::Entry &entry )
  : d( new Private( entry.d->addressee, entry.d->email ) )
{
}

DistributionList::Entry::Entry( const Addressee &_addressee, const QString &_email )
  : d( new Private( _addressee, _email ) )
{
}

DistributionList::Entry::~Entry()
{
  delete d;
}

DistributionList::Entry &DistributionList::Entry::operator=( const DistributionList::Entry &entry )
{
  d->addressee = entry.d->addressee;
  d->email = entry.d->email;

  return *this;
}

Addressee DistributionList::Entry::addressee() const
{
  return d->addressee;
}

QString DistributionList::Entry::email() const
{
  return d->email;
}

class DistributionList::Private
{
  public:
    Private( Resource *resource, const QString &identifier,
             const QString &name )
      : mResource( resource ), mIdentifier( identifier ), mName( name )
    {
    }

    Resource *mResource;
    QString mIdentifier;
    QString mName;
    Entry::List mEntries;
};

DistributionList::DistributionList( Resource *resource, const QString &name )
  : d( new Private( resource, KRandom::randomString(10), name ) )
{
  d->mResource->insertDistributionList( this );
}

DistributionList::DistributionList( Resource *resource, const QString &identifier, const QString &name )
  : d( new Private( resource, identifier, name ) )
{
  d->mResource->insertDistributionList( this );
}

DistributionList::~DistributionList()
{
  d->mResource->removeDistributionList( this );

  delete d;
}

void DistributionList::setIdentifier( const QString &identifier )
{
  d->mIdentifier = identifier;
}

QString DistributionList::identifier() const
{
  return d->mIdentifier;
}

void DistributionList::setName( const QString &name )
{
  d->mName = name;
}

QString DistributionList::name() const
{
  return d->mName;
}

void DistributionList::insertEntry( const Addressee &a, const QString &email )
{
  Entry e( a, email );

  QList<Entry>::Iterator it;
  for ( it = d->mEntries.begin(); it != d->mEntries.end(); ++it ) {
    if ( (*it).addressee().uid() == a.uid() ) {
      /**
        We have to check if both email addresses contains no data,
        a simple 'email1 == email2' wont work here
       */
      if ( ( (*it).email().isNull() && email.isEmpty() ) ||
           ( (*it).email().isEmpty() && email.isNull() ) ||
           ( (*it).email() == email ) ) {
        *it = e;
        return;
      }
    }
  }
  d->mEntries.append( e );
}

void DistributionList::removeEntry( const Addressee &a, const QString &email )
{
  QList<Entry>::Iterator it;
  for ( it = d->mEntries.begin(); it != d->mEntries.end(); ++it ) {
    if ( (*it).addressee().uid() == a.uid() && (*it).email() == email ) {
      d->mEntries.erase( it );
      return;
    }
  }
}

QStringList DistributionList::emails() const
{
  QStringList emails;

  Entry::List::ConstIterator it;
  for ( it = d->mEntries.begin(); it != d->mEntries.end(); ++it ) {
    const Addressee a = (*it).addressee();
    QString email = (*it).email().isEmpty() ? a.fullEmail() :
                                              a.fullEmail( (*it).email() );

    if ( !email.isEmpty() ) {
      emails.append( email );
    }
  }

  return emails;
}

DistributionList::Entry::List DistributionList::entries() const
{
  return d->mEntries;
}

Resource* DistributionList::resource() const
{
    return d->mResource;
}

typedef QList< QPair<QString, QString> > MissingEntryList;

#if 0
class DistributionListManager::Private
{
  public:
    Private( AddressBook *addressBook )
      : mAddressBook( addressBook )
    {
    }

    ~Private()
    {
      qDeleteAll( mLists );
      mLists.clear();
    }

    AddressBook *mAddressBook;
    QMap< QString, MissingEntryList > mMissingEntries;
    QList<DistributionList*> mLists;
};

DistributionListManager::DistributionListManager( AddressBook *ab )
  : d( new Private( ab ) )
{
}

DistributionListManager::~DistributionListManager()
{
  delete d;
}

DistributionList *DistributionListManager::list( const QString &name,
                                                 Qt::CaseSensitivity _caseSensitivity )
{
  QListIterator<DistributionList*> it( d->mLists );
  bool caseSensitive = ( _caseSensitivity == Qt::CaseSensitive );
  QString newName = caseSensitive ?  name : name.toLower();
  while ( it.hasNext() ) {
    DistributionList *list = it.next();
    if ( !caseSensitive ) {
      if ( list->name().toLower() == newName ) {
        return list;
      }
    } else {
      if ( list->name() == newName ) {
        return list;
      }
    }
  }

  return 0;
}

void DistributionListManager::insert( DistributionList *l )
{
  if ( !l ) {
    return;
  }

  QMutableListIterator<DistributionList*> it( d->mLists );
  while ( it.hasNext() ) {
    DistributionList *list = it.next();
    if ( list->name() == l->name() ) {
      d->mLists.removeAll( list );
      break;
    }
  }

  d->mLists.append( l );
}

void DistributionListManager::remove( DistributionList *l )
{
  if ( !l ) {
    return;
  }

  QMutableListIterator<DistributionList*> it( d->mLists );
  while ( it.hasNext() ) {
    DistributionList *list = it.next();
    if ( list->name() == l->name() ) {
      d->mLists.removeAll( list );
      break;
    }
  }
}

QStringList DistributionListManager::listNames()
{
  QStringList names;

  QListIterator<DistributionList*> it( d->mLists );
  while ( it.hasNext() ) {
    const DistributionList *list = it.next();
    names.append( list->name() );
  }

  return names;
}

bool DistributionListManager::load()
{
  KConfig cfg( KStandardDirs::locateLocal( "data", "kabc/distlists" ) );

  KConfigGroup cg( &cfg, "DistributionLists" );
  QMap<QString,QString> entryMap = cg.entryMap();

  // clear old lists
  qDeleteAll( d->mLists );
  d->mLists.clear();
  d->mMissingEntries.clear();

  QMap<QString,QString>::ConstIterator it;
  for ( it = entryMap.constBegin(); it != entryMap.constEnd(); ++it ) {
    QString name = it.key();
    QStringList value = cg.readEntry( name, QStringList() );

    kDebug(5700) << "DLM::load():" << name << ":" << value.join( "," );

    // FIXME new distribution list handling
    DistributionList *list = 0;//new DistributionList( this, name );

    MissingEntryList missingEntries;
    QStringList::ConstIterator entryIt = value.constBegin();
    while ( entryIt != value.constEnd() ) {
      QString id = *entryIt++;
      QString email = entryIt != value.constEnd() ? *entryIt : QString();

      kDebug(5700) << "----- Entry" << id;

      Addressee a = d->mAddressBook->findByUid( id );
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

bool DistributionListManager::save()
{
  kDebug(5700) << "DistListManager::save()";

  KConfig cfg( KStandardDirs::locateLocal( "data", "kabc/distlists" ) );
  KConfigGroup cg( &cfg, "DistributionLists" );
  cg.deleteGroup();

  QListIterator<DistributionList*> it( d->mLists );
  while ( it.hasNext() ) {
    DistributionList *list = it.next();
    kDebug(5700) << "  Saving '" << list->name() << "'";

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
  }

  cg.sync();

  return true;
}
#endif
class DistributionListWatcher::Private
{
  public:
    Private()
      : mDirWatch( new KDirWatch )
    {
      mDirWatch->addFile( KStandardDirs::locateLocal( "data", "kabc/distlists" ) );
    }

    ~Private()
    {
      delete mDirWatch;
      mDirWatch = 0;
    }

    static DistributionListWatcher *mSelf;
    KDirWatch *mDirWatch;
};

DistributionListWatcher *DistributionListWatcher::Private::mSelf = 0;

DistributionListWatcher::DistributionListWatcher()
 : QObject( qApp ), d( new Private )
{
  connect( d->mDirWatch, SIGNAL( dirty( const QString& ) ), SIGNAL( changed() ) );
  d->mDirWatch->startScan();
}

DistributionListWatcher::~DistributionListWatcher()
{
  delete d;
}

DistributionListWatcher *DistributionListWatcher::self()
{
  kWarning( !qApp ) << "No QApplication object available, you'll get a memleak!";

  if ( !Private::mSelf ) {
    Private::mSelf = new DistributionListWatcher();
  }

  return Private::mSelf;
}

#include "distributionlist.moc"
