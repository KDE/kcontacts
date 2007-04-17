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

#ifndef KABC_KEY_H
#define KABC_KEY_H

#include "kabc.h"

#include <QtCore/QDataStream>
#include <QtCore/QSharedDataPointer>

namespace KABC {

/**
 * @short A class to store an encryption key.
 */
class KABC_EXPORT Key
{
  friend KABC_EXPORT QDataStream &operator<<( QDataStream &, const Key & );
  friend KABC_EXPORT QDataStream &operator>>( QDataStream &, Key & );

  public:
    /**
     * List of keys.
     */
    typedef QList<Key> List;

    /**
     * Key types
     *
     * @li X509   - X509 key
     * @li PGP    - Pretty Good Privacy key
     * @li Custom - Custom or IANA conform key
     */
    enum Type {
      X509,
      PGP,
      Custom
    };

    /**
     * List of key types.
     */
    typedef QList<Type> TypeList;

    /**
     * Creates a new key.
     *
     * @param text  The text data.
     * @param type  The key type, see Types.
     */
    explicit Key( const QString &text = QString(), Type type = PGP );

    /**
     * Copy constructor.
     */
    Key( const Key &other );

    /**
     * Destroys the key.
     */
    ~Key();

    bool operator==( const Key & ) const;
    bool operator!=( const Key & ) const;
    Key& operator=( const Key &other );

    /**
     * Sets the unique @p identifier.
     */
    void setId( const QString &identifier );

    /**
     * Returns the unique identifier.
     */
    QString id() const;

    /**
     * Sets binary @p data.
     */
    void setBinaryData( const QByteArray &data );

    /**
     * Returns the binary data.
     */
    QByteArray binaryData() const;

    /**
     * Sets text @p data.
     */
    void setTextData( const QString &data );

    /**
     * Returns the text data.
     */
    QString textData() const;

    /**
     * Returns whether the key contains binary or text data.
     */
    bool isBinary() const;

    /**
     * Sets the @p type.
     * @see Type
     */
    void setType( Type type );

    /**
     * Sets custom @p type string.
     */
    void setCustomTypeString( const QString &type );

    /**
     * Returns the type, see Type.
     */
    Type type() const;

    /**
     * Returns the custom type string.
     */
    QString customTypeString() const;

    /**
     * Returns a string representation of the key.
     */
    QString toString() const;

    /**
     * Returns a list of all available key types.
     */
    static TypeList typeList();

    /**
     * Returns a translated label for a given key @p type.
     */
    static QString typeLabel( Type type );

  private:
    class Private;
    QSharedDataPointer<Private> d;
};

/**
 * Serializes the @p key object into the @p stream.
 */
KABC_EXPORT QDataStream &operator<<( QDataStream &stream, const Key &key );

/**
 * Initializes the @p key object from the @p stream.
 */
KABC_EXPORT QDataStream &operator>>( QDataStream &stream, Key &key );

}
#endif
