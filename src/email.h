/*
    This file is part of the KContacts framework.
    Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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

#ifndef EMAIL_H
#define EMAIL_H

#include "kcontacts_export.h"

#include <QMap>
#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts {

/** @short Class that holds a Email for a contact.
 *  @since 4.14.5
 */
class KCONTACTS_EXPORT Email
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Email &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Email &);

    Q_GADGET
    Q_PROPERTY(QString email READ mail WRITE setEmail)
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(bool isPreferred READ isPreferred)

public:
    /**
     * Creates an empty email object.
     */
    Email();
    Email(const Email &other);
    Email(const QString &mail);

    ~Email();

    typedef QVector<Email> List;

    /** Email types. */
    enum TypeFlag {
        Unknown = 0, /**< No or unknown email type is set. */
        Home = 1,    /**< Personal email. */
        Work = 2,    /**< Work email. */
        Other = 4,   /**< Other email. */
        Preferred = 8     /**< Preferred email address. */
    };

    Q_DECLARE_FLAGS(Type, TypeFlag)
    Q_FLAG(Type)

    void setEmail(const QString &mail);
    Q_REQUIRED_RESULT QString mail() const;

    Q_REQUIRED_RESULT bool isValid() const;

    /**
     * Returns the type of the email.
     * @since 5.12
     */
    Type type() const;
    /**
     * Sets the email type.
     * @since 5.12
     */
    void setType(Type type);

    /**
     * Returns whether this is the preferred email address.
     * @since 5.12
     */
    bool isPreferred() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Email &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Email &other) const;

    Email &operator=(const Email &other);

    Q_REQUIRED_RESULT QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Email::Type)

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Email &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Email &object);
}
Q_DECLARE_METATYPE(KContacts::Email)
Q_DECLARE_TYPEINFO(KContacts::Email, Q_MOVABLE_TYPE);
#endif // EMAIL_H
