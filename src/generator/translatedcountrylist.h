/*
    SPDX-FileCopyrightText: 2018 Sune Vuorela <sune@vuorela.dk>

    SPDX-License-Identifier: MIT
*/

#ifndef TRANSLATEDCOUNTRYLIST_H
#define TRANSLATEDCOUNTRYLIST_H

#include <QString>

#include <vector>

class QIODevice;

namespace TranslatedCountries
{
struct CountryNameMapping {
    QString name;
    QString isoCode;
    QString language;
};

using TranslationCountryMap = std::vector<CountryNameMapping>;

/**
 * \param directoryPath directory path to scan recursively for cldr xml files
 * On Debian systems, /usr/share/unicode/cldr/common/main is a good value for this.
 * \return map of translation name => country id
 */
TranslationCountryMap parseFilesRecursive(const QString &directoryPath);
}
#endif
