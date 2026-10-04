/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PRONOUNS_H
#define PRONOUNS_H

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
 * \class KContacts::Pronouns
 * \inheaderfile KContacts/Pronouns
 * \inmodule KContacts
 *
 * \brief An RFC 9554 Pronouns property for a contact.
 * \since 6.31
 */
class KCONTACTS_EXPORT Pronouns
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Pronouns &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Pronouns &);
    friend class Addressee;
    friend class VCardTool;

    Q_GADGET
    Q_PROPERTY(QString pronouns READ pronouns WRITE setPronouns)
    Q_PROPERTY(bool isValid READ isValid)

public:
    /*!
     */
    Pronouns();

    Pronouns(const Pronouns &other);

    /*!
     */
    Q_INVOKABLE explicit Pronouns(const QString &pronouns);

    ~Pronouns();

    /*!
     */
    void setPronouns(const QString &pronouns);

    /*!
     */
    [[nodiscard]] QString pronouns() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const Pronouns &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const Pronouns &other) const;

    Pronouns &operator=(const Pronouns &other);

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
 * \relates KContacts::Pronouns
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Pronouns &object);

/*!
 * \relates KContacts::Pronouns
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Pronouns &object);
}
Q_DECLARE_TYPEINFO(KContacts::Pronouns, Q_RELOCATABLE_TYPE);
#endif // PRONOUNS_H
