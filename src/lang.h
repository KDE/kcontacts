/*
    This file is part of the KContacts framework.
    Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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

#ifndef LANG_H
#define LANG_H

#include "kcontacts_export.h"
#include <QSharedDataPointer>
#include <QString>
#include <QMap>

/** @short Class that holds a Language for a contact.
 *  @since 4.14.5
 */
namespace KContacts {
class KCONTACTS_EXPORT Lang
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Lang &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Lang &);
public:
    Lang();
    Lang(const Lang &other);
    Lang(const QString &language);

    ~Lang();

    typedef QVector<Lang> List;

    void setLanguage(const QString &lang);
    Q_REQUIRED_RESULT QString language() const;

    Q_REQUIRED_RESULT bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Lang &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Lang &other) const;

    Lang &operator=(const Lang &other);

    Q_REQUIRED_RESULT QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Lang &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Lang &object);
}
Q_DECLARE_TYPEINFO(KContacts::Lang, Q_MOVABLE_TYPE);
#endif // LANG_H
