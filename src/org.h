/*
    This file is part of the KContacts framework.
    Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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

#ifndef ORG_H
#define ORG_H

#include "kcontacts_export.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include <QtCore/QMap>

/** @short Class that holds a Organization for a contact.
 *  @since 5.3
 */
namespace KContacts
{
class KCONTACTS_EXPORT Org
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Org &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Org &);
public:
    Org();
    Org(const Org &other);
    Org(const QString &org);

    ~Org();

    typedef QVector<Org> List;

    void setOrganization(const QString &org);
    QString organization() const;

    bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const Org &other) const;
    bool operator!=(const Org &other) const;

    Org &operator=(const Org &other);

    QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Org &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Org &object);

}
Q_DECLARE_TYPEINFO(KContacts::Org, Q_MOVABLE_TYPE);
#endif // ORG_H
