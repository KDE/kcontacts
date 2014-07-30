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

#include "geo.h"

#include <QtCore/QDataStream>
#include <QtCore/QSharedData>

using namespace KABC;

class Geo::Private : public QSharedData
{
public:
    Private()
        : mLatitude(91), mLongitude(181),
          mValidLatitude(false), mValidLongitude(false)
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mLatitude = other.mLatitude;
        mLongitude = other.mLongitude;
        mValidLatitude = other.mValidLatitude;
        mValidLongitude = other.mValidLongitude;
    }

    float mLatitude;
    float mLongitude;

    bool mValidLatitude;
    bool mValidLongitude;
};

Geo::Geo()
    : d(new Private)
{
}

Geo::Geo(float latitude, float longitude)
    : d(new Private)
{
    setLatitude(latitude);
    setLongitude(longitude);
}

Geo::Geo(const Geo &other)
    : d(other.d)
{
}

Geo::~Geo()
{
}

void Geo::setLatitude(float latitude)
{
    if (latitude >= -90 && latitude <= 90) {
        d->mLatitude = latitude;
        d->mValidLatitude = true;
    } else {
        d->mLatitude = 91;
        d->mValidLatitude = false;
    }
}

float Geo::latitude() const
{
    return d->mLatitude;
}

void Geo::setLongitude(float longitude)
{
    if (longitude >= -180 && longitude <= 180) {
        d->mLongitude = longitude;
        d->mValidLongitude = true;
    } else {
        d->mLongitude = 181;
        d->mValidLongitude = false;
    }
}

float Geo::longitude() const
{
    return d->mLongitude;
}

bool Geo::isValid() const
{
    return d->mValidLatitude && d->mValidLongitude;
}

bool Geo::operator==(const Geo &other) const
{
    if (!other.isValid() && !isValid()) {
        return true;
    }

    if (!other.isValid() || !isValid()) {
        return false;
    }

    if (other.d->mLatitude == d->mLatitude && other.d->mLongitude == d->mLongitude) {
        return true;
    }

    return false;
}

bool Geo::operator!=(const Geo &other) const
{
    return !(*this == other);
}

Geo &Geo::operator=(const Geo &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Geo::toString() const
{
    QString str;

    str += QLatin1String("Geo {\n");
    str += QString::fromLatin1("  Valid: %1\n").
           arg(isValid() ? QLatin1String("true") : QLatin1String("false"));
    str += QString::fromLatin1("  Latitude: %1\n").arg(d->mLatitude);
    str += QString::fromLatin1("  Longitude: %1\n").arg(d->mLongitude);
    str += QLatin1String("}\n");

    return str;
}

QDataStream &KABC::operator<<(QDataStream &s, const Geo &geo)
{
    return s << geo.d->mLatitude << geo.d->mValidLatitude
           << geo.d->mLongitude << geo.d->mValidLongitude;
}

QDataStream &KABC::operator>>(QDataStream &s, Geo &geo)
{
    s >> geo.d->mLatitude >> geo.d->mValidLatitude
      >> geo.d->mLongitude >> geo.d->mValidLongitude;

    return s;
}
