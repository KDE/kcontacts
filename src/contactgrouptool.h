/*
  This file is part of the KContacts framework.
  Copyright (c) 2008 Tobias Koenig <tokoe@kde.org>
  Copyright (c) 2008 Kevin Krammer <kevin.krammer@gmx.at>

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

#ifndef KCONTACTS_CONTACTGROUPTOOL_H
#define KCONTACTS_CONTACTGROUPTOOL_H

#include "kcontacts_export.h"

class QIODevice;
class QString;

template <class T> class QList;

namespace KContacts {

class ContactGroup;

/**
 * Static methods for converting ContactGroup to XML format and vice versa.
 *
 * @author Kevin Krammer <kevin.krammer@gmx.at>
 * @since 4.3
 */
namespace ContactGroupTool {

/**
 * Converts XML data coming from a @p device into a contact @p group.
 * If an error occurs, @c false is returned and @p errorMessage is set.
 */
KCONTACTS_EXPORT bool convertFromXml(QIODevice *device, ContactGroup &group,
                                     QString *errorMessage = 0);

/**
 * Converts a contact @p group into XML data and writes them to a @p device.
 * If an error occurs, @c false is returned and @p errorMessage is set.
 */
KCONTACTS_EXPORT bool convertToXml(const ContactGroup &group, QIODevice *device,
                                   QString *errorMessage = 0);

/**
 * Converts XML data coming from a @p device into a @p list of contact groups.
 * If an error occurs, @c false is returned and @p errorMessage is set.
 */
KCONTACTS_EXPORT bool convertFromXml(QIODevice *device,
                                     QList<ContactGroup> &list,
                                     QString *errorMessage = 0);

/**
 * Converts a @p list of contact groups into XML data and writes them to a @p device.
 * If an error occurs, @c false is returned and @p errorMessage is set.
 */
KCONTACTS_EXPORT bool convertToXml(const QList<ContactGroup> &list,
                                   QIODevice *device, QString *errorMessage = 0);
}

}

#endif
