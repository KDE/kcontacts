/*
    This file is part of the KContacts framework.
    Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#ifndef RELATED_H
#define RELATED_H

#include "kcontacts_export.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include <QtCore/QMap>

namespace KContacts
{
class KCONTACTS_EXPORT Related
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Related &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Related &);
public:
    Related();
    Related(const Related &other);
    Related(const QString &related);

    ~Related();

    typedef QVector<Related> List;

    void setRelated(const QString &relatedTo);
    QString related() const;

    bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const Related &other) const;
    bool operator!=(const Related &other) const;

    Related &operator=(const Related &other);

    QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Related &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Related &object);

}
Q_DECLARE_TYPEINFO(KContacts::Related, Q_MOVABLE_TYPE);
#endif // LANG_H
