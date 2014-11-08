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

#include <QtCore/QFile>

#include <kaboutdata.h>
#include <klocalizedstring.h>

#include <QCoreApplication>
#include <KAboutData>
#include <QCommandLineParser>

#include "kcontacts/addressee.h"
#include "kcontacts/phonenumber.h"
#include "kcontacts/address.h"
#include "kcontacts/key.h"
#include "kcontacts/picture.h"
#include "kcontacts/sound.h"
#include "kcontacts/secrecy.h"
#include "kcontacts/vcardconverter.h"

int main(int argc, char **argv)
{
    KAboutData aboutData(QLatin1String("testwrite"), i18n("vCard test writer"), QLatin1String("0.1"));

    QCoreApplication app(argc, argv);
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KContacts::Addressee addressee;

    addressee.setNameFromString(QLatin1String("Mr. Tobias Koenig Jr."));
    addressee.setNickName(QLatin1String("tokoe"));
    addressee.setBirthday(QDateTime(QDate(1982, 7, 19)));
    addressee.setMailer(QLatin1String("mutt1.2"));
    addressee.setTimeZone(KContacts::TimeZone(+2));

    KContacts::Geo geo;
    geo.setLatitude(30);
    geo.setLongitude(51);
    addressee.setGeo(geo);

    addressee.setTitle(QLatin1String("nerd"));
    addressee.setRole(QLatin1String("Maintainer"));
    addressee.setOrganization(QLatin1String("KDE"));
    addressee.setNote(QLatin1String("nerver\ntouch a running system"));
    addressee.setProductId(QLatin1String("testId"));
    addressee.setRevision(QDateTime::currentDateTime());
    addressee.setSortString(QLatin1String("koenig"));
    addressee.setUrl(QUrl(QLatin1String("http://wgess16.dyndns.org")));
    addressee.setSecrecy(KContacts::Secrecy(KContacts::Secrecy::Confidential));

    addressee.insertEmail(QLatin1String("tokoe@kde.org"), true);
    addressee.insertEmail(QLatin1String("tokoe82@yahoo.de"), true);

    KContacts::PhoneNumber phone1(QLatin1String("3541523475"),
                             KContacts::PhoneNumber::Pref | KContacts::PhoneNumber::Home);
    KContacts::PhoneNumber phone2(QLatin1String("+46745673475"),
                             KContacts::PhoneNumber::Work);
    addressee.insertPhoneNumber(phone1);
    addressee.insertPhoneNumber(phone2);

    KContacts::Key key(QLatin1String("secret key"), KContacts::Key::X509);
    addressee.insertKey(key);

    QStringList categories;
    categories << QLatin1String("Friends") << QLatin1String("School") << QLatin1String("KDE");
    addressee.setCategories(categories);

    KContacts::Address a(KContacts::Address::Work | KContacts::Address::Postal | KContacts::Address::Parcel);
    a.setStreet(QLatin1String("6544 Battleford Drive"));
    a.setLocality(QLatin1String("Raleigh"));
    a.setRegion(QLatin1String("NC"));
    a.setPostalCode(QLatin1String("27613-3502"));
    a.setCountry(QLatin1String("U.S.A."));
    addressee.insertAddress(a);

    addressee.insertCustom(QLatin1String("1hsdf"), QLatin1String("ertuer"),
                           QLatin1String("iurt"));
    addressee.insertCustom(QLatin1String("2hsdf"), QLatin1String("ertuer"),
                           QLatin1String("iurt"));
    addressee.insertCustom(QLatin1String("3hsdf"), QLatin1String("ertuer"),
                           QLatin1String("iurt"));

    KContacts::Addressee::List list;
    for (int i = 0; i < 1000; ++i) {
        KContacts::Addressee addr = addressee;
        addr.setUid(QString::number(i));
        list.append(addr);
    }

    KContacts::VCardConverter converter;
    QByteArray txt = converter.createVCards(list);

    QFile file(QLatin1String("out.vcf"));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug("Can't open file '%s' fro writing", qPrintable(file.fileName()));
        return 1;
    }

    file.write(txt);
    file.close();

    return 0;
}
