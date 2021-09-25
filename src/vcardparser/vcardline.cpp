/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2003 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vcardline.h"

using namespace KContacts;

VCardLine::VCardLine()
{
}

VCardLine::VCardLine(const QString &identifier)
{
    mIdentifier = identifier;
}

VCardLine::VCardLine(const QString &identifier, const QVariant &value)
{
    mIdentifier = identifier;
    mValue = value;
}

VCardLine::VCardLine(const VCardLine &line)
{
    mParamMap = line.mParamMap;
    mValue = line.mValue;
    mIdentifier = line.mIdentifier;
    mGroup = line.mGroup;
}

VCardLine::~VCardLine()
{
}

VCardLine &VCardLine::operator=(const VCardLine &line)
{
    if (&line == this) {
        return *this;
    }

    mParamMap = line.mParamMap;
    mValue = line.mValue;
    mIdentifier = line.mIdentifier;
    mGroup = line.mGroup;

    return *this;
}

bool VCardLine::operator==(const VCardLine &other) const
{
    // clang-format off
    return (mParamMap == other.parameterMap())
           && (mValue == other.value())
           && (mIdentifier == other.identifier())
           && (mGroup == other.group());
    // clang-format on
}

void VCardLine::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QString VCardLine::identifier() const
{
    return mIdentifier;
}

void VCardLine::setValue(const QVariant &value)
{
    mValue = value;
}

QVariant VCardLine::value() const
{
    return mValue;
}

void VCardLine::setGroup(const QString &group)
{
    mGroup = group;
}

QString VCardLine::group() const
{
    return mGroup;
}

bool VCardLine::hasGroup() const
{
    return !mGroup.isEmpty();
}

QStringList VCardLine::parameterList() const
{
    return mParamMap.keys();
}

void VCardLine::addParameter(const QString &param, const QString &value)
{
    QStringList &list = mParamMap[param];
    if (!list.contains(value)) { // not included yet
        list.append(value);
    }
}

QStringList VCardLine::parameters(const QString &param) const
{
    auto it = mParamMap.constFind(param);
    return it != mParamMap.cend() ? it.value() : QStringList();
}

QString VCardLine::parameter(const QString &param) const
{
    auto it = mParamMap.constFind(param);
    if (it != mParamMap.cend()) {
        const QStringList lst = it.value();
        return !lst.isEmpty() ? lst.first() : QString{};
    }

    return QString();
}

VCardLine::ParamMap VCardLine::parameterMap() const
{
    return mParamMap;
}
