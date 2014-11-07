/*
    This file is part of the KContacts framework..
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

#include "soundtest.h"
#include "kcontacts/sound.h"
#include <qtest.h>

QTEST_MAIN(SoundTest)

static QByteArray testData()
{
    static QByteArray data;

    if (data.isNull()) {
        for (int i = 0; i < 20; ++i) {
            data.append(char(i));
        }
    }

    return data;
}

void SoundTest::emptyTest()
{
    KContacts::Sound sound;

    QVERIFY(sound.isEmpty());
}

void SoundTest::storeTestIntern()
{
    KContacts::Sound sound;

    sound.setData(testData());

    QVERIFY(sound.isIntern() == true);
    QVERIFY(sound.data() == testData());
}

void SoundTest::storeTestExtern()
{
    KContacts::Sound sound;

    sound.setUrl(QLatin1String("http://myhomepage.com/sound.wav"));

    QVERIFY(sound.isIntern() == false);
    QVERIFY(sound.url() == QLatin1String(QLatin1String("http://myhomepage.com/sound.wav")));
}

void SoundTest::equalsTestIntern()
{
    KContacts::Sound sound1, sound2;

    sound1.setData(testData());
    sound2.setData(testData());

    QVERIFY(sound1 == sound2);
}

void SoundTest::equalsTestExtern()
{
    KContacts::Sound sound1, sound2;

    sound1.setUrl(QLatin1String("http://myhomepage.com/sound.wav"));
    sound2.setUrl(QLatin1String("http://myhomepage.com/sound.wav"));

    QVERIFY(sound1 == sound2);
}

void SoundTest::differsTest()
{
    KContacts::Sound sound1, sound2;

    sound1.setUrl(QLatin1String("http://myhomepage.com/sound.wav"));
    sound2.setData(testData());

    QVERIFY(sound1 != sound2);
}

void SoundTest::assignmentTestIntern()
{
    KContacts::Sound sound1, sound2;

    sound1.setData(testData());

    sound2 = sound1;

    QVERIFY(sound1 == sound2);
}

void SoundTest::assignmentTestExtern()
{
    KContacts::Sound sound1, sound2;

    sound1.setUrl(QLatin1String("http://myhomepage.com/sound.wav"));

    sound2 = sound1;

    QVERIFY(sound1 == sound2);
}

void SoundTest::serializeTest()
{
    KContacts::Sound sound1, sound2;

    sound1.setUrl(QLatin1String("http://myhomepage.com/sound.wav"));
    sound1.setData(testData());

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << sound1;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> sound2;

    QVERIFY(sound1 == sound2);
}
