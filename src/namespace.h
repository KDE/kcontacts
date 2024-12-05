/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_NAMESPACE_H
#define KCONTACTS_NAMESPACE_H

#include "kcontacts_export.h"

#include <QMetaType>

/*!
 * \namespace KContacts
 * \inheaderfile KContacts/Namespace
 * \inmodule KContacts
 * \brief Everything that needs to go in to the KContacts Q_NAMESPACE meta object.
 */
namespace KContacts
{
KCONTACTS_EXPORT Q_NAMESPACE

    /*!
     * Address formatting styles.
     * \sa KContacts::Address::formatted
     * \since 5.92
     *
     * \value Postal Format used for addressing postal mail
     * \value MultiLineDomestic Multi-line format without country, for displaying
     * \value MultiLineInternational Multi-line format including the country, for displaying
     * \value SingleLineDomestic Single-line format without country, for displaying
     * \value SingleLineInternational Single-line format including the country, for displaying
     * \value GeoUriQuery Format used in geo: URI query expressions
     */
    enum class AddressFormatStyle {
    Postal,
    MultiLineDomestic,
    MultiLineInternational,
    SingleLineDomestic,
    SingleLineInternational,
    GeoUriQuery,
    };

Q_ENUM_NS(AddressFormatStyle)

/*!
 * Address field types.
 *
 * These are the field types that can be referenced in address format rules.
 *
 * \note Not all of those are represented by vCard and thus KContacts, but exist
 * only for compatibility with libaddressinput, so format rules from that can be
 * consumed directly.
 *
 * \value NoField
 * \value Country
 * \value Region
 * \value Locality
 * \value DependentLocality
 * \value SortingCode
 * \value PostalCode
 * \value StreetAddress
 * \value Organization
 * \value Name
 * \value PostOfficeBox
 *
 * \sa KContacts::AddressFormat
 * \since 5.92
 */
enum class AddressFormatField {
    NoField = 0,
    // from libaddressinput
    Country = 1,
    Region = 2,
    Locality = 4,
    DependentLocality = 8, // not used by us
    SortingCode = 16, // not used by us
    PostalCode = 32,
    StreetAddress = 64,
    Organization = 128,
    Name = 256,
    // added by us for vCard compat
    PostOfficeBox = 512,
};
Q_ENUM_NS(AddressFormatField)

Q_DECLARE_FLAGS(AddressFormatFields, AddressFormatField)
Q_FLAG_NS(AddressFormatFields)

/*!
 * Indicate whether to use a address format in the local script or a Latin transliteration.
 * \sa KContacts::AddressFormatRepository
 * \since 5.92
 *
 * \value Local
 * \value Latin
 */
enum class AddressFormatScriptPreference {
    Local,
    Latin,
};
Q_ENUM_NS(AddressFormatScriptPreference)

/*!
 * Indicate whether to prefer an address format for (postal) business address or a generic one.
 * \sa KContacts::AddressFormatRepository
 * \since 5.92
 *
 * \value Generic
 * \value Business
 */
enum class AddressFormatPreference { Generic, Business };
Q_ENUM_NS(AddressFormatPreference)
}

Q_DECLARE_OPERATORS_FOR_FLAGS(KContacts::AddressFormatFields)

#endif // KCONTACTS_NAMESPACE_H
