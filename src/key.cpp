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

#include "key.h"

#include <klocalizedstring.h>
#include <krandom.h>

#include <QtCore/QDataStream>
#include <QtCore/QSharedData>

using namespace KContacts;

class Key::Private : public QSharedData
{
public:
    Private()
        : mId(KRandom::randomString(8))
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mId = other.mId;
        mBinaryData = other.mBinaryData;
        mTextData = other.mTextData;
        mCustomTypeString = other.mCustomTypeString;
        mIsBinary = other.mIsBinary;
        mType = other.mType;
    }

    QString mId;
    QByteArray mBinaryData;
    QString mTextData;
    QString mCustomTypeString;

    bool mIsBinary;
    Type mType;
};

Key::Key(const QString &text, Type type)
    : d(new Private)
{
    d->mTextData = text;
    d->mIsBinary = false;
    d->mType = type;
}

Key::Key(const Key &other)
    : d(other.d)
{
}

Key::~Key()
{
}

bool Key::operator==(const Key &other) const
{
    if (d->mId != other.d->mId) {
        return false;
    }

    if (d->mType != other.d->mType) {
        return false;
    }

    if (d->mIsBinary != other.d->mIsBinary) {
        return false;
    }

    if (d->mIsBinary) {
        if (d->mBinaryData != other.d->mBinaryData) {
            return false;
        }
    } else {
        if (d->mTextData != other.d->mTextData) {
            return false;
        }
    }

    if (d->mCustomTypeString != other.d->mCustomTypeString) {
        return false;
    }

    return true;
}

bool Key::operator!=(const Key &other) const
{
    return !(*this == other);
}

Key &Key::operator=(const Key &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

void Key::setId(const QString &id)
{
    d->mId = id;
}

QString Key::id() const
{
    return d->mId;
}

void Key::setBinaryData(const QByteArray &binary)
{
    d->mBinaryData = binary;
    d->mIsBinary = true;
}

QByteArray Key::binaryData() const
{
    return d->mBinaryData;
}

void Key::setTextData(const QString &text)
{
    d->mTextData = text;
    d->mIsBinary = false;
}

QString Key::textData() const
{
    return d->mTextData;
}

bool Key::isBinary() const
{
    return d->mIsBinary;
}

void Key::setType(Type type)
{
    d->mType = type;
}

void Key::setCustomTypeString(const QString &custom)
{
    d->mCustomTypeString = custom;
}

Key::Type Key::type() const
{
    return d->mType;
}

QString Key::customTypeString() const
{
    return d->mCustomTypeString;
}

QString Key::toString() const
{
    QString str;

    str += QLatin1String("Key {\n");
    str += QString::fromLatin1("  Id: %1\n").arg(d->mId);
    str += QString::fromLatin1("  Type: %1\n").arg(typeLabel(d->mType));
    if (d->mType == Custom) {
        str += QString::fromLatin1("  CustomType: %1\n").arg(d->mCustomTypeString);
    }
    str += QString::fromLatin1("  IsBinary: %1\n").
           arg(d->mIsBinary ? QLatin1String("true") : QLatin1String("false"));
    if (d->mIsBinary) {
        str += QString::fromLatin1("  Binary: %1\n").
               arg(QString::fromLatin1(d->mBinaryData.toBase64()));
    } else {
        str += QString::fromLatin1("  Text: %1\n").arg(d->mTextData);
    }
    str += QLatin1String("}\n");

    return str;
}

Key::TypeList Key::typeList()
{
    static TypeList list;

    if (list.isEmpty()) {
        list << X509 << PGP << Custom;
    }

    return list;
}

QString Key::typeLabel(Type type)
{
    switch (type) {
    case X509:
        return i18nc("X.509 public key", "X509");
        break;
    case PGP:
        return i18nc("Pretty Good Privacy key", "PGP");
        break;
    case Custom:
        return i18nc("A custom key", "Custom");
        break;
    default:
        return i18nc("another type of encryption key", "Unknown type");
        break;
    }
}

QDataStream &KContacts::operator<<(QDataStream &s, const Key &key)
{
    return s << key.d->mId << key.d->mType << key.d->mIsBinary << key.d->mBinaryData
           << key.d->mTextData << key.d->mCustomTypeString;
}

QDataStream &KContacts::operator>>(QDataStream &s, Key &key)
{
    uint type;
    s >> key.d->mId >> type >> key.d->mIsBinary >> key.d->mBinaryData >> key.d->mTextData
      >> key.d->mCustomTypeString;

    key.d->mType = Key::Type(type);

    return s;
}
