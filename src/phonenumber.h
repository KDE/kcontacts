/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_PHONENUMBER_H
#define KCONTACTS_PHONENUMBER_H

#include "kcontacts_export.h"

#include <QList>
#include <QMap>
#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
class ParameterMap;

/*!
 * \qmlvaluetype phoneNumber
 * \inqmlmodule org.kde.contacts
 * \nativetype KContacts::PhoneNumber
 *
 * \brief Phonenumber information.
 *
 * This class provides phone number information. A phone number is classified by
 * a type. The following types are available, it's possible to use multiple types
 * Types for a number by combining them through a logical or.
 */

/*!
 * \class KContacts::PhoneNumber
 * \inheaderfile KContacts/PhoneNumber
 * \inmodule KContacts
 *
 * \brief Phonenumber information.
 *
 * This class provides phone number information. A phone number is classified by
 * a type. The following types are available, it's possible to use multiple types
 * Types for a number by combining them through a logical or.
 */
class KCONTACTS_EXPORT PhoneNumber
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const PhoneNumber &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, PhoneNumber &);
    friend class VCardTool;

    Q_GADGET

    /*!
     * \qmlproperty string phoneNumber::id
     */

    /*!
     * \property KContacts::PhoneNumber::id
     */
    Q_PROPERTY(QString id READ id WRITE setId)

    /*!
     * \qmlproperty string phoneNumber::number
     */

    /*!
     * \property KContacts::PhoneNumber::number
     */
    Q_PROPERTY(QString number READ number WRITE setNumber)

    /*!
     * \qmlproperty string phoneNumber::normalizedNumber
     */

    /*!
     * \property KContacts::PhoneNumber::normalizedNumber
     */
    Q_PROPERTY(QString normalizedNumber READ normalizedNumber)

    /*!
     * \qmlproperty enumeration phoneNumber::type
     * \qmlenumeratorsfrom KContacts::PhoneNumber::TypeFlag
     */

    /*!
     * \property KContacts::PhoneNumber::type
     */
    Q_PROPERTY(Type type READ type WRITE setType)

    /*!
     * \qmlproperty string phoneNumber::typeLabel
     */

    /*!
     * \property KContacts::PhoneNumber::typeLabel
     */
    Q_PROPERTY(QString typeLabel READ typeLabel)

    /*!
     * \qmlproperty bool phoneNumber::isEmpty
     */

    /*!
     * \property KContacts::PhoneNumber::isEmpty
     */
    Q_PROPERTY(bool isEmpty READ isEmpty)

    /*!
     * \qmlproperty bool phoneNumber::isPreferred
     */

    /*!
     * \property KContacts::PhoneNumber::isPreferred
     */
    Q_PROPERTY(bool isPreferred READ isPreferred)

    /*!
     * \qmlproperty bool phoneNumber::supportsSms
     */

    /*!
     * \property KContacts::PhoneNumber::supportsSms
     */
    Q_PROPERTY(bool supportsSms READ supportsSms)

public:
    /*!
      Phone number types.

      \value Home Home number
      \value Work Office number
      \value Msg Messaging
      \value Pref Preferred number
      \value Voice Voice
      \value Fax Fax machine
      \value Cell Cell phone
      \value Video Video phone
      \value Bbs Mailbox
      \value Modem Modem
      \value Car Car phone
      \value Isdn ISDN connection
      \value Pcs Personal Communication Service
      \value Pager Pager
      \value Undefined Undefined number type
    */
    enum TypeFlag {
        Home = 1,
        Work = 2,
        Msg = 4,
        Pref = 8,
        Voice = 16,
        Fax = 32,
        Cell = 64,
        Video = 128,
        Bbs = 256,
        Modem = 512,
        Car = 1024,
        Isdn = 2048,
        Pcs = 4096,
        Pager = 8192,
        // TODO add Text and textphone support vcard4
        Undefined = 16384,
    };

    Q_DECLARE_FLAGS(Type, TypeFlag)
    Q_FLAG(Type)

    /*!
     * List of phone number types.
     */
    typedef QList<TypeFlag> TypeList;

    /*!
     * List of phone numbers.
     */
    typedef QList<PhoneNumber> List;

    /*!
     * Creates an empty phone number object.
     */
    PhoneNumber();

    /*!
     * Creates a phone number object.
     *
     * \a number Number
     *
     * \a type Type as defined in enum. Multiple types can be
     *               specified by combining them by a logical or.
     */
    PhoneNumber(const QString &number, Type type = Home); // krazy:exclude=explicit

    /*!
     * Copy constructor.
     *
     * Fast operation, PhoneNumber's data is implicitly shared.
     *
     * \a other The PhoneNumber object to copy from
     */
    PhoneNumber(const PhoneNumber &other);

    ~PhoneNumber();

    /*!
     * Equality operator.
     *
     * Returns \c true if number, type and identifier are equal,
     *         otherwise \c false
     */
    [[nodiscard]] bool operator==(const PhoneNumber &other) const;

    /*!
     * Not-Equal operator.
     */
    [[nodiscard]] bool operator!=(const PhoneNumber &other) const;

    /*!
     * Assignment operator.
     *
     * Fast operation, PhoneNumber's data is implicitly shared.
     *
     * \a other The PhoneNumber object to asssign to \c this
     */
    PhoneNumber &operator=(const PhoneNumber &other);

    /*!
     * Returns true, if the phone number is empty.
     */
    [[nodiscard]] bool isEmpty() const;

    /*!
     * Sets the unique \a identifier.
     */
    void setId(const QString &identifier);

    /*!
     * Returns the unique identifier.
     */
    [[nodiscard]] QString id() const;

    /*!
     * Sets the phone \a number.
     */
    void setNumber(const QString &number);

    /*!
     * Returns the phone number.
     * This is the number as entered/stored with all formatting preserved. Preferred for display.
     * \sa normalizedNumber()
     */
    [[nodiscard]] QString number() const;

    /*!
     * Returns the phone number normalized for dialing.
     * This has all formatting stripped for passing to dialers or tel: URLs.
     * \sa number()
     * \since 5.12
     */
    [[nodiscard]] QString normalizedNumber() const;

    /*!
     * Sets the \a type.
     * Multiple types can be specified by combining them by a logical or.
     *
     * \a type The #Type of the phone number
     */
    void setType(Type type);

    /*!
     * Returns the type. Can be a multiple types combined by a logical or.
     *
     * \sa TypeFlag
     * \sa typeLabel()
     */
    [[nodiscard]] Type type() const;

    /*!
     * Returns a translated string of the address' type.
     */
    [[nodiscard]] QString typeLabel() const;

    /*!
     * Returns a list of all available types
     */
    [[nodiscard]] static TypeList typeList();

    /*!
     * Returns whether this phone number is marked as preferred.
     * \since 5.12
     */
    [[nodiscard]] bool isPreferred() const;
    /*!
     * Returns whether this phone number is expected to support receiving SMS messages.
     * \since 5.12
     */
    [[nodiscard]] bool supportsSms() const;

    /*!
     * Returns the translated label for phone number \a type.
     *
     * In opposite to typeFlagLabel( TypeFlag type ), it returns all types
     * of the phone number concatenated by '/'.
     *
     * \a type An OR'ed combination of #TypeFlag
     *
     * \sa type()
     */
    static QString typeLabel(Type type);

    /*!
     * Returns the translated label for phone number \a type.
     *
     * \a type An OR'ed combination of #TypeFlag
     *
     * \sa typeLabel()
     * \since 4.5
     */
    [[nodiscard]] static QString typeFlagLabel(TypeFlag type);

    /*!
     * Returns a string representation of the phone number.
     */
    QString toString() const;

private:
    KCONTACTS_NO_EXPORT void setParams(const ParameterMap &params);
    [[nodiscard]] KCONTACTS_NO_EXPORT ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PhoneNumber::Type)

/*!
 * \relates KContacts::PhoneNumber
 *
 * Serializes the phone \a number object into the \a stream.
 *
 * \a stream The stream to write into
 *
 * \a number The phone number object to serialize
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const PhoneNumber &number);

/*!
 * \relates KContacts::PhoneNumber
 *
 * Initializes the phone \a number object from the \a stream.
 *
 * \a stream The stream to read from
 *
 * \a number The phone number object to deserialize into
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, PhoneNumber &number);
}

Q_DECLARE_TYPEINFO(KContacts::PhoneNumber, Q_RELOCATABLE_TYPE);

#endif
