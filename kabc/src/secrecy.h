/*
    This file is part of libkabc.
    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>

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

#ifndef KABC_SECRECY_H
#define KABC_SECRECY_H

#include "kabc_export.h"
#include <QtCore/QList>
#include <QtCore/QSharedDataPointer>

namespace KABC
{

class KABC_EXPORT Secrecy
{
    friend KABC_EXPORT QDataStream &operator<<(QDataStream &, const Secrecy &);
    friend KABC_EXPORT QDataStream &operator>>(QDataStream &, Secrecy &);

public:
    /**
     * Secrecy types
     *
     * @li Public       - for public access
     * @li Private      - only private access
     * @li Confidential - access for confidential persons
     */
    enum Type {
        Public,
        Private,
        Confidential,
        Invalid
    };

    /**
     * List of secrecy types.
     */
    typedef QList<Type> TypeList;

    /**
     * Creates a new secrecy of the given type.
     *
     * @param type  The secrecy type. @see Type
     */
    Secrecy(Type type = Invalid);

    /**
     * Copy constructor.
     */
    Secrecy(const Secrecy &other);

    /**
     * Destroys the secrecy.
     */
    ~Secrecy();

    Secrecy &operator=(const Secrecy &);

    bool operator==(const Secrecy &) const;
    bool operator!=(const Secrecy &) const;

    /**
     * Returns if the Secrecy object has a valid value.
     */
    bool isValid() const;

    /**
     * Sets the @p type.
     *
     * @param type The #Type of secrecy
     */
    void setType(Type type);

    /**
     * Returns the type.
     */
    Type type() const;

    /**
     * Returns a list of all available secrecy types.
     */
    static TypeList typeList();

    /**
     * Returns a translated label for a given secrecy @p type.
     */
    static QString typeLabel(Type type);

    /**
     * Returns a string representation of the secrecy.
     */
    QString toString() const;

private:
    class PrivateData;
    QSharedDataPointer<PrivateData> d;
};

/**
 * Serializes the @p secrecy object into the @p stream.
 */
KABC_EXPORT QDataStream &operator<<(QDataStream &stream, const Secrecy &secrecy);

/**
 * Initializes the @p secrecy object from the @p stream.
 */
KABC_EXPORT QDataStream &operator>>(QDataStream &stream, Secrecy &secrecy);

}
#endif
