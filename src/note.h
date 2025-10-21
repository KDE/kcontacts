/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef NOTE_H
#define NOTE_H

#include "kcontacts_export.h"

#include <QMap>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::Note
 * \inheaderfile KContacts/Note
 * \inmodule KContacts
 *
 * \brief Class that holds a Note for a contact.
 * \since 5.3
 */
class KCONTACTS_EXPORT Note
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Note &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Note &);
    friend class VCardTool;

public:
    /*!
     */
    Note();

    Note(const Note &other);

    /*!
     */
    Note(const QString &note);

    ~Note();

    /*!
     */
    typedef QList<Note> List;

    /*!
     */
    void setNote(const QString &note);

    /*!
     */
    [[nodiscard]] QString note() const;

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const Note &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const Note &other) const;

    Note &operator=(const Note &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

private:
    KCONTACTS_NO_EXPORT void setParams(const ParameterMap &params);
    [[nodiscard]] KCONTACTS_NO_EXPORT ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Note
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Note &object);

/*!
 * \relates KContacts::Note
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Note &object);
}
Q_DECLARE_TYPEINFO(KContacts::Note, Q_RELOCATABLE_TYPE);
#endif // NOTE_H
