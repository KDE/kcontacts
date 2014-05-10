/*
    This file is part of libkabc.

    Copyright (c) 2003 Cornelius Schumacher <schumacher@kde.org>

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

#include "locknull.h"
#include "kabc_debug.h"

#include <klocalizedstring.h>
#include <QDebug>

using namespace KABC;

class LockNull::Private
{
  public:
    Private( bool allowAccess )
      : mAllowAccess( allowAccess )
    {
    }

    bool mAllowAccess;
};

LockNull::LockNull( bool allowAccess )
  : Lock( QString() ), d( new Private( allowAccess ) )
{
}

LockNull::~LockNull()
{
  unlock();

  delete d;
}

bool LockNull::lock()
{
  if ( !d->mAllowAccess ) {
    return false;
  }

  qCWarning( KABC_LOG ) << "LockNull::lock() force success. Doesn't actually lock.";

  emit locked();

  return true;
}

bool LockNull::unlock()
{
  emit unlocked();
  return true;
}

QString LockNull::error() const
{
  if ( d->mAllowAccess ) {
    return i18n( "LockNull: All locks succeed but no actual locking is done." );
  } else {
    return i18n( "LockNull: All locks fail." );
  }
}
