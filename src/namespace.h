/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_NAMESPACE_H
#define KCONTACTS_NAMESPACE_H

#include "kcontacts_export.h"

#include <QMetaType>

/** @file namespace.h
 *  Everything that needs to go in to the KContacts Q_NAMESPACE meta object.
 */

namespace KContacts
{
Q_NAMESPACE_EXPORT(KCONTACTS_EXPORT)

/** Address formatting styles.
 *  @see KContacts::Address::formatted
 *  @since 5.92
 */
enum class AddressFormatStyle {
    Postal, ///< Format used for addressing postal mail
    MultiLineDomestic, ///< Multi-line format without country, for displaying
    MultiLineInternational, ///< Multi-line format including the country, for displaying
    SingleLineDomestic, ///< Single-line format without country, for displaying
    SingleLineInternational, ///< Single-line format including the country, for displaying
    GeoUriQuery, ///< Format used in geo: URI query expressions
};

Q_ENUM_NS(AddressFormatStyle)

/** Address field types.
 *  These are the field types that can be referenced in address format rules.
 *  @note Not all of those are represented by vCard and thus KContacts, but exist
 *  only for compatibility with libaddressinput, so format rules from that can be
 *  consumed directly.
 *  @see KContacts::AddressFormat
 *  @since 5.92
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

/** A set of address fields.
 *  @see KContacts::AddressFormat
 *  @since 5.92
 */
Q_DECLARE_FLAGS(AddressFormatFields, AddressFormatField)
Q_FLAG_NS(AddressFormatFields)

/** Indicate whether to use a address format in the local script or a Latin transliteration.
 *  @see KContacts::AddressFormatRepository
 *  @since 5.92
 */
enum class AddressFormatScriptPreference {
    Local,
    Latin,
};
Q_ENUM_NS(AddressFormatScriptPreference)

/** Indicate whether to prefer an address format for (postal) business address or a generic one.
 *  @see KContacts::AddressFormatRepository
 *  @since 5.92
 */
enum class AddressFormatPreference { Generic, Business };
Q_ENUM_NS(AddressFormatPreference)

/**
 * The address book's access mode, i.e. whether it can be written to or is read only.
 * @since 6.0
 */
enum AccessMode {
    ReadOnly,
    ReadWrite,
};
Q_ENUM_NS(AccessMode)
}

Q_DECLARE_OPERATORS_FOR_FLAGS(KContacts::AddressFormatFields)

#endif // KCONTACTS_NAMESPACE_H
