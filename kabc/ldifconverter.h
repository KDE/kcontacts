/*
    This file is part of libkabc.
    Copyright (c) 2003 Helge Deller <deller@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 only as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef KABC_LDIFCONVERTER_H
#define KABC_LDIFCONVERTER_H

#include <qstring.h>
#include <qdatetime.h>

#include "addressee.h"
#include "addresseelist.h"

namespace KABC {

    /**
     * A set of functions to convert a string with LDIF information to addressees and vice versa.
     * ble for searches on LDAP servers, for addressbook
     * import- and exportfilters and might be used to read and write Mozilla and Netscape addresssbooks.
     */

  namespace LDIFConverter {

  /**
   * Converts a LDIF string to a list of addressees.
   *
   * @param str     The vcard string.
   * @param addr    The addresseelist.
   * @param dt      The date & time value of the last modification (e.g. file modification time).
   * @since 3.2
   */
  bool LDIFToAddressee( const QString &str, AddresseeList &addrList, QDateTime dt = QDateTime::currentDateTime() );

  /**
   * Converts a list of addressees to a LDIF string.
   *
   * @param addr    The addresseelist.
   * @param str     The LDIF string.
   * @since 3.2
   */
  bool addresseeToLDIF( const AddresseeList &addrList, QString &str );

  /**
   * Converts an addressee to a LDIF string.
   *
   * @param addr    The addressee.
   * @param str     The LDIF string.
   * @since 3.2
   */
  bool addresseeToLDIF( const Addressee &addr, QString &str );

  /**
   * Returns a LDAP search string based on the contents of an addressee.
   *
   * @param addr    The addressee.
   * @param str     The LDAP search string.
   * @since 3.2
   */
  // bool createLDAPSearchString( const Addressee &addr, QString &str );

  /**
   * Returns a LDIF compatible string representing a given field/value pair.
   * If necessary, the value parameter will be base64encoded and split into multiple.
   * This function will return an empty string if the given value is empty.
   *
   * @param field   The LDAP field name or a complete LDIF field string (e.g. "cn" or "cn = %1\n").
   * @param value   The value for this field.
   * @param allowEncode Set to false if you wish no encoding of the value.
   * @since 3.2
   */
  QString makeLDIFfieldString( QString field, QString value, bool allowEncode = true );



  /* internal functions - do not use !! */

  bool parseSingleLine( Addressee &a,
		Address &homeAddr, Address &workAddr, QString &line );


  bool splitLine( QString &line, QString &fieldname, QString &value);


  bool evaluatePair( Addressee &a, Address &homeAddr, Address &workAddr,
	QString &fieldname, QString &value );

  }

}
#endif

