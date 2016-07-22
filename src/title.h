/*
    This file is part of the KContacts framework.
    Copyright (c) 2016 Laurent Montel <montel@kde.org>

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

#ifndef TITLE_H
#define TITLE_H

#include "kcontacts_export.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include <QtCore/QMap>

/** @short Class that holds a Title for a contact.
 *  @since 5.3
 */
namespace KContacts
{
class KCONTACTS_EXPORT Title
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Title &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Title &);
public:
    Title();
    Title(const Title &other);
    Title(const QString &titleuage);

    ~Title();

    typedef QVector<Title> List;

    void setTitle(const QString &title);
    QString title() const;

    bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const Title &other) const;
    bool operator!=(const Title &other) const;

    Title &operator=(const Title &other);

    QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Title &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Title &object);

}
Q_DECLARE_TYPEINFO(KContacts::Title, Q_MOVABLE_TYPE);
#endif // TITLE_H
