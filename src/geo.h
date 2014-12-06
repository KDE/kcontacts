/*
    This file is part of the KContacts framework.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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

#ifndef KCONTACTS_GEO_H
#define KCONTACTS_GEO_H

#include "kcontacts_export.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>

namespace KContacts {

/**
 * @short Geographic position
 *
 * This class represents a geographic position.
 */
class KCONTACTS_EXPORT Geo
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Geo &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Geo &);

public:
    /**
     * Creates an invalid geographics position object.
     */
    Geo();

    /**
     * Creates a geographics position object.
     *
     * @param latitude  Geographical latitude
     * @param longitude Geographical longitude
     */
    Geo(float latitude, float longitude);

    /**
     * Copy constructor.
     */
    Geo(const Geo &other);

    /**
     * Destroys the geographics position object.
     */
    ~Geo();

    /**
     * Sets the @p latitude.
     *
     * @param latitude The location's latitude coordinate
     */
    void setLatitude(float latitude);

    /**
     * Returns the latitude.
     */
    float latitude() const;

    /**
     * Sets the @p longitude.
     *
     * @param longitude The location's longitude coordinate
     */
    void setLongitude(float longitude);

    /**
     * Returns the longitude.
     */
    float longitude() const;

    /**
     * Returns, whether this object contains a valid geographical position.
     */
    bool isValid() const;

    /**
     * Equality operator.
     *
     * @note Two invalid Geo instance will return @c true
     */
    bool operator==(const Geo &other) const;

    /**
     * Not-Equal operator.
     */
    bool operator!=(const Geo &other) const;

    /**
     * Assignment operator.
     *
     * @param other The Geo instance to assign to @c this
     */
    Geo &operator=(const Geo &other);

    /**
     * Returns string representation of geographical position.
     */
    QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

/**
 * Serializes the geographical position @p object into the @p stream.
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Geo &object);

/**
 * Initializes the geographical position @p object from the @p stream.
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Geo &object);

}

#endif
