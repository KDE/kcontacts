/*
    This file is part of the KContacts framework.
    Copyright (c) 2007 KDE-PIM team <kde-pim@kde.org>

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

#include <iostream>
#include <stdlib.h>

#include <QtCore/QFile>

#include <qdebug.h>
#include <KAboutData>
#include <QCoreApplication>
#include <klocalizedstring.h>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "kcontacts/vcardconverter.h"
#include "vcard.h"

int main(int argc, char **argv)
{
    KAboutData aboutData(QLatin1String("testread"), i18n("vCard test reader"), QLatin1String("0.1"));
    aboutData.addAuthor(i18n("Cornelius Schumacher"), QString(), QLatin1String("schumacher@kde.org"));

    QCoreApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("vcard21"), i18n("vCard 2.1")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("+inputfile"), i18n("Input file")));

    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    if (parser.positionalArguments().count() != 1) {
        std::cerr << "Missing argument" << std::endl;
        return 1;
    }

    QString inputFile(parser.positionalArguments().at(0));

    QFile file(inputFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug("Unable to open file '%s' for reading!", qPrintable(file.fileName()));
        return 1;
    }

    QByteArray text = file.readAll();
    file.close();

    KContacts::VCardConverter converter;
    KContacts::Addressee::List list = converter.parseVCards(text);

    if (parser.isSet(QLatin1String("vcard21"))) {
        text = converter.createVCards(list, KContacts::VCardConverter::v2_1);   // uses version 2.1
    } else {
        text = converter.createVCards(list);   // uses version 3.0
    }

    std::cout << text.data();

    return 0;
}
