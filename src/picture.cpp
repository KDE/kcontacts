/*
    This file is part of the KContacts framework.
    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>

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

#include "picture.h"

#include <QtCore/QBuffer>
#include <QtCore/QSharedData>

namespace KContacts
{

class PicturePrivate : public QSharedData
{
public:
    PicturePrivate()
        : mIntern(false)
    {
    }

    PicturePrivate(const PicturePrivate &other)
        : QSharedData(other)
    {
        mUrl = other.mUrl;
        mType = other.mType;
        mData = other.mData;
        mIntern = other.mIntern;
    }

    QString mUrl;
    QString mType;
    mutable QImage mData;
    mutable QByteArray mRawData;

    bool mIntern;
};

}

Q_GLOBAL_STATIC_WITH_ARGS(QSharedDataPointer<KContacts::PicturePrivate>, s_sharedEmpty, (new KContacts::PicturePrivate))

using namespace KContacts;

Picture::Picture()
    : d(*s_sharedEmpty())
{
}

Picture::Picture(const QString &url)
    : d(new PicturePrivate)
{
    d->mUrl = url;
}

Picture::Picture(const QImage &data)
    : d(new PicturePrivate)
{
    setData(data);
}

Picture::Picture(const Picture &other)
    : d(other.d)
{
}

Picture::~Picture()
{
}

Picture &Picture::operator=(const Picture &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

bool Picture::operator==(const Picture &p) const
{
    if (d->mIntern != p.d->mIntern) {
        return false;
    }

    if (d->mType != p.d->mType) {
        return false;
    }

    if (d->mIntern) {
        if (!d->mData.isNull() && !p.d->mData.isNull()) {
            if (d->mData != p.d->mData) {
                return false;
            }
        } else if (!d->mRawData.isEmpty() && !p.d->mRawData.isEmpty()) {
            if (d->mRawData != p.d->mRawData) {
                return false;
            }
        } else if ((!d->mData.isNull() || !d->mRawData.isEmpty()) &&
                   (!p.d->mData.isNull() || !p.d->mRawData.isEmpty())) {
            if (data() != p.data()) {
                return false;
            }
        } else {
            // if one picture is empty and the other is not
            return false;
        }
    } else {
        if (d->mUrl != p.d->mUrl) {
            return false;
        }
    }

    return true;
}

bool Picture::operator!=(const Picture &p) const
{
    return !(p == *this);
}

bool Picture::isEmpty() const
{
    return
        ((d->mIntern == false && d->mUrl.isEmpty()) ||
         (d->mIntern == true && d->mData.isNull() && d->mRawData.isEmpty()));
}

void Picture::setUrl(const QString &url)
{
    d->mUrl = url;
    d->mType.clear();
    d->mIntern = false;
}

void Picture::setUrl(const QString &url, const QString &type)
{
    d->mUrl = url;
    d->mType = type;
    d->mIntern = false;
}

void Picture::setData(const QImage &data)
{
    d->mRawData.clear();
    d->mData = data;
    d->mIntern = true;

// set the type, the raw data will have when accessed through Picture::rawData()
    if (!d->mData.hasAlphaChannel()) {
        d->mType = QLatin1String("jpeg");
    } else {
        d->mType = QLatin1String("png");
    }
}

void Picture::setRawData(const QByteArray &rawData, const QString &type)
{
    d->mRawData = rawData;
    d->mType = type;
    d->mData = QImage();
    d->mIntern = true;
}

void Picture::setType(const QString &type)
{
    d->mType = type;
}

bool Picture::isIntern() const
{
    return d->mIntern;
}

QString Picture::url() const
{
    return d->mUrl;
}

QImage Picture::data() const
{
    if (d->mData.isNull() && !d->mRawData.isEmpty()) {
        d->mData.loadFromData(d->mRawData);
    }

    return d->mData;
}

QByteArray Picture::rawData() const
{
    if (d->mRawData.isEmpty() && !d->mData.isNull()) {
        QBuffer buffer(&d->mRawData);
        buffer.open(QIODevice::WriteOnly);

        // d->mType was already set accordingly by Picture::setData()
        d->mData.save(&buffer, d->mType.toUpper().toLatin1().data());
    }

    return d->mRawData;
}

QString Picture::type() const
{
    return d->mType;
}

QString Picture::toString() const
{
    QString str;

    str += QLatin1String("Picture {\n");
    str += QString::fromLatin1("  Type: %1\n").arg(d->mType);
    str += QString::fromLatin1("  IsIntern: %1\n").
           arg(d->mIntern ? QLatin1String("true") : QLatin1String("false"));
    if (d->mIntern) {
        str += QString::fromLatin1("  Data: %1\n").arg(QString::fromLatin1(rawData().toBase64()));
    } else {
        str += QString::fromLatin1("  Url: %1\n").arg(d->mUrl);
    }
    str += QLatin1String("}\n");

    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Picture &picture)
{
    return s << picture.d->mIntern << picture.d->mUrl << picture.d->mType << picture.data();
}

QDataStream &KContacts::operator>>(QDataStream &s, Picture &picture)
{
    s >> picture.d->mIntern >> picture.d->mUrl >> picture.d->mType >> picture.d->mData;

    return s;
}
