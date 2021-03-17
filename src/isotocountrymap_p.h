/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2018 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_ISOTOCOUNTRYMAP_P_H
#define KCONTACTS_ISOTOCOUNTRYMAP_P_H

#include <QString>

namespace KContacts
{
/* ISO code to country name string table lookup table entry. */
struct IsoToCountryIndex {
    explicit constexpr inline IsoToCountryIndex(const char isoCode[2], int offset)
        : m_c1(isoCode[0])
        , m_c2(isoCode[1])
        , m_offset(offset)
    {
    }

    char m_c1;
    char m_c2;
    uint16_t m_offset;
};
}

#endif
