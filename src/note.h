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
/** @short Class that holds a Note for a contact.
 *  @since 5.3
 */
class KCONTACTS_EXPORT Note
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Note &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Note &);

public:
    Note();
    Note(const Note &other);
    Note(const QString &note);

    ~Note();

    typedef QVector<Note> List;

    void setNote(const QString &note);
    Q_REQUIRED_RESULT QString note() const;

    Q_REQUIRED_RESULT bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Note &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Note &other) const;

    Note &operator=(const Note &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Note &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Note &object);
}
Q_DECLARE_TYPEINFO(KContacts::Note, Q_MOVABLE_TYPE);
#endif // NOTE_H
