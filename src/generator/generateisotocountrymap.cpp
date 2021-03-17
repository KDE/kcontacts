/*
    SPDX-FileCopyrightText: 2018 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCommandLineParser p;
    QCommandLineOption inputFile({QStringLiteral("i")},
                                 QStringLiteral("Location of iso_3166-1.json"),
                                 QStringLiteral("iso-codes"),
                                 QStringLiteral("/usr/share/iso-codes/json/iso_3166-1.json"));
    p.addOption(inputFile);
    QCommandLineOption outputFile({QStringLiteral("outputFile"), QStringLiteral("o")}, QStringLiteral("Output file"), QStringLiteral("output file"), {});
    p.addOption(outputFile);
    p.addHelpOption();
    p.process(app);
    if (!p.isSet(inputFile) || !p.isSet(outputFile)) {
        p.showHelp(0);
    }

    QFile input(p.value(inputFile));
    if (!input.open(QFile::ReadOnly)) {
        qFatal("failed to open input file: %s", qPrintable(input.errorString()));
    }
    const auto doc = QJsonDocument::fromJson(input.readAll());

    struct Entry {
        QString isoCode;
        QString name;
        int offset;
    };
    std::vector<Entry> countries;
    for (const auto v : doc.object().value(QLatin1String("3166-1")).toArray()) {
        const auto obj = v.toObject();
        countries.push_back(Entry{obj.value(QLatin1String("alpha_2")).toString().toLower(), obj.value(QLatin1String("name")).toString(), 0});
    }
    std::sort(countries.begin(), countries.end(), [](const Entry &lhs, const Entry &rhs) {
        return lhs.isoCode < rhs.isoCode;
    });

    QFile out(p.value(outputFile));
    if (!out.open(QFile::WriteOnly)) {
        qFatal("failed to open file: %s", qPrintable(out.errorString()));
    }

    out.write(R"(
/*
    Generated file based on iso-codes data. Do not edit!
    See src/generator/ for how to re-generate this.

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "isotocountrymap_p.h"

using namespace KContacts;

static const char en_country_name_stringtable[] = {
)");

    int off = 0;
    for (auto &c : countries) {
        out.write("    \"");
        out.write(c.name.toUtf8());
        out.write("\\0\"\n");
        c.offset = off;
        off += c.name.toUtf8().size() + 1;
    }
    out.write(R"(
};

static const IsoToCountryIndex iso_to_country_index[] = {
)");

    for (const auto &c : countries) {
        out.write("    IsoToCountryIndex{\"");
        out.write(c.isoCode.toUtf8());
        out.write("\", ");
        out.write(QByteArray::number(c.offset));
        out.write("},\n");
    }
    out.write("};\n");
}
