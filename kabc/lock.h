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
#ifndef KABC_LOCK_H
#define KABC_LOCK_H

#include "kabc_export.h"
#include <QtCore/QString>
#include <QtCore/QObject>

namespace KABC {

/**
  This class provides locking functionality for a file, directory or an
  arbitrary string-represented resource.
*/
class KABC_EXPORT Lock : public QObject
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
      Destruct lock object. This also removes the lock on the resource.
    */
    ~Lock();

    /**
      Lock resource.
    */
    virtual bool lock();

    /**
      Unlock resource.
    */
    virtual bool unlock();

    /**
      Returns the lastest error message.
    */
    virtual QString error() const;

    /**
      Returns the path of the lock file.

      The file will be located in the directory returned by locksDir()
      and have the file extension @c .lock
    */
    QString lockFileName() const;

    /**
      Reads the process ID and the application name from a lock file.

      @param filename The lock file to read from. Full path or relative to
                      current working directory
      @param pid The variable the process ID will be read into
      @param app The variable the application name will be read into

      @return @c false if @p filename could not be read, otherwise @c true, even
              when reading the values fails
    */
    static bool readLockFile( const QString &filename, int &pid, QString &app );

    /**
      Writes the process ID and the application name to a lock file.

      @param filename The lock file to write to

      @return @c false if the file could not be opened for writing, otherwise
              @c true
    */
    static bool writeLockFile( const QString &filename );

    /**
      Returns the path of the directory where locks are created.
    */
    static QString locksDir();

  Q_SIGNALS:
    /**
      Emitted after the lock has been locked.
    */
    void locked();
    /**
      Emitted after the lock has been unlocked.
    */
    void unlocked();

  private:
    class Private;
    Private *const d;
};

}

#endif
