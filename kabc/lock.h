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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#ifndef KABC_LOCK_H
#define KABC_LOCK_H

#include <qstring.h>
#include <qobject.h>

namespace KABC {

/**
  This class provides locking functionality for a file, directory or an
  arbitrary string-represented resource.
*/
class Lock : public QObject
{
    Q_OBJECT
  public:
    /**
      Constructor.

      @param identifier An identifier for the resource to be locked, e.g. a file
                        name.
     */
    Lock( const QString &identifier );

    /**
      Lock resource.
    */
    bool lock();
    
    /**
      Unlock resource.
    */
    bool unlock();

    QString error() const;

  signals:
    void locked();
    void unlocked();

  private:
    QString mIdentifier;
    
    QString mLockUniqueName;

    QString mError;
};

}

#endif
