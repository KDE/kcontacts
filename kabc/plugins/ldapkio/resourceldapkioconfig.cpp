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

#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QRadioButton>

#include <kacceleratormanager.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kdialogbase.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <klineedit.h>
#include <kmessagebox.h>
#include <kvbox.h>

#include "resourceldapkio.h"

#include "resourceldapkioconfig.h"
#include "resourceldapkioconfig.moc"

using namespace KABC;

ResourceLDAPKIOConfig::ResourceLDAPKIOConfig( QWidget* parent )
    : KRES::ConfigWidget( parent )
{
  QBoxLayout *mainLayout = new QVBoxLayout( this );
  cfg = new LdapConfigWidget( 
        LdapConfigWidget::W_USER |
        LdapConfigWidget::W_PASS |
        LdapConfigWidget::W_BINDDN |
        LdapConfigWidget::W_REALM |
        LdapConfigWidget::W_HOST |
        LdapConfigWidget::W_PORT |
        LdapConfigWidget::W_VER |
        LdapConfigWidget::W_DN |
        LdapConfigWidget::W_FILTER |
        LdapConfigWidget::W_SECBOX |
        LdapConfigWidget::W_AUTHBOX |
        LdapConfigWidget::W_TIMELIMIT |
        LdapConfigWidget::W_SIZELIMIT,
        this );

  mSubTree = new QCheckBox( i18n( "Sub-tree query" ), this );
  KHBox *box = new KHBox( this );
  box->setSpacing( KDialog::spacingHint() );
  mEditButton = new QPushButton( i18n( "Edit Attributes..." ), box );
  mCacheButton = new QPushButton( i18n( "Offline Use..." ), box );

  mainLayout->addWidget( cfg );
  mainLayout->addWidget( mSubTree );
  mainLayout->addWidget( box );

  connect( mEditButton, SIGNAL( clicked() ), SLOT( editAttributes() ) );
  connect( mCacheButton, SIGNAL( clicked() ), SLOT( editCache() ) );
}

void ResourceLDAPKIOConfig::loadSettings( KRES::Resource *res )
{
  ResourceLDAPKIO *resource = dynamic_cast<ResourceLDAPKIO*>( res );

  if ( !resource ) {
    kDebug(5700) << "ResourceLDAPKIOConfig::loadSettings(): cast failed" << endl;
    return;
  }

  cfg->setUser( resource->user() );
  cfg->setPassword( resource->password() );
  cfg->setRealm( resource->realm() );
  cfg->setBindDN( resource->bindDN() );
  cfg->setHost( resource->host() );
  cfg->setPort(  resource->port() );
  cfg->setVer(  resource->ver() );
  cfg->setTimeLimit( resource->timeLimit() );
  cfg->setSizeLimit( resource->sizeLimit() );
  cfg->setDn( resource->dn() );
  cfg->setFilter( resource->filter() );
  cfg->setMech( resource->mech() );
  if ( resource->isTLS() ) cfg->setSecTLS();
  else if ( resource->isSSL() ) cfg->setSecSSL();
  else cfg->setSecNO();
  if ( resource->isAnonymous() ) cfg->setAuthAnon();
  else if ( resource->isSASL() ) cfg->setAuthSASL();
  else cfg->setAuthSimple();

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
    kDebug(5700) << "ResourceLDAPKIOConfig::saveSettings(): cast failed" << endl;
    return;
  }

  resource->setUser( cfg->user() );
  resource->setPassword( cfg->password() );
  resource->setRealm( cfg->realm() );
  resource->setBindDN( cfg->bindDN() );
  resource->setHost( cfg->host() );
  resource->setPort( cfg->port() );
  resource->setVer( cfg->ver() );
  resource->setTimeLimit( cfg->timeLimit() );
  resource->setSizeLimit( cfg->sizeLimit() );
  resource->setDn( cfg->dn() );
  resource->setFilter( cfg->filter() );
  resource->setIsAnonymous( cfg->isAuthAnon() );
  resource->setIsSASL( cfg->isAuthSASL() );
  resource->setMech( cfg->mech() );
  resource->setIsTLS( cfg->isSecTLS() );
  resource->setIsSSL( cfg->isSecSSL() );
  resource->setIsSubTree( mSubTree->isChecked() );
  resource->setAttributes( mAttributes );
  resource->setRDNPrefix( mRDNPrefix );
  resource->setCachePolicy( mCachePolicy );
  resource->init();

}

void ResourceLDAPKIOConfig::editAttributes()
{
  AttributesDialog dlg( mAttributes, mRDNPrefix, this );
  if ( dlg.exec() ) {
    mAttributes = dlg.attributes();
    mRDNPrefix = dlg.rdnprefix();
  }
}

void ResourceLDAPKIOConfig::editCache()
{
  LDAPUrl src;
  QStringList attr;

  src = cfg->url();
  src.setScope( mSubTree->isChecked() ? LDAPUrl::Sub : LDAPUrl::One );
  if (!mAttributes.empty()) {
    QMap<QString,QString>::Iterator it;
    QStringList attr;
    for ( it = mAttributes.begin(); it != mAttributes.end(); ++it ) {
      if ( !it.value().isEmpty() && it.key() != "objectClass" )
        attr.append( it.value() );
    }
    src.setAttributes( attr );
  }
  src.setExtension( "x-dir", "base" );
  OfflineDialog dlg( mAutoCache, mCachePolicy, src, mCacheDst, this );
  if ( dlg.exec() ) {
    mCachePolicy = dlg.cachePolicy();
    mAutoCache = dlg.autoCache();
  }

}

AttributesDialog::AttributesDialog( const QMap<QString, QString> &attributes,
                                    int rdnprefix,
                                    QWidget *parent, const char *name )
  : KDialogBase( Plain, i18n( "Attributes Configuration" ), Ok | Cancel,
                 Ok, parent, name, true, true )
{
  mNameDict.insert( "objectClass", i18n( "Object classes" ) );
  mNameDict.insert( "commonName",  i18n( "Common name" ) );
  mNameDict.insert( "formattedName", i18n( "Formatted name" ) );
  mNameDict.insert( "familyName", i18n( "Family name" ) );
  mNameDict.insert( "givenName", i18n( "Given name" ) );
  mNameDict.insert( "organization", i18n( "Organization" ) );
  mNameDict.insert( "title", i18n( "Title" ) );
  mNameDict.insert( "street", i18n( "Street" ) );
  mNameDict.insert( "state", i18n( "State" ) );
  mNameDict.insert( "city", i18n( "City" ) );
  mNameDict.insert( "postalcode", i18n( "Postal code" ) );
  mNameDict.insert( "mail", i18n( "Email" ) );
  mNameDict.insert( "mailAlias", i18n( "Email alias" ) );
  mNameDict.insert( "phoneNumber", i18n( "Telephone number" ) );
  mNameDict.insert( "telephoneNumber", i18n( "Work telephone number" ) );
  mNameDict.insert( "facsimileTelephoneNumber", i18n( "Fax number" ) );
  mNameDict.insert( "mobile", i18n( "Cell phone number" ) );
  mNameDict.insert( "pager", i18n( "Pager" ) );
  mNameDict.insert( "description", i18n( "Note" ) );
  mNameDict.insert( "uid", i18n( "UID" ) );
  mNameDict.insert( "jpegPhoto", i18n( "Photo" ) );

  // default map
  mDefaultMap.insert( "objectClass", "inetOrgPerson" );
  mDefaultMap.insert( "commonName", "cn" );
  mDefaultMap.insert( "formattedName", "displayName" );
  mDefaultMap.insert( "familyName", "sn" );
  mDefaultMap.insert( "givenName", "givenName" );
  mDefaultMap.insert( "title", "title" );
  mDefaultMap.insert( "street", "street" );
  mDefaultMap.insert( "state", "st" );
  mDefaultMap.insert( "city", "l" );
  mDefaultMap.insert( "organization", "o" );
  mDefaultMap.insert( "postalcode", "postalCode" );
  mDefaultMap.insert( "mail", "mail" );
  mDefaultMap.insert( "mailAlias", "" );
  mDefaultMap.insert( "phoneNumber", "homePhone" );
  mDefaultMap.insert( "telephoneNumber", "telephoneNumber" );
  mDefaultMap.insert( "facsimileTelephoneNumber", "facsimileTelephoneNumber" );
  mDefaultMap.insert( "mobile", "mobile" );
  mDefaultMap.insert( "pager", "pager" );
  mDefaultMap.insert( "description", "description" );
  mDefaultMap.insert( "uid", "uid" );
  mDefaultMap.insert( "jpegPhoto", "jpegPhoto" );

  // overwrite the default values here
  QMap<QString, QString> kolabMap, netscapeMap, evolutionMap, outlookMap;

  // kolab
  kolabMap.insert( "formattedName", "display-name" );
  kolabMap.insert( "mailAlias", "mailalias" );

  // evolution
  evolutionMap.insert( "formattedName", "fileAs" );

  mMapList.append( attributes );
  mMapList.append( kolabMap );
  mMapList.append( netscapeMap );
  mMapList.append( evolutionMap );
  mMapList.append( outlookMap );

  QFrame *page = plainPage();
  QGridLayout *layout = new QGridLayout( page );

  QLabel *label = new QLabel( i18n( "Template:" ), page );
  layout->addWidget( label, 0, 0 );
  mMapCombo = new KComboBox( page );
  layout->addWidget( mMapCombo, 0, 1 );

  mMapCombo->addItem( i18n( "User Defined" ) );
  mMapCombo->addItem( i18n( "Kolab" ) );
  mMapCombo->addItem( i18n( "Netscape" ) );
  mMapCombo->addItem( i18n( "Evolution" ) );
  mMapCombo->addItem( i18n( "Outlook" ) );
  connect( mMapCombo, SIGNAL( activated( int ) ), SLOT( mapChanged( int ) ) );

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
    if ( mNameDict[ it.key() ] == 0 ) {
      i--;
      continue;
    }
    if ( (i - 2) == ( mNameDict.count()  >> 1 ) ) {
      i = 0;
      j = 2;
    }
    kDebug(7125) << "itkey: " << it.key() << " i: " << i << endl;
    label = new QLabel( mNameDict[ it.key() ] + ":", page );
    KLineEdit *lineedit = new KLineEdit( page );
    mLineEditDict.insert( it.key(), lineedit );
    lineedit->setText( it.value() );
    label->setBuddy( lineedit );
    layout->addWidget( label, i, j );
    layout->addWidget( lineedit, i, j+1 );
  }

  for ( i = 1; i < mMapCombo->count(); i++ ) {
	QHash<QString,KLineEdit*>::const_iterator it2 = mLineEditDict.constBegin();
	while (it2 != mLineEditDict.constEnd()){
      if ( mMapList[ i ].contains( it2.key() ) ) {
        if ( mMapList[ i ][ it2.key() ] != it2.value()->text() ) break;
      } else {
        if ( mDefaultMap[ it2.key() ] != it2.value()->text() ) break;
      }
	  ++it2;
    }
    if ( !it2 ) {
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
  while (it != mLineEditDict.constEnd())
  {
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
  for ( it = mDefaultMap.begin(); it != mDefaultMap.end(); ++it )
    mLineEditDict[ it.key() ]->setText( it.value() );

  for ( it = mMapList[ pos ].begin(); it != mMapList[ pos ].end(); ++it ) {
    if ( !it.value().isEmpty() ) {
      KLineEdit *le = mLineEditDict[ it.key() ];
      if ( le ) le->setText( it.value() );
    }
  }
}

OfflineDialog::OfflineDialog( bool autoCache, int cachePolicy, const KUrl &src,
  const QString &dst, QWidget *parent, const char *name )
  : KDialogBase( Plain, i18n( "Offline Configuration" ), Ok | Cancel,
                 Ok, parent, name, true, true )
{
  QFrame *page = plainPage();
  QVBoxLayout *layout = new QVBoxLayout( page );

  mSrc = src; mDst = dst;
  mCacheBox = new QGroupBox( i18n("Offline Cache Policy"), page );
  QVBoxLayout *cacheBoxLayout = new QVBoxLayout( mCacheBox );

  mCacheGroup = new QButtonGroup( this );

  QRadioButton *bt;
  bt = new QRadioButton( i18n("Do not use offline cache"), mCacheBox );
  cacheBoxLayout->addWidget( bt );
  mCacheGroup->addButton( bt );

  bt = new QRadioButton( i18n("Use local copy if no connection"), mCacheBox );
  cacheBoxLayout->addWidget( bt );
  mCacheGroup->addButton( bt );

  bt = new QRadioButton( i18n("Always use local copy"), mCacheBox );
  cacheBoxLayout->addWidget( bt );
  mCacheGroup->addButton( bt );

  if ( mCacheGroup->button( cachePolicy ) )
    mCacheGroup->button( cachePolicy )->setDown( true );

  mAutoCache = new QCheckBox( i18n("Refresh offline cache automatically"),
    page );
  mAutoCache->setChecked( autoCache );
  mAutoCache->setEnabled( bt->isChecked() );

  connect( bt, SIGNAL(toggled(bool)), mAutoCache, SLOT(setEnabled(bool)) );

  QPushButton *lcache = new QPushButton( i18n("Load into Cache"), page );
  connect( lcache, SIGNAL( clicked() ), SLOT( loadCache() ) );

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
      i18n("Successfully downloaded directory server contents!") );
  } else {
    KMessageBox::error( this,
      i18n("An error occurred downloading directory server contents into file %1.").arg(mDst) );
  }
}
