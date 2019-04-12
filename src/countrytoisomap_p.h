/*
    This file is part of the KContacts framework.
    Copyright (c) 2018 Volker Krause <vkrause@kde.org>

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

#ifndef KCONTACTS_COUNTRYTOISOMAP_P_H
#define KCONTACTS_COUNTRYTOISOMAP_P_H

#include <QString>

class QByteArray;

namespace KContacts {

/* Pack the string table offset and the country ISO code into 32 bit.
 * The ISO code needs 2x 5bit, leaving 22 bit for the offset, which is plenty
 * considering the string is only about 500k chars long.
 */
struct CountryToIsoIndex
{
    explicit constexpr inline CountryToIsoIndex(int offset, const char isoCode[2])
        : m_offset(offset)
        , m_c1(isoCode[0] - 'a')
        , m_c2(isoCode[1] - 'a')
    {
    }

    inline QString isoCode() const
    {
        char s[2];
        s[0] = 'a' + m_c1;
        s[1] = 'a' + m_c2;
        return QLatin1String(s, 2);
    }

    uint32_t m_offset: 22;
    uint32_t m_c1: 5;
    uint32_t m_c2: 5;
};

QByteArray normalizeCountryName(const QString &name);

}

#endif
