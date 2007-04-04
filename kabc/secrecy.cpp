/*
    This file is part of libkabc.
    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>

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

#include <QtCore/QDataStream>
#include <QtCore/QSharedData>

#include <klocale.h>

#include "secrecy.h"

using namespace KABC;

class Secrecy::PrivateData : public QSharedData
{
  public:
    PrivateData()
      : mType( Secrecy::Invalid )
    {
    }

    PrivateData( const PrivateData &other )
      : QSharedData( other )
    {
      mType = other.mType;
    }

    int mType;
};

Secrecy::Secrecy( int type )
  : d( new PrivateData )
{
  d->mType = type;
}

Secrecy::Secrecy( const Secrecy &other )
  : d( other.d )
{
}

Secrecy::~Secrecy()
{
}

Secrecy& Secrecy::operator=( const Secrecy &other )
{
  if ( this != &other )
    d = other.d;

  return *this;
}

bool Secrecy::operator==( const Secrecy &s ) const
{
  return ( d->mType == s.d->mType );
}

bool Secrecy::operator!=( const Secrecy &s ) const
{
  return !( *this == s );
}

bool Secrecy::isValid() const
{
  return d->mType != Invalid;
}

void Secrecy::setType( int type )
{
  d->mType = type;
}

int Secrecy::type() const
{
  return d->mType;
}

Secrecy::TypeList Secrecy::typeList()
{
  static TypeList list;

  if ( list.isEmpty() )
    list << Public << Private << Confidential;

  return list;
}

QString Secrecy::typeLabel( int type )
{
  switch ( type ) {
    case Public:
      return i18n( "Public" );
      break;
    case Private:
      return i18n( "Private" );
      break;
    case Confidential:
      return i18n( "Confidential" );
      break;
    default:
      return i18n( "Unknown type" );
      break;
  }
}

QString Secrecy::toString() const
{
  return QString( "Secrecy: %1" ).arg( typeLabel( d->mType ) );
}

QDataStream &KABC::operator<<( QDataStream &s, const Secrecy &secrecy )
{
    return s << secrecy.d->mType;
}

QDataStream &KABC::operator>>( QDataStream &s, Secrecy &secrecy )
{
    s >> secrecy.d->mType;

    return s;
}
