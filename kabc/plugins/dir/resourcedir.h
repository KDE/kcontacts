/*
    This file is part of libkabc.
    Copyright (c) 2002 - 2003 Tobias Koenig <tokoe@kde.org>

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

#ifndef KABC_RESOURCEDIR_H
#define KABC_RESOURCEDIR_H

#include "kabc/resource.h"
#include <kdirwatch.h>
#include <sys/types.h>

namespace KABC {

class Format;

/**
  @internal
*/
class KABC_DIR_EXPORT ResourceDir : public Resource
{
  Q_OBJECT

  public:
    ResourceDir();
    explicit ResourceDir( const KConfigGroup &group );
    explicit ResourceDir( const QString &path, const QString &type = QLatin1String( "vcard" ) );
    ~ResourceDir();

    virtual void writeConfig( KConfigGroup &group );

    virtual bool doOpen();
    virtual void doClose();

    virtual Ticket *requestSaveTicket();
    virtual void releaseSaveTicket( Ticket *ticket );

    virtual bool load();
    virtual bool asyncLoad();
    virtual bool save( Ticket * ticket );
    virtual bool asyncSave( Ticket * ticket );

    /**
      Set path to be used for saving.
     */
    void setPath( const QString & );

    /**
      Return path used for loading and saving the address book.
     */
    QString path() const;

    /**
      Set the format by name.
     */
    void setFormat( const QString &format );

    /**
      Returns the format name.
     */
    QString format() const;

    /**
      Remove a addressee from its source.
      This method is mainly called by KABC::AddressBook.
     */
    virtual void removeAddressee( const Addressee &addr );

  private:
    class Private;
    Private *const d;

    Q_PRIVATE_SLOT( d, void pathChanged() )
};

}

#endif
