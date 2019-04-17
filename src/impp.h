/*
    This file is part of libkabc.
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

#ifndef IMPP_H
#define IMPP_H

#include "kcontacts_export.h"
#include <QSharedDataPointer>
#include <QString>
#include <QMap>

namespace KContacts {
/** @short Class that holds a IMPP for a contact.
 *
 *  IMPP stands for "Instant Messaging and Presence Protocol". This field is defined
 *  in the vCard 3.0 extension RFC 4770 and is part of vCard 4.0 (RFC 6350).
 *
 *  @since 4.14.5
 */
class KCONTACTS_EXPORT Impp
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Impp &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Impp &);
public:
    Impp();
    Impp(const Impp &other);
    Impp(const QString &address);

    ~Impp();

    typedef QVector<Impp> List;
    enum ImppType {
        Unknown = 0,
        Skype = 1,
        Xmpp = 2,
        Jabber = 3,
        Sip = 4,
        Aim = 5,
        Msn = 6,
        Twitter = 7,
        GoogleTalk = 8,
        Yahoo = 9,
        Qq = 10,
        GaduGadu = 11,
        Ownclound = 12,
        Icq = 13,
        Facebook = 14,
        EndList
    };
    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT ImppType type() const;
    void setType(ImppType type);

    void setAddress(const QString &address);
    Q_REQUIRED_RESULT QString address() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Impp &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Impp &other) const;

    Impp &operator=(const Impp &other);

    Q_REQUIRED_RESULT QString toString() const;

    Q_REQUIRED_RESULT static QString typeToString(ImppType type);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Impp &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Impp &object);
}

Q_DECLARE_TYPEINFO(KContacts::Impp, Q_MOVABLE_TYPE);
#endif // IMPP_H
