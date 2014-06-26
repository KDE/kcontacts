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

#include "resourceldapkioconfig.h"
#include "resourceldapkio.h"

#include <kio/netaccess.h>
#include <kacceleratormanager.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <klocalizedstring.h>
#include <klineedit.h>
#include <kmessagebox.h>
#include <kpagewidget.h>
#include <kvbox.h>

#include <QtCore/QPointer>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QRadioButton>

using namespace KABC;

ResourceLDAPKIOConfig::ResourceLDAPKIOConfig( QWidget *parent )
  : KRES::ConfigWidget( parent )
{
  QBoxLayout *mainLayout = new QVBoxLayout( this );
  mainLayout->setMargin( 0 );

  mCfg = new KLDAP::LdapConfigWidget(
        KLDAP::LdapConfigWidget::W_USER |
        KLDAP::LdapConfigWidget::W_PASS |
        KLDAP::LdapConfigWidget::W_BINDDN |
        KLDAP::LdapConfigWidget::W_REALM |
        KLDAP::LdapConfigWidget::W_HOST |
        KLDAP::LdapConfigWidget::W_PORT |
        KLDAP::LdapConfigWidget::W_VER |
        KLDAP::LdapConfigWidget::W_DN |
        KLDAP::LdapConfigWidget::W_FILTER |
        KLDAP::LdapConfigWidget::W_TIMELIMIT |
        KLDAP::LdapConfigWidget::W_SIZELIMIT |
        KLDAP::LdapConfigWidget::W_SECBOX |
        KLDAP::LdapConfigWidget::W_AUTHBOX,
        this );

  mSubTree = new QCheckBox( i18n( "Sub-tree query" ), this );
  KHBox *box = new KHBox( this );
  box->setSpacing( -1 );
  mEditButton = new QPushButton( i18n( "Edit Attributes..." ), box );
  mCacheButton = new QPushButton( i18n( "Offline Use..." ), box );

  mainLayout->addWidget( mCfg );
  mainLayout->addWidget( mSubTree );
  mainLayout->addWidget( box );

  connect( mEditButton, SIGNAL(clicked()), SLOT(editAttributes()) );
  connect( mCacheButton, SIGNAL(clicked()), SLOT(editCache()) );
}

void ResourceLDAPKIOConfig::loadSettings( KRES::Resource *res )
{
  ResourceLDAPKIO *resource = dynamic_cast<ResourceLDAPKIO*>( res );

  if ( !resource ) {
    kDebug() << "cast failed";
    return;
  }

  mCfg->setUser( resource->user() );
  mCfg->setPassword( resource->password() );
  mCfg->setRealm( resource->realm() );
  mCfg->setBindDn( resource->bindDN() );
  mCfg->setHost( resource->host() );
  mCfg->setPort( resource->port() );
  mCfg->setVersion( resource->ver() );
  mCfg->setTimeLimit( resource->timeLimit() );
  mCfg->setSizeLimit( resource->sizeLimit() );
  mCfg->setDn( KLDAP::LdapDN( resource->dn() ) );
  mCfg->setFilter( resource->filter() );
  mCfg->setMech( resource->mech() );
  if ( resource->isTLS() ) {
    mCfg->setSecurity( KLDAP::LdapConfigWidget::TLS );
  } else if ( resource->isSSL() ) {
    mCfg->setSecurity( KLDAP::LdapConfigWidget::SSL );
  } else {
    mCfg->setSecurity( KLDAP::LdapConfigWidget::None );
  }
  if ( resource->isAnonymous() ) {
    mCfg->setAuth( KLDAP::LdapConfigWidget::Anonymous );
  } else if ( resource->isSASL() ) {
    mCfg->setAuth( KLDAP::LdapConfigWidget::SASL );
  } else {
    mCfg->setAuth( KLDAP::LdapConfigWidget::Simple );
  }
  mSubTree->setChecked( resource->isSubTree() );
  mAttributes = resource->attributes();
  mRDNPrefix = resource->RDNPrefix();
  mCachePolicy = resource->cachePolicy();
  mCacheDst = resource->cacheDst();
  mAutoCache = resource->autoCache();
}

void ResourceLDAPKIOConfig::saveSettings( KRES::Resource *res )
{
  ResourceLDAPKIO *resource = dynamic_cast<ResourceLDAPKIO*>( res );

  if ( !resource ) {
    kDebug() << "cast failed";
    return;
  }

  resource->setUser( mCfg->user() );
  resource->setPassword( mCfg->password() );
  resource->setRealm( mCfg->realm() );
  resource->setBindDN( mCfg->bindDn() );
  resource->setHost( mCfg->host() );
  resource->setPort( mCfg->port() );
  resource->setVer( mCfg->version() );
  resource->setTimeLimit( mCfg->timeLimit() );
  resource->setSizeLimit( mCfg->sizeLimit() );
  resource->setDn( mCfg->dn().toString() );
  resource->setFilter( mCfg->filter() );
  resource->setIsAnonymous( mCfg->auth() ==
                            KLDAP::LdapConfigWidget::Anonymous );
  resource->setIsSASL( mCfg->auth() == KLDAP::LdapConfigWidget::SASL );
  resource->setMech( mCfg->mech() );
  resource->setIsTLS( mCfg->security() == KLDAP::LdapConfigWidget::TLS );
  resource->setIsSSL( mCfg->security() == KLDAP::LdapConfigWidget::SSL );
  resource->setIsSubTree( mSubTree->isChecked() );
  resource->setAttributes( mAttributes );
  resource->setRDNPrefix( mRDNPrefix );
  resource->setCachePolicy( mCachePolicy );
  resource->init();

}

void ResourceLDAPKIOConfig::editAttributes()
{
  QPointer<AttributesDialog> dlg = new AttributesDialog( mAttributes, mRDNPrefix, this );
  if ( dlg->exec() && dlg ) {
    mAttributes = dlg->attributes();
    mRDNPrefix = dlg->rdnprefix();
  }

  delete dlg;
}

void ResourceLDAPKIOConfig::editCache()
{
  KLDAP::LdapUrl src;
  QStringList attr;

  src = mCfg->url();
  src.setScope( mSubTree->isChecked() ? KLDAP::LdapUrl::Sub : KLDAP::LdapUrl::One );
  if ( !mAttributes.empty() ) {
    QMap<QString,QString>::Iterator it;
    QStringList attr;
    for ( it = mAttributes.begin(); it != mAttributes.end(); ++it ) {
      if ( !it.value().isEmpty() && it.key() != QLatin1String( "objectClass" ) ) {
        attr.append( it.value() );
      }
    }
    src.setAttributes( attr );
  }
  src.setExtension( QLatin1String( "x-dir" ), QLatin1String( "base" ) );

  QPointer<OfflineDialog> dlg = new OfflineDialog( mAutoCache, mCachePolicy, src, mCacheDst, this );
  if ( dlg->exec() && dlg ) {
    mCachePolicy = dlg->cachePolicy();
    mAutoCache = dlg->autoCache();
  }

  delete dlg;
}

AttributesDialog::AttributesDialog( const QMap<QString, QString> &attributes,
                                    int rdnprefix,
                                    QWidget *parent )
  : KDialog( parent )
{
  setCaption( i18n( "Attributes Configuration" ) );
  setButtons( Ok | Cancel );
  setDefaultButton( Ok );
  setModal( true );
  showButtonSeparator( true );

  mNameDict.insert( QLatin1String( "objectClass" ), i18n( "Object classes" ) );
  mNameDict.insert( QLatin1String( "commonName" ), i18n( "Common name" ) );
  mNameDict.insert( QLatin1String( "formattedName" ), i18n( "Formatted name" ) );
  mNameDict.insert( QLatin1String( "familyName" ), i18n( "Family name" ) );
  mNameDict.insert( QLatin1String( "givenName" ), i18n( "Given name" ) );
  mNameDict.insert( QLatin1String( "organization" ), i18n( "Organization" ) );
  mNameDict.insert( QLatin1String( "title" ), i18nc( "job title", "Title" ) );
  mNameDict.insert( QLatin1String( "street" ), i18n( "Street" ) );
  mNameDict.insert( QLatin1String( "state" ), i18nc( "state/province", "State" ) );
  mNameDict.insert( QLatin1String( "city" ), i18n( "City" ) );
  mNameDict.insert( QLatin1String( "postalcode" ), i18n( "Postal code" ) );
  mNameDict.insert( QLatin1String( "mail" ), i18nc( "email address", "Email" ) );
  mNameDict.insert( QLatin1String( "mailAlias" ), i18n( "Email alias" ) );
  mNameDict.insert( QLatin1String( "phoneNumber" ), i18n( "Telephone number" ) );
  mNameDict.insert( QLatin1String( "telephoneNumber" ), i18n( "Work telephone number" ) );
  mNameDict.insert( QLatin1String( "facsimileTelephoneNumber" ), i18n( "Fax number" ) );
  mNameDict.insert( QLatin1String( "mobile" ), i18n( "Cell phone number" ) );
  mNameDict.insert( QLatin1String( "pager" ), i18n( "Pager" ) );
  mNameDict.insert( QLatin1String( "description" ), i18n( "Note" ) );
  mNameDict.insert( QLatin1String( "uid" ), i18n( "UID" ) );
  mNameDict.insert( QLatin1String( "jpegPhoto" ), i18n( "Photo" ) );

  // default map
  mDefaultMap.insert( QLatin1String( "objectClass" ), QLatin1String( "inetOrgPerson" ) );
  mDefaultMap.insert( QLatin1String( "commonName" ), QLatin1String( "cn" ) );
  mDefaultMap.insert( QLatin1String( "formattedName" ), QLatin1String( "displayName" ) );
  mDefaultMap.insert( QLatin1String( "familyName" ), QLatin1String( "sn" ) );
  mDefaultMap.insert( QLatin1String( "givenName" ), QLatin1String( "givenName" ) );
  mDefaultMap.insert( QLatin1String( "title" ), QLatin1String( "title" ) );
  mDefaultMap.insert( QLatin1String( "street" ), QLatin1String( "street" ) );
  mDefaultMap.insert( QLatin1String( "state" ), QLatin1String( "st" ) );
  mDefaultMap.insert( QLatin1String( "city" ), QLatin1String( "l" ) );
  mDefaultMap.insert( QLatin1String( "organization" ), QLatin1String( "o" ) );
  mDefaultMap.insert( QLatin1String( "postalcode" ), QLatin1String( "postalCode" ) );
  mDefaultMap.insert( QLatin1String( "mail" ), QLatin1String( "mail" ) );
  mDefaultMap.insert( QLatin1String( "mailAlias" ), QString() );
  mDefaultMap.insert( QLatin1String( "phoneNumber" ), QLatin1String( "homePhone" ) );
  mDefaultMap.insert( QLatin1String( "telephoneNumber" ), QLatin1String( "telephoneNumber" ) );
  mDefaultMap.insert( QLatin1String( "facsimileTelephoneNumber" ),
                      QLatin1String( "facsimileTelephoneNumber" ) );
  mDefaultMap.insert( QLatin1String( "mobile" ), QLatin1String( "mobile" ) );
  mDefaultMap.insert( QLatin1String( "pager" ), QLatin1String( "pager" ) );
  mDefaultMap.insert( QLatin1String( "description" ), QLatin1String( "description" ) );
  mDefaultMap.insert( QLatin1String( "uid" ), QLatin1String( "uid" ) );
  mDefaultMap.insert( QLatin1String( "jpegPhoto" ), QLatin1String( "jpegPhoto" ) );

  // overwrite the default values here
  QMap<QString, QString> kolab2Map, kolab3Map, netscapeMap, evolutionMap, outlookMap;

  // Kolab 2
  kolab2Map.insert( QLatin1String( "formattedName" ), QLatin1String( "display-name" ) );
  kolab2Map.insert( QLatin1String( "mailAlias" ), QLatin1String( "mailalias" ) );
  // Kolab 3
  kolab3Map.insert( QLatin1String( "mailAlias" ), QLatin1String( "alias" ) );

  // evolution
  evolutionMap.insert( QLatin1String( "formattedName" ), QLatin1String( "fileAs" ) );

  mMapList.append( attributes );
  mMapList.append( kolab2Map );
  mMapList.append( kolab3Map );
  mMapList.append( netscapeMap );
  mMapList.append( evolutionMap );
  mMapList.append( outlookMap );

  QFrame *page = new QFrame( this );
  setMainWidget( page );
  QGridLayout *layout = new QGridLayout( page );

  QLabel *label = new QLabel( i18n( "Template:" ), page );
  layout->addWidget( label, 0, 0 );
  mMapCombo = new KComboBox( page );
  layout->addWidget( mMapCombo, 0, 1 );

  mMapCombo->addItem( i18n( "User Defined" ) );
  mMapCombo->addItem( i18n( "Kolab 2" ) );
  mMapCombo->addItem( i18n( "Kolab 3" ) );
  mMapCombo->addItem( i18n( "Netscape" ) );
  mMapCombo->addItem( i18n( "Evolution" ) );
  mMapCombo->addItem( i18n( "Outlook" ) );
  connect( mMapCombo, SIGNAL(activated(int)), SLOT(mapChanged(int)) );

  label = new QLabel( i18n( "RDN prefix attribute:" ), page );
  layout->addWidget( label, 1, 0 );
  mRDNCombo = new KComboBox( page );
  layout->addWidget( mRDNCombo, 1, 1 );
  mRDNCombo->addItem( i18n( "commonName" ) );
  mRDNCombo->addItem( i18n( "UID" ) );
  mRDNCombo->setCurrentIndex( rdnprefix );

  QMap<QString, QString>::ConstIterator it;
  int i, j = 0;
  for ( i = 2, it = attributes.begin(); it != attributes.end(); ++it, ++i ) {
    if ( mNameDict[ it.key() ].isEmpty() ) {
      --i;
      continue;
    }
    if ( ( i - 2 ) == ( mNameDict.count() >> 1 ) ) {
      i = 0;
      j = 2;
    }
    kDebug() << "itkey:" << it.key() << "i:" << i;
    label = new QLabel( mNameDict[ it.key() ] + QLatin1Char( ':' ), page );
    KLineEdit *lineedit = new KLineEdit( page );
    mLineEditDict.insert( it.key(), lineedit );
    lineedit->setText( it.value() );
    label->setBuddy( lineedit );
    layout->addWidget( label, i, j );
    layout->addWidget( lineedit, i, j+1 );
  }

  for ( i = 1; i < mMapCombo->count(); ++i ) {
    QHash<QString,KLineEdit*>::const_iterator it2 = mLineEditDict.constBegin();
    while ( it2 != mLineEditDict.constEnd() ) {
      if ( mMapList[ i ].contains( it2.key() ) ) {
        if ( mMapList[ i ][ it2.key() ] != it2.value()->text() ) {
          break;
        }
      } else {
        if ( mDefaultMap[ it2.key() ] != it2.value()->text() ) {
          break;
        }
      }
      ++it2;
    }
    if ( it2 != mLineEditDict.constEnd() ) {
      mMapCombo->setCurrentIndex( i );
      break;
    }
  }

  KAcceleratorManager::manage( this );
}

AttributesDialog::~AttributesDialog()
{
  mNameDict.clear();
}

QMap<QString, QString> AttributesDialog::attributes() const
{
  QMap<QString, QString> map;

  QHash<QString,KLineEdit*>::const_iterator it = mLineEditDict.constBegin();
  while ( it != mLineEditDict.constEnd() ) {
    map.insert( it.key(), it.value()->text() );
    ++it;
  }
  return map;
}

int AttributesDialog::rdnprefix() const
{
  return mRDNCombo->currentIndex();
}

void AttributesDialog::mapChanged( int pos )
{

  // apply first the default and than the spezific changes
  QMap<QString, QString>::Iterator it;
  for ( it = mDefaultMap.begin(); it != mDefaultMap.end(); ++it ) {
    mLineEditDict[ it.key() ]->setText( it.value() );
  }

  for ( it = mMapList[ pos ].begin(); it != mMapList[ pos ].end(); ++it ) {
    if ( !it.value().isEmpty() ) {
      KLineEdit *le = mLineEditDict[ it.key() ];
      if ( le ) {
        le->setText( it.value() );
      }
    }
  }
}

OfflineDialog::OfflineDialog( bool autoCache, int cachePolicy, const KUrl &src,
  const QString &dst, QWidget *parent )
  : KDialog( parent )
{
  setCaption( i18n( "Offline Configuration" ) );
  setButtons( Ok | Cancel );
  setDefaultButton( Ok );
  setModal( true );
  showButtonSeparator( true );

  QFrame *page = new QFrame( this );
  setMainWidget( page );
  QVBoxLayout *layout = new QVBoxLayout( page );

  mSrc = src;
  mDst = dst;
  mCacheBox = new QGroupBox( i18n( "Offline Cache Policy" ), page );
  QVBoxLayout *cacheBoxLayout = new QVBoxLayout( mCacheBox );

  mCacheGroup = new QButtonGroup( this );

  QRadioButton *bt;
  bt = new QRadioButton( i18n( "Do not use offline cache" ), mCacheBox );
  cacheBoxLayout->addWidget( bt );
  bt->setDown( true );
  mCacheGroup->addButton( bt );

  bt = new QRadioButton( i18n( "Use local copy if no connection" ), mCacheBox );
  cacheBoxLayout->addWidget( bt );
  mCacheGroup->addButton( bt );

  bt = new QRadioButton( i18n( "Always use local copy" ), mCacheBox );
  cacheBoxLayout->addWidget( bt );
  mCacheGroup->addButton( bt );

  if ( mCacheGroup->button( cachePolicy ) ) {
    mCacheGroup->button( cachePolicy )->setDown( true );
  }

  mAutoCache = new QCheckBox( i18n( "Refresh offline cache automatically" ),
    page );
  mAutoCache->setChecked( autoCache );
  mAutoCache->setEnabled( bt->isChecked() );

  connect( bt, SIGNAL(toggled(bool)), mAutoCache, SLOT(setEnabled(bool)) );

  QPushButton *lcache = new QPushButton( i18n( "Load into Cache" ), page );
  connect( lcache, SIGNAL(clicked()), SLOT(loadCache()) );

  layout->addWidget( mCacheBox );
  layout->addWidget( mAutoCache );
  layout->addWidget( lcache );
}

OfflineDialog::~OfflineDialog()
{
}

bool OfflineDialog::autoCache() const
{
  return mAutoCache->isChecked();
}

int OfflineDialog::cachePolicy() const
{
  return mCacheGroup->checkedId();
}

void OfflineDialog::loadCache()
{
  if ( KIO::NetAccess::download( mSrc, mDst, this ) ) {
    KMessageBox::information( this,
      i18n( "Successfully downloaded directory server contents." ) );
  } else {
    KMessageBox::error( this,
      i18n( "An error occurred downloading directory server contents into file %1.", mDst ) );
  }
}
