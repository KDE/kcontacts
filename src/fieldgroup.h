/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FIELDGROUP_H
#define FIELDGROUP_H

#include "kcontacts_export.h"
#include <QMap>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
/** @short Class that holds a FieldGroup for a contact.
 *  @since 5.3
 */
class KCONTACTS_EXPORT FieldGroup
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const FieldGroup &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, FieldGroup &);

public:
    FieldGroup();
    FieldGroup(const FieldGroup &other);
    FieldGroup(const QString &fieldGroupName);

    ~FieldGroup();

    typedef QVector<FieldGroup> List;

    void setFieldGroupName(const QString &fieldGroup);
    Q_REQUIRED_RESULT QString fieldGroupName() const;

    Q_REQUIRED_RESULT bool isValid() const;

    void setValue(const QString &value);
    Q_REQUIRED_RESULT QString value() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const FieldGroup &other) const;
    Q_REQUIRED_RESULT bool operator!=(const FieldGroup &other) const;

    FieldGroup &operator=(const FieldGroup &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const FieldGroup &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, FieldGroup &object);
}
Q_DECLARE_TYPEINFO(KContacts::FieldGroup, Q_MOVABLE_TYPE);
#endif // FIELDGROUP_H
