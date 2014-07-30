/*
    This file is part of libkabc.
    Copyright (c) 2007 Tobias Koenig <tokoe@kde.org>

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

#include "picturetest.h"
#include "kabc/picture.h"
#include <QPainter>
#include <qtest.h>
#include <QBuffer>

QTEST_MAIN(PictureTest)

static QImage testImage()
{
    static QImage image;

    if (image.isNull()) {
        image = QImage(200, 200, QImage::Format_RGB32);
        QPainter p(&image);
        p.drawRect(10, 10, 50, 50);
    }

    return image;
}

static QByteArray testImageRawPNG()
{
    static QByteArray raw;

    if (raw.isNull()) {
        QBuffer buffer(&raw);
        buffer.open(QIODevice::WriteOnly);
        testImage().save(&buffer, "PNG");
    }

    return raw;
}

static QByteArray testImageRawJPEG()
{
    static QByteArray raw;

    if (raw.isNull()) {
        QBuffer buffer(&raw);
        buffer.open(QIODevice::WriteOnly);
        testImage().save(&buffer, "JPEG");
    }

    return raw;
}

void PictureTest::emptyTest()
{
    KABC::Picture picture;

    QVERIFY(picture.isEmpty());
}

void PictureTest::storeTestInternImage()
{
    KABC::Picture picture;

    picture.setData(testImage());

    QVERIFY(picture.isEmpty() == false);
    QVERIFY(picture.isIntern() == true);
    QVERIFY(picture.type() == QLatin1String("jpeg"));
    QVERIFY(picture.data() == testImage());
    QVERIFY(picture.rawData() == testImageRawJPEG());
}

void PictureTest::storeTestInternRawData()
{
    KABC::Picture picture;

    picture.setRawData(testImageRawPNG(), QLatin1String("png"));

    QVERIFY(picture.isEmpty() == false);
    QVERIFY(picture.isIntern() == true);
    QVERIFY(picture.type() == QLatin1String("png"));
    QVERIFY(picture.rawData() == testImageRawPNG());
    QVERIFY(picture.data() == testImage());
}

void PictureTest::storeTestExtern()
{
    KABC::Picture picture;

    picture.setUrl(QLatin1String("http://myhomepage.com/foto.png"), QLatin1String("png"));

    QVERIFY(picture.isEmpty() == false);
    QVERIFY(picture.isIntern() == false);
    QVERIFY(picture.type() == QLatin1String("png"));
    QVERIFY(picture.url() == QLatin1String("http://myhomepage.com/foto.png"));
}

void PictureTest::equalsTestInternImage()
{
    KABC::Picture picture1, picture2;

    picture1.setData(testImage());

    picture2.setData(testImage());

    QVERIFY(picture1 == picture2);

    // access rawData() so a QByteArray is created
    picture1.rawData();
    QVERIFY(picture1 == picture2);
}

void PictureTest::equalsTestInternRawData()
{
    KABC::Picture picture1, picture2;

    picture1.setRawData(testImageRawPNG(), QLatin1String("png"));

    picture2.setRawData(testImageRawPNG(), QLatin1String("png"));

    QVERIFY(picture1 == picture2);

    // access data() so a QImage is created
    picture1.data();
    QVERIFY(picture1 == picture2);
}

void PictureTest::equalsTestInternImageAndRawData()
{
    KABC::Picture picture1, picture2;

    picture1.setData(testImage());

    picture2.setRawData(testImageRawJPEG(), QLatin1String("jpeg"));

    QVERIFY(picture1.rawData() == picture2.rawData());
}

void PictureTest::equalsTestExtern()
{
    KABC::Picture picture1, picture2;

    picture1.setUrl(QLatin1String("http://myhomepage.com/foto.png"), QLatin1String("png"));

    picture2.setUrl(QLatin1String("http://myhomepage.com/foto.png"), QLatin1String("png"));

    QVERIFY(picture1 == picture2);
}

void PictureTest::differsTest()
{
    KABC::Picture picture1, picture2;

    picture1.setUrl(QLatin1String("http://myhomepage.com/foto.png"), QLatin1String("png"));

    picture2.setData(testImage());

    QVERIFY(picture1 != picture2);
}

void PictureTest::differsTestInternRawData()
{
    KABC::Picture picture1, picture2;

    picture1.setRawData(testImageRawJPEG(), QLatin1String("jpeg"));

    picture2.setRawData(testImageRawPNG(), QLatin1String("png"));

    QVERIFY(picture1 != picture2);
    QVERIFY(picture1.rawData() != picture2.rawData());
}

void PictureTest::differsTestExtern()
{
    KABC::Picture picture1, picture2;

    picture1.setUrl(QLatin1String("http://myhomepage.com/foto.png"), QLatin1String("png"));

    picture1.setUrl(QLatin1String("http://myhomepage.com/foto2.png"), QLatin1String("png"));

    QVERIFY(picture1 != picture2);
}

void PictureTest::assignmentTestIntern()
{
    KABC::Picture picture1, picture2;

    picture1.setData(testImage());

    picture2 = picture1;

    QVERIFY(picture1 == picture2);
}

void PictureTest::assignmentTestExtern()
{
    KABC::Picture picture1, picture2;

    picture1.setUrl(QLatin1String("http://myhomepage.com/foto.png"), QLatin1String("png"));

    picture2 = picture1;

    QVERIFY(picture1 == picture2);
}

void PictureTest::serializeTestInternImage()
{
    KABC::Picture picture1, picture2;

    picture1.setData(testImage());

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << picture1;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> picture2;

    QVERIFY(picture1 == picture2);
}

void PictureTest::serializeTestInternRawData()
{
    KABC::Picture picture1, picture2;

    picture1.setRawData(testImageRawPNG(), QLatin1String("png"));

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << picture1;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> picture2;

    QVERIFY(picture1 == picture2);
}

void PictureTest::serializeTestExtern()
{
    KABC::Picture picture1, picture2;

    picture1.setUrl(QLatin1String("http://myhomepage.com/foto.png"), QLatin1String("png"));

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << picture1;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> picture2;

    QVERIFY(picture1 == picture2);
}

