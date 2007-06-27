// -*- c-basic-offset: 2 -*-
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
#include "resourceldapkio.h"
#include "resourceldapkioconfig.h"

#include "kldap/ldif.h"
#include "kldap/ldapdn.h"
#include "kldap/ldapurl.h"

#include <kio/netaccess.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <klineedit.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstringhandler.h>
#include <ktemporaryfile.h>

#include <QtCore/QBuffer>
#include <QtCore/QEventLoop>
#include <QtCore/QFile>

#include <stdlib.h>

using namespace KABC;

class ResourceLDAPKIO::Private
{
  public:
    Private( ResourceLDAPKIO *parent )
      : mParent( parent ), mPort( 389 ), mAnonymous( true ), mTLS( false ),
        mSSL( false ), mSubTree( false ), mSASL( false ), mVer( 3 ),
        mRDNPrefix( 0 ), mTimeLimit( 0 ), mSizeLimit( 0 ),
        mCachePolicy( Cache_No ), mAutoCache( true )
    {
    }

    KIO::Job *loadFromCache();
    void createCache();
    void activateCache();
    void enter_loop();
    QByteArray addEntry( const QString &attr, const QString &value, bool mod );
    QString findUid( const QString &uid );
    bool AddresseeToLDIF( QByteArray &ldif, const Addressee &addr, const QString &olddn );

    ResourceLDAPKIO *mParent;
    QString mUser;
    QString mPassword;
    QString mDn;
    QString mHost;
    QString mFilter;
    int mPort;
    bool mAnonymous;
    QMap<QString, QString> mAttributes;

    QString mErrorMsg;

    KLDAP::Ldif mLdif;
    bool mTLS, mSSL, mSubTree;
    QString mResultDn;
    Addressee mAddr;
    Address mAd;
    Resource::Iterator mSaveIt;
    bool mSASL;
    QString mMech;
    QString mRealm, mBindDN;
    KLDAP::LdapUrl mLDAPUrl;
    int mVer;
    int mRDNPrefix;
    int mTimeLimit;
    int mSizeLimit;
    int mError;
    int mCachePolicy;
    bool mReadOnly;
    bool mAutoCache;
    QString mCacheDst;
    KTemporaryFile *mTmp;
};

ResourceLDAPKIO::ResourceLDAPKIO()
  : Resource(), d( new Private( this ) )
{
  d->mCacheDst = KGlobal::dirs()->saveLocation( "cache", "ldapkio" ) + '/' +
                 type() + '_' + identifier();
  init();
}

ResourceLDAPKIO::ResourceLDAPKIO( const KConfigGroup &group )
  : Resource( group ), d( new Private( this ) )
{
  QMap<QString, QString> attrList;
  QStringList attributes = group.readEntry( "LdapAttributes", QStringList() );
  for ( int pos = 0; pos < attributes.count(); pos += 2 ) {
    d->mAttributes.insert( attributes[ pos ], attributes[ pos + 1 ] );
  }

  d->mUser = group.readEntry( "LdapUser" );
  d->mPassword = KStringHandler::obscure( group.readEntry( "LdapPassword" ) );
  d->mDn = group.readEntry( "LdapDn" );
  d->mHost = group.readEntry( "LdapHost" );
  d->mPort = group.readEntry( "LdapPort", 389 );
  d->mFilter = group.readEntry( "LdapFilter" );
  d->mAnonymous = group.readEntry( "LdapAnonymous", false );
  d->mTLS = group.readEntry( "LdapTLS", false );
  d->mSSL = group.readEntry( "LdapSSL", false );
  d->mSubTree = group.readEntry( "LdapSubTree", false );
  d->mSASL = group.readEntry( "LdapSASL", false );
  d->mMech = group.readEntry( "LdapMech" );
  d->mRealm = group.readEntry( "LdapRealm" );
  d->mBindDN = group.readEntry( "LdapBindDN" );
  d->mVer = group.readEntry( "LdapVer", 3 );
  d->mTimeLimit = group.readEntry( "LdapTimeLimit", 0 );
  d->mSizeLimit = group.readEntry( "LdapSizeLimit", 0 );
  d->mRDNPrefix = group.readEntry( "LdapRDNPrefix", 0 );
  d->mCachePolicy = group.readEntry( "LdapCachePolicy", 0 );
  d->mAutoCache = group.readEntry( "LdapAutoCache", true );
  d->mCacheDst = KGlobal::dirs()->saveLocation( "cache", "ldapkio" ) + '/' +
                 type() + '_' + identifier();
  init();
}

ResourceLDAPKIO::~ResourceLDAPKIO()
{
  delete d;
}

void ResourceLDAPKIO::Private::enter_loop()
{
  QEventLoop eventLoop;
  mParent->connect( mParent, SIGNAL( leaveModality() ), &eventLoop, SLOT( quit() ) );
  eventLoop.exec( QEventLoop::ExcludeUserInputEvents );
}

void ResourceLDAPKIO::entries( KIO::Job*, const KIO::UDSEntryList & list )
{
  KIO::UDSEntryList::ConstIterator it = list.begin();
  KIO::UDSEntryList::ConstIterator end = list.end();
  for ( ; it != end; ++it ) {
    const QString urlStr = (*it).stringValue( KIO::UDS_URL );
    if ( !urlStr.isEmpty() ) {
      KUrl tmpurl( urlStr );
      d->mResultDn = tmpurl.path();
      kDebug(7125) << "findUid(): " << d->mResultDn << endl;
      if ( d->mResultDn.startsWith( '/' ) ) {
        d->mResultDn.remove( 0, 1 );
      }
      return;
    }
  }
}

void ResourceLDAPKIO::listResult( KJob *job )
{
  d->mError = job->error();
  if ( d->mError && d->mError != KIO::ERR_USER_CANCELED ) {
    d->mErrorMsg = job->errorString();
  } else {
    d->mErrorMsg = "";
  }
  emit leaveModality();
}

QString ResourceLDAPKIO::Private::findUid( const QString &uid )
{
  KLDAP::LdapUrl url( mLDAPUrl );
  KIO::UDSEntry entry;

  mErrorMsg.clear();
  mResultDn.clear();

  url.setAttributes( QStringList( "dn" ) );
  url.setFilter( '(' + mAttributes[ "uid" ] + '=' + uid + ')' + mFilter );
  url.setExtension( "x-dir", "one" );

  kDebug(7125) << "ResourceLDAPKIO::findUid() uid: " << uid << " url " <<
    url.prettyUrl() << endl;

  KIO::ListJob *listJob = KIO::listDir( url, false /* no GUI */);
  mParent->connect( listJob, SIGNAL( entries( KIO::Job *, const KIO::UDSEntryList& ) ),
                    SLOT( entries( KIO::Job*, const KIO::UDSEntryList& ) ) );
  mParent->connect( listJob, SIGNAL( result( KJob* ) ),
                    mParent, SLOT( listResult( KJob* ) ) );

  enter_loop();
  return mResultDn;
}

QByteArray ResourceLDAPKIO::Private::addEntry( const QString &attr, const QString &value, bool mod )
{
  QByteArray tmp;
  if ( !attr.isEmpty() ) {
    if ( mod ) {
      tmp += KLDAP::Ldif::assembleLine( "replace", attr ) + '\n';
    }
    tmp += KLDAP::Ldif::assembleLine( attr, value ) + '\n';
    if ( mod ) {
      tmp += "-\n";
    }
  }
  return tmp;
}

bool ResourceLDAPKIO::Private::AddresseeToLDIF( QByteArray &ldif, const Addressee &addr,
                                                const QString &olddn )
{
  QByteArray tmp;
  QString dn;
  QByteArray data;
  bool mod = false;

  if ( olddn.isEmpty() ) {
    //insert new entry
    switch ( mRDNPrefix ) {
      case 1:
        dn = mAttributes[ "uid" ] + '=' + addr.uid() + ',' + mDn;
        break;
      case 0:
      default:
        dn = mAttributes[ "commonName" ] + '=' + addr.assembledName() + ',' + mDn;
        break;
    }
  } else {
    //modify existing entry
    mod = true;
    if ( olddn.startsWith( mAttributes[ "uid" ] ) ) {
      dn = mAttributes[ "uid" ] + '=' + addr.uid() + ',' + olddn.section( ',', 1 );
    } else if ( olddn.startsWith( mAttributes[ "commonName" ] ) ) {
      dn = mAttributes[ "commonName" ] + '=' + addr.assembledName() + ',' +
           olddn.section( ',', 1 );
    } else {
      dn = olddn;
    }

    if ( olddn.toLower() != dn.toLower() ) {
      tmp = KLDAP::Ldif::assembleLine( "dn", olddn ) + '\n';
      tmp += "changetype: modrdn\n";
      tmp += KLDAP::Ldif::assembleLine( "newrdn", dn.section( ',', 0, 0 ) ) + '\n';
      tmp += "deleteoldrdn: 1\n\n";
    }
  }

  tmp += KLDAP::Ldif::assembleLine( "dn", dn ) + '\n';
  if ( mod ) {
    tmp += "changetype: modify\n";
  }
  if ( !mod ) {
    tmp += "objectClass: top\n";
    QStringList obclass = mAttributes[ "objectClass" ].split( ',', QString::SkipEmptyParts );
    for ( QStringList::const_iterator it = obclass.constBegin(); it != obclass.constEnd(); ++it ) {
      tmp += KLDAP::Ldif::assembleLine( "objectClass", *it ) + '\n';
    }
  }

  tmp += addEntry( mAttributes[ "commonName" ], addr.assembledName(), mod );
  tmp += addEntry( mAttributes[ "formattedName" ], addr.formattedName(), mod );
  tmp += addEntry( mAttributes[ "givenName" ], addr.givenName(), mod );
  tmp += addEntry( mAttributes[ "familyName" ], addr.familyName(), mod );
  tmp += addEntry( mAttributes[ "uid" ], addr.uid(), mod );

  PhoneNumber number;
  number = addr.phoneNumber( PhoneNumber::Home );
  tmp += addEntry( mAttributes[ "phoneNumber" ], number.number().toUtf8(), mod );
  number = addr.phoneNumber( PhoneNumber::Work );
  tmp += addEntry( mAttributes[ "telephoneNumber" ], number.number().toUtf8(), mod );
  number = addr.phoneNumber( PhoneNumber::Fax );
  tmp += addEntry( mAttributes[ "facsimileTelephoneNumber" ], number.number().toUtf8(), mod );
  number = addr.phoneNumber( PhoneNumber::Cell );
  tmp += addEntry( mAttributes[ "mobile" ], number.number().toUtf8(), mod );
  number = addr.phoneNumber( PhoneNumber::Pager );
  tmp += addEntry( mAttributes[ "pager" ], number.number().toUtf8(), mod );

  tmp += addEntry( mAttributes[ "description" ], addr.note(), mod );
  tmp += addEntry( mAttributes[ "title" ], addr.title(), mod );
  tmp += addEntry( mAttributes[ "organization" ], addr.organization(), mod );

  Address ad = addr.address( Address::Home );
  if ( !ad.isEmpty() ) {
    tmp += addEntry( mAttributes[ "street" ], ad.street(), mod );
    tmp += addEntry( mAttributes[ "state" ], ad.region(), mod );
    tmp += addEntry( mAttributes[ "city" ], ad.locality(), mod );
    tmp += addEntry( mAttributes[ "postalcode" ], ad.postalCode(), mod );
  }

  QStringList emails = addr.emails();
  QStringList::ConstIterator mailIt = emails.begin();

  if ( !mAttributes[ "mail" ].isEmpty() ) {
    if ( mod ) {
      tmp += KLDAP::Ldif::assembleLine( "replace", mAttributes[ "mail" ] ) + '\n';
    }
    if ( mailIt != emails.end() ) {
      tmp += KLDAP::Ldif::assembleLine( mAttributes[ "mail" ], *mailIt ) + '\n';
      mailIt ++;
    }
    if ( mod && mAttributes[ "mail" ] != mAttributes[ "mailAlias" ] ) {
      tmp += "-\n";
    }
  }

  if ( !mAttributes[ "mailAlias" ].isEmpty() ) {
    if ( mod && mAttributes[ "mail" ] != mAttributes[ "mailAlias" ] ) {
      tmp += KLDAP::Ldif::assembleLine( "replace", mAttributes[ "mailAlias" ] ) + '\n';
    }
    for ( ; mailIt != emails.end(); ++mailIt ) {
      tmp += KLDAP::Ldif::assembleLine( mAttributes[ "mailAlias" ], *mailIt ) + '\n';
    }
    if ( mod ) {
      tmp += "-\n";
    }
  }

  if ( !mAttributes[ "jpegPhoto" ].isEmpty() ) {
    QByteArray pic;
    QBuffer buffer( &pic );
    buffer.open( QIODevice::WriteOnly );
    addr.photo().data().save( &buffer, "JPEG" );

    if ( mod ) {
      tmp += KLDAP::Ldif::assembleLine( "replace", mAttributes[ "jpegPhoto" ] ) + '\n';
    }
    tmp += KLDAP::Ldif::assembleLine( mAttributes[ "jpegPhoto" ], pic, 76 ) + '\n';
    if ( mod ) {
      tmp += "-\n";
    }
  }

  tmp += '\n';
  kDebug(7125) << "ldif: " << QString::fromUtf8( tmp ) << endl;
  ldif = tmp;
  return true;
}

void ResourceLDAPKIO::setReadOnly( bool value )
{
  //save the original readonly flag, because offline using disables writing
  d->mReadOnly = true;
  Resource::setReadOnly( value );
}

void ResourceLDAPKIO::init()
{
  if ( d->mPort == 0 ) {
    d->mPort = 389;
  }

  /**
    If you want to add new attributes, append them here, add a
    translation string in the ctor of AttributesDialog and
    handle them in the load() method below.
    These are the default values
   */
  if ( !d->mAttributes.contains( "objectClass" ) ) {
    d->mAttributes.insert( "objectClass", "inetOrgPerson" );
  }
  if ( !d->mAttributes.contains( "commonName" ) ) {
    d->mAttributes.insert( "commonName", "cn" );
  }
  if ( !d->mAttributes.contains( "formattedName" ) ) {
    d->mAttributes.insert( "formattedName", "displayName" );
  }
  if ( !d->mAttributes.contains( "familyName" ) ) {
    d->mAttributes.insert( "familyName", "sn" );
  }
  if ( !d->mAttributes.contains( "givenName" ) ) {
    d->mAttributes.insert( "givenName", "givenName" );
  }
  if ( !d->mAttributes.contains( "mail" ) ) {
    d->mAttributes.insert( "mail", "mail" );
  }
  if ( !d->mAttributes.contains( "mailAlias" ) ) {
    d->mAttributes.insert( "mailAlias", "" );
  }
  if ( !d->mAttributes.contains( "phoneNumber" ) ) {
    d->mAttributes.insert( "phoneNumber", "homePhone" );
  }
  if ( !d->mAttributes.contains( "telephoneNumber" ) ) {
    d->mAttributes.insert( "telephoneNumber", "telephoneNumber" );
  }
  if ( !d->mAttributes.contains( "facsimileTelephoneNumber" ) ) {
    d->mAttributes.insert( "facsimileTelephoneNumber", "facsimileTelephoneNumber" );
  }
  if ( !d->mAttributes.contains( "mobile" ) ) {
    d->mAttributes.insert( "mobile", "mobile" );
  }
  if ( !d->mAttributes.contains( "pager" ) ) {
    d->mAttributes.insert( "pager", "pager" );
  }
  if ( !d->mAttributes.contains( "description" ) ) {
    d->mAttributes.insert( "description", "description" );
  }
  if ( !d->mAttributes.contains( "title" ) ) {
    d->mAttributes.insert( "title", "title" );
  }
  if ( !d->mAttributes.contains( "street" ) ) {
    d->mAttributes.insert( "street", "street" );
  }
  if ( !d->mAttributes.contains( "state" ) ) {
    d->mAttributes.insert( "state", "st" );
  }
  if ( !d->mAttributes.contains( "city" ) ) {
    d->mAttributes.insert( "city", "l" );
  }
  if ( !d->mAttributes.contains( "organization" ) ) {
    d->mAttributes.insert( "organization", "o" );
  }
  if ( !d->mAttributes.contains( "postalcode" ) ) {
    d->mAttributes.insert( "postalcode", "postalCode" );
  }
  if ( !d->mAttributes.contains( "uid" ) ) {
    d->mAttributes.insert( "uid", "uid" );
  }
  if ( !d->mAttributes.contains( "jpegPhoto" ) ) {
    d->mAttributes.insert( "jpegPhoto", "jpegPhoto" );
  }

  d->mLDAPUrl = KLDAP::LdapUrl( KUrl() );
  if ( !d->mAnonymous ) {
    d->mLDAPUrl.setUser( d->mUser );
    d->mLDAPUrl.setPass( d->mPassword );
  }
  d->mLDAPUrl.setProtocol( d->mSSL ? "ldaps" : "ldap" );
  d->mLDAPUrl.setHost( d->mHost );
  d->mLDAPUrl.setPort( d->mPort );
  d->mLDAPUrl.setDn( KLDAP::LdapDN( d->mDn ) );

  if ( !d->mAttributes.empty() ) {
    QMap<QString,QString>::Iterator it;
    QStringList attr;
    for ( it = d->mAttributes.begin(); it != d->mAttributes.end(); ++it ) {
      if ( !it.value().isEmpty() && it.key() != "objectClass" ) {
        attr.append( it.value() );
      }
    }
    d->mLDAPUrl.setAttributes( attr );
  }

  d->mLDAPUrl.setScope( d->mSubTree ? KLDAP::LdapUrl::Sub : KLDAP::LdapUrl::One );
  if ( !d->mFilter.isEmpty() && d->mFilter != "(objectClass=*)" ) {
    d->mLDAPUrl.setFilter( d->mFilter );
  }
  d->mLDAPUrl.setExtension( "x-dir", "base" );
  if ( d->mTLS ) {
    d->mLDAPUrl.setExtension( "x-tls", "" );
  }
  d->mLDAPUrl.setExtension( "x-ver", QString::number( d->mVer ) );
  if ( d->mSizeLimit ) {
    d->mLDAPUrl.setExtension( "x-sizelimit", QString::number( d->mSizeLimit ) );
  }
  if ( d->mTimeLimit ) {
    d->mLDAPUrl.setExtension( "x-timelimit", QString::number( d->mTimeLimit ) );
  }
  if ( d->mSASL ) {
    d->mLDAPUrl.setExtension( "x-sasl", "" );
    if ( !d->mBindDN.isEmpty() ) {
      d->mLDAPUrl.setExtension( "bindname", d->mBindDN );
    }
    if ( !d->mMech.isEmpty() ) {
      d->mLDAPUrl.setExtension( "x-mech", d->mMech );
    }
    if ( !d->mRealm.isEmpty() ) {
      d->mLDAPUrl.setExtension( "x-realm", d->mRealm );
    }
  }

  d->mReadOnly = readOnly();

  kDebug(7125) << "resource_ldapkio url: " << d->mLDAPUrl.prettyUrl() << endl;
}

void ResourceLDAPKIO::writeConfig( KConfigGroup &group )
{
  Resource::writeConfig( group );

  group.writeEntry( "LdapUser", d->mUser );
  group.writeEntry( "LdapPassword", KStringHandler::obscure( d->mPassword ) );
  group.writeEntry( "LdapDn", d->mDn );
  group.writeEntry( "LdapHost", d->mHost );
  group.writeEntry( "LdapPort", d->mPort );
  group.writeEntry( "LdapFilter", d->mFilter );
  group.writeEntry( "LdapAnonymous", d->mAnonymous );
  group.writeEntry( "LdapTLS", d->mTLS );
  group.writeEntry( "LdapSSL", d->mSSL );
  group.writeEntry( "LdapSubTree", d->mSubTree );
  group.writeEntry( "LdapSASL", d->mSASL );
  group.writeEntry( "LdapMech", d->mMech );
  group.writeEntry( "LdapVer", d->mVer );
  group.writeEntry( "LdapTimeLimit", d->mTimeLimit );
  group.writeEntry( "LdapSizeLimit", d->mSizeLimit );
  group.writeEntry( "LdapRDNPrefix", d->mRDNPrefix );
  group.writeEntry( "LdapRealm", d->mRealm );
  group.writeEntry( "LdapBindDN", d->mBindDN );
  group.writeEntry( "LdapCachePolicy", d->mCachePolicy );
  group.writeEntry( "LdapAutoCache", d->mAutoCache );

  QStringList attributes;
  QMap<QString, QString>::const_iterator it;
  for ( it = d->mAttributes.constBegin(); it != d->mAttributes.constEnd(); ++it ) {
    attributes << it.key() << it.value();
  }

  group.writeEntry( "LdapAttributes", attributes );
}

Ticket *ResourceLDAPKIO::requestSaveTicket()
{
  if ( !addressBook() ) {
    kDebug(7125) << "no addressbook" << endl;
    return 0;
  }

  return createTicket( this );
}

void ResourceLDAPKIO::releaseSaveTicket( Ticket *ticket )
{
  delete ticket;
}

bool ResourceLDAPKIO::doOpen()
{
  return true;
}

void ResourceLDAPKIO::doClose()
{
}

void ResourceLDAPKIO::Private::createCache()
{
  mTmp = 0;
  if ( mCachePolicy == Cache_NoConnection && mAutoCache ) {
    mTmp = new KTemporaryFile;
    mTmp->setPrefix( mCacheDst );
    mTmp->setSuffix( "tmp" );
    mTmp->open();
  }
}

void ResourceLDAPKIO::Private::activateCache()
{
  if ( mTmp && mError == 0 ) {
    QString filename = mTmp->fileName();
    delete mTmp;
    mTmp = 0;
    rename( QFile::encodeName( filename ), QFile::encodeName( mCacheDst ) );
  }
}

KIO::Job *ResourceLDAPKIO::Private::loadFromCache()
{
  KIO::Job *job = 0;
  if ( mCachePolicy == Cache_Always ||
     ( mCachePolicy == Cache_NoConnection &&
      mError == KIO::ERR_COULD_NOT_CONNECT ) ) {

    mAddr = Addressee();
    mAd = Address( Address::Home );
    //initialize ldif parser
    mLdif.startParsing();

    mParent->Resource::setReadOnly( true );

    KUrl url( mCacheDst );
    job = KIO::get( url, true, false );
    mParent->connect( job, SIGNAL( data( KIO::Job*, const QByteArray& ) ),
                      mParent, SLOT( data( KIO::Job*, const QByteArray& ) ) );
  }

  return job;
}

bool ResourceLDAPKIO::load()
{
  kDebug(7125) << "ResourceLDAPKIO::load()" << endl;
  KIO::Job *job;

  clear();
  //clear the addressee
  d->mAddr = Addressee();
  d->mAd = Address( Address::Home );
  //initialize ldif parser
  d->mLdif.startParsing();

  //set to original settings, offline use will disable writing
  Resource::setReadOnly( d->mReadOnly );

  d->createCache();
  if ( d->mCachePolicy != Cache_Always ) {
    job = KIO::get( d->mLDAPUrl, true, false );
    connect( job, SIGNAL( data( KIO::Job*, const QByteArray& ) ),
      this, SLOT( data( KIO::Job*, const QByteArray& ) ) );
    connect( job, SIGNAL( result( KJob* ) ),
      this, SLOT( syncLoadSaveResult( KJob* ) ) );
    d->enter_loop();
  }

  job = d->loadFromCache();
  if ( job ) {
    connect( job, SIGNAL( result( KJob* ) ),
      this, SLOT( syncLoadSaveResult( KJob* ) ) );
    d->enter_loop();
  }
  if ( d->mErrorMsg.isEmpty() ) {
    kDebug(7125) << "ResourceLDAPKIO load ok!" << endl;
    return true;
  } else {
    kDebug(7125) << "ResourceLDAPKIO load finished with error: " << d->mErrorMsg << endl;
    addressBook()->error( d->mErrorMsg );
    return false;
  }
}

bool ResourceLDAPKIO::asyncLoad()
{
  clear();
  //clear the addressee
  d->mAddr = Addressee();
  d->mAd = Address( Address::Home );
  //initialize ldif parser
  d->mLdif.startParsing();

  Resource::setReadOnly( d->mReadOnly );

  d->createCache();
  if ( d->mCachePolicy != Cache_Always ) {
    KIO::Job *job = KIO::get( d->mLDAPUrl, true, false );
    connect( job, SIGNAL( data( KIO::Job*, const QByteArray& ) ),
      this, SLOT( data( KIO::Job*, const QByteArray& ) ) );
    connect( job, SIGNAL( result( KJob* ) ),
      this, SLOT( result( KJob* ) ) );
  } else {
    result( NULL );
  }
  return true;
}

void ResourceLDAPKIO::data( KIO::Job *job, const QByteArray &data )
{
  Q_UNUSED( job );
  if ( data.size() ) {
    d->mLdif.setLdif( data );
    if ( d->mTmp ) {
      d->mTmp->write( data );
    }
  } else {
    d->mLdif.endLdif();
  }

  KLDAP::Ldif::ParseValue ret;
  QString name;
  QByteArray value;
  do {
    ret = d->mLdif.nextItem();
    switch ( ret ) {
      case KLDAP::Ldif::NewEntry:
        kDebug(7125) << "new entry: " << d->mLdif.dn().toString() << endl;
        break;
      case KLDAP::Ldif::Item:
        name = d->mLdif.attr().toLower();
        value = d->mLdif.value();
        if ( name == d->mAttributes[ "commonName" ].toLower() ) {
          if ( !d->mAddr.formattedName().isEmpty() ) {
            QString fn = d->mAddr.formattedName();
            d->mAddr.setNameFromString( QString::fromUtf8( value, value.size() ) );
            d->mAddr.setFormattedName( fn );
          } else {
            d->mAddr.setNameFromString( QString::fromUtf8( value, value.size() ) );
          }
        } else if ( name == d->mAttributes[ "formattedName" ].toLower() ) {
          d->mAddr.setFormattedName( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "givenName" ].toLower() ) {
          d->mAddr.setGivenName( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "mail" ].toLower() ) {
          d->mAddr.insertEmail( QString::fromUtf8( value, value.size() ), true );
        } else if ( name == d->mAttributes[ "mailAlias" ].toLower() ) {
          d->mAddr.insertEmail( QString::fromUtf8( value, value.size() ), false );
        } else if ( name == d->mAttributes[ "phoneNumber" ].toLower() ) {
          PhoneNumber phone;
          phone.setNumber( QString::fromUtf8( value, value.size() ) );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ "telephoneNumber" ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Work );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ "facsimileTelephoneNumber" ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Fax );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ "mobile" ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Cell );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ "pager" ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Pager );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ "description" ].toLower() ) {
          d->mAddr.setNote( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "title" ].toLower() ) {
          d->mAddr.setTitle( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "street" ].toLower() ) {
          d->mAd.setStreet( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "state" ].toLower() ) {
          d->mAd.setRegion( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "city" ].toLower() ) {
          d->mAd.setLocality( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "postalcode" ].toLower() ) {
          d->mAd.setPostalCode( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "organization" ].toLower() ) {
          d->mAddr.setOrganization( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "familyName" ].toLower() ) {
          d->mAddr.setFamilyName( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "uid" ].toLower() ) {
          d->mAddr.setUid( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ "jpegPhoto" ].toLower() ) {
          KABC::Picture photo;
          QImage img = QImage::fromData( value );
          if ( !img.isNull() ) {
            photo.setData( img );
            photo.setType( "image/jpeg" );
            d->mAddr.setPhoto( photo );
          }
        }

        break;
      case KLDAP::Ldif::EndEntry:
      {
        d->mAddr.setResource( this );
        d->mAddr.insertAddress( d->mAd );
        d->mAddr.setChanged( false );
        insertAddressee( d->mAddr );
        //clear the addressee
        d->mAddr = Addressee();
        d->mAd = Address( Address::Home );
      }
      break;
      default:
        break;
    }
  } while ( ret != KLDAP::Ldif::MoreData );
}

void ResourceLDAPKIO::loadCacheResult( KJob *job )
{
  d->mErrorMsg.clear();
  d->mError = job->error();
  if ( d->mError && d->mError != KIO::ERR_USER_CANCELED ) {
    d->mErrorMsg = job->errorString();
  }
  if ( !d->mErrorMsg.isEmpty() ) {
    emit loadingError( this, d->mErrorMsg );
  } else {
    emit loadingFinished( this );
  }
}

void ResourceLDAPKIO::result( KJob *job )
{
  d->mErrorMsg.clear();
  if ( job ) {
    d->mError = job->error();
    if ( d->mError && d->mError != KIO::ERR_USER_CANCELED ) {
      d->mErrorMsg = job->errorString();
    }
  } else {
    d->mError = 0;
  }
  d->activateCache();

  KIO::Job *cjob;
  cjob = d->loadFromCache();
  if ( cjob ) {
    connect( cjob, SIGNAL( result( KJob* ) ),
      this, SLOT( loadCacheResult( KJob* ) ) );
  } else {
    if ( !d->mErrorMsg.isEmpty() ) {
      emit loadingError( this, d->mErrorMsg );
    } else {
      emit loadingFinished( this );
    }
  }
}

bool ResourceLDAPKIO::save( Ticket *ticket )
{
  Q_UNUSED( ticket );
  kDebug(7125) << "ResourceLDAPKIO save" << endl;

  d->mSaveIt = begin();
  KIO::Job *job = KIO::put( d->mLDAPUrl, -1, true, false, false );
  connect( job, SIGNAL( dataReq( KIO::Job*, QByteArray& ) ),
    this, SLOT( saveData( KIO::Job*, QByteArray& ) ) );
  connect( job, SIGNAL( result( KJob* ) ),
    this, SLOT( syncLoadSaveResult( KJob* ) ) );
  d->enter_loop();
  if ( d->mErrorMsg.isEmpty() ) {
    kDebug(7125) << "ResourceLDAPKIO save ok!" << endl;
    return true;
  } else {
    kDebug(7125) << "ResourceLDAPKIO finished with error: " << d->mErrorMsg << endl;
    addressBook()->error( d->mErrorMsg );
    return false;
  }
}

bool ResourceLDAPKIO::asyncSave( Ticket *ticket )
{
  Q_UNUSED( ticket );
  kDebug(7125) << "ResourceLDAPKIO asyncSave" << endl;
  d->mSaveIt = begin();
  KIO::Job *job = KIO::put( d->mLDAPUrl, -1, true, false, false );
  connect( job, SIGNAL( dataReq( KIO::Job*, QByteArray& ) ),
    this, SLOT( saveData( KIO::Job*, QByteArray& ) ) );
  connect( job, SIGNAL( result( KJob* ) ),
    this, SLOT( saveResult( KJob* ) ) );
  return true;
}

void ResourceLDAPKIO::syncLoadSaveResult( KJob *job )
{
  d->mError = job->error();
  if ( d->mError && d->mError != KIO::ERR_USER_CANCELED ) {
    d->mErrorMsg = job->errorString();
  } else {
    d->mErrorMsg.clear();
  }
  d->activateCache();

  emit leaveModality();
}

void ResourceLDAPKIO::saveResult( KJob *job )
{
  d->mError = job->error();
  if ( d->mError && d->mError != KIO::ERR_USER_CANCELED ) {
    emit savingError( this, job->errorString() );
  } else {
    emit savingFinished( this );
  }
}

void ResourceLDAPKIO::saveData( KIO::Job *job, QByteArray &data )
{
  Q_UNUSED( job );
  while ( d->mSaveIt != end() && !(*d->mSaveIt).changed() ) {
    d->mSaveIt++;
  }

  if ( d->mSaveIt == end() ) {
    kDebug(7125) << "ResourceLDAPKIO endData" << endl;
    data.resize( 0 );
    return;
  }

  kDebug(7125) << "ResourceLDAPKIO saveData: " << (*d->mSaveIt).assembledName() << endl;

  d->AddresseeToLDIF( data, *d->mSaveIt, d->findUid( (*d->mSaveIt).uid() ) );
//  kDebug(7125) << "ResourceLDAPKIO save LDIF: " << QString::fromUtf8(data) << endl;
  // mark as unchanged
  (*d->mSaveIt).setChanged( false );

  d->mSaveIt++;
}

void ResourceLDAPKIO::removeAddressee( const Addressee &addr )
{
  QString dn = d->findUid( addr.uid() );

  kDebug(7125) << "ResourceLDAPKIO: removeAddressee: " << dn << endl;

  if ( !d->mErrorMsg.isEmpty() ) {
    addressBook()->error( d->mErrorMsg );
    return;
  }
  if ( !dn.isEmpty() ) {
    kDebug(7125) << "ResourceLDAPKIO: found uid: " << dn << endl;
    KLDAP::LdapUrl url( d->mLDAPUrl );
    url.setPath( '/' + dn );
    url.setExtension( "x-dir", "base" );
    url.setScope( KLDAP::LdapUrl::Base );
    if ( KIO::NetAccess::del( url, NULL ) ) {
      mAddrMap.remove( addr.uid() );
    }
  } else {
    //maybe it's not saved yet
    mAddrMap.remove( addr.uid() );
  }
}

void ResourceLDAPKIO::setUser( const QString &user )
{
  d->mUser = user;
}

QString ResourceLDAPKIO::user() const
{
  return d->mUser;
}

void ResourceLDAPKIO::setPassword( const QString &password )
{
  d->mPassword = password;
}

QString ResourceLDAPKIO::password() const
{
  return d->mPassword;
}

void ResourceLDAPKIO::setDn( const QString &dn )
{
  d->mDn = dn;
}

QString ResourceLDAPKIO::dn() const
{
  return d->mDn;
}

void ResourceLDAPKIO::setHost( const QString &host )
{
  d->mHost = host;
}

QString ResourceLDAPKIO::host() const
{
  return d->mHost;
}

void ResourceLDAPKIO::setPort( int port )
{
  d->mPort = port;
}

int ResourceLDAPKIO::port() const
{
  return d->mPort;
}

void ResourceLDAPKIO::setVer( int ver )
{
  d->mVer = ver;
}

int ResourceLDAPKIO::ver() const
{
  return d->mVer;
}

void ResourceLDAPKIO::setSizeLimit( int sizelimit )
{
  d->mSizeLimit = sizelimit;
}

int ResourceLDAPKIO::sizeLimit()
{
  return d->mSizeLimit;
}

void ResourceLDAPKIO::setTimeLimit( int timelimit )
{
  d->mTimeLimit = timelimit;
}

int ResourceLDAPKIO::timeLimit()
{
  return d->mTimeLimit;
}

void ResourceLDAPKIO::setFilter( const QString &filter )
{
  d->mFilter = filter;
}

QString ResourceLDAPKIO::filter() const
{
  return d->mFilter;
}

void ResourceLDAPKIO::setIsAnonymous( bool value )
{
  d->mAnonymous = value;
}

bool ResourceLDAPKIO::isAnonymous() const
{
  return d->mAnonymous;
}

void ResourceLDAPKIO::setIsTLS( bool value )
{
  d->mTLS = value;
}

bool ResourceLDAPKIO::isTLS() const
{
  return d->mTLS;
}
void ResourceLDAPKIO::setIsSSL( bool value )
{
  d->mSSL = value;
}

bool ResourceLDAPKIO::isSSL() const
{
  return d->mSSL;
}

void ResourceLDAPKIO::setIsSubTree( bool value )
{
  d->mSubTree = value;
}

bool ResourceLDAPKIO::isSubTree() const
{
  return d->mSubTree;
}

void ResourceLDAPKIO::setAttributes( const QMap<QString, QString> &attributes )
{
  d->mAttributes = attributes;
}

QMap<QString, QString> ResourceLDAPKIO::attributes() const
{
  return d->mAttributes;
}

void ResourceLDAPKIO::setRDNPrefix( int value )
{
  d->mRDNPrefix = value;
}

int ResourceLDAPKIO::RDNPrefix() const
{
  return d->mRDNPrefix;
}

void ResourceLDAPKIO::setIsSASL( bool value )
{
  d->mSASL = value;
}

bool ResourceLDAPKIO::isSASL() const
{
  return d->mSASL;
}

void ResourceLDAPKIO::setMech( const QString &mech )
{
  d->mMech = mech;
}

QString ResourceLDAPKIO::mech() const
{
  return d->mMech;
}

void ResourceLDAPKIO::setRealm( const QString &realm )
{
  d->mRealm = realm;
}

QString ResourceLDAPKIO::realm() const
{
  return d->mRealm;
}

void ResourceLDAPKIO::setBindDN( const QString &binddn )
{
  d->mBindDN = binddn;
}

QString ResourceLDAPKIO::bindDN() const
{
  return d->mBindDN;
}

void ResourceLDAPKIO::setCachePolicy( int pol )
{
  d->mCachePolicy = pol;
}

int ResourceLDAPKIO::cachePolicy() const
{
  return d->mCachePolicy;
}

void ResourceLDAPKIO::setAutoCache( bool value )
{
  d->mAutoCache = value;
}

bool ResourceLDAPKIO::autoCache()
{
  return d->mAutoCache;
}

QString ResourceLDAPKIO::cacheDst() const
{
  return d->mCacheDst;
}

#include "resourceldapkio.moc"
