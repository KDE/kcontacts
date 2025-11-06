/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "timezone.h"

#include <QDataStream>
#include <QSharedData>

using namespace KContacts;

class Q_DECL_HIDDEN TimeZone::Private : public QSharedData
{
public:
    Private(int offset = 0, bool valid = false)
        : mOffset(offset)
        , mValid(valid)
    {
    }

    Private(const QByteArray &name, bool valid = false)
        : mTimeZoneName(name)
        , mValid(valid)
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mOffset = other.mOffset;
        mValid = other.mValid;
        mTimeZoneName = other.mTimeZoneName;
    }

    QByteArray mTimeZoneName;
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

TimeZone::TimeZone(const QByteArray &name)
    : d(new Private(name, true))
{
}

TimeZone::TimeZone(const TimeZone &other)
    : d(other.d)
{
}

TimeZone::~TimeZone() = default;

void TimeZone::setOffset(int offset)
{
    d->mOffset = offset;
    d->mValid = true;
}

int TimeZone::offset() const
{
    return d->mOffset;
}

void TimeZone::setTimeZoneName(const QByteArray &name)
{
    d->mTimeZoneName = name;
    d->mValid = true;
}

QByteArray TimeZone::timeZoneName() const
{
    return d->mTimeZoneName;
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

    if ((t.d->mOffset == d->mOffset) && (t.d->mTimeZoneName == d->mTimeZoneName)) {
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
    QString str = QLatin1String("TimeZone {\n");
    if (d->mTimeZoneName.isEmpty()) {
        str += QStringLiteral("  Offset: %1\n").arg(d->mOffset);
    } else {
        str += QStringLiteral("  TimeZone: %1\n").arg(d->mTimeZoneName);
    }
    str += QLatin1String("}\n");

    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const TimeZone &zone)
{
    return s << zone.d->mOffset << zone.d->mValid << zone.d->mTimeZoneName;
}

QDataStream &KContacts::operator>>(QDataStream &s, TimeZone &zone)
{
    s >> zone.d->mOffset >> zone.d->mValid >> zone.d->mTimeZoneName;

    return s;
}
