/*
    This file is part of the KContacts framework.
    Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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
#include <QSharedDataPointer>
#include <QString>
#include <QMap>

/** @short Class that holds a Email for a contact.
 *  @since 4.14.5
 */

namespace KContacts
{

class KCONTACTS_EXPORT Email
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Email &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Email &);
public:
    /**
     * Creates an empty email object.
     */
    Email();
    Email(const Email &other);
    Email(const QString &mail);

    ~Email();

    typedef QVector<Email> List;

    void setEmail(const QString &mail);
    QString mail() const;

    bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const Email &other) const;
    bool operator!=(const Email &other) const;

    Email &operator=(const Email &other);

    QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Email &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Email &object);

}
Q_DECLARE_TYPEINFO(KContacts::Email, Q_MOVABLE_TYPE);
#endif // EMAIL_H
