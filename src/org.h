/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef ORG_H
#define ORG_H

#include "kcontacts_export.h"
#include <QHash>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
/** @short Class that holds a Organization for a contact.
 *  @since 5.3
 */
class KCONTACTS_EXPORT Org
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Org &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Org &);

public:
    Org();
    Org(const Org &other);
    Org(const QString &org);

    ~Org();

    typedef QVector<Org> List;

    void setOrganization(const QString &org);
    Q_REQUIRED_RESULT QString organization() const;

    Q_REQUIRED_RESULT bool isValid() const;

    void setParameters(const QHash<QString, QStringList> &params);
    Q_REQUIRED_RESULT QHash<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Org &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Org &other) const;

    Org &operator=(const Org &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Org &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Org &object);
}
Q_DECLARE_TYPEINFO(KContacts::Org, Q_MOVABLE_TYPE);
#endif // ORG_H
