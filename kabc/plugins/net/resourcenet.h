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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KABC_RESOURCENET_H
#define KABC_RESOURCENET_H

#include <sys/types.h>

#include "kabc/resource.h"

class QFile;
class QTimer;
class KTemporaryFile;

class KJob;

namespace KABC {

class Format;

/**
  @internal
*/
class KABC_NET_EXPORT ResourceNet : public Resource
{
  Q_OBJECT

  public:
    ResourceNet();
    explicit ResourceNet( const KConfigGroup &group );
    ResourceNet( const KUrl &url, const QString &format );
    ~ResourceNet();

    virtual void writeConfig( KConfigGroup &group );

    virtual bool doOpen();
    virtual void doClose();

    virtual Ticket *requestSaveTicket();
    virtual void releaseSaveTicket( Ticket* );

    virtual bool load();
    virtual bool asyncLoad();
    virtual bool save( Ticket* ticket );
    virtual bool asyncSave( Ticket* ticket );

    /**
      Set url of directory to be used for saving.
     */
    void setUrl( const KUrl & );

    /**
      Return url of directory used for loading and saving the address book.
     */
    KUrl url() const;

    /**
      Sets a new format by name.
     */
    void setFormat( const QString &name );

    /**
      Returns the format name.
     */
    QString format() const;

  protected:
    void init( const KUrl &url, const QString &format );

  private Q_SLOTS:
    void downloadFinished( KJob* );
    void uploadFinished( KJob* );

  private:
    bool clearAndLoad( QFile *file );
    void saveToFile( QFile *file );
    bool hasTempFile() const { return mTempFile != 0; }
    void abortAsyncLoading();
    void abortAsyncSaving();
    bool createLocalTempFile();
    void deleteLocalTempFile();
    void deleteStaleTempFile();

    Format *mFormat;
    QString mFormatName;

    KUrl mUrl;
    KTemporaryFile *mTempFile;

    class ResourceNetPrivate;
    ResourceNetPrivate *d;
};

}

#endif
