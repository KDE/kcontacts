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

#include "sound.h"

#include <QtCore/QDataStream>
#include <QtCore/QSharedData>

using namespace KABC;

class Sound::Private : public QSharedData
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
      mData = other.mData;
      mIntern = other.mIntern;
    }

    QString mUrl;
    QByteArray mData;

    int mIntern;
};

Sound::Sound()
  : d( new Private )
{
}

Sound::Sound( const QString &url )
  : d( new Private )
{
  d->mUrl = url;
}

Sound::Sound( const QByteArray &data )
  : d( new Private )
{
  d->mIntern = true;
  d->mData = data;
}

Sound::Sound( const Sound &other )
  : d( other.d )
{
}

Sound::~Sound()
{
}

Sound& Sound::operator=( const Sound &other )
{
  if ( this != &other )
    d = other.d;

  return *this;
}

bool Sound::operator==( const Sound &s ) const
{
  if ( d->mIntern != s.d->mIntern ) return false;

  if ( d->mIntern ) {
    if ( d->mData != s.d->mData )
      return false;
  } else {
    if ( d->mUrl != s.d->mUrl )
      return false;
  }

  return true;
}

bool Sound::operator!=( const Sound &s ) const
{
  return !( s == *this );
}

void Sound::setUrl( const QString &url )
{
  d->mIntern = false;
  d->mUrl = url;
}

void Sound::setData( const QByteArray &data )
{
  d->mIntern = true;
  d->mData = data;
}

bool Sound::isIntern() const
{
  return d->mIntern;
}

bool Sound::isEmpty() const
{
  return (!d->mIntern) && d->mUrl.isEmpty();

}

QString Sound::url() const
{
  return d->mUrl;
}

QByteArray Sound::data() const
{
  return d->mData;
}

QString Sound::asString() const
{
  if ( d->mIntern )
    return "intern sound";
  else
    return d->mUrl;
}

QDataStream &KABC::operator<<( QDataStream &s, const Sound &sound )
{
  return s << sound.d->mIntern << sound.d->mUrl << sound.d->mData;
}

QDataStream &KABC::operator>>( QDataStream &s, Sound &sound )
{
  s >> sound.d->mIntern >> sound.d->mUrl >> sound.d->mData;

  return s;
}
