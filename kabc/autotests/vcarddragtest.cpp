/*
  This file is part of libkabc.
  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.net
  Author: Kevin Krammer, krake@kdab.com

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

#include <qtest.h>

#include "kabc/vcardconverter.h"
#include "kabc/vcarddrag.h"

#include <QMimeData>
using namespace KABC;

class VCardDragTest : public QObject
{
    Q_OBJECT

public:
    VCardDragTest();

private:
    Addressee mAddressee1;
    Addressee mAddressee2;

private Q_SLOTS:
    void testPopulate();
    void testCanDecode();
    void testFromMimeData();
};

QTEST_MAIN(VCardDragTest)

VCardDragTest::VCardDragTest() : QObject()
{
    mAddressee1.setGivenName(QLatin1String("Kevin"));
    mAddressee1.setFamilyName(QLatin1String("Krammer"));
    mAddressee1.insertEmail(QLatin1String("kevin.krammer@gmx.at"));

    mAddressee2.setGivenName(QLatin1String("Tobias"));
    mAddressee2.setFamilyName(QLatin1String("König"));
    mAddressee2.insertEmail(QLatin1String("tokoe@kde.org"));
}

void VCardDragTest::testPopulate()
{
    VCardConverter converter;
    const QByteArray vcard = converter.createVCard(mAddressee1);
    QVERIFY(!vcard.isEmpty());

    QMimeData *data = new QMimeData();
    bool result = VCardDrag::populateMimeData(data, vcard);
    QVERIFY(result);
    QVERIFY(data->hasFormat(KABC::Addressee::mimeType()));
    QCOMPARE(data->data(KABC::Addressee::mimeType()), vcard);
    delete data;

    data = new QMimeData();
    result = VCardDrag::populateMimeData(data, Addressee::List() << mAddressee1);
    QVERIFY(result);
    QVERIFY(data->hasFormat(KABC::Addressee::mimeType()));
    QCOMPARE(data->data(KABC::Addressee::mimeType()), vcard);
    delete data;

    const QByteArray vcards = converter.createVCards(Addressee::List() << mAddressee1 << mAddressee2);
    data = new QMimeData();
    result = VCardDrag::populateMimeData(data, Addressee::List() << mAddressee1 << mAddressee2);
    QVERIFY(result);
    QVERIFY(data->hasFormat(KABC::Addressee::mimeType()));
    QCOMPARE(data->data(KABC::Addressee::mimeType()), vcards);
    delete data;

    data = new QMimeData();
    result = VCardDrag::populateMimeData(data, Addressee::List());
    QVERIFY(!result);
    QVERIFY(!data->hasFormat(KABC::Addressee::mimeType()));
    delete data;
}

void VCardDragTest::testCanDecode()
{
    VCardConverter converter;
    const QByteArray vcard = converter.createVCard(mAddressee1);
    QVERIFY(!vcard.isEmpty());

    QMimeData *data = new QMimeData();
    data->setData(Addressee::mimeType(), vcard);
    QVERIFY(VCardDrag::canDecode(data));
    delete data;

    data = new QMimeData();
    data->setData(QLatin1String("text/x-vcard"), vcard);
    QVERIFY(VCardDrag::canDecode(data));
    delete data;

    data = new QMimeData();
    data->setData(QLatin1String("application/octetstream"), vcard);
    QVERIFY(!VCardDrag::canDecode(data));
    delete data;
}

void VCardDragTest::testFromMimeData()
{
    VCardConverter converter;
    const QByteArray vcard = converter.createVCard(mAddressee1);
    QVERIFY(!vcard.isEmpty());

    QByteArray content;

    QMimeData *data = new QMimeData();
    data->setData(Addressee::mimeType(), vcard);
    bool result = VCardDrag::fromMimeData(data, content);
    QVERIFY(result);
    QCOMPARE(content, vcard);
    delete data;

    data = new QMimeData();
    data->setData(QLatin1String("text/x-vcard"), vcard);
    result = VCardDrag::fromMimeData(data, content);
    QVERIFY(result);
    QCOMPARE(content, vcard);
    delete data;

    data = new QMimeData();
    data->setData(QLatin1String("application/octetstream"), vcard);
    result = VCardDrag::fromMimeData(data, content);
    QVERIFY(!result);
    delete data;

    const QByteArray vcards = converter.createVCards(Addressee::List() << mAddressee1 << mAddressee2);
    QVERIFY(!vcards.isEmpty());

    Addressee::List addresseeList;

    data = new QMimeData();
    data->setData(Addressee::mimeType(), vcards);
    result = VCardDrag::fromMimeData(data, addresseeList);
    QVERIFY(result);
    QCOMPARE(addresseeList.count(), 2);
    QCOMPARE(addresseeList[ 0 ], mAddressee1);
    QCOMPARE(addresseeList[ 1 ], mAddressee2);
    delete data;

    data = new QMimeData();
    data->setData(QLatin1String("text/x-vcard"), vcards);
    result = VCardDrag::fromMimeData(data, addresseeList);
    QVERIFY(result);
    QCOMPARE(addresseeList.count(), 2);
    QCOMPARE(addresseeList[ 0 ], mAddressee1);
    QCOMPARE(addresseeList[ 1 ], mAddressee2);
    delete data;

    data = new QMimeData();
    data->setData(QLatin1String("application/octetstream"), vcards);
    result = VCardDrag::fromMimeData(data, addresseeList);
    QVERIFY(!result);
    delete data;

    data = new QMimeData();
    data->setData(Addressee::mimeType(), QByteArray());
    result = VCardDrag::fromMimeData(data, addresseeList);
    QVERIFY(!result);
    delete data;
}

#include "vcarddragtest.moc"

