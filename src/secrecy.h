/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2002 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_SECRECY_H
#define KCONTACTS_SECRECY_H

#include "kcontacts_export.h"
#include <QList>
#include <QSharedDataPointer>

namespace KContacts
{
/*!
 * \class KContacts::Secrecy
 * \inheaderfile KContacts/Secrecy
 * \inmodule KContacts
 *
 * \brief Describes the confidentiality of an addressee.
 */
class KCONTACTS_EXPORT Secrecy
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Secrecy &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Secrecy &);

public:
    /*!
     * Secrecy types
     *
     * \value Public for public access
     * \value Private only private access
     * \value Confidential access for confidential persons
     * \value Invalid
     */
    enum Type {
        Public,
        Private,
        Confidential,
        Invalid,
    };

    /*!
     * List of secrecy types.
     */
    typedef QList<Type> TypeList;

    /*!
     * Creates a new secrecy of the given type.
     *
     * \a type The secrecy type
     */
    Secrecy(Type type = Invalid);

    Secrecy(const Secrecy &other);

    ~Secrecy();

    Secrecy &operator=(const Secrecy &other);

    /*!
     */
    Q_REQUIRED_RESULT bool operator==(const Secrecy &other) const;

    /*!
     */
    Q_REQUIRED_RESULT bool operator!=(const Secrecy &other) const;

    /*!
     * Returns if the Secrecy object has a valid value.
     */
    Q_REQUIRED_RESULT bool isValid() const;

    /*!
     * Sets the \a type.
     *
     * \a type The Type of secrecy
     */
    void setType(Type type);

    /*!
     * Returns the type.
     */
    Q_REQUIRED_RESULT Type type() const;

    /*!
     * Returns a list of all available secrecy types.
     */
    Q_REQUIRED_RESULT static TypeList typeList();

    /*!
     * Returns a translated label for a given secrecy \a type.
     */
    Q_REQUIRED_RESULT static QString typeLabel(Type type);

    /*!
     * Returns a string representation of the secrecy.
     */
    Q_REQUIRED_RESULT QString toString() const;

private:
    class PrivateData;
    QSharedDataPointer<PrivateData> d;
};

/*!
 * \relates KContacts::Secrecy
 *
 * Serializes the \a secrecy object into the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Secrecy &secrecy);

/*!
 * \relates KContacts::Secrecy
 *
 * Initializes the \a secrecy object from the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Secrecy &secrecy);
}
Q_DECLARE_TYPEINFO(KContacts::Secrecy, Q_RELOCATABLE_TYPE);
#endif
