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

#ifndef KCONTACTS_TIMEZONE_H
#define KCONTACTS_TIMEZONE_H

#include "kcontacts_export.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>

namespace KContacts
{

/**
 * @short Time zone information.
 *
 * This class stores information about a time zone.
 */
class KCONTACTS_EXPORT TimeZone
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const TimeZone &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, TimeZone &);

public:
    /**
     * Construct invalid time zone.
     */
    TimeZone();

    /**
     * Construct time zone.
     *
     * @param offset Offset in minutes relative to UTC.
     */
    TimeZone(int offset);

    /**
     * Copy constructor.
     */
    TimeZone(const TimeZone &other);

    /**
     * Destroys the time zone.
     */
    ~TimeZone();

    /**
     * Set time zone offset relative to UTC.
     *
     * @param offset Offset in minutes.
     */
    void setOffset(int offset);

    /**
     * Return offset in minutes relative to UTC.
     */
    int offset() const;

    /**
     * Return, if this time zone object is valid.
     */
    bool isValid() const;

    bool operator==(const TimeZone &) const;
    bool operator!=(const TimeZone &) const;
    TimeZone &operator=(const TimeZone &other);

    /**
     * Return string representation of time zone offset.
     */
    QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

/**
 * Serializes the @p timezone object into the @p stream.
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const TimeZone &timeZone);

/**
 * Initializes the @p timezone object from the @p stream.
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, TimeZone &timeZone);

}

#endif
