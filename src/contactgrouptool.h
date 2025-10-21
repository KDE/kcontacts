/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2008 Tobias Koenig <tokoe@kde.org>
    SPDX-FileCopyrightText: 2008 Kevin Krammer <kevin.krammer@gmx.at>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_CONTACTGROUPTOOL_H
#define KCONTACTS_CONTACTGROUPTOOL_H

#include "kcontacts_export.h"
#include <QString>
class QIODevice;

template<class T>
class QList;

namespace KContacts
{
class ContactGroup;

/*!
 * \namespace KContacts::ContactGroupTool
 * \inheaderfile KContacts/ContactGroupTool
 * \inmodule KContacts
 *
 * \brief Static methods for converting ContactGroup to XML format and vice versa.
 *
 * \since 4.3
 */
namespace ContactGroupTool
{
/*!
 * Converts XML data coming from a \a device into a contact \a group.
 * If an error occurs, \c false is returned and \a errorMessage is set.
 */
[[nodiscard]] KCONTACTS_EXPORT bool convertFromXml(QIODevice *device, ContactGroup &group, QString *errorMessage = nullptr);

/*!
 * Converts a contact \a group into XML data and writes them to a \a device.
 * If an error occurs, \c false is returned and \a errorMessage is set.
 */
[[nodiscard]] KCONTACTS_EXPORT bool convertToXml(const ContactGroup &group, QIODevice *device, QString *errorMessage = nullptr);

/*!
 * Converts XML data coming from a \a device into a \a list of contact groups.
 * If an error occurs, \c false is returned and \a errorMessage is set.
 */
[[nodiscard]] KCONTACTS_EXPORT bool convertFromXml(QIODevice *device, QList<ContactGroup> &list, QString *errorMessage = nullptr);

/*!
 * Converts a \a list of contact groups into XML data and writes them to a \a device.
 * If an error occurs, \c false is returned and \a errorMessage is set.
 */
[[nodiscard]] KCONTACTS_EXPORT bool convertToXml(const QList<ContactGroup> &list, QIODevice *device, QString *errorMessage = nullptr);
}
}

#endif
