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
#include <kio/udsentry.h>
#include <kdebug.h>
#include <kde_file.h>
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
      KGlobal::locale()->insertCatalog( QLatin1String( "libkldap" ) );
    }

    KIO::TransferJob *loadFromCache();
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
  d->mCacheDst = KGlobal::dirs()->saveLocation( "cache", QLatin1String( "ldapkio" ) ) +
                 QLatin1Char( '/' ) + type() + QLatin1Char( '_' ) + identifier();
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
  d->mCacheDst = KGlobal::dirs()->saveLocation(
    "cache", QLatin1String( "ldapkio" ) ) + QLatin1Char( '/' ) +
                 type() + QLatin1Char( '_' ) + identifier();
  init();
}

ResourceLDAPKIO::~ResourceLDAPKIO()
{
  delete d;
}

void ResourceLDAPKIO::Private::enter_loop()
{
  QEventLoop eventLoop;
  mParent->connect( mParent, SIGNAL(leaveModality()), &eventLoop, SLOT(quit()) );
  eventLoop.exec( QEventLoop::ExcludeUserInputEvents );
}

void ResourceLDAPKIO::entries( KIO::Job *, const KIO::UDSEntryList &list )
{
  KIO::UDSEntryList::ConstIterator it = list.begin();
  KIO::UDSEntryList::ConstIterator end = list.end();
  for ( ; it != end; ++it ) {
    const QString urlStr = (*it).stringValue( KIO::UDSEntry::UDS_URL );
    if ( !urlStr.isEmpty() ) {
      KUrl tmpurl( urlStr );
      d->mResultDn = tmpurl.path();
      kDebug() << "findUid():" << d->mResultDn;
      if ( d->mResultDn.startsWith( QLatin1Char( '/' ) ) ) {
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
    d->mErrorMsg.clear();
  }
  emit leaveModality();
}

QString ResourceLDAPKIO::Private::findUid( const QString &uid )
{
  KLDAP::LdapUrl url( mLDAPUrl );
  KIO::UDSEntry entry;

  mErrorMsg.clear();
  mResultDn.clear();

  url.setAttributes( QStringList( QLatin1String( "dn" ) ) );
  url.setFilter( QLatin1Char( '(' ) + mAttributes[ QLatin1String( "uid" ) ] +
                 QLatin1Char( '=' ) + uid + QLatin1Char( ')' ) + mFilter );
  url.setExtension( QLatin1String( "x-dir" ), QLatin1String( "one" ) );

  kDebug() << uid << "url" << url.prettyUrl();

  KIO::ListJob *listJob = KIO::listDir( url, KIO::HideProgressInfo );
  mParent->connect( listJob, SIGNAL(entries(KIO::Job*,KIO::UDSEntryList)),
                    SLOT(entries(KIO::Job*,KIO::UDSEntryList)) );
  mParent->connect( listJob, SIGNAL(result(KJob*)),
                    mParent, SLOT(listResult(KJob*)) );

  enter_loop();
  return mResultDn;
}

QByteArray ResourceLDAPKIO::Private::addEntry( const QString &attr, const QString &value, bool mod )
{
  QByteArray tmp;
  if ( !attr.isEmpty() ) {
    if ( mod ) {
      tmp += KLDAP::Ldif::assembleLine( QLatin1String( "replace" ), attr ) + '\n';
    }
    if ( !value.isEmpty() ) {
      tmp += KLDAP::Ldif::assembleLine( attr, value ) + '\n';
    }
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
        dn = mAttributes[ QLatin1String( "uid" ) ] +
             QLatin1Char( '=' ) + addr.uid() + QLatin1Char( ',' ) + mDn;
        break;
      case 0:
      default:
        dn = mAttributes[ QLatin1String( "commonName" ) ] +
             QLatin1Char( '=' ) + addr.assembledName() + QLatin1Char( ',' ) + mDn;
        break;
    }
  } else {
    //modify existing entry
    mod = true;
    if ( olddn.startsWith( mAttributes[ QLatin1String( "uid" ) ] ) ) {
      dn = mAttributes[ QLatin1String( "uid" ) ] + QLatin1Char( '=' ) + addr.uid() +
           QLatin1Char( ',' ) + olddn.section( QLatin1Char( ',' ), 1 );
    } else if ( olddn.startsWith( mAttributes[ QLatin1String( "commonName" ) ] ) ) {
      dn = mAttributes[ QLatin1String( "commonName" ) ] +
           QLatin1Char( '=' ) + addr.assembledName()
           + QLatin1Char( ',' ) + olddn.section( QLatin1Char( ',' ), 1 );
    } else {
      dn = olddn;
    }

    if ( olddn.toLower() != dn.toLower() ) {
      tmp = KLDAP::Ldif::assembleLine( QLatin1String( "dn" ), olddn ) + '\n';
      tmp += "changetype: modrdn\n";
      tmp += KLDAP::Ldif::assembleLine( QLatin1String( "newrdn" ),
                                        dn.section( QLatin1Char( ',' ), 0, 0 ) ) + '\n';
      tmp += "deleteoldrdn: 1\n\n";
    }
  }

  tmp += KLDAP::Ldif::assembleLine( QLatin1String( "dn" ), dn ) + '\n';
  if ( mod ) {
    tmp += "changetype: modify\n";
  }
  if ( !mod ) {
    tmp += "objectClass: top\n";
    const QStringList obclass =
      mAttributes[ QLatin1String( "objectClass" ) ].split( QLatin1Char( ',' ),
                                                           QString::SkipEmptyParts );
    for ( QStringList::const_iterator it = obclass.constBegin(); it != obclass.constEnd(); ++it ) {
      tmp += KLDAP::Ldif::assembleLine( QLatin1String( "objectClass" ), *it ) + '\n';
    }
  }

  tmp += addEntry( mAttributes[ QLatin1String( "commonName" ) ], addr.assembledName(), mod );
  tmp += addEntry( mAttributes[ QLatin1String( "formattedName" ) ], addr.formattedName(), mod );
  tmp += addEntry( mAttributes[ QLatin1String( "givenName" ) ], addr.givenName(), mod );
  tmp += addEntry( mAttributes[ QLatin1String( "familyName" ) ], addr.familyName(), mod );
  tmp += addEntry( mAttributes[ QLatin1String( "uid" ) ], addr.uid(), mod );

  PhoneNumber number;
  number = addr.phoneNumber( PhoneNumber::Home );
  tmp += addEntry( mAttributes[ QLatin1String( "phoneNumber" ) ], number.number(), mod );
  number = addr.phoneNumber( PhoneNumber::Work );
  tmp += addEntry( mAttributes[ QLatin1String( "telephoneNumber" ) ], number.number(), mod );
  number = addr.phoneNumber( PhoneNumber::Fax );
  tmp += addEntry( mAttributes[ QLatin1String( "facsimileTelephoneNumber" ) ],
                   number.number(), mod );
  number = addr.phoneNumber( PhoneNumber::Cell );
  tmp += addEntry( mAttributes[ QLatin1String( "mobile" ) ], number.number(), mod );
  number = addr.phoneNumber( PhoneNumber::Pager );
  tmp += addEntry( mAttributes[ QLatin1String( "pager" ) ], number.number(), mod );

  tmp += addEntry( mAttributes[ QLatin1String( "description" ) ], addr.note(), mod );
  tmp += addEntry( mAttributes[ QLatin1String( "title" ) ], addr.title(), mod );
  tmp += addEntry( mAttributes[ QLatin1String( "organization" ) ], addr.organization(), mod );

  Address ad = addr.address( Address::Home );
  if ( !ad.isEmpty() ) {
    tmp += addEntry( mAttributes[ QLatin1String( "street" ) ], ad.street(), mod );
    tmp += addEntry( mAttributes[ QLatin1String( "state" ) ], ad.region(), mod );
    tmp += addEntry( mAttributes[ QLatin1String( "city" ) ], ad.locality(), mod );
    tmp += addEntry( mAttributes[ QLatin1String( "postalcode" ) ], ad.postalCode(), mod );
  }

  QStringList emails = addr.emails();
  QStringList::ConstIterator mailIt = emails.constBegin();

  if ( !mAttributes[ QLatin1String( "mail" ) ].isEmpty() ) {
    if ( mod ) {
      tmp += KLDAP::Ldif::assembleLine( QLatin1String( "replace" ),
                                        mAttributes[ QLatin1String( "mail" ) ] ) + '\n';
    }
    if ( mailIt != emails.constEnd() ) {
      tmp += KLDAP::Ldif::assembleLine( mAttributes[ QLatin1String( "mail" ) ], *mailIt ) + '\n';
      mailIt ++;
    }
    if ( mod &&
         mAttributes[ QLatin1String( "mail" ) ] != mAttributes[ QLatin1String( "mailAlias" ) ] ) {
      tmp += "-\n";
    }
  }

  if ( !mAttributes[ QLatin1String( "mailAlias" ) ].isEmpty() ) {
    if ( mod &&
         mAttributes[ QLatin1String( "mail" ) ] != mAttributes[ QLatin1String( "mailAlias" ) ] ) {
      tmp += KLDAP::Ldif::assembleLine( QLatin1String( "replace" ),
                                        mAttributes[ QLatin1String( "mailAlias" ) ] ) + '\n';
    }
    for ( ; mailIt != emails.constEnd(); ++mailIt ) {
      tmp += KLDAP::Ldif::assembleLine(
        mAttributes[ QLatin1String( "mailAlias" ) ], *mailIt ) + '\n';
    }
    if ( mod ) {
      tmp += "-\n";
    }
  }

  if ( !mAttributes[ QLatin1String( "jpegPhoto" ) ].isEmpty() ) {
    QByteArray pic;
    QBuffer buffer( &pic );
    buffer.open( QIODevice::WriteOnly );
    addr.photo().data().save( &buffer, "JPEG" );

    if ( mod ) {
      tmp += KLDAP::Ldif::assembleLine( QLatin1String( "replace" ),
                                        mAttributes[ QLatin1String( "jpegPhoto" ) ] ) + '\n';
    }
    tmp += KLDAP::Ldif::assembleLine( mAttributes[ QLatin1String( "jpegPhoto" ) ], pic, 76 ) + '\n';
    if ( mod ) {
      tmp += "-\n";
    }
  }

  tmp += '\n';
  kDebug() << "ldif:" << QString::fromUtf8( tmp );
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
  if ( !d->mAttributes.contains( QLatin1String( "objectClass" ) ) ) {
    d->mAttributes.insert( QLatin1String( "objectClass" ), QLatin1String( "inetOrgPerson" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "commonName" ) ) ) {
    d->mAttributes.insert( QLatin1String( "commonName" ), QLatin1String( "cn" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "formattedName" ) ) ) {
    d->mAttributes.insert( QLatin1String( "formattedName" ), QLatin1String( "displayName" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "familyName" ) ) ) {
    d->mAttributes.insert( QLatin1String( "familyName" ), QLatin1String( "sn" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "givenName" ) ) ) {
    d->mAttributes.insert( QLatin1String( "givenName" ), QLatin1String( "givenName" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "mail" ) ) ) {
    d->mAttributes.insert( QLatin1String( "mail" ), QLatin1String( "mail" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "mailAlias" ) ) ) {
    d->mAttributes.insert( QLatin1String( "mailAlias" ), QString() );
  }
  if ( !d->mAttributes.contains( QLatin1String( "phoneNumber" ) ) ) {
    d->mAttributes.insert( QLatin1String( "phoneNumber" ), QLatin1String( "homePhone" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "telephoneNumber" ) ) ) {
    d->mAttributes.insert( QLatin1String( "telephoneNumber" ), QLatin1String( "telephoneNumber" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "facsimileTelephoneNumber" ) ) ) {
    d->mAttributes.insert( QLatin1String( "facsimileTelephoneNumber" ),
                           QLatin1String( "facsimileTelephoneNumber" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "mobile" ) ) ) {
    d->mAttributes.insert( QLatin1String( "mobile" ), QLatin1String( "mobile" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "pager" ) ) ) {
    d->mAttributes.insert( QLatin1String( "pager" ), QLatin1String( "pager" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "description" ) ) ) {
    d->mAttributes.insert( QLatin1String( "description" ), QLatin1String( "description" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "title" ) ) ) {
    d->mAttributes.insert( QLatin1String( "title" ), QLatin1String( "title" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "street" ) ) ) {
    d->mAttributes.insert( QLatin1String( "street" ), QLatin1String( "street" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "state" ) ) ) {
    d->mAttributes.insert( QLatin1String( "state" ), QLatin1String( "st" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "city" ) ) ) {
    d->mAttributes.insert( QLatin1String( "city" ), QLatin1String( "l" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "organization" ) ) ) {
    d->mAttributes.insert( QLatin1String( "organization" ), QLatin1String( "o" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "postalcode" ) ) ) {
    d->mAttributes.insert( QLatin1String( "postalcode" ), QLatin1String( "postalCode" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "uid" ) ) ) {
    d->mAttributes.insert( QLatin1String( "uid" ), QLatin1String( "uid" ) );
  }
  if ( !d->mAttributes.contains( QLatin1String( "jpegPhoto" ) ) ) {
    d->mAttributes.insert( QLatin1String( "jpegPhoto" ), QLatin1String( "jpegPhoto" ) );
  }

  d->mLDAPUrl = KLDAP::LdapUrl( KUrl() );
  if ( !d->mAnonymous ) {
    d->mLDAPUrl.setExtension( QLatin1String( "bindname" ), d->mBindDN );
    d->mLDAPUrl.setUser( d->mUser );
    d->mLDAPUrl.setPass( d->mPassword );
  }
  d->mLDAPUrl.setProtocol( d->mSSL ? QLatin1String( "ldaps" ) : QLatin1String( "ldap" ) );
  d->mLDAPUrl.setHost( d->mHost );
  d->mLDAPUrl.setPort( d->mPort );
  d->mLDAPUrl.setDn( KLDAP::LdapDN( d->mDn ) );

  if ( !d->mAttributes.empty() ) {
    QMap<QString,QString>::Iterator it;
    QStringList attr;
    for ( it = d->mAttributes.begin(); it != d->mAttributes.end(); ++it ) {
      if ( !it.value().isEmpty() && it.key() != QLatin1String( "objectClass" ) ) {
        attr.append( it.value() );
      }
    }
    d->mLDAPUrl.setAttributes( attr );
  }

  d->mLDAPUrl.setScope( d->mSubTree ? KLDAP::LdapUrl::Sub : KLDAP::LdapUrl::One );
  if ( !d->mFilter.isEmpty() && d->mFilter != QLatin1String( "(objectClass=*)" ) ) {
    d->mLDAPUrl.setFilter( d->mFilter );
  }
  d->mLDAPUrl.setExtension( QLatin1String( "x-dir" ), QLatin1String( "base" ) );
  if ( d->mTLS ) {
    d->mLDAPUrl.setExtension( QLatin1String( "x-tls" ), QString() );
  }
  d->mLDAPUrl.setExtension( QLatin1String( "x-ver" ), QString::number( d->mVer ) );
  if ( d->mSizeLimit ) {
    d->mLDAPUrl.setExtension( QLatin1String( "x-sizelimit" ), QString::number( d->mSizeLimit ) );
  }
  if ( d->mTimeLimit ) {
    d->mLDAPUrl.setExtension( QLatin1String( "x-timelimit" ), QString::number( d->mTimeLimit ) );
  }
  if ( d->mSASL ) {
    d->mLDAPUrl.setExtension( QLatin1String( "x-sasl" ), QString() );
    if ( !d->mMech.isEmpty() ) {
      d->mLDAPUrl.setExtension( QLatin1String( "x-mech" ), d->mMech );
    }
    if ( !d->mRealm.isEmpty() ) {
      d->mLDAPUrl.setExtension( QLatin1String( "x-realm" ), d->mRealm );
    }
  }

  d->mReadOnly = readOnly();

  kDebug() << "resource_ldapkio url:" << d->mLDAPUrl.prettyUrl();
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
    kDebug() << "no addressbook";
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
    mTmp->setSuffix( QLatin1String( "tmp" ) );
    mTmp->open();
  }
}

void ResourceLDAPKIO::Private::activateCache()
{
  if ( mTmp && mError == 0 ) {
    QString filename = mTmp->fileName();
    delete mTmp;
    mTmp = 0;
    KDE_rename( QFile::encodeName( filename ), QFile::encodeName( mCacheDst ) );
  }
}

KIO::TransferJob *ResourceLDAPKIO::Private::loadFromCache()
{
  KIO::TransferJob *job = 0;
  if ( mCachePolicy == Cache_Always ||
     ( mCachePolicy == Cache_NoConnection &&
      mError == KIO::ERR_COULD_NOT_CONNECT ) ) {

    mAddr = Addressee();
    mAd = Address( Address::Home );
    //initialize ldif parser
    mLdif.startParsing();

    mParent->Resource::setReadOnly( true );

    KUrl url( mCacheDst );
    job = KIO::get( url, KIO::Reload, KIO::HideProgressInfo );
    mParent->connect( job, SIGNAL(data(KIO::Job*,QByteArray)),
                      mParent, SLOT(data(KIO::Job*,QByteArray)) );
  }

  return job;
}

bool ResourceLDAPKIO::load()
{
  kDebug();
  KIO::TransferJob *job;

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
    job = KIO::get( d->mLDAPUrl, KIO::Reload, KIO::HideProgressInfo );
    connect( job, SIGNAL(data(KIO::Job*,QByteArray)),
      this, SLOT(data(KIO::Job*,QByteArray)) );
    connect( job, SIGNAL(result(KJob*)),
      this, SLOT(syncLoadSaveResult(KJob*)) );
    d->enter_loop();
  }

  job = d->loadFromCache();
  if ( job ) {
    connect( job, SIGNAL(result(KJob*)),
      this, SLOT(syncLoadSaveResult(KJob*)) );
    d->enter_loop();
  }
  if ( d->mErrorMsg.isEmpty() ) {
    kDebug() << "ResourceLDAPKIO load ok!";
    return true;
  } else {
    kDebug() << "ResourceLDAPKIO load finished with error:" << d->mErrorMsg;
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
    KIO::TransferJob *job = KIO::get( d->mLDAPUrl, KIO::Reload, KIO::HideProgressInfo );
    connect( job, SIGNAL(data(KIO::Job*,QByteArray)),
      this, SLOT(data(KIO::Job*,QByteArray)) );
    connect( job, SIGNAL(result(KJob*)),
      this, SLOT(result(KJob*)) );
  } else {
    result( 0 );
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
        kDebug() << "new entry:" << d->mLdif.dn().toString();
        break;
      case KLDAP::Ldif::Item:
        name = d->mLdif.attr().toLower();
        value = d->mLdif.value();
        if ( name == d->mAttributes[ QLatin1String( "commonName" ) ].toLower() ) {
          if ( !d->mAddr.formattedName().isEmpty() ) {
            QString fn = d->mAddr.formattedName();
            d->mAddr.setNameFromString( QString::fromUtf8( value, value.size() ) );
            d->mAddr.setFormattedName( fn );
          } else {
            d->mAddr.setNameFromString( QString::fromUtf8( value, value.size() ) );
          }
        } else if ( name == d->mAttributes[ QLatin1String( "formattedName" ) ].toLower() ) {
          d->mAddr.setFormattedName( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "givenName" ) ].toLower() ) {
          d->mAddr.setGivenName( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "mail" ) ].toLower() ) {
          d->mAddr.insertEmail( QString::fromUtf8( value, value.size() ), true );
        } else if ( name == d->mAttributes[ QLatin1String( "mailAlias" ) ].toLower() ) {
          d->mAddr.insertEmail( QString::fromUtf8( value, value.size() ), false );
        } else if ( name == d->mAttributes[ QLatin1String( "phoneNumber" ) ].toLower() ) {
          PhoneNumber phone;
          phone.setNumber( QString::fromUtf8( value, value.size() ) );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ QLatin1String( "telephoneNumber" ) ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Work );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name ==
                    d->mAttributes[ QLatin1String( "facsimileTelephoneNumber" ) ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Fax );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ QLatin1String( "mobile" ) ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Cell );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ QLatin1String( "pager" ) ].toLower() ) {
          PhoneNumber phone( QString::fromUtf8( value, value.size() ),
            PhoneNumber::Pager );
          d->mAddr.insertPhoneNumber( phone );
        } else if ( name == d->mAttributes[ QLatin1String( "description" ) ].toLower() ) {
          d->mAddr.setNote( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "title" ) ].toLower() ) {
          d->mAddr.setTitle( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "street" ) ].toLower() ) {
          d->mAd.setStreet( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "state" ) ].toLower() ) {
          d->mAd.setRegion( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "city" ) ].toLower() ) {
          d->mAd.setLocality( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "postalcode" ) ].toLower() ) {
          d->mAd.setPostalCode( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "organization" ) ].toLower() ) {
          d->mAddr.setOrganization( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "familyName" ) ].toLower() ) {
          d->mAddr.setFamilyName( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "uid" ) ].toLower() ) {
          d->mAddr.setUid( QString::fromUtf8( value, value.size() ) );
        } else if ( name == d->mAttributes[ QLatin1String( "jpegPhoto" ) ].toLower() ) {
          KABC::Picture photo;
          QImage img = QImage::fromData( value );
          if ( !img.isNull() ) {
            photo.setData( img );
            photo.setType( QLatin1String( "image/jpeg" ) );
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

  KIO::TransferJob *cjob;
  cjob = d->loadFromCache();
  if ( cjob ) {
    connect( cjob, SIGNAL(result(KJob*)),
      this, SLOT(loadCacheResult(KJob*)) );
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
  kDebug();

  d->mSaveIt = begin();
  KIO::TransferJob *job = KIO::put( d->mLDAPUrl, -1, KIO::Overwrite | KIO::HideProgressInfo );
  connect( job, SIGNAL(dataReq(KIO::Job*,QByteArray&)),
    this, SLOT(saveData(KIO::Job*,QByteArray&)) );
  connect( job, SIGNAL(result(KJob*)),
    this, SLOT(syncLoadSaveResult(KJob*)) );
  d->enter_loop();
  if ( d->mErrorMsg.isEmpty() ) {
    kDebug() << "ResourceLDAPKIO save ok!";
    return true;
  } else {
    kDebug() << "ResourceLDAPKIO finished with error:" << d->mErrorMsg;
    addressBook()->error( d->mErrorMsg );
    return false;
  }
}

bool ResourceLDAPKIO::asyncSave( Ticket *ticket )
{
  Q_UNUSED( ticket );
  kDebug();
  d->mSaveIt = begin();
  KIO::TransferJob *job = KIO::put( d->mLDAPUrl, -1, KIO::Overwrite | KIO::HideProgressInfo );
  connect( job, SIGNAL(dataReq(KIO::Job*,QByteArray&)),
    this, SLOT(saveData(KIO::Job*,QByteArray&)) );
  connect( job, SIGNAL(result(KJob*)),
    this, SLOT(saveResult(KJob*)) );
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
    kDebug() << "ResourceLDAPKIO endData";
    data.resize( 0 );
    return;
  }

  kDebug() << "ResourceLDAPKIO saveData:" << (*d->mSaveIt).assembledName();

  d->AddresseeToLDIF( data, *d->mSaveIt, d->findUid( (*d->mSaveIt).uid() ) );
//  kDebug() << "ResourceLDAPKIO save LDIF:" << QString::fromUtf8(data);
  // mark as unchanged
  (*d->mSaveIt).setChanged( false );

  d->mSaveIt++;
}

void ResourceLDAPKIO::removeAddressee( const Addressee &addr )
{
  QString dn = d->findUid( addr.uid() );

  kDebug() << dn;

  if ( !d->mErrorMsg.isEmpty() ) {
    addressBook()->error( d->mErrorMsg );
    return;
  }
  if ( !dn.isEmpty() ) {
    kDebug() << "ResourceLDAPKIO: found uid:" << dn;
    KLDAP::LdapUrl url( d->mLDAPUrl );
    url.setPath( QLatin1Char( '/' ) + dn );
    url.setExtension( QLatin1String( "x-dir" ), QLatin1String( "base" ) );
    url.setScope( KLDAP::LdapUrl::Base );
    if ( KIO::NetAccess::del( url, 0 ) ) {
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
