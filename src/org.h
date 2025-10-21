/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef ORG_H
#define ORG_H

#include "kcontacts_export.h"

#include <QMap>
#include <QSharedDataPointer>
#include <QString>

class OrgTest;

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::Org
 * \inheaderfile KContacts/Org
 * \inmodule KContacts
 *
 * \brief Class that holds a Organization for a contact.
 * \since 5.3
 */
class KCONTACTS_EXPORT Org
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Org &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Org &);
    friend class VCardTool;
    friend class ::OrgTest;

public:
    /*!
     */
    Org();

    Org(const Org &other);

    /*!
     */
    Org(const QString &org);

    ~Org();

    /*!
     */
    typedef QList<Org> List;

    /*!
     */
    void setOrganization(const QString &org);

    /*!
     */
    [[nodiscard]] QString organization() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const Org &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const Org &other) const;

    Org &operator=(const Org &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

private:
    // exported for OrgTest
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Org
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Org &object);

/*!
 * \relates KContacts::Org
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Org &object);
}
Q_DECLARE_TYPEINFO(KContacts::Org, Q_RELOCATABLE_TYPE);
#endif // ORG_H
