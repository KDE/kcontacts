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

#ifndef KABC_PHONENUMBER_H
#define KABC_PHONENUMBER_H

#include "kabc_export.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>

namespace KABC {

/**
 * @short Phonenumber information.
 *
 * This class provides phone number information. A phone number is classified by
 * a type. The following types are available, it's possible to use multiple types
 * Types for a number by combining them through a logical or.
*/
class KABC_EXPORT PhoneNumber
{
    friend KABC_EXPORT QDataStream &operator<<( QDataStream &, const PhoneNumber & );
    friend KABC_EXPORT QDataStream &operator>>( QDataStream &, PhoneNumber & );

  public:
    /**
      Phone number types.
    */
    enum TypeFlag {
      Home = 1,     /**< Home number */
      Work = 2,     /**< Office number */
      Msg = 4,      /**< Messaging */
      Pref = 8,     /**< Preferred number */
      Voice = 16,   /**< Voice */
      Fax = 32,     /**< Fax machine */
      Cell = 64,    /**< Cell phone */
      Video = 128,  /**< Video phone */
      Bbs = 256,    /**< Mailbox */
      Modem = 512,  /**< Modem */
      Car = 1024,   /**< Car phone */
      Isdn = 2048,  /**< ISDN connection */
      Pcs = 4096,   /**< Personal Communication Service*/
      Pager = 8192  /**< Pager */
    };

    Q_DECLARE_FLAGS( Type, TypeFlag )

    /**
     * List of phone number types.
     */
    typedef QList<TypeFlag> TypeList;

    /**
     * List of phone numbers.
     */
    typedef QList<PhoneNumber> List;

    /**
     * Creates an empty phone number object.
     */
    PhoneNumber();

    /**
     * Creates a phone number object.
     *
     * @param number Number
     * @param type   Type as defined in enum. Multiple types can be
     *               specified by combining them by a logical or.
     */
    PhoneNumber( const QString &number, Type type = Home ); //krazy:exclude=explicit

    /**
     * Copy constructor.
     */
    PhoneNumber( const PhoneNumber &other );

    /**
     * Destroys the phone number.
     */
    ~PhoneNumber();

    bool operator==( const PhoneNumber & ) const;
    bool operator!=( const PhoneNumber & ) const;
    PhoneNumber &operator=( const PhoneNumber &other );

    /**
     * Returns true, if the phone number is empty.
     */
    bool isEmpty() const;

    /**
     * Sets the unique @p identifier.
     */
    void setId( const QString &identifier );

    /**
     * Returns the unique identifier.
     */
    QString id() const;

    /**
     * Sets the phone @p number.
     */
    void setNumber( const QString &number );

    /**
     * Returns the phone number.
     */
    QString number() const;

    /**
     * Sets the @p type.
     * Multiple types can be specified by combining them by a logical or.
     */
    void setType( Type type );

    /**
     * Returns the type. Can be a multiple types combined by a logical or.
     */
    Type type() const;

    /**
     * Returns a translated string of the address' type.
     */
    QString typeLabel() const;

    /**
     * Returns a list of all available types
     */
    static TypeList typeList();

    /**
     * Returns the translated label for phone number @p type.
     */
    static QString typeLabel( Type type );

    /**
     * Returns a string representation of the phone number.
     */
    QString toString() const;

  private:
    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( PhoneNumber::Type )

/**
 * Serializes the phone @p number object into the @p stream.
 */
KABC_EXPORT QDataStream &operator<<( QDataStream &stream, const PhoneNumber &number );

/**
 * Initializes the phone @p number object from the @p stream.
 */
KABC_EXPORT QDataStream &operator>>( QDataStream &stream, const PhoneNumber &number );

}

#endif
