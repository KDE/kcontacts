/*
    SPDX-FileCopyrightText: 2018 Sune Vuorela <sune@vuorela.dk>
    SPDX-FileCopyrightText: 2018 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#include "../countrytoisomap_p.h"
#include "translatedcountrylist.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("generate country name map"));
    app.setApplicationVersion(QStringLiteral("0.1"));

    QCommandLineParser p;
    QCommandLineOption sourceDir({QStringLiteral("sourceDir"), QStringLiteral("s")},
                                 QStringLiteral("Location of CLDR data"),
                                 QStringLiteral("cldrdata"),
                                 QStringLiteral("/usr/share/unicode/cldr/common/main/"));
    p.addOption(sourceDir);
    QCommandLineOption outputFile({QStringLiteral("outputFile"), QStringLiteral("o")},
                                  QStringLiteral("Output file"),
                                  QStringLiteral("output file"),
                                  QStringLiteral("countrytransl.map"));
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
 * SPDX-FileCopyrightText: 1991-2018 Unicode Inc. All rights reserved.
 * SPDX-License-Identifier: Unicode-DFS-2016
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
    for (const auto &country : parsedList) {
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
        if (c == 0) {
            return lhs.isoCode < rhs.isoCode;
        }
        return c < 0;
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
        qDebug() << "Removing ambigious string:" << QString::fromUtf8((*it).name) << (*prevIt).isoCode.toUpper() << (*prevIt).language
                 << (*it).isoCode.toUpper() << (*it).language;
        it = processedList.erase(prevIt);
        it = processedList.erase(it);
    }

    int offset = 0;
    for (auto &elem : processedList) {
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

    for (const auto &elem : processedList) {
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
