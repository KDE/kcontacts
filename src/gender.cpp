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

#include "gender.h"

#include <QMap>
#include <QStringList>

using namespace KContacts;

class Q_DECL_HIDDEN Gender::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        comment = other.comment;
        gender = other.gender;
    }

    QString gender;
    QString comment;
};

Gender::Gender()
    : d(new Private)
{
}

Gender::Gender(const QString &gender)
    : d(new Private)
{
    d->gender = gender;
}

Gender::Gender(const Gender &other)
    : d(other.d)
{
}

Gender::~Gender()
{
}

bool Gender::operator==(const Gender &other) const
{
    return (d->comment == other.comment()) && (d->gender == other.gender());
}

bool Gender::operator!=(const Gender &other) const
{
    return !(other == *this);
}

Gender &Gender::operator=(const Gender &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Gender::toString() const
{
    QString str = QLatin1String("Gender {\n");
    str += QStringLiteral("    gender: %1\n").arg(d->gender);
    str += QStringLiteral("    comment: %1\n").arg(d->comment);
    str += QLatin1String("}\n");
    return str;
}

void Gender::setGender(const QString &gender)
{
    d->gender = gender;
}

QString Gender::gender() const
{
    return d->gender;
}

void Gender::setComment(const QString &comment)
{
    d->comment = comment;
}

QString Gender::comment() const
{
    return d->comment;
}

bool Gender::isValid() const
{
    return !d->gender.isEmpty() || !d->comment.isEmpty();
}

QDataStream &KContacts::operator<<(QDataStream &s, const Gender &gender)
{
    return s << gender.d->comment << gender.d->gender;
}

QDataStream &KContacts::operator>>(QDataStream &s, Gender &gender)
{
    s >> gender.d->comment >> gender.d->gender;
    return s;
}
