/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2002 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_KEY_H
#define KCONTACTS_KEY_H

#include "kcontacts_export.h"

#include <QDataStream>
#include <QSharedDataPointer>

namespace KContacts
{
/*!
 * \class KContacts::Key
 * \inheaderfile KContacts/Key
 * \inmodule KContacts
 *
 * \brief A class to store an encryption key.
 */
class KCONTACTS_EXPORT Key
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Key &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Key &);

public:
    /*!
      List of keys.
    */
    typedef QList<Key> List;

    /*!
      Key types

      \value X509 X509 key
      \value PGP Pretty Good Privacy key
      \value Custom Custom or IANA conform key
    */
    enum Type {
        X509,
        PGP,
        Custom,
    };

    /*!
      List of key types.
    */
    typedef QList<Type> TypeList;

    /*!
      Creates a new key.

      \a text The text data.

      \a type The key type, see Types.
    */
    explicit Key(const QString &text = QString(), Type type = PGP);

    Key(const Key &other);

    ~Key();

    /*!
      Equality operator.
    */
    [[nodiscard]] bool operator==(const Key &other) const;

    /*!
      Not-equal operator.
    */
    [[nodiscard]] bool operator!=(const Key &other) const;

    /*!
      Assignment operator.

      \a other The Key instance to assign to \c this
    */
    Key &operator=(const Key &other);

    /*!
      Sets the unique \a identifier.
    */
    void setId(const QString &identifier);

    /*!
      Returns the unique identifier.
    */
    [[nodiscard]] QString id() const;

    /*!
      Sets binary \a data.
    */
    void setBinaryData(const QByteArray &data);

    /*!
      Returns the binary data.
    */
    [[nodiscard]] QByteArray binaryData() const;

    /*!
      Sets text \a data.
    */
    void setTextData(const QString &data);

    /*!
      Returns the text data.
    */
    [[nodiscard]] QString textData() const;

    /*!
      Returns whether the key contains binary or text data.
    */
    [[nodiscard]] bool isBinary() const;

    /*!
      Sets the \a type.

      \a type The type of the key

      \sa Type
    */
    void setType(Type type);

    /*!
      Sets custom \a type string.
    */
    void setCustomTypeString(const QString &type);

    /*!
      Returns the type, see Type.
    */
    [[nodiscard]] Type type() const;

    /*!
      Returns the custom type string.
    */
    [[nodiscard]] QString customTypeString() const;

    /*!
      Returns a string representation of the key.
    */
    [[nodiscard]] QString toString() const;

    /*!
      Returns a list of all available key types.
    */
    [[nodiscard]] static TypeList typeList();

    /*!
      Returns a translated label for a given key \a type.
    */
    [[nodiscard]] static QString typeLabel(Type type);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Key
 *
 * Serializes the \a key object into the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Key &key);

/*!
 * \relates KContacts::Key
 *
 * Initializes the \a key object from the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Key &key);
}
Q_DECLARE_TYPEINFO(KContacts::Key, Q_RELOCATABLE_TYPE);
#endif
