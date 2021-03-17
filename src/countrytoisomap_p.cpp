/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2019 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "isotocountrymap_p.h"

namespace KContacts
{
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
