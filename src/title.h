/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TITLE_H
#define TITLE_H

#include "kcontacts_export.h"

#include <QMap>
#include <QSharedDataPointer>
#include <QString>

class TitleTest;

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::Title
 * \inheaderfile KContacts/Title
 * \inmodule KContacts
 *
 * \brief Class that holds a Title for a contact.
 * \since 5.3
 */
class KCONTACTS_EXPORT Title
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Title &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Title &);
    friend class VCardTool;
    friend class ::TitleTest;

public:
    /*!
     */
    Title();

    Title(const Title &other);

    /*!
     */
    Title(const QString &title);

    ~Title();

    /*!
     */
    typedef QList<Title> List;

    /*!
     */
    void setTitle(const QString &title);

    /*!
     */
    [[nodiscard]] QString title() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const Title &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const Title &other) const;

    Title &operator=(const Title &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

private:
    // exported for TitleTest
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Title
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Title &object);

/*!
 * \relates KContacts::Title
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Title &object);
}
Q_DECLARE_TYPEINFO(KContacts::Title, Q_RELOCATABLE_TYPE);
#endif // TITLE_H
