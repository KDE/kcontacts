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

#ifndef KABC_ADDRESS_H
#define KABC_ADDRESS_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>

#include "kabc_export.h"

namespace KABC {

/**
  @brief
  Postal address information.

  This class represents information about a postal address.
*/
class KABC_EXPORT Address
{
    friend KABC_EXPORT QDataStream &operator<<( QDataStream &s, const Address &addr );
    friend KABC_EXPORT QDataStream &operator>>( QDataStream &s, Address &addr );

  public:
    /**
      List of addresses.
    */
    typedef QList<Address> List;

    /**
      Address types:
    */
    enum TypeFlag {
      Dom = 1,     /**< domestic */
      Intl = 2,    /**< international */
      Postal = 4,  /**< postal */
      Parcel = 8,  /**< parcel */
      Home = 16,   /**< home address */
      Work = 32,   /**< address at work */
      Pref = 64    /**< preferred address */
    };

    Q_DECLARE_FLAGS( Type, TypeFlag )

    /**
      List of address types.
    */
    typedef QList<TypeFlag> TypeList;

    /**
      Creates an empty address.
    */
    Address();

    /**
      Creates an address of the given @p type.
    */
    Address( Type type );

    /**
      Copy constructor.
    */
    Address( const Address &address );

    /**
      Destroys the address.
    */
    ~Address();

    /**
      Equality operator.

      @param addr the address to compare to
      @return @c true if @c this and @p addr are equal, otherwise @c false
    */
    bool operator==( const Address &addr ) const;

    /**
      Not-equal operator.

      @param addr the address to compare to
      @return @c true if @c this and @p addr are not equal, otherwise @c false
    */
    bool operator!=( const Address &addr ) const;

    /**
      Assignment operator.

      @param addr the address data to assign to @c this
      @return a reference to @c this
    */
    Address &operator=( const Address &addr );

    /**
      Returns true, if the address is empty.
    */
    bool isEmpty() const;

    /**
      Clears all entries of the address.
    */
    void clear();

    /**
      Sets the unique @p identifier.
    */
    void setId( const QString &identifier );

    /**
      Returns the unique identifier.
    */
    QString id() const;

    /**
      Sets the type of address. See enum for definiton of types.

      @param type type, can be a bitwise or of multiple types.
    */
    void setType( Type type );

    /**
      Returns the type of address. Can be a bitwise or of multiple types.
    */
    Type type() const;

    /**
      Returns a translated string of all types the address has.
    */
    QString typeLabel() const;

    /**
      Sets the post office box.
    */
    void setPostOfficeBox( const QString &postOfficeBox );

    /**
      Returns the post office box.
    */
    QString postOfficeBox() const;

    /**
      Returns the translated label for post office box field.
    */
    static QString postOfficeBoxLabel();

    /**
      Sets the @p extended address information.
    */
    void setExtended( const QString &extended );

    /**
      Returns the extended address information.
    */
    QString extended() const;

    /**
      Returns the translated label for extended field.
    */
    static QString extendedLabel();

    /**
      Sets the @p street (including house number).
    */
    void setStreet( const QString &street );

    /**
      Returns the street.
    */
    QString street() const;

    /**
      Returns the translated label for street field.
    */
    static QString streetLabel();

    /**
      Sets the @p locality, e.g. city.

      @param locality the locality of the address, e.g. city
    */
    void setLocality( const QString &locality );

    /**
      Returns the locality.
    */
    QString locality() const;

    /**
      Returns the translated label for locality field.
    */
    static QString localityLabel();

    /**
      Sets the @p region, e.g. state.

      @param region the region the address falls into, e.g. state
    */
    void setRegion( const QString &region );

    /**
      Returns the region.
    */
    QString region() const;

    /**
      Returns the translated label for region field.
    */
    static QString regionLabel();

    /**
      Sets the postal @p code.
    */
    void setPostalCode( const QString &code );

    /**
      Returns the postal code.
    */
    QString postalCode() const;

    /**
      Returns the translated label for postal code field.
    */
    static QString postalCodeLabel();

    /**
      Sets the @p country.
    */
    void setCountry( const QString &country );

    /**
      Returns the country.
    */
    QString country() const;

    /**
      Returns the translated label for country field.
    */
    static QString countryLabel();

    /**
      Sets the delivery @p label. This is the literal text to be used as label.

      @param label the string to use for delivery labels
    */
    void setLabel( const QString &label );

    /**
      Returns the delivery label.
    */
    QString label() const;

    /**
      Returns the translated label for delivery label field.
    */
    static QString labelLabel();

    /**
      Returns the list of available types.
    */
    static TypeList typeList();

    /**
      Returns the translated label for the given @p type.
    */
    static QString typeLabel( Type type );

    /**
      Returns a string representation of the address.
    */
    QString toString() const;

    /**
      Returns this address formatted according to the country-specific
      address formatting rules. The formatting rules applied depend on
      either the addresses {@link #country country} field, or (if the
      latter is empty) on the system country setting. If companyName is
      provided, an available business address format will be preferred.

      @param realName   the formatted name of the contact
      @param orgaName   the name of the organization or company
      @return           the formatted address (containing newline characters)
    */
    QString formattedAddress( const QString &realName = QString(),
                              const QString &orgaName = QString() ) const;

    /**
      Returns ISO code for a localized country name. Only localized country
      names will be understood. This might be replaced by a KLocale method in
      the future.
      @param cname  name of the country
      @return       two digit ISO code
    */
    static QString countryToISO( const QString &cname );

    /**
      Returns a localized country name for a ISO code.
      This might be replaced by a KLocale method in the future.
      @param ISOname two digit ISO code
      @return        localized name of the country
    */
    static QString ISOtoCountry( const QString &ISOname );

  private:
    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( Address::Type )

/**
  Serializes the @p address object into the @p stream.
*/
KABC_EXPORT QDataStream &operator<<( QDataStream &stream, const Address &address );

/**
  Initializes the @p address object from the @p stream.
*/
KABC_EXPORT QDataStream &operator>>( QDataStream &stream, Address &address );

}

#endif
