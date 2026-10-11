/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAMMATICALGENDER_H
#define GRAMMATICALGENDER_H

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
 * \class KContacts::GrammaticalGender
 * \inheaderfile KContacts/GrammaticalGender
 * \inmodule KContacts
 *
 * \brief Defines which grammatical gender to use in salutations and other grammatical constructs.
 * \since 6.32
 */
class KCONTACTS_EXPORT GrammaticalGender
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const GrammaticalGender &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, GrammaticalGender &);
    friend class Addressee;
    friend class VCardTool;

    Q_GADGET
    /*!
      \qmlproperty string grammaticalGender::gender
      This property holds the grammatical gender used in salutations and other grammatical constructs.
    */
    /*!
      \property KContacts::GrammaticalGender::gender
      This property holds the grammatical gender used in salutations and other grammatical constructs.
    */
    Q_PROPERTY(QString gender READ gender WRITE setGender)
    /*!
      \qmlproperty string grammaticalGender::language
      The language tag of the LANGUAGE parameter.
      An empty value indicates that no LANGUAGE parameter is set.
      \since 6.32
    */
    /*!
      \property KContacts::GrammaticalGender::language
      The language tag of the LANGUAGE parameter.
      An empty value indicates that no LANGUAGE parameter is set.
      \since 6.32
    */
    Q_PROPERTY(QString language READ language WRITE setLanguage)
    /*!
      \qmlproperty bool grammaticalGender::isValid
      This property holds whether the grammatical gender is nonempty.
    */
    /*!
      \property KContacts::GrammaticalGender::isValid
      This property holds whether the grammatical gender is nonempty.
    */
    Q_PROPERTY(bool isValid READ isValid)

public:
    /*!
     */
    GrammaticalGender();

    GrammaticalGender(const GrammaticalGender &other);

    /*!
     */
    Q_INVOKABLE explicit GrammaticalGender(const QString &gender);

    ~GrammaticalGender();

    /*!
     */
    void setGender(const QString &gender);

    /*!
     */
    [[nodiscard]] QString gender() const;

    /*!
     */
    [[nodiscard]] QString language() const;

    /*!
     */
    void setLanguage(const QString &language);

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const GrammaticalGender &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const GrammaticalGender &other) const;

    GrammaticalGender &operator=(const GrammaticalGender &other);

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
 * \relates KContacts::GrammaticalGender
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const GrammaticalGender &object);

/*!
 * \relates KContacts::GrammaticalGender
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, GrammaticalGender &object);
}
Q_DECLARE_TYPEINFO(KContacts::GrammaticalGender, Q_RELOCATABLE_TYPE);
#endif // GRAMMATICALGENDER_H
