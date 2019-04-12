/*
 * Copyright (c) 2018 Sune Vuorela <sune@vuorela.dk>
 * Copyright (c) 2018 Volker Krause <vkrause@kde.org>
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
#include "../countrytoisomap_p.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFile>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("generate country name map"));
    app.setApplicationVersion(QStringLiteral("0.1"));

    QCommandLineParser p;
    QCommandLineOption sourceDir({QStringLiteral("sourceDir"), QStringLiteral("s")}, QStringLiteral("Location of CLDR data"), QStringLiteral("cldrdata"), QStringLiteral("/usr/share/unicode/cldr/common/main/"));
    p.addOption(sourceDir);
    QCommandLineOption outputFile({QStringLiteral("outputFile"), QStringLiteral("o")}, QStringLiteral("Output file"), QStringLiteral("output file"), QStringLiteral("countrytransl.map"));
    p.addOption(outputFile);
    p.addHelpOption();
    p.addVersionOption();

    p.process(app);

    if (!p.isSet(sourceDir)) {
        p.showHelp(0);
    }
    if (!p.isSet(outputFile)) {
        p.showHelp(0);
    }


    QString sourceDirPath = p.value(sourceDir);
    if (!QDir().exists(sourceDirPath)) {
        qCritical("Specified CLDR source dir does not exist.");
        return 1;
    }
    QString outputFilePath = p.value(outputFile);

    QFile f(outputFilePath);
    bool success = f.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if (!success) {
        qCritical("failed to open file: %s", qPrintable(f.errorString()));
        return 1;
    }

    f.write(R"(
/*
 * Generated file based on CLDR data. Do not edit!
 * See src/generator/ for how to re-generate this.
 *
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright © 1991-2018 Unicode, Inc. All rights reserved.
 * Distributed under the Terms of Use in http://www.unicode.org/copyright.html.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of the Unicode data files and any associated documentation
 * (the "Data Files") or Unicode software and any associated documentation
 * (the "Software") to deal in the Data Files or Software
 * without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, and/or sell copies of
 * the Data Files or Software, and to permit persons to whom the Data Files
 * or Software are furnished to do so, provided that either
 * (a) this copyright and permission notice appear with all copies
 * of the Data Files or Software, or
 * (b) this copyright and permission notice appear in associated
 * Documentation.
 *
 * THE DATA FILES AND SOFTWARE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT OF THIRD PARTY RIGHTS.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS
 * NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL
 * DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THE DATA FILES OR SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder
 * shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in these Data Files or Software without prior
 * written authorization of the copyright holder.
 */

#include "countrytoisomap_p.h"

using namespace KContacts;

static const char country_name_stringtable[] = {
)");

    const auto parsedList = TranslatedCountries::parseFilesRecursive(sourceDirPath);

    struct Elem {
        QByteArray name;
        QString isoCode;
        QString language;
        int offset;
    };
    std::vector<Elem> processedList;
    processedList.reserve(parsedList.size());
    for(const auto &country : parsedList) {
        const auto name = KContacts::normalizeCountryName(country.name);
        if (name.isEmpty()) {
            qWarning() << "Skipping empty normalized country name:" << country.name << country.isoCode << country.language;
            continue;
        }
        processedList.push_back(Elem{name, country.isoCode, country.language, 0});
    }
    // we must sort exactly as we do this for lookup
    std::sort(processedList.begin(), processedList.end(), [](const Elem &lhs, const Elem &rhs) {
        const auto c = strcmp(lhs.name.constData(), rhs.name.constData());
        if (c == 0)
            return lhs.isoCode < rhs.isoCode;
        return  c < 0;
    });

    // remove all duplicates referring to the same ISO code
    const auto it = std::unique(processedList.begin(), processedList.end(), [](const Elem &lhs, const Elem &rhs) {
        return strcmp(lhs.name.constData(), rhs.name.constData()) == 0 && lhs.isoCode == rhs.isoCode;
    });
    processedList.erase(it, processedList.end());

    // find duplicates that refer to different ISO codes, and remove those entirely
    for (auto it = processedList.begin() + 1; it != processedList.end();) {
        auto prevIt = it - 1;
        if (strcmp((*it).name.constData(), (*prevIt).name.constData()) != 0) {
            ++it;
            continue;
        }
        qDebug() << "Removing ambigious string:" << QString::fromUtf8((*it).name)
                 << (*prevIt).isoCode.toUpper() << (*prevIt).language
                 << (*it).isoCode.toUpper() << (*it).language;
        it = processedList.erase(prevIt);
        it = processedList.erase(it);
    }

    int offset = 0;
    for(auto& elem : processedList) {
        f.write("    ");
        bool encodedChar = false;
        // MSVC has a limit on strings of 65535 bytes, however arrays can be longer
        // so we have to encode this ~500k string as an char array manually...
        for (const char c : elem.name) {
            f.write("'");
            if (c >= 32 && c < 127) {
                f.write(&c, 1);
            } else {
                f.write("\\x");
                f.write(QByteArray::number(c, 16).right(2));
                encodedChar = true;
            }
            f.write("',");
        }
        f.write("0, // ");
        if (encodedChar) {
            f.write(elem.name);
            f.write(" ");
        }
        f.write(elem.isoCode.toUtf8());
        f.write("\n");

        elem.offset = offset;
        offset += elem.name.size() + 1; // +1 for the terminating \0
    }
    f.write(R"(
};

static const CountryToIsoIndex country_to_iso_index[] = {
)");

    for (const auto &elem: processedList) {
        f.write("    CountryToIsoIndex{");
        f.write(QByteArray::number(elem.offset));
        f.write(", \"");
        f.write(elem.isoCode.toUtf8());
        f.write("\"},\n");
    }
    f.write("};\n");

    f.close();
    return 0;
}
