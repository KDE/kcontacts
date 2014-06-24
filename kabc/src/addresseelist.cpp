/*
    This file is part of libkabc.
    Copyright (c) 2002 Jost Schenck <jost@schenck.de>
                  2003 Tobias Koenig <tokoe@kde.org>

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

#include "addresseelist.h"
#include "field.h"
#include "sortmode.h"
#include "kabc_debug.h"


#include <QtCore/QSharedData>

using namespace KABC;

//
//
// Traits
//
//

SortingTraits::Uid::Uid()
  : d( 0 )
{
}

SortingTraits::Uid::~Uid()
{
}

bool SortingTraits::Uid::eq( const Addressee &a1, const Addressee &a2 )
{
  // locale awareness doesn't make sense sorting ids
  return QString::compare( a1.uid(), a2.uid() ) == 0;
}

bool SortingTraits::Uid::lt( const Addressee &a1, const Addressee &a2 )
{
  // locale awareness doesn't make sense sorting ids
  return QString::compare( a1.uid(), a2.uid() ) < 0;
}

SortingTraits::Name::Name()
  : d( 0 )
{
}

SortingTraits::Name::~Name()
{
}

bool SortingTraits::Name::eq( const Addressee &a1, const Addressee &a2 )
{
  return QString::localeAwareCompare( a1.name(), a2.name() ) == 0;
}

bool SortingTraits::Name::lt( const Addressee &a1, const Addressee &a2 )
{
  return QString::localeAwareCompare( a1.name(), a2.name() ) < 0;
}

SortingTraits::FormattedName::FormattedName()
  : d( 0 )
{
}

SortingTraits::FormattedName::~FormattedName()
{
}

bool SortingTraits::FormattedName::eq( const Addressee &a1, const Addressee &a2 )
{
  return QString::localeAwareCompare( a1.formattedName(), a2.formattedName() ) == 0;
}

bool SortingTraits::FormattedName::lt( const Addressee &a1, const Addressee &a2 )
{
  return QString::localeAwareCompare( a1.formattedName(), a2.formattedName() ) < 0;
}

SortingTraits::FamilyName::FamilyName()
  : d( 0 )
{
}

SortingTraits::FamilyName::~FamilyName()
{
}

bool SortingTraits::FamilyName::eq( const Addressee &a1, const Addressee &a2 )
{
  return
    QString::localeAwareCompare( a1.familyName(), a2.familyName() ) == 0 &&
    QString::localeAwareCompare( a1.givenName(), a2.givenName() ) == 0;
}

bool SortingTraits::FamilyName::lt( const Addressee &a1, const Addressee &a2 )
{
  int family = QString::localeAwareCompare( a1.familyName(), a2.familyName() );
  if ( 0 == family ) {
    return QString::localeAwareCompare( a1.givenName(), a2.givenName() ) < 0;
  } else {
    return family < 0;
  }
}

SortingTraits::GivenName::GivenName()
  : d( 0 )
{
}

SortingTraits::GivenName::~GivenName()
{
}

bool SortingTraits::GivenName::eq( const Addressee &a1, const Addressee &a2 )
{
  return
    QString::localeAwareCompare( a1.givenName(), a2.givenName() ) == 0 &&
    QString::localeAwareCompare( a1.familyName(), a2.familyName() ) == 0;
}

bool SortingTraits::GivenName::lt( const Addressee &a1, const Addressee &a2 )
{
  int given = QString::localeAwareCompare( a1.givenName(), a2.givenName() );
  if ( 0 == given ) {
    return QString::localeAwareCompare( a1.familyName(), a2.familyName() ) < 0;
  } else {
    return given < 0;
  }
}

//
//
// AddresseeList
//
//

static Field *sActiveField=0;

class AddresseeList::Private : public QSharedData
{
  public:
    Private()
      : mReverseSorting( false ), mActiveSortingCriterion( FormattedName )
    {
    }

    Private( const Private &other )
      : QSharedData( other )
    {
      mReverseSorting = other.mReverseSorting;
      mActiveSortingCriterion = other.mActiveSortingCriterion;
    }

    bool mReverseSorting;
    SortingCriterion mActiveSortingCriterion;
};

AddresseeList::AddresseeList()
  : QList<Addressee>(), d( new Private )
{
}

AddresseeList::~AddresseeList()
{
}

AddresseeList::AddresseeList( const AddresseeList &other )
  : QList<Addressee>( other ), d( other.d )
{
}

AddresseeList::AddresseeList( const QList<Addressee> &l )
  : QList<Addressee>( l ), d( new Private )
{
}

AddresseeList &AddresseeList::operator=( const AddresseeList &other )
{
  if ( this != &other ) {
    QList<Addressee>::operator=( other );
    d = other.d;
  }

  return *this;
}

QString AddresseeList::toString() const
{
  QString str;

  str += QLatin1String( "AddresseeList {\n" );
  str += QString::fromLatin1( "   Reverse Order: %1\n" ).arg( d->mReverseSorting ?
                                                              QLatin1String( "true" ) :
                                                              QLatin1String( "false" ) );

  QString crit;
  if ( Uid == d->mActiveSortingCriterion ) {
    crit = QLatin1String( "Uid" );
  } else if ( Name == d->mActiveSortingCriterion ) {
    crit = QLatin1String( "Name" );
  } else if ( FormattedName == d->mActiveSortingCriterion ) {
    crit = QLatin1String( "FormattedName" );
  } else if ( FamilyName == d->mActiveSortingCriterion ) {
    crit = QLatin1String( "FamilyName" );
  } else if ( GivenName == d->mActiveSortingCriterion ) {
    crit = QLatin1String( "GivenName" );
  } else {
    crit = QLatin1String( "unknown -- update dump method" );
  }

  str += QString::fromLatin1( "   Sorting criterion: %1\n" ).arg( crit );
#if 0 //code commented in loop => comment it too
  for ( const_iterator it = begin(); it != end(); ++it ) {
//    str += (*it).toString();
  }
#endif

  str += QLatin1String( "}\n" );

  return str;
}

void AddresseeList::setReverseSorting( bool reverseSorting )
{
  d->mReverseSorting = reverseSorting;
}

bool AddresseeList::reverseSorting() const
{
  return d->mReverseSorting;
}

void AddresseeList::sortBy( SortingCriterion c )
{
  d->mActiveSortingCriterion = c;
  if ( Uid == c ) {
    sortByTrait<SortingTraits::Uid>();
  } else if ( Name == c ) {
    sortByTrait<SortingTraits::Name>();
  } else if ( FormattedName == c ) {
    sortByTrait<SortingTraits::FormattedName>();
  } else if ( FamilyName == c ) {
    sortByTrait<SortingTraits::FamilyName>();
  } else if ( GivenName == c ) {
    sortByTrait<SortingTraits::GivenName>();
  } else {
    qCCritical( KABC_LOG ) << "AddresseeList sorting criterion passed for which a trait is not known."
                 << "No sorting done.";
  }
}

void AddresseeList::sort()
{
  sortBy( d->mActiveSortingCriterion );
}

template<class Trait>
void AddresseeList::sortByTrait()
{
  // FIXME: better sorting algorithm, bubblesort is not acceptable for larger lists.
  //
  // for i := 1 to n - 1
  //   do for j := 1 to n - i
  //     do if A[j] > A[j+1]
  //       then temp :=  A[j]
  //         A[j] := A[j + 1]
  //         A[j + 1 ] := temp

  iterator i1 = begin();
  iterator endIt = end();
  --endIt;
  if ( i1 == endIt ) { // don't need sorting
    return;
  }

  iterator i2 = endIt;
  while ( i1 != endIt ) {
    iterator j1 = begin();
    iterator j2 = j1;
    ++j2;
    while ( j1 != i2 ) {
      if ( ( !d->mReverseSorting && Trait::lt( *j2, *j1 ) ) ||
           ( d->mReverseSorting && Trait::lt( *j1, *j2 ) ) ) {
        qSwap( *j1, *j2 );
      }
      ++j1;
      ++j2;
    }
    ++i1;
    --i2;
  }
}

void AddresseeList::sortByField( Field *field )
{
  if ( !field ) {
    qCWarning(KABC_LOG) << "sortByField called with no active sort field";
    return;
  }

  sActiveField = field;

  if ( count() == 0 ) {
    return;
  }

  KABC::FieldSortMode *mode = new KABC::FieldSortMode( sActiveField, !d->mReverseSorting );

  KABC::Addressee::setSortMode( mode );
  qSort( *this );
  KABC::Addressee::setSortMode( 0 );

  delete mode;
}

void AddresseeList::sortByMode( SortMode *mode )
{
  if ( count() == 0 ) {
    return;
  }

  KABC::Addressee::setSortMode( mode );
  qSort( *this );
  KABC::Addressee::setSortMode( 0 );
}

SortingCriterion AddresseeList::sortingCriterion() const
{
  return d->mActiveSortingCriterion;
}

Field *AddresseeList::sortingField() const
{
  return sActiveField;
}
