/*
    This file is part of libkabc.
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>

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

#ifndef KABC_RESOURCENET_H
#define KABC_RESOURCENET_H

#include <kconfig.h>

#include <sys/types.h>

#include <kabc/resource.h>

class QTimer;
class KTempFile;

namespace KABC {

class FormatPlugin;

/**
  @internal
*/
class ResourceNet : public Resource
{
  public:
    ResourceNet( const KConfig* );
    ResourceNet( const KURL &url, const QString &format );
    ~ResourceNet();

    virtual void writeConfig( KConfig* );

    virtual bool doOpen();
    virtual void doClose();
  
    virtual Ticket *requestSaveTicket();

    virtual bool load();
    virtual bool save( Ticket* );

    /**
      Set url of directory to be used for saving.
     */
    void setUrl( const KURL & );

    /**
      Return url of directory used for loading and saving the address book.
     */
    KURL url() const;

    /**
      Sets a new format by name.
     */
    void setFormat( const QString &name );

    /**
      Returns the format name.
     */
    QString format() const;

    /**
      This method is called by an error handler if the application
      crashed
     */
    virtual void cleanUp();

  protected:
    void init( const KURL &url, const QString &format );

  private:
    FormatPlugin *mFormat;
    QString mFormatName;

    KURL mUrl;
    QString mTempFile;
    KTempFile *mLocalTempFile;
    bool mUseLocalTempFile;
};

}

#endif
