/*
    This file is part of the KContacts framework.
    Copyright (c) 2019 Volker Krause <vkrause@kde.org>

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

#include "isotocountrymap_p.h"

namespace KContacts {

QByteArray normalizeCountryName(const QString &name)
{
    QString res;
    res.reserve(name.size());
    for (const auto c : name) {
        // the following needs to be done fairly fine-grained, as this can easily mess up scripts
        // that rely on some non-letter characters to work
        // when changing this, check changes to the ambiguity warnings of the generator
        switch (c.category()) {
            // strip decorative elements that don't contribute to identification (parenthesis, dashes, quotes, etc)
            case QChar::Punctuation_Connector:
            case QChar::Punctuation_Dash:
            case QChar::Punctuation_Open:
            case QChar::Punctuation_Close:
            case QChar::Punctuation_InitialQuote:
            case QChar::Punctuation_FinalQuote:
            case QChar::Punctuation_Other:
                continue;
            // strip spaces
            case QChar::Separator_Space:
            case QChar::Separator_Line:
            case QChar::Separator_Paragraph:
                continue;
            default:
                break;
        }

        if (c.isSpace()) {
            continue;
        }

        // if the character has a canonical decomposition skip the combining diacritic markers following it
        // this works particularly well for Latin, but seems to mess things up in Hangul
        if (c.script() != QChar::Script_Hangul && c.decompositionTag() == QChar::Canonical) {
            res.push_back(c.decomposition().at(0).toCaseFolded());
        } else {
            res.push_back(c.toCaseFolded());
        }
    }

    return res.toUtf8();
}

}
