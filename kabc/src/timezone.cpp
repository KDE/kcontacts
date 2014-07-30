/*
    This file is part of libkabc.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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

#include "timezone.h"

#include <QtCore/QDataStream>
#include <QtCore/QSharedData>

using namespace KABC;

class TimeZone::Private : public QSharedData
{
public:
    Private(int offset = 0, bool valid = false)
        : mOffset(offset), mValid(valid)
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mOffset = other.mOffset;
        mValid = other.mValid;
    }

    int mOffset;
    bool mValid;
};

TimeZone::TimeZone()
    : d(new Private)
{
}

TimeZone::TimeZone(int offset)
    : d(new Private(offset, true))
{
}

TimeZone::TimeZone(const TimeZone &other)
    : d(other.d)
{
}

TimeZone::~TimeZone()
{
}

void TimeZone::setOffset(int offset)
{
    d->mOffset = offset;
    d->mValid = true;
}

int TimeZone::offset() const
{
    return d->mOffset;
}

bool TimeZone::isValid() const
{
    return d->mValid;
}

bool TimeZone::operator==(const TimeZone &t) const
{
    if (!t.isValid() && !isValid()) {
        return true;
    }

    if (!t.isValid() || !isValid()) {
        return false;
    }

    if (t.d->mOffset == d->mOffset) {
        return true;
    }

    return false;
}

bool TimeZone::operator!=(const TimeZone &t) const
{
    return !(*this == t);
}

TimeZone &TimeZone::operator=(const TimeZone &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString TimeZone::toString() const
{
    QString str;

    str += QString::fromLatin1("TimeZone {\n");
    str += QString::fromLatin1("  Offset: %1\n").arg(d->mOffset);
    str += QString::fromLatin1("}\n");

    return str;
}

QDataStream &KABC::operator<<(QDataStream &s, const TimeZone &zone)
{
    return s << zone.d->mOffset << zone.d->mValid;
}

QDataStream &KABC::operator>>(QDataStream &s, TimeZone &zone)
{
    s >> zone.d->mOffset >> zone.d->mValid;

    return s;
}
