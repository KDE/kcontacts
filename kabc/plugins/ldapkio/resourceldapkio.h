/*
    This file is part of libkabc.
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>
    Copyright (c) 2004 Szombathelyi György <gyurco@freemail.hu>

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

#ifndef KABC_RESOURCELDAPKIO_H
#define KABC_RESOURCELDAPKIO_H

#include <kio/job.h>
#include "kabc/resource.h"

namespace KABC {

class KABC_LDAPKIO_EXPORT ResourceLDAPKIO : public Resource
{
  Q_OBJECT

  public:
    enum CachePolicy {
      Cache_No,
      Cache_NoConnection,
      Cache_Always
    };

    ResourceLDAPKIO();
    explicit ResourceLDAPKIO( const KConfigGroup &group );
    virtual ~ResourceLDAPKIO();
    /**
     *  Call this after you used one of the set... methods
     */
    virtual void init();

    virtual void writeConfig( KConfigGroup &group );

    virtual bool doOpen();
    virtual void doClose();

    virtual Ticket *requestSaveTicket();
    virtual void releaseSaveTicket( Ticket *ticket );

    virtual bool readOnly() const { return Resource::readOnly(); }
    virtual void setReadOnly( bool value );

    virtual bool load();
    virtual bool asyncLoad();
    virtual bool save( Ticket *ticket );
    virtual bool asyncSave( Ticket *ticket );

    virtual void removeAddressee( const Addressee &addr );

    void setUser( const QString &user );
    QString user() const;

    void setPassword( const QString &password );
    QString password() const;

    void setRealm( const QString &realm );
    QString realm() const;

    void setBindDN( const QString &binddn );
    QString bindDN() const;

    void setDn( const QString &dn );
    QString dn() const;

    void setHost( const QString &host );
    QString host() const;

    void setPort( int port );
    int port() const;

    void setVer( int ver );
    int ver() const;

    void setSizeLimit( int sizelimit );
    int sizeLimit();

    void setTimeLimit( int timelimit );
    int timeLimit();

    void setFilter( const QString &filter );
    QString filter() const;

    void setIsAnonymous( bool value );
    bool isAnonymous() const;

    void setAttributes( const QMap<QString, QString> &attributes );
    QMap<QString, QString> attributes() const;

    void setRDNPrefix( int value );
    int RDNPrefix() const;

    void setIsTLS( bool value );
    bool isTLS() const;

    void setIsSSL( bool value );
    bool isSSL() const;

    void setIsSubTree( bool value );
    bool isSubTree() const;

    void setIsSASL( bool value );
    bool isSASL() const;

    void setMech( const QString &mech );
    QString mech() const;

    void setCachePolicy( int pol );
    int cachePolicy() const;

    void setAutoCache( bool value );
    bool autoCache();

    QString cacheDst() const;

protected Q_SLOTS:
    void entries( KIO::Job *job, const KIO::UDSEntryList &list );
    void data( KIO::Job *job, const QByteArray &data );
    void result( KJob *job );
    void listResult( KJob *job );
    void syncLoadSaveResult( KJob *job );
    void saveResult( KJob *job );
    void saveData( KIO::Job *job, QByteArray &data );
    void loadCacheResult( KJob *job );

Q_SIGNALS:
    void leaveModality();

  private:
    class Private;
    Private *const d;
};

}

#endif
