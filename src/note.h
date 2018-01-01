/*
    This file is part of the KContacts framework.
    Copyright (C) 2016-2018 Laurent Montel <montel@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef NOTE_H
#define NOTE_H

#include "kcontacts_export.h"
#include <QSharedDataPointer>
#include <QString>
#include <QMap>

/** @short Class that holds a Note for a contact.
 *  @since 5.3
 */
namespace KContacts
{
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
    QString note() const;

    bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const Note &other) const;
    bool operator!=(const Note &other) const;

    Note &operator=(const Note &other);

    QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Note &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Note &object);

}
Q_DECLARE_TYPEINFO(KContacts::Note, Q_MOVABLE_TYPE);
#endif // NOTE_H
