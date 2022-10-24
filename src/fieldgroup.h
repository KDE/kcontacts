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

class FieldGroupTest;

namespace KContacts
{
class ParameterMap;

/** @short Class that holds a FieldGroup for a contact.
 *  @since 5.3
 */
class KCONTACTS_EXPORT FieldGroup
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const FieldGroup &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, FieldGroup &);
    friend class VCardTool;
    friend class ::FieldGroupTest;

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

#if KCONTACTS_ENABLE_DEPRECATED_SINCE(5, 88)
    /**
     * @deprecated Since 5.88 for lack of usage
     */
    KCONTACTS_DEPRECATED_VERSION(5, 88, "For lack of usage.")
    void setParameters(const QMap<QString, QStringList> &params);
#endif

#if KCONTACTS_ENABLE_DEPRECATED_SINCE(5, 88)
    /**
     * @deprecated Since 5.88 for lack of usage
     */
    Q_REQUIRED_RESULT
    KCONTACTS_DEPRECATED_VERSION(5, 88, "For lack of usage.")
    QMap<QString, QStringList> parameters() const;
#endif

    Q_REQUIRED_RESULT bool operator==(const FieldGroup &other) const;
    Q_REQUIRED_RESULT bool operator!=(const FieldGroup &other) const;

    FieldGroup &operator=(const FieldGroup &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    void setParams(const ParameterMap &params);
    Q_REQUIRED_RESULT ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const FieldGroup &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, FieldGroup &object);
}
Q_DECLARE_TYPEINFO(KContacts::FieldGroup, Q_MOVABLE_TYPE);
#endif // FIELDGROUP_H
