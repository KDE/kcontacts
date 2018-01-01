/*
    This file is part of the KContacts framework.
    Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#include "calendarurltest.h"

#include "calendarurl.h"
#include "vcardtool.h"
#include <qtest.h>

using namespace KContacts;
Q_DECLARE_METATYPE(KContacts::CalendarUrl::CalendarType)
CalendarUrlTest::CalendarUrlTest(QObject *parent)
    : QObject(parent)
{

}

CalendarUrlTest::~CalendarUrlTest()
{

}

void CalendarUrlTest::shouldHaveDefaultValue()
{
    CalendarUrl calendarUrl(CalendarUrl::FBUrl);
    QVERIFY(!calendarUrl.isValid());
    QVERIFY(calendarUrl.url().isEmpty());
    QVERIFY(calendarUrl.parameters().isEmpty());
}

void CalendarUrlTest::shouldAssignValue()
{
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    CalendarUrl calendarUrl(CalendarUrl::FBUrl);
    calendarUrl.setParameters(params);
    QVERIFY(!calendarUrl.isValid());
    QCOMPARE(calendarUrl.type(), CalendarUrl::FBUrl);
    QVERIFY(calendarUrl.url().isEmpty());
    QVERIFY(!calendarUrl.parameters().isEmpty());
    QCOMPARE(calendarUrl.parameters(), params);
}

void CalendarUrlTest::shouldAssignExternal()
{
    CalendarUrl calendarUrl(CalendarUrl::FBUrl);
    const CalendarUrl::CalendarType type = CalendarUrl::CALUri;
    calendarUrl.setType(type);
    QVERIFY(calendarUrl.url().isEmpty());
    QCOMPARE(calendarUrl.type(), type);
}

void CalendarUrlTest::shouldSerialized()
{
    for (int i = 0; i < CalendarUrl::EndCalendarType; ++i) {
        CalendarUrl calendarUrl;
        CalendarUrl result;
        CalendarUrl::CalendarType type = static_cast<CalendarUrl::CalendarType>(i);
        calendarUrl.setType(type);
        QMap<QString, QStringList> params;
        params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
        params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
        calendarUrl.setParameters(params);
        calendarUrl.setUrl(QUrl(QStringLiteral("mailto:foo@kde.org")));

        QByteArray data;
        QDataStream s(&data, QIODevice::WriteOnly);
        s << calendarUrl;

        QDataStream t(&data, QIODevice::ReadOnly);
        t >> result;

        QVERIFY(calendarUrl == result);
    }
}

void CalendarUrlTest::shouldEqualCalendarUrl()
{
    for (int i = 0; i < CalendarUrl::EndCalendarType; ++i) {
        CalendarUrl calendarUrl;
        CalendarUrl result;
        CalendarUrl::CalendarType type = static_cast<CalendarUrl::CalendarType>(i);
        calendarUrl.setType(type);
        QMap<QString, QStringList> params;
        params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
        params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
        calendarUrl.setUrl(QUrl(QStringLiteral("mailto:foo@kde.org")));
        calendarUrl.setParameters(params);

        result = calendarUrl;
        QVERIFY(calendarUrl == result);
    }
}

void CalendarUrlTest::shouldParseCalendarUrl()
{
    for (int i = CalendarUrl::Unknown + 1; i < CalendarUrl::EndCalendarType; ++i) {
        CalendarUrl::CalendarType type = static_cast<CalendarUrl::CalendarType>(i);
        QByteArray baType;
        switch (type) {
        case CalendarUrl::Unknown:
        case CalendarUrl::EndCalendarType:
            break;
        case CalendarUrl::FBUrl:
            baType = QByteArray("FBURL");
            break;
        case CalendarUrl::CALUri:
            baType = QByteArray("CALURI");
            break;
        case CalendarUrl::CALADRUri:
            baType = QByteArray("CALADRURI");
            break;

        }

        QByteArray vcarddata("BEGIN:VCARD\n"
                             "VERSION:3.0\n"
                             "N:LastName;FirstName;;;\n"
                             "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n");
        vcarddata += baType;
        vcarddata += QByteArray(
                         ";PREF=1:https://sherlockholmes.com/calendar/sherlockholmes\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

        KContacts::VCardTool vcard;
        const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
        QCOMPARE(lst.count(), 1);
        QCOMPARE(lst.at(0).calendarUrlList().count(), 1);
        const CalendarUrl calurl = lst.at(0).calendarUrlList().at(0);
        QCOMPARE(calurl.type(), type);
        QCOMPARE(calurl.url(), QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
        QVERIFY(!calurl.parameters().isEmpty());
    }
}

void CalendarUrlTest::shouldGenerateVCard_data()
{
    QTest::addColumn<KContacts::CalendarUrl::CalendarType>("type");
    QTest::addColumn<QByteArray>("value");
    for (int i = CalendarUrl::Unknown + 1; i < CalendarUrl::EndCalendarType; ++i) {
        KContacts::CalendarUrl::CalendarType type = static_cast<KContacts::CalendarUrl::CalendarType>(i);
        QByteArray baType;
        switch (type) {
        case CalendarUrl::Unknown:
        case CalendarUrl::EndCalendarType:
            break;
        case CalendarUrl::FBUrl:
            baType = QByteArray("FBURL");
            break;
        case CalendarUrl::CALUri:
            baType = QByteArray("CALURI");
            break;
        case CalendarUrl::CALADRUri:
            baType = QByteArray("CALADRURI");
            break;

        }
        QTest::newRow(baType.constData()) << type << baType;
    }
}

void CalendarUrlTest::shouldGenerateVCard()
{
    QFETCH(KContacts::CalendarUrl::CalendarType, type);
    QFETCH(QByteArray, value);

    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    CalendarUrl url;
    url.setType(type);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    addr.insertCalendarUrl(url);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected;
    // Different order
    if (type == KContacts::CalendarUrl::FBUrl) {
        expected = QByteArray("BEGIN:VCARD\r\n"
                              "VERSION:4.0\r\n"
                              "EMAIL:foo@kde.org\r\n");
        expected += value + QByteArray(":https://sherlockholmes.com/calendar/sherlockholmes\r\n"
                                       "N:;;;;\r\n"
                                       "UID:testuid\r\n"
                                       "END:VCARD\r\n\r\n");
    } else {
        expected = QByteArray("BEGIN:VCARD\r\n"
                              "VERSION:4.0\r\n");
        expected += value + QByteArray(":https://sherlockholmes.com/calendar/sherlockholmes\r\n"
                                       "EMAIL:foo@kde.org\r\n"
                                       "N:;;;;\r\n"
                                       "UID:testuid\r\n"
                                       "END:VCARD\r\n\r\n");
    }

    QCOMPARE(ba, expected);
}

void CalendarUrlTest::shouldGenerateVCardWithParameter()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    CalendarUrl url;
    url.setType(CalendarUrl::CALUri);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    url.setParameters(params);
    addr.insertCalendarUrl(url);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected = QByteArray("BEGIN:VCARD\r\n"
                                     "VERSION:4.0\r\n"
                                     "CALURI;FOO2=bla2,blo2:https://sherlockholmes.com/calendar/sherlockholmes\r\n"
                                     "EMAIL:foo@kde.org\r\n"
                                     "N:;;;;\r\n"
                                     "UID:testuid\r\n"
                                     "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

void CalendarUrlTest::shouldNotGeneratedAttributeForVcard3()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    CalendarUrl url;
    url.setType(CalendarUrl::CALUri);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    url.setParameters(params);
    addr.insertCalendarUrl(url);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v3_0);
    QByteArray expected = QByteArray("BEGIN:VCARD\r\n"
                                     "VERSION:3.0\r\n"
                                     "EMAIL:foo@kde.org\r\n"
                                     "N:;;;;\r\n"
                                     "UID:testuid\r\n"
                                     "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

void CalendarUrlTest::shouldGenerateMultiCalendarUrl()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    CalendarUrl url;
    url.setType(CalendarUrl::CALUri);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    url.setParameters(params);
    addr.insertCalendarUrl(url);

    url.setType(CalendarUrl::FBUrl);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes2")));
    params.clear();

    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    url.setParameters(params);
    addr.insertCalendarUrl(url);

    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected = QByteArray("BEGIN:VCARD\r\n"
                                     "VERSION:4.0\r\n"
                                     "CALURI;FOO2=bla2,blo2:https://sherlockholmes.com/calendar/sherlockholmes\r\n"
                                     "EMAIL:foo@kde.org\r\n"
                                     "FBURL;FOO1=bla1,blo1:https://sherlockholmes.com/calendar/sherlockholmes2\r\n"
                                     "N:;;;;\r\n"
                                     "UID:testuid\r\n"
                                     "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);

}

QTEST_MAIN(CalendarUrlTest)

