/*
 * Copyright (c) 2018 Sune Vuorela <sune@vuorela.dk>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


#include "translatedcountrylist.h"

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QXmlStreamReader>

static void parseFile(QIODevice *device, const QString &lang, TranslatedCountries::TranslationCountryMap &map)
{
    QXmlStreamReader reader(device);
    const QStringList blacklist = { QLatin1String("eu"), QLatin1String("un"), QLatin1String("zz") };
    while(!reader.atEnd()) {
        QXmlStreamReader::TokenType type = reader.readNext();
        switch(type) {
            case QXmlStreamReader::StartElement:
                if (reader.name() == QLatin1String("territory")) {
                    const auto territory = reader.attributes().value(QLatin1String("type")).toString().toLower();
                    if (territory.size() != 2 || !territory.at(0).isLetter() || !territory.at(1).isLetter() || blacklist.contains(territory)) {
                        break;
                    }
                    const auto name = reader.readElementText();
                    if (!name.isEmpty()) {
                        map.push_back({name, territory, lang});
                    }
                }
                break;
            default:
                break;
        }
    }
}

static void parseFilePath(const QString &filePath, TranslatedCountries::TranslationCountryMap &map)
{
    QFile f(filePath);
    bool success = f.open(QIODevice::ReadOnly);
    if (!success) {
        qWarning() << "Failed to open" << f.fileName() << f.errorString();
        return;
    }
    return parseFile(&f, QFileInfo(filePath).baseName(), map);
}

TranslatedCountries::TranslationCountryMap TranslatedCountries::parseFilesRecursive(const QString &directoryPath)
{
    QDirIterator it(directoryPath, {QStringLiteral("*.xml")}, QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
    TranslationCountryMap map;
    while (it.hasNext()) {
        it.next();
        QString path = it.filePath();
        parseFilePath(path, map);
    }
    return map;
}
