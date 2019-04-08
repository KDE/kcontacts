/*
    This file is part of the KContacts framework.
    Copyright (c) 2016 David Faure <faure@kde.org>

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

#include "vcardtool_benchmark.h"
#include "title.h"
#include "vcardtool_p.h"
#include <QTest>

PerformanceTest::PerformanceTest(QObject *parent)
    : QObject(parent)
{
}

PerformanceTest::~PerformanceTest()
{
}

void PerformanceTest::testParserPerformance()
{
    QByteArray vcarddata("BEGIN:VCARD\n"
                         "VERSION:3.0\n"
                         "N:LastName;FirstName;;;\n"
                         "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
                         "Title:boo\n"
                         "REV:2015-03-14T09:24:45+00:00\n"
                         "FN:FirstName LastName\n"
                         "END:VCARD\n");

    QBENCHMARK {
        KContacts::VCardTool vcard;
        const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
        QCOMPARE(lst.count(), 1);
    }
}

void PerformanceTest::testExportPerformance()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Title::List lstTitle;
    KContacts::Title title(QStringLiteral("fr"));
    lstTitle << title;
    addr.setExtraTitleList(lstTitle);
    lst << addr;
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "N:;;;;\r\n"
                        "TITLE:fr\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");
    QBENCHMARK {
        KContacts::VCardTool vcard;
        const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
        QCOMPARE(ba, expected);
    }
}

QTEST_GUILESS_MAIN(PerformanceTest)
