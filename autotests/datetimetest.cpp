/*
    This file is part of kcontacts.
    Copyright (c) 2016 Benjamin Löwe <benni@mytum.de>

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

#include "datetimetest.h"
#include <QTest>
#include <vcardtool.h>


DateTimeTest::DateTimeTest(QObject *parent)
    : QObject(parent)
{
}

DateTimeTest::~DateTimeTest()
{
}

void DateTimeTest::shouldParseDateTime()
{
    using namespace KContacts;

    QDateTime expected, dt;
    bool timeIsValid;

    dt = VCardTool::parseDateTime(QStringLiteral("2016-01-20T12:33:30+0200"));
    expected = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 30), Qt::OffsetFromUTC, 2 * 3600);
    QCOMPARE(dt, expected);
    QCOMPARE(dt.isValid(), true);

    dt = VCardTool::parseDateTime(QStringLiteral("2016-01-20T12:33:30+02"));
    QCOMPARE(dt, expected);
    QCOMPARE(dt.isValid(), true);

    dt = VCardTool::parseDateTime(QStringLiteral("20160120T123330+0200"));
    QCOMPARE(dt, expected);
    QCOMPARE(dt.isValid(), true);

    dt = VCardTool::parseDateTime(QStringLiteral("2016-01-20T12:33Z"));
    expected = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 0), Qt::UTC);
    QCOMPARE(dt, expected);
    QCOMPARE(dt.isValid(), true);

    dt = VCardTool::parseDateTime(QStringLiteral("2016-01-20T12:33-0300"));
    expected = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 0), Qt::OffsetFromUTC, -3 * 3600);
    QCOMPARE(dt, expected);
    QCOMPARE(dt.isValid(), true);

    dt = VCardTool::parseDateTime(QStringLiteral("2016-01-20T12:33"), &timeIsValid);
    expected = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 0), Qt::LocalTime);
    QCOMPARE(dt, expected);
    QCOMPARE(timeIsValid, true);
    QCOMPARE(dt.isValid(), true);

    dt = VCardTool::parseDateTime(QStringLiteral("2016-01-20"), &timeIsValid);
    expected = QDateTime(QDate(2016, 1, 20), QTime(), Qt::LocalTime);
    QCOMPARE(dt, expected);
    QCOMPARE(dt.isValid(), true);
    QCOMPARE(timeIsValid, false);
    QCOMPARE(dt.time(), QTime(0, 0));

    dt = VCardTool::parseDateTime(QStringLiteral("T1233Z"), &timeIsValid);
    QCOMPARE(dt.isValid(), false);
    QCOMPARE(timeIsValid, true);
    QCOMPARE(dt.time(), QTime(12, 33));
}

void DateTimeTest::shouldCreateDateTime()
{
    using namespace KContacts;

    QDateTime dt;
    QString str, expected;

    dt = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 30), Qt::UTC);
    str = VCardTool::createDateTime(dt, VCard::v4_0);
    expected = QStringLiteral("20160120T123330Z");
    QCOMPARE(str, expected);

    dt = QDateTime(QDate(2016, 1, 20), QTime(), Qt::UTC);
    str = VCardTool::createDateTime(dt, VCard::v4_0);
    expected = QStringLiteral("20160120T000000Z");
    QCOMPARE(str, expected);

    dt = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 30), Qt::LocalTime);
    str = VCardTool::createDateTime(dt, VCard::v4_0);
    expected = QStringLiteral("20160120T123330");
    QCOMPARE(str, expected);

    dt = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 30), Qt::OffsetFromUTC, -2 * 3600);
    str = VCardTool::createDateTime(dt, VCard::v4_0);
    expected = QStringLiteral("20160120T123330-0200");
    QCOMPARE(str, expected);

    dt = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 30), Qt::OffsetFromUTC, +2.5 * 3600);
    str = VCardTool::createDateTime(dt, VCard::v4_0);
    expected = QStringLiteral("20160120T123330+0230");
    QCOMPARE(str, expected);

    dt = QDateTime(QDate(2016, 1, 20), QTime(12, 33, 30), Qt::OffsetFromUTC, -2 * 3600);
    str = VCardTool::createDateTime(dt, VCard::v3_0);
    expected = QStringLiteral("2016-01-20T12:33:30-02:00");
    QCOMPARE(str, expected);

    dt = QDateTime(QDate(), QTime(12, 33, 30), Qt::OffsetFromUTC, -2 * 3600);
    str = VCardTool::createDateTime(dt, VCard::v3_0);
    expected = QString();
    QCOMPARE(str, expected);
}

QTEST_MAIN(DateTimeTest)
