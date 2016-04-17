/*
    This file is part of kcontacts.
    Copyright (c) 2016 Laurent Montel <montel@kde.org>

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

#include "birthdaytest.h"
#include <QTest>
#include <vcardtool.h>
#include "addressee.h"
#include "vcard.h"

BirthDayTest::BirthDayTest(QObject *parent)
    : QObject(parent)
{

}

BirthDayTest::~BirthDayTest()
{

}

void BirthDayTest::shouldExportVcard4()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    const QDateTime dt(QDate(1976, 5, 5), QTime(12, 5, 5));
    addr.setBirthday(dt);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:4.0\r\n"
                        "BDAY:19760505T120505\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "EMAIL:bla@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

void BirthDayTest::shouldExportVcard3()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org") << QStringLiteral("bla@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    const QDateTime dt(QDate(1976, 5, 5), QTime(12, 5, 5));
    addr.setBirthday(dt);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v3_0);
    QByteArray expected("BEGIN:VCARD\r\n"
                        "VERSION:3.0\r\n"
                        "BDAY:1976-05-05T12:05:05\r\n"
                        "EMAIL:foo@kde.org\r\n"
                        "EMAIL:bla@kde.org\r\n"
                        "N:;;;;\r\n"
                        "UID:testuid\r\n"
                        "END:VCARD\r\n\r\n");

    QCOMPARE(ba, expected);
}

QTEST_MAIN(BirthDayTest)
