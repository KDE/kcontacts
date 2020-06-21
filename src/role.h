/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef ROLE_H
#define ROLE_H

#include "kcontacts_export.h"
#include <QSharedDataPointer>
#include <QString>
#include <QMap>

namespace KContacts {
/** @short Class that holds a Role for a contact.
 *  @since 5.3
 */
class KCONTACTS_EXPORT Role
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Role &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Role &);
public:
    Role();
    Role(const Role &other);
    Role(const QString &role);

    ~Role();

    typedef QVector<Role> List;

    void setRole(const QString &role);
    Q_REQUIRED_RESULT QString role() const;

    Q_REQUIRED_RESULT bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Role &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Role &other) const;

    Role &operator=(const Role &other);

    Q_REQUIRED_RESULT QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Role &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Role &object);
}
Q_DECLARE_TYPEINFO(KContacts::Role, Q_MOVABLE_TYPE);
#endif // ROLE_H
