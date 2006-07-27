/*
    This file is part of libkabc.
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

#ifndef KABC_LDAPCONFIGWIDGET_H
#define KABC_LDAPCONFIGWIDGET_H

#include <QWidget>
#include <QString>

#include <kio/job.h>

#include "kabc/ldapurl.h"
#include "kabc/ldif.h"

class QGridLayout;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QRadioButton;
class QProgressDialog;
class KComboBox;
class KLineEdit;

namespace KABC {

  /**
    @short LDAP Configuration widget

    This class can be used to query the user for LDAP connection parameters.
    It's KConfigXT compatible, using widget names starting with kcfg_
  */

  class KABC_EXPORT LdapConfigWidget : public QWidget
  {
    Q_OBJECT
    Q_FLAGS(WinFlags)
    Q_PROPERTY( WinFlags features READ features WRITE setFeatures )
    Q_PROPERTY( QString user READ user WRITE setUser )
    Q_PROPERTY( QString password READ password WRITE setPassword )
    Q_PROPERTY( QString bindDN READ bindDN WRITE setBindDN )
    Q_PROPERTY( QString realm READ realm WRITE setRealm )
    Q_PROPERTY( QString host READ host WRITE setHost )
    Q_PROPERTY( int port READ port WRITE setPort )
    Q_PROPERTY( int ver READ ver WRITE setVer )
    Q_PROPERTY( QString dn READ dn WRITE setDn )
    Q_PROPERTY( QString filter READ filter WRITE setFilter )
    Q_PROPERTY( QString mech READ mech WRITE setMech )
    Q_PROPERTY( bool secNO READ isSecNO WRITE setSecNO )
    Q_PROPERTY( bool secSSL READ isSecSSL WRITE setSecSSL )
    Q_PROPERTY( bool secTLS READ isSecSSL WRITE setSecTLS )
    Q_PROPERTY( bool authAnon READ isAuthAnon WRITE setAuthAnon )
    Q_PROPERTY( bool authSimple READ isAuthSimple WRITE setAuthSimple )
    Q_PROPERTY( bool authSASL READ isAuthSASL WRITE setAuthSASL )
    Q_PROPERTY( int sizeLimit READ sizeLimit WRITE setSizeLimit )
    Q_PROPERTY( int timeLimit READ timeLimit WRITE setTimeLimit )
    Q_PROPERTY( int pageSize READ pageSize WRITE setPageSize )

    public:

      enum WinFlag {
        W_USER = 0x1,
        W_PASS = 0x2,
        W_BINDDN = 0x4,
        W_REALM = 0x8,
        W_HOST = 0x10,
        W_PORT = 0x20,
        W_VER = 0x40,
        W_DN = 0x80,
        W_FILTER = 0x100,
        W_SECBOX = 0x200,
        W_AUTHBOX = 0x400,
        W_TIMELIMIT = 0x800,
        W_SIZELIMIT = 0x1000,
	W_PAGESIZE = 0x2000,
        W_ALL = 0x2fff
      };

      Q_DECLARE_FLAGS( WinFlags, WinFlag )

      /** Constructs an empty configuration widget.
       * You need to call setFlags() after this.
       */
      LdapConfigWidget( QWidget* parent = 0, Qt::WFlags fl = 0 );
      /** Constructs a configuration widget */
      LdapConfigWidget( WinFlags flags, QWidget* parent = 0,
                        Qt::WFlags fl = 0 );
      /** Destructs a configuration widget */
      virtual ~LdapConfigWidget();

      /** Sets the user name. Kconfig widget name: kcfg_ldapuser */
      void setUser( const QString &user );
      /** Gets the user name. Kconfig widget name: kcfg_ldapuser */
      QString user() const;

      /** Sets the password. Kconfig widget name: kcfg_ldappassword */
      void setPassword( const QString &password );
      /** Gets the password. Kconfig widget name: kcfg_ldappassword */
      QString password() const;

      /**
       * Sets the bind dn. Useful for SASL proxy auth.
       * Kconfig widget name: kcfg_ldapbinddn
       */
      void setBindDN( const QString &binddn );
      /** Gets the bind dn. Kconfig widget name: kcfg_ldapbinddn*/
      QString bindDN() const;

      /** Sets the SASL realm. Kconfig widget name: kcfg_ldaprealm */
      void setRealm( const QString &realm );
      /** Gets the SASL realm. Kconfig widget name: kcfg_ldaprealm */
      QString realm() const;

      /** Sets the host name. Kconfig widget name: kcfg_ldaphost */
      void setHost( const QString &host );
      /** Gets the host name. Kconfig widget name: kcfg_ldaphost */
      QString host() const;

      /** Sets the LDAP port. Kconfig widget name: kcfg_ldapport */
      void setPort( int port );
      /** Gets the LDAP port. Kconfig widget name: kcfg_ldapport */
      int port() const;

      /** Sets the LDAP protocol version. Kconfig widget name: kcfg_ldapver */
      void setVer( int ver );
      /** Gets the LDAP protocol version. Kconfig widget name: kcfg_ldapver */
      int ver() const;

      /** Sets the LDAP Base DN. Kconfig widget name: kcfg_ldapdn */
      void setDn( const QString &dn );
      /** Gets the LDAP Base DN. Kconfig widget name: kcfg_ldapdn */
      QString dn() const;

      /** Sets the LDAP Filter. Kconfig widget name: kcfg_ldapfilter */
      void setFilter( const QString &filter );
      /** Gets the LDAP Filter. Kconfig widget name: kcfg_ldapfilter */
      QString filter() const;

      /** Sets the SASL Mechanism. Kconfig widget name: kcfg_ldapsaslmech */
      void setMech( const QString &mech );
      /** Gets the SASL Mechanism. Kconfig widget name: kcfg_ldapsaslmech */
      QString mech() const;

      /**
       * Sets the configuration to no transport security.
       * Kconfig widget name: kcfg_ldapnosec
       */
      void setSecNO( bool b = true );
      /**
       * Returns true if no transport security selected.
       * Kconfig widget name: kcfg_ldapnosec
       */
      bool isSecNO() const;

      /**
       * Sets the configuration to TLS.
       * Kconfig widget name: kcfg_ldaptls
       */
      void setSecTLS( bool b = true );
      /**
       * Returns true if TLS selected.
       * Kconfig widget name: kcfg_ldaptls
       */
      bool isSecTLS() const;

      /**
       * Sets the configuration to SSL.
       * Kconfig widget name: kcfg_ldapssl
       */
      void setSecSSL( bool b = true );
      /**
       * Returns true if SSL selected.
       * Kconfig widget name: kcfg_ldapssl
       */
      bool isSecSSL() const;

      /**
       * Sets the authentication to anonymous.
       * Kconfig widget name: kcfg_ldapanon
       */
      void setAuthAnon( bool b = true );
      /**
       * Returns true if Anonymous authentication selected.
       * Kconfig widget name: kcfg_ldapanon
       */
      bool isAuthAnon() const;

      /**
       * Sets the authentication to simple.
       * Kconfig widget name: kcfg_ldapsimple
       */
      void setAuthSimple( bool b = true );
      /**
       * Returns true if Simple authentication selected.
       * Kconfig widget name: kcfg_ldapsimple
       */
      bool isAuthSimple() const;

      /**
       * Sets the authentication to SASL.
       * Kconfig widget name: kcfg_ldapsasl
       */
      void setAuthSASL( bool b = true );
      /**
       * Returns true if SASL authentication selected.
       * Kconfig widget name: kcfg_ldapsasl
       */
      bool isAuthSASL() const;

      /**
       * Sets the size limit.
       * KConfig widget name: kcfg_ldapsizelimit
       */
      void setSizeLimit( int sizelimit );
      /**
       * Returns the size limit.
       * KConfig widget name: kcfg_ldapsizelimit
       */
      int sizeLimit() const;

      /**
       * Sets the time limit.
       * KConfig widget name: kcfg_ldaptimelimit
       */
      void setTimeLimit( int timelimit );
      /**
       * Returns the time limit.
       * KConfig widget name: kcfg_ldaptimelimit
       */
      int timeLimit() const;

      /**
       * Sets the page size.
       * KConfig widget name: kcfg_ldappagesize
       */
      void setPageSize( int pagesize );
      /**
       * Returns the page size.
       * KConfig widget name: kcfg_ldappagesize
       */
      int pageSize() const;

      WinFlags features() const;
      void setFeatures( WinFlags features );

      /**
       * Returns a LDAP Url constructed from the settings given.
       * Extensions are filled for use in the LDAP ioslave
       */
      KABC::LDAPUrl url() const;

    private Q_SLOTS:
      void setLDAPPort();
      void setLDAPSPort();
      void setAnonymous( bool on );
      void setSimple( bool on );
      void setSASL( bool on );
      void mQueryDNClicked();
      void mQueryMechClicked();
      void loadData( KIO::Job*, const QByteArray& );
      void loadResult( KJob* );
    private:

      WinFlags mFeatures;
      KABC::LDIF mLdif;
      QStringList mQResult;
      QString mAttr;

      KLineEdit *mUser;
      KLineEdit *mPassword;
      KLineEdit *mHost;
      QSpinBox  *mPort, *mVer, *mSizeLimit, *mTimeLimit, *mPageSize;
      KLineEdit *mDn, *mBindDN, *mRealm;
      KLineEdit *mFilter;
      QRadioButton *mAnonymous,*mSimple,*mSASL;
      QCheckBox *mSubTree;
      QPushButton *mEditButton;
      QPushButton *mQueryMech;
      QRadioButton *mSecNO,*mSecTLS,*mSecSSL;
      KComboBox *mMech;

      QString mErrorMsg;
      bool mCancelled;
      QProgressDialog *mProg;

      QGridLayout *mainLayout;
      class LDAPConfigWidgetPrivate;
      LDAPConfigWidgetPrivate *d;

      void sendQuery();
      void initWidget();
  };

  Q_DECLARE_OPERATORS_FOR_FLAGS(LdapConfigWidget::WinFlags)

}

#endif
