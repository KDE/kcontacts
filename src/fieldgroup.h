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

/*!
 * \class KContacts::FieldGroup
 * \inheaderfile KContacts/FieldGroup
 * \inmodule KContacts
 *
 * \brief Class that holds a FieldGroup for a contact.
 * \since 5.3
 */
class KCONTACTS_EXPORT FieldGroup
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const FieldGroup &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, FieldGroup &);
    friend class VCardTool;
    friend class ::FieldGroupTest;

public:
    /*!
     */
    FieldGroup();

    FieldGroup(const FieldGroup &other);

    /*!
     */
    FieldGroup(const QString &fieldGroupName);

    ~FieldGroup();

    typedef QList<FieldGroup> List;

    /*!
     */
    void setFieldGroupName(const QString &fieldGroup);

    /*!
     */
    [[nodiscard]] QString fieldGroupName() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    void setValue(const QString &value);

    /*!
     */
    [[nodiscard]] QString value() const;

    /*!
     */
    [[nodiscard]] bool operator==(const FieldGroup &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const FieldGroup &other) const;

    FieldGroup &operator=(const FieldGroup &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

private:
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::FieldGroup
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const FieldGroup &object);

/*!
 * \relates KContacts::FieldGroup
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, FieldGroup &object);
}
Q_DECLARE_TYPEINFO(KContacts::FieldGroup, Q_RELOCATABLE_TYPE);
#endif // FIELDGROUP_H
