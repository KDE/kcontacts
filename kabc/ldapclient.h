/* kldapclient.h - LDAP access
 *      Copyright (C) 2002 Klar�lvdalens Datakonsult AB
 *
 *      Author: Steffen Hansen <hansen@kde.org>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */


#ifndef KABC_LDAPCLIENT_H
#define KABC_LDAPCLIENT_H


#include <qobject.h>
#include <qstring.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qmemarray.h>
#include <qguardedptr.h>
#include <qtimer.h>

#include <kio/job.h>

namespace KABC {

typedef QValueList<QByteArray> LdapAttrValue;
typedef QMap<QString,LdapAttrValue > LdapAttrMap;

/**
  * This class is internal. Binary compatibiliy might be broken any time 
  * without notification. Do not use it.
  *
  * We mean it!
  *
  */
class LdapObject
{
  public:
    LdapObject()
      : dn( QString::null ) {};  
    explicit LdapObject( QString _dn ) : dn( _dn ) {};
    LdapObject( const LdapObject& that ) { assign( that ); }
  
    LdapObject& operator=( const LdapObject& that )
    {
      assign( that );
      return *this;
    }

    QString toString() const;

    void clear();

    QString dn;
    LdapAttrMap attrs;

  protected:
    void assign( const LdapObject& that );

  private:
    class LdapObjectPrivate* d;
};

/**
  * This class is internal. Binary compatibiliy might be broken any time 
  * without notification. Do not use it.
  *
  * We mean it!
  *
  */
class LdapClient : public QObject
{
  Q_OBJECT

  public:
    LdapClient( QObject* parent = 0, const char* name = 0 );
    virtual ~LdapClient();

    /*! returns true if there is a query running */
    bool isActive() const { return mActive; }

  signals:
    /*! Emitted when the query is done */
    void done();

    /*! Emitted in case of error */
    void error( const QString& );

    /*! Emitted once for each object returned
     * from the query
     */
    void result( const KABC::LdapObject& );

  public slots:
    /*!
     * Set the name or IP of the LDAP server
     */
    void setHost( const QString& host );
    QString host() const { return mHost; }

    /*!
     * Set the port of the LDAP server
     * if using a nonstandard port
     */
    void setPort( const QString& port );
    QString port() const { return mPort; }

    /*!
     * Set the base DN 
     */
    void setBase( const QString& base );
    QString base() const { return mBase; }

    /*!
     * Set the bind DN 
     */
    void setBindDN( const QString& bindDN );
    QString bindDN() const { return mBindDN; }

    /*!
     * Set the bind password DN 
     */
    void setPwdBindDN( const QString& pwdBindDN );
    QString pwdBindDN() const { return mPwdBindDN; }

    /*! Set the attributes that should be
     * returned, or an empty list if
     * all attributes are wanted
     */
    void setAttrs( const QStringList& attrs );
    QStringList attrs() const { return mAttrs; }

    void setScope( const QString scope ) { mScope = scope; }

    /*!
     * Start the query with filter filter
     */
    void startQuery( const QString& filter );

    /*!
     * Abort a running query
     */
    void cancelQuery();

  protected slots:
    void slotData( KIO::Job*, const QByteArray &data );
    void slotInfoMessage( KIO::Job*, const QString &info );
    void slotDone();

  protected:
    void startParseLDIF();
    void parseLDIF( const QByteArray& data );
    void endParseLDIF();

    QString mHost;
    QString mPort;
    QString mBase;
    QString mBindDN;
    QString mPwdBindDN;
    QString mScope;
    QStringList mAttrs;
  
    QGuardedPtr<KIO::SimpleJob> mJob;
    bool mActive;
  
    LdapObject mCurrentObject;
    QCString mBuf;
    QCString mLastAttrName;
    QCString mLastAttrValue;
    bool mIsBase64;

  private:
    class LdapClientPrivate* d;
};


/**
  * This class is internal. Binary compatibiliy might be broken any time 
  * without notification. Do not use it.
  *
  * We mean it!
  *
  */
class LdapSearch : public QObject
{
  Q_OBJECT

  public:
    LdapSearch();

    void startSearch( const QString& txt );
    void cancelSearch();
    bool isAvailable() const;

  signals:
    void searchData( const QStringList& );
    void searchDone();

  private slots:
    void slotLDAPResult( const KABC::LdapObject& );
    void slotLDAPError( const QString& );
    void slotLDAPDone();
    void slotDataTimer();

  private:
    void finish();
    QStringList makeSearchData();
    QValueList< LdapClient* > mClients;
    QString mSearchText;
    QTimer mDataTimer;
    int mActiveClients;
    bool mNoLDAPLookup;
    QValueList< LdapObject > mResults;

  private:
    class LdapSearchPrivate* d;
};

}    
#endif // KABC_LDAPCLIENT_H
