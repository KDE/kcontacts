/*
    This file is part of libkabc.
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

#ifndef KABC_GEO_H
#define KABC_GEO_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>

#include "kabc_export.h"

namespace KABC {

/**
 * @short Geographic position
 *
 * This class represents a geographic position.
 */
class KABC_EXPORT Geo
{
    friend KABC_EXPORT QDataStream &operator<<( QDataStream &, const Geo & );
    friend KABC_EXPORT QDataStream &operator>>( QDataStream &, Geo & );

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
    Geo( float latitude, float longitude );

    /**
     * Copy constructor.
     */
    Geo( const Geo &other );

    /**
     * Destroys the geographics position object.
     */
    ~Geo();

    /**
     * Sets the @p latitude.
     */
    void setLatitude( float latitude );

    /**
     * Returns the latitude.
     */
    float latitude() const;

    /**
     * Sets the @p longitude.
     */
    void setLongitude( float longitude );

    /**
     * Returns the longitude.
     */
    float longitude() const;

    /**
     * Returns, whether this object contains a valid geographical position.
     */
    bool isValid() const;

    bool operator==( const Geo & ) const;
    bool operator!=( const Geo & ) const;
    Geo &operator=( const Geo &other );

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
KABC_EXPORT QDataStream &operator<<( QDataStream &stream, const Geo &object );

/**
 * Initializes the geographical position @p object from the @p stream.
 */
KABC_EXPORT QDataStream &operator>>( QDataStream &stream, Geo &object );

}

#endif
