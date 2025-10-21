/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef NICKNAME_H
#define NICKNAME_H

#include "kcontacts_export.h"

#include <QMap>
#include <QSharedDataPointer>
#include <QString>

class NickNameTest;

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::NickName
 * \inheaderfile KContacts/NickName
 * \inmodule KContacts
 *
 * \brief Class that holds a NickName for a contact.
 * \since 5.3
 */
class KCONTACTS_EXPORT NickName
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const NickName &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, NickName &);
    friend class VCardTool;
    friend class ::NickNameTest;

public:
    /*!
     */
    NickName();

    NickName(const NickName &other);

    /*!
     */
    NickName(const QString &nickname);

    ~NickName();

    /*!
     */
    typedef QList<NickName> List;

    /*!
     */
    void setNickName(const QString &nickname);

    /*!
     */
    [[nodiscard]] QString nickname() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const NickName &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const NickName &other) const;

    NickName &operator=(const NickName &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

private:
    // exported for NickNameTest
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::NickName
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const NickName &object);

/*!
 * \relates KContacts::NickName
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, NickName &object);
}
Q_DECLARE_TYPEINFO(KContacts::NickName, Q_RELOCATABLE_TYPE);
#endif // NICKNAME_H
