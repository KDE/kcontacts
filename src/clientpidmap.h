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

#ifndef CLIENTPIDMAP_H
#define CLIENTPIDMAP_H

#include "kcontacts_export.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include <QtCore/QMap>

/** @short Class that holds a ClientPidMap for a contact.
 *  @since 5.4
 */
namespace KContacts
{
class KCONTACTS_EXPORT ClientPidMap
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const ClientPidMap &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, ClientPidMap &);
public:
    ClientPidMap();
    ClientPidMap(const ClientPidMap &other);
    ClientPidMap(const QString &clientpidmap);

    ~ClientPidMap();

    typedef QVector<ClientPidMap> List;

    void setClientPidMap(const QString &clientpidmap);
    QString clientPidMap() const;

    bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const ClientPidMap &other) const;
    bool operator!=(const ClientPidMap &other) const;

    ClientPidMap &operator=(const ClientPidMap &other);

    QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const ClientPidMap &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, ClientPidMap &object);

}
Q_DECLARE_TYPEINFO(KContacts::ClientPidMap, Q_MOVABLE_TYPE);
#endif // CLIENTPIDMAP_H
