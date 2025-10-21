/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef RELATED_H
#define RELATED_H

#include "kcontacts_export.h"

#include <QMap>
#include <QSharedDataPointer>
#include <QString>

class RelatedTest;

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::Related
 * \inheaderfile KContacts/Related
 * \inmodule KContacts
 *
 * \brief Describes a relationship of an Addressee.
 */
class KCONTACTS_EXPORT Related
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Related &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Related &);
    friend class VCardTool;
    friend class ::RelatedTest;

public:
    /*!
     */
    Related();

    Related(const Related &other);

    /*!
     */
    Related(const QString &related);

    ~Related();

    /*!
     */
    typedef QList<Related> List;

    /*!
     */
    void setRelated(const QString &relatedTo);

    /*!
     */
    [[nodiscard]] QString related() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const Related &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const Related &other) const;

    Related &operator=(const Related &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

private:
    // exported for RelatedTest
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Related
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Related &object);

/*!
 * \relates KContacts::Related
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Related &object);
}
Q_DECLARE_TYPEINFO(KContacts::Related, Q_RELOCATABLE_TYPE);
#endif // LANG_H
