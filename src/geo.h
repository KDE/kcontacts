/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_GEO_H
#define KCONTACTS_GEO_H

#include "kcontacts_export.h"

#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
/*!
 * \qmlvaluetype geo
 * \inqmlmodule org.kde.contacts
 * \nativetype KContacts::Geo
 *
 * \brief Geographic position.
 *
 * This class represents a geographic position.
 */

/*!
 * \class KContacts::Geo
 * \inheaderfile KContacts/Geo
 * \inmodule KContacts
 *
 * \brief Geographic position.
 *
 * This class represents a geographic position.
 */
class KCONTACTS_EXPORT Geo
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Geo &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Geo &);

    Q_GADGET

    /*!
     * \qmlproperty real geo::latitude
     */

    /*!
     * \property KContacts::Geo::latitude
     */
    Q_PROPERTY(float latitude READ latitude WRITE setLatitude)

    /*!
     * \qmlproperty real geo::longitude
     */

    /*!
     * \property KContacts::Geo::longitude
     */
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude)

    /*!
     * \qmlproperty real geo::isValid
     */

    /*!
     * \property KContacts::Geo::isValid
     */
    Q_PROPERTY(bool isValid READ isValid)

public:
    /*!
     * Creates an invalid geographics position object.
     */
    Geo();

    /*!
     * Creates a geographics position object.
     *
     * \a latitude  Geographical latitude
     *
     * \a longitude Geographical longitude
     */
    Geo(float latitude, float longitude);

    Geo(const Geo &other);

    ~Geo();

    /*!
     * Sets the \a latitude.
     *
     * \a latitude The location's latitude coordinate
     */
    void setLatitude(float latitude);

    /*!
     * Returns the latitude.
     */
    [[nodiscard]] float latitude() const;

    /*!
     * Sets the \a longitude.
     *
     * \a longitude The location's longitude coordinate
     */
    void setLongitude(float longitude);

    /*!
     * Returns the longitude.
     */
    [[nodiscard]] float longitude() const;

    /*!
     * Returns, whether this object contains a valid geographical position.
     */
    [[nodiscard]] bool isValid() const;

    /*!
     * Equality operator.
     *
     * \note Two invalid Geo instance will return \c true
     */
    [[nodiscard]] bool operator==(const Geo &other) const;

    /*!
     * Not-Equal operator.
     */
    bool operator!=(const Geo &other) const;

    /*!
     * Assignment operator.
     *
     * \a other The Geo instance to assign to \c this
     */
    Geo &operator=(const Geo &other);

    /*!
     * Returns string representation of geographical position.
     */
    [[nodiscard]] QString toString() const;

    /*!
     * Clears the class, marking it as invalid.
     *
     * \since 5.6
     */
    void clear();

private:
    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Geo
 *
 * Serializes the geographical position \a object into the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Geo &object);

/*!
 * \relates KContacts::Geo
 *
 * Initializes the geographical position \a object from the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Geo &object);
}

Q_DECLARE_TYPEINFO(KContacts::Geo, Q_RELOCATABLE_TYPE);

#endif
