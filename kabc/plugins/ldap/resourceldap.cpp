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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include <kdebug.h>
#include <kglobal.h>
#include <klineedit.h>
#include <klocale.h>
#include <kconfig.h>
#include <stdlib.h>

#include "resourceldap.h"
#include "resourceldapconfig.h"

using namespace KABC;

extern "C"
{
  KRES::ResourceConfigWidget *config_widget( QWidget *parent ) {
    KGlobal::locale()->insertCatalogue( "kabc_ldap" );
    return new ResourceLDAPConfig( parent, "ResourceLDAPConfig" );
  }

  Resource *resource( const KConfig *config ) {
    KGlobal::locale()->insertCatalogue( "kabc_ldap" );
    return new ResourceLDAP( config );
  }
}

void addModOp( LDAPMod ***pmods, const QString &attr, const QString &value );

ResourceLDAP::ResourceLDAP( const KConfig *config )
  : Resource( config )
{
  mLdap = 0;
  if ( config )
  {
     mUser = config->readEntry( "LdapUser" );
     mPassword = decryptStr( config->readEntry( "LdapPassword" ) );
     mDn = config->readEntry( "LdapDn" );
     mHost = config->readEntry( "LdapHost" );
     mPort = config->readNumEntry( "LdapPort", 389 );
     mFilter = config->readEntry( "LdapFilter" );
     mAnonymous = config->readBoolEntry( "LdapAnonymous" );
  }
  else mPort = 389;
}

void ResourceLDAP::writeConfig( KConfig *config )
{
  Resource::writeConfig( config );

  config->writeEntry( "LdapUser", mUser );
  config->writeEntry( "LdapPassword", encryptStr( mPassword ) );
  config->writeEntry( "LdapDn", mDn );
  config->writeEntry( "LdapHost", mHost );
  config->writeEntry( "LdapPort", mPort );
  config->writeEntry( "LdapFilter", mFilter );
  config->writeEntry( "LdapAnonymous", mAnonymous );
}

Ticket *ResourceLDAP::requestSaveTicket()
{
  if ( !addressBook() ) {
	  kdDebug(5700) << "no addressbook" << endl;
    return 0;
  }

  return createTicket( this );
}

bool ResourceLDAP::doOpen()
{
  if ( mLdap )
	  return false;

  if ( !mPort )
	  mPort = 389;

  mLdap = ldap_init( mHost.local8Bit(), mPort );
  if ( !mLdap ) {
	  addressBook()->error( i18n( "Unable to connect to server '%1' on port '%2'" ).arg( mHost ).arg( mPort ) );
	  return false;
  }

  if ( !mUser.isEmpty() && !mAnonymous ) {
	  if ( ldap_simple_bind_s( mLdap, mUser.local8Bit(), mPassword.local8Bit() ) != LDAP_SUCCESS ) {
	    addressBook()->error( i18n( "Unable to bind to server '%1'" ).arg( mHost ) );
      return false;
    }

    kdDebug(5700) << "ResourceLDAP: bind to server successfully" << endl;
  }

  int deref = LDAP_DEREF_ALWAYS;
  if ( ldap_set_option( mLdap, LDAP_OPT_DEREF, (void *) &deref ) != LDAP_OPT_SUCCESS ) {
    kdDebug(5700) << "ResourceLDAP: can't set 'deref' option" << endl;
    return false;
  }

  if ( ldap_set_option( mLdap, LDAP_OPT_REFERRALS, LDAP_OPT_ON ) != LDAP_OPT_SUCCESS ) {
    kdDebug(5700) << "ResourceLDAP: can't set 'referrals' option" << endl;
    return false;
  }

  return true;
}

void ResourceLDAP::doClose()
{
  if ( ldap_unbind_s( mLdap ) != LDAP_SUCCESS ) {
    kdDebug(5700) << "ResourceLDAP: can't unbind from server" << endl;
    return;
  }

  mLdap = 0;
}

bool ResourceLDAP::load()
{
  LDAPMessage *res;
  LDAPMessage *msg;
  BerElement *track;
  char *names;
  char **values;

  const char *LdapSearchAttr[ 9 ] = {
    "cn",
    "display-name",
    "givenname",
    "mail",
    "mailalias",
    "phoneNumber",
    "sn",
    "uid",
    0 };

  if ( ldap_search_s( mLdap, mDn.local8Bit(), LDAP_SCOPE_SUBTREE, QString( "(%1)" ).arg( mFilter ).local8Bit(),
      (char **)LdapSearchAttr, 0, &res ) != LDAP_SUCCESS ) {
    addressBook()->error( i18n( "Unable to search on server '%1'" ).arg( mHost ) );
    return false;
  }

  for ( msg = ldap_first_entry( mLdap, res ); msg; msg = ldap_next_entry( mLdap, msg ) ) {
    Addressee addr;
    addr.setResource( this );
    for ( names = ldap_first_attribute( mLdap, msg, &track ); names; names = ldap_next_attribute( mLdap, msg, track ) ) {
      values = ldap_get_values( mLdap, msg, names );
      for ( int i = 0; i < ldap_count_values( values ); ++i ) {
        QString name = QString::fromUtf8( names );
        QString value = QString::fromUtf8( values[ i ] );

        if ( name == "cn" ) {
          if ( !addr.formattedName().isEmpty() ) {
            QString fn = addr.formattedName();
            addr.setNameFromString( value );
            addr.setFormattedName( fn );
          } else
            addr.setNameFromString( value );
          continue;
        }
        if ( name == "display-name" ) {
          addr.setFormattedName( value );
          continue;
        }
        if ( name == "givenname" ) {
          addr.setGivenName( value );
          continue;
        }
        if ( name == "mail" ) {
          addr.insertEmail( value, true );
          continue;
        }
        if ( name == "mailalias" ) {
          addr.insertEmail( value, false );
          continue;
        }
        if ( name == "phoneNumber" ) {
          PhoneNumber phone;
          phone.setNumber( value );
          addr.insertPhoneNumber( phone );
          break; // read only the home number
        }
        if ( name == "sn" ) {
          addr.setFamilyName( value );
          continue;
        }
        if ( name == "uid" ) {
          addr.setUid( value );
          continue;
        }
      }
      ldap_value_free( values );
    }
    ber_free( track, 0 );

    addressBook()->insertAddressee( addr );
  }

  ldap_msgfree( res );

  return true;
}

bool ResourceLDAP::save( Ticket * )
{
  AddressBook::Iterator it;
  for ( it = addressBook()->begin(); it != addressBook()->end(); ++it ) {
    if ( (*it).resource() == this && (*it).changed() ) {
      LDAPMod **mods = NULL;

      addModOp( &mods, "objectClass", "organizationalPerson" );
      addModOp( &mods, "objectClass", "person" );
      addModOp( &mods, "objectClass", "Top" );
      addModOp( &mods, "cn", (*it).assembledName() );
      addModOp( &mods, "display-name", (*it).formattedName() );
      addModOp( &mods, "givenname", (*it).givenName() );
      addModOp( &mods, "sn", (*it).familyName() );
      addModOp( &mods, "uid", (*it).uid() );

      QStringList emails = (*it).emails();
      QStringList::ConstIterator mailIt;
      bool first = true;
      for ( mailIt = emails.begin(); mailIt != emails.end(); ++mailIt ) {
        if ( first ) {
          addModOp( &mods, "mail", (*mailIt) );
          first = false;
        } else
          addModOp( &mods, "mailalias", (*mailIt) );
      }

      PhoneNumber number = (*it).phoneNumber( PhoneNumber::Home );
      addModOp( &mods, "phoneNumber", number.number() );

      QString dn = "cn=" + (*it).assembledName() + "," + mDn;

      int retval;
      if ( (retval = ldap_add_s( mLdap, dn.local8Bit(), mods )) != LDAP_SUCCESS )
         addressBook()->error( i18n( "Unable to modify '%1' on server '%2'" ).arg( (*it).uid() ).arg( mHost ) );

      ldap_mods_free( mods, 1 );

      // mark as unchanged
      (*it).setChanged( false );
    }
  }

  return true;
}

void ResourceLDAP::removeAddressee( const Addressee &addr )
{
  LDAPMessage *res;
  LDAPMessage *msg;

  QString filter = QString( "(&(uid=%1)(%2))" ).arg( addr.uid() ).arg( mFilter );

  kdDebug(5700) << "ldap:removeAddressee" << filter << endl;

  ldap_search_s( mLdap, mDn.local8Bit(), LDAP_SCOPE_SUBTREE, filter.local8Bit(),
      0, 0, &res );

  for ( msg = ldap_first_entry( mLdap, res ); msg; msg = ldap_next_entry( mLdap, msg ) ) {
    char *dn = ldap_get_dn( mLdap, msg );
    kdDebug(5700) << "found " << dn << endl;
    if ( ldap_delete_s( mLdap, dn ) != LDAP_SUCCESS )
      addressBook()->error( i18n( "Unable to delete '%1' on server '%2'" ).arg( dn ).arg( mHost ) );
    ldap_memfree( dn );
  }

  ldap_msgfree( res );
}

void ResourceLDAP::setUser( const QString &user )
{
  mUser = user;
}

QString ResourceLDAP::user() const
{
  return mUser;
}

void ResourceLDAP::setPassword( const QString &password )
{
  mPassword = password;
}

QString ResourceLDAP::password() const
{
  return mPassword;
}

void ResourceLDAP::setDn( const QString &dn )
{
  mDn = dn;
}

QString ResourceLDAP::dn() const
{
  return mDn;
}

void ResourceLDAP::setHost( const QString &host )
{
  mHost = host;
}

QString ResourceLDAP::host() const
{
  return mHost;
}

void ResourceLDAP::setPort( int port )
{
  mPort = port;
}

int ResourceLDAP::port() const
{
  return mPort;
}

void ResourceLDAP::setFilter( const QString &filter )
{
  mFilter = filter;
}

QString ResourceLDAP::filter() const
{
  return mFilter;
}

void ResourceLDAP::setIsAnonymous( bool value )
{
  mAnonymous = value;
}

bool ResourceLDAP::isAnonymous() const
{
  return mAnonymous;
}

void addModOp( LDAPMod ***pmods, const QString &attr, const QString &value )
{
  if ( value.isNull() )
    return;

  LDAPMod	**mods;

  mods = *pmods;

  uint i = 0;
  if ( mods != 0 )
    for ( ; mods[ i ] != 0; ++i );

  if (( mods = (LDAPMod **)realloc( mods, (i + 2) * sizeof( LDAPMod * ))) == 0 ) {
    kdError() << "ResourceLDAP: realloc" << endl;
    return;
  }

  *pmods = mods;
  mods[ i + 1 ] = 0;

  mods[ i ] = new LDAPMod;

  mods[ i ]->mod_op = 0;
  mods[ i ]->mod_type = strdup( attr.utf8() );
  mods[ i ]->mod_values = new char*[2];
  mods[ i ]->mod_values[0] = strdup( value.utf8() );
  mods[ i ]->mod_values[1] = 0;
}

