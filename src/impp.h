/*
    This file is part of libkabc.
    Copyright (c) 2015 Laurent Montel <montel@kde.org>

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
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include <QtCore/QMap>

/** @short Class that holds a IMPP for a contact.
 *  @since 4.14.5
 */
namespace KContacts
{
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
        Apple = 10,
        GaduGadu = 11,
        Ownclound = 12,
        Icq = 13
    };
    bool isValid() const;

    ImppType type() const;
    void setType(ImppType type);

    void setAddress(const QString &address);
    QString address() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const Impp &other) const;
    bool operator!=(const Impp &other) const;

    Impp &operator=(const Impp &other);

    QString toString() const;

    QString typeToString(ImppType type);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Impp &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Impp &object);

}

#endif // IMPP_H
