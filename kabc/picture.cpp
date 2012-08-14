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

#include "picture.h"

#include <QtCore/QBuffer>
#include <QtCore/QSharedData>

using namespace KABC;

class Picture::Private : public QSharedData
{
  public:
    Private()
      : mIntern( false )
    {
    }

    Private( const Private &other )
      : QSharedData( other )
    {
      mUrl = other.mUrl;
      mType = other.mType;
      mData = other.mData;
      mIntern = other.mIntern;
    }

    QString mUrl;
    QString mType;
    QImage mData;

    bool mIntern;
};

Picture::Picture()
  : d( new Private )
{
}

Picture::Picture( const QString &url )
  : d( new Private )
{
  d->mUrl = url;
}

Picture::Picture( const QImage &data )
  : d( new Private )
{
  d->mIntern = true;
  d->mData = data;
}

Picture::Picture( const Picture &other )
  : d( other.d )
{
}

Picture::~Picture()
{
}

Picture &Picture::operator=( const Picture &other )
{
  if ( this != &other ) {
    d = other.d;
  }

  return *this;
}

bool Picture::operator==( const Picture &p ) const
{
  if ( d->mIntern != p.d->mIntern ) {
    return false;
  }

  if ( d->mIntern ) {
    if ( d->mData != p.d->mData ) {
      return false;
    }
  } else {
    if ( d->mUrl != p.d->mUrl ) {
      return false;
    }
  }

  return true;
}

bool Picture::operator!=( const Picture &p ) const
{
  return !( p == *this );
}

bool Picture::isEmpty() const
{
  return
    ( ( d->mIntern == false && d->mUrl.isEmpty() ) ||
      ( d->mIntern == true && d->mData.isNull() ) );
}

void Picture::setUrl( const QString &url )
{
  d->mUrl = url;
  d->mIntern = false;
}

void Picture::setData( const QImage &data )
{
  d->mData = data;
  d->mIntern = true;
}

void Picture::setType( const QString &type )
{
  d->mType = type;
}

bool Picture::isIntern() const
{
  return d->mIntern;
}

QString Picture::url() const
{
  return d->mUrl;
}

QImage Picture::data() const
{
  return d->mData;
}

QString Picture::type() const
{
  return d->mType;
}

QString Picture::toString() const
{
  QString str;

  str += QLatin1String( "Picture {\n" );
  str += QString::fromLatin1( "  Type: %1\n" ).arg( d->mType );
  str += QString::fromLatin1( "  IsIntern: %1\n" ).
         arg( d->mIntern ? QLatin1String( "true" ) : QLatin1String( "false" ) );
  if ( d->mIntern ) {
    QByteArray data;
    QBuffer buffer( &data );
    buffer.open( QIODevice::WriteOnly );
    d->mData.save( &buffer, "PNG" );
    str += QString::fromLatin1( "  Data: %1\n" ).arg( QString::fromLatin1( data.toBase64() ) );
  } else {
    str += QString::fromLatin1( "  Url: %1\n" ).arg( d->mUrl );
  }
  str += QLatin1String( "}\n" );

  return str;
}

QDataStream &KABC::operator<<( QDataStream &s, const Picture &picture )
{
  return s << picture.d->mIntern << picture.d->mUrl << picture.d->mType << picture.d->mData;
}

QDataStream &KABC::operator>>( QDataStream &s, Picture &picture )
{
  s >> picture.d->mIntern >> picture.d->mUrl >> picture.d->mType >> picture.d->mData;

  return s;
}
