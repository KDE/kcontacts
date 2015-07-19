/*
    This file is part of the KContacts framework.
    Copyright (c) 2015 Laurent Montel <montel@kde.org>

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

#include "resourcelocatorurltest.h"

#include "resourcelocatorurl.h"
#include "vcardtool.h"
#include <qtest.h>

using namespace KContacts;
ResourceLocatorUrlTest::ResourceLocatorUrlTest(QObject *parent)
    : QObject(parent)
{

}

ResourceLocatorUrlTest::~ResourceLocatorUrlTest()
{

}

void ResourceLocatorUrlTest::shouldHaveDefaultValue()
{
    ResourceLocatorUrl ResourceLocatorUrl;
    QVERIFY(!ResourceLocatorUrl.isValid());
    QVERIFY(ResourceLocatorUrl.url().isEmpty());
    QVERIFY(ResourceLocatorUrl.parameters().isEmpty());
}

void ResourceLocatorUrlTest::shouldAssignValue()
{
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    ResourceLocatorUrl ResourceLocatorUrl;
    ResourceLocatorUrl.setParameters(params);
    QVERIFY(!ResourceLocatorUrl.isValid());
    QVERIFY(ResourceLocatorUrl.url().isEmpty());
    QVERIFY(!ResourceLocatorUrl.parameters().isEmpty());
    QCOMPARE(ResourceLocatorUrl.parameters(), params);
}

void ResourceLocatorUrlTest::shouldAssignExternal()
{
    //TODO
    ResourceLocatorUrl ResourceLocatorUrl;
    QVERIFY(ResourceLocatorUrl.url().isEmpty());
}

void ResourceLocatorUrlTest::shouldSerialized()
{
    ResourceLocatorUrl resourcelocatorurl;
    ResourceLocatorUrl result;
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    resourcelocatorurl.setParameters(params);
    resourcelocatorurl.setUrl(QUrl(QStringLiteral("mailto:foo@kde.org")));

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << resourcelocatorurl;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(resourcelocatorurl == result);
}

void ResourceLocatorUrlTest::shouldEqualResourceLocatorUrl()
{
    ResourceLocatorUrl resourcelocatorurl;
    ResourceLocatorUrl result;
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    resourcelocatorurl.setUrl(QUrl(QStringLiteral("mailto:foo@kde.org")));
    resourcelocatorurl.setParameters(params);

    result = resourcelocatorurl;
    QVERIFY(resourcelocatorurl == result);
}

void ResourceLocatorUrlTest::shouldParseResourceLocatorUrl()
{
#if 0
    for (int i = ResourceLocatorUrl::Unknown + 1; i < ResourceLocatorUrl::EndCalendarType; ++i) {
        ResourceLocatorUrl::CalendarType type = static_cast<ResourceLocatorUrl::CalendarType>(i);
        QByteArray baType;
        switch (type) {
        case ResourceLocatorUrl::Unknown:
        case ResourceLocatorUrl::EndCalendarType:
            break;
        case ResourceLocatorUrl::FBUrl:
            baType = QByteArray("FBURL");
            break;
        case ResourceLocatorUrl::CALUri:
            baType = QByteArray("CALURI");
            break;
        case ResourceLocatorUrl::CALADRUri:
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
        QCOMPARE(lst.at(0).ResourceLocatorUrlList().count(), 1);
        const ResourceLocatorUrl calurl = lst.at(0).ResourceLocatorUrlList().at(0);
        QCOMPARE(calurl.type(), type);
        QCOMPARE(calurl.url(), QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
        QVERIFY(!calurl.parameters().isEmpty());
    }
#endif
}

void ResourceLocatorUrlTest::shouldGenerateVCard_data()
{
#if 0
    QTest::addColumn<KContacts::ResourceLocatorUrl::CalendarType>("type");
    QTest::addColumn<QByteArray>("value");
    for (int i = ResourceLocatorUrl::Unknown + 1; i < ResourceLocatorUrl::EndCalendarType; ++i) {
        KContacts::ResourceLocatorUrl::CalendarType type = static_cast<KContacts::ResourceLocatorUrl::CalendarType>(i);
        QByteArray baType;
        switch (type) {
        case ResourceLocatorUrl::Unknown:
        case ResourceLocatorUrl::EndCalendarType:
            break;
        case ResourceLocatorUrl::FBUrl:
            baType = QByteArray("FBURL");
            break;
        case ResourceLocatorUrl::CALUri:
            baType = QByteArray("CALURI");
            break;
        case ResourceLocatorUrl::CALADRUri:
            baType = QByteArray("CALADRURI");
            break;

        }
        QTest::newRow(baType.constData()) << type << baType;
    }
#endif
}

void ResourceLocatorUrlTest::shouldGenerateVCard()
{
#if 0
    QFETCH(KContacts::ResourceLocatorUrl::CalendarType, type);
    QFETCH(QByteArray, value);

    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    ResourceLocatorUrl url;
    url.setType(type);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    addr.insertResourceLocatorUrl(url);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected;
    // Different order
    if (type == KContacts::ResourceLocatorUrl::FBUrl) {
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
#endif
}

void ResourceLocatorUrlTest::shouldGenerateVCardWithParameter()
{
#if 0
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    ResourceLocatorUrl url;
    url.setType(ResourceLocatorUrl::CALUri);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    url.setParameters(params);
    addr.insertResourceLocatorUrl(url);
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
#endif
}

void ResourceLocatorUrlTest::shouldNotGeneratedAttributeForVcard3()
{
#if 0
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    ResourceLocatorUrl url;
    url.setType(ResourceLocatorUrl::CALUri);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    url.setParameters(params);
    addr.insertResourceLocatorUrl(url);
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
#endif
}

void ResourceLocatorUrlTest::shouldGenerateMultiResourceLocatorUrl()
{
#if 0
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    ResourceLocatorUrl url;
    url.setType(ResourceLocatorUrl::CALUri);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes")));
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    url.setParameters(params);
    addr.insertResourceLocatorUrl(url);

    url.setType(ResourceLocatorUrl::FBUrl);
    url.setUrl(QUrl(QStringLiteral("https://sherlockholmes.com/calendar/sherlockholmes2")));
    params.clear();

    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    url.setParameters(params);
    addr.insertResourceLocatorUrl(url);

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
#endif
}

QTEST_MAIN(ResourceLocatorUrlTest)

