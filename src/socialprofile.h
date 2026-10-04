/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SOCIALPROFILE_H
#define SOCIALPROFILE_H

#include "kcontacts_export.h"

#include <QList>
#include <QMap>
#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>
#include <QStringList>

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::SocialProfile
 * \inheaderfile KContacts/SocialProfile
 * \inmodule KContacts
 *
 * \brief An RFC 9554 SocialProfile property for a contact.
 * \since 6.31
 */
class KCONTACTS_EXPORT SocialProfile
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const SocialProfile &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, SocialProfile &);
    friend class Addressee;
    friend class VCardTool;

    Q_GADGET
    Q_PROPERTY(QString profile READ profile WRITE setProfile)
    Q_PROPERTY(bool isValid READ isValid)

public:
    /*!
     */
    SocialProfile();

    SocialProfile(const SocialProfile &other);

    /*!
     */
    Q_INVOKABLE explicit SocialProfile(const QString &profile);

    ~SocialProfile();

    /*!
     */
    void setProfile(const QString &profile);

    /*!
     */
    [[nodiscard]] QString profile() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const SocialProfile &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const SocialProfile &other) const;

    SocialProfile &operator=(const SocialProfile &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

    /*! Property parameters, with lower-case names. */
    [[nodiscard]] QMap<QString, QStringList> parameters() const;
    /*! Sets parameters, normalizing their names to lower case. */
    void setParameters(const QMap<QString, QStringList> &parameters);

private:
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::SocialProfile
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const SocialProfile &object);

/*!
 * \relates KContacts::SocialProfile
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, SocialProfile &object);
}
Q_DECLARE_TYPEINFO(KContacts::SocialProfile, Q_RELOCATABLE_TYPE);
#endif // SOCIALPROFILE_H
