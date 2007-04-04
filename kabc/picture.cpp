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

#include <QtCore/QSharedData>

#include "picture.h"

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

    int mIntern;
};

Picture::Picture()
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

Picture& Picture::operator=( const Picture &other )
{
  if ( this != &other )
    d = other.d;

  return *this;
}

bool Picture::operator==( const Picture &p ) const
{
  if ( d->mIntern != p.d->mIntern ) return false;

  if ( d->mIntern ) {
    if ( d->mData != p.d->mData )
      return false;
  } else {
    if ( d->mUrl != p.d->mUrl )
      return false;
  }

  return true;
}

bool Picture::operator!=( const Picture &p ) const
{
  return !( p == *this );
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
  if ( d->mIntern )
    return "intern picture";
  else
    return d->mUrl;
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
