/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2003 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vcard.h"

using namespace KContacts;

VCard::VCard()
{
}

VCard::VCard(const VCard &vcard)
{
    mLineMap = vcard.mLineMap;
}

VCard::~VCard()
{
}

VCard &VCard::operator=(const VCard &vcard)
{
    if (&vcard == this) {
        return *this;
    }

    mLineMap = vcard.mLineMap;

    return *this;
}

void VCard::clear()
{
    mLineMap.clear();
}

QStringList VCard::identifiers() const
{
    return mLineMap.keys();
}

void VCard::addLine(const VCardLine &line)
{
    mLineMap[line.identifier()].append(line);
}

VCardLine::List VCard::lines(const QString &identifier) const
{
    auto it = mLineMap.constFind(identifier);
    return it != mLineMap.cend() ? it.value() : VCardLine::List();
}

VCardLine VCard::line(const QString &identifier) const
{
    auto it = mLineMap.constFind(identifier);
    if (it != mLineMap.cend()) {
        const auto &vcardLine = it.value();
        return !vcardLine.isEmpty() ? vcardLine.first() : VCardLine{};
    }

    return {};
}

void VCard::setVersion(Version version)
{
    mLineMap.remove(QStringLiteral("VERSION"));

    VCardLine line;
    line.setIdentifier(QStringLiteral("VERSION"));
    if (version == v2_1) {
        line.setIdentifier(QStringLiteral("2.1"));
    } else if (version == v3_0) {
        line.setIdentifier(QStringLiteral("3.0"));
    } else if (version == v4_0) {
        line.setIdentifier(QStringLiteral("4.0"));
    }

    mLineMap[QStringLiteral("VERSION")].append(line);
}

VCard::Version VCard::version() const
{
    auto versionEntryIt = mLineMap.constFind(QStringLiteral("VERSION"));
    if (versionEntryIt == mLineMap.cend()) {
        return v3_0;
    }

    const VCardLine line = versionEntryIt.value().at(0);
    if (line.value() == QLatin1String("2.1")) {
        return v2_1;
    }
    if (line.value() == QLatin1String("3.0")) {
        return v3_0;
    }

    return v4_0;
}
