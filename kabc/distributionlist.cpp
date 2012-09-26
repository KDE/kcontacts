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

#include <QApplication>
#include <QtCore/QPair>
#include <QtCore/QPointer>

using namespace KABC;

class DistributionList::Entry::Private
{
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

    QPointer<Resource> mResource;
    QString mIdentifier;
    QString mName;
    Entry::List mEntries;
};

DistributionList::DistributionList( Resource *resource, const QString &name )
  : d( new Private( resource, KRandom::randomString(10), name ) )
{
  d->mResource->insertDistributionList( this );
}

DistributionList::DistributionList( Resource *resource,
                                    const QString &identifier, const QString &name )
  : d( new Private( resource, identifier, name ) )
{
  d->mResource->insertDistributionList( this );
}

DistributionList::~DistributionList()
{
  if ( d->mResource ) {
    d->mResource->removeDistributionList( this );
  }

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
    if ( ( *it ).addressee().uid() == a.uid() ) {
      /**
        We have to check if both email addresses contains no data,
        a simple 'email1 == email2' wont work here
       */
      if ( ( ( *it ).email().isNull() && email.isEmpty() ) ||
           ( ( *it ).email().isEmpty() && email.isNull() ) ||
           ( ( *it ).email() == email ) ) {
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
    if ( ( *it ).addressee().uid() == a.uid() && ( *it ).email() == email ) {
      d->mEntries.erase( it );
      return;
    }
  }
}

QStringList DistributionList::emails() const
{
  QStringList emails;

  Entry::List::ConstIterator it;
  for ( it = d->mEntries.constBegin(); it != d->mEntries.constEnd(); ++it ) {
    const Addressee a = ( *it ).addressee();
    QString email = ( *it ).email().isEmpty() ? a.fullEmail() :
                                                a.fullEmail( ( *it ).email() );

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

Resource *DistributionList::resource() const
{
    return d->mResource;
}

typedef QList< QPair<QString, QString> > MissingEntryList;
