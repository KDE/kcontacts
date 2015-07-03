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

#include "calendarurltest.h"

#include "emailtest.h"
#include "calendarurl.h"
#include <qtest.h>

using namespace KContacts;

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
    CalendarUrl calendarUrl(CalendarUrl::FBUrl);
    CalendarUrl result;
    CalendarUrl::CalendarType type = CalendarUrl::CALUri;
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

void CalendarUrlTest::shouldEqualCalendarUrl()
{
    CalendarUrl calendarUrl(CalendarUrl::FBUrl);
    CalendarUrl result;
    CalendarUrl::CalendarType type = CalendarUrl::CALUri;
    calendarUrl.setType(type);
    QMap<QString, QStringList> params;
    params.insert(QStringLiteral("Foo1"), QStringList() << QStringLiteral("bla1") << QStringLiteral("blo1"));
    params.insert(QStringLiteral("Foo2"), QStringList() << QStringLiteral("bla2") << QStringLiteral("blo2"));
    calendarUrl.setUrl(QUrl(QStringLiteral("mailto:foo@kde.org")));
    calendarUrl.setParameters(params);

    result = calendarUrl;
    QVERIFY(calendarUrl == result);
}

QTEST_MAIN(CalendarUrlTest)

