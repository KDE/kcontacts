/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2003 Helge Deller <deller@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_LDIFCONVERTER_H
#define KCONTACTS_LDIFCONVERTER_H

#include "addressee.h"
#include "contactgroup.h"

#include <QDateTime>
#include <QString>

namespace KContacts
{
/*!
 * \namespace KContacts::LDIFConverter
 * \inheaderfile KContacts/LDIFConverter
 * \inmodule KContacts
 *
 * \brief A set of functions to convert a string with LDIF information to addressees
 * and vice versa.
 *
 * It is useful for addressbook import- and exportfilters
 * and might be used to read and write Mozilla and Netscape addresssbooks.
 */
namespace LDIFConverter
{
/*!
  Converts a LDIF string to a list of addressees.

  \a str The vcard string.

  \a addrList The addresseelist.

  \a contactGroupList the contactGroupList

  \a dt The date & time value of the last modification (e.g. file modification time).
*/
[[nodiscard]] KCONTACTS_EXPORT bool LDIFToAddressee(const QString &str,
                                                    AddresseeList &addrList,
                                                    QList<KContacts::ContactGroup> &contactGroupList,
                                                    const QDateTime &dt = QDateTime::currentDateTime());

/*!
  Converts a list of addressees to a LDIF string.

  \a addrList The addresseelist.

  \a str The LDIF string.
*/
[[nodiscard]] KCONTACTS_EXPORT bool addresseeToLDIF(const AddresseeList &addrList, QString &str);

/*!
  Converts a list of addressees and contactgrouplist to a LDIF string.

  \a addrList The addresseelist.

  \a contactGroupList The contact group list

  \a str The LDIF string.

*/
[[nodiscard]] KCONTACTS_EXPORT bool
addresseeAndContactGroupToLDIF(const AddresseeList &addrList, const QList<KContacts::ContactGroup> &contactGroupList, QString &str);
/*!
  Converts an addressee to a LDIF string.

  \a addr The addressee.

  \a str The LDIF string.
*/
[[nodiscard]] KCONTACTS_EXPORT bool addresseeToLDIF(const Addressee &addr, QString &str);

/*!
  Converts a list of contact group to a LDIF string.

  \a contactGroupList The contact group list

  \a str The LDIF string.
*/
[[nodiscard]] KCONTACTS_EXPORT bool contactGroupToLDIF(const ContactGroup::List &contactGroupList, QString &str);

/*!
  Converts a contact group to a LDIF string.

  \a contactGroup The contact group

  \a str The LDIF string.
*/
[[nodiscard]] KCONTACTS_EXPORT bool contactGroupToLDIF(const ContactGroup &contactGroup, QString &str);
}
}
#endif
