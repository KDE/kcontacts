/*
    This file is part of the KContacts framework.
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

#include "vcardlinetest.h"
#include "vcardline.h"
#include <QTest>

VCardLineTest::VCardLineTest(QObject *parent)
    : QObject(parent)
{

}

VCardLineTest::~VCardLineTest()
{

}

void VCardLineTest::shouldHaveDefaultValue()
{
    KContacts::VCardLine line;
    QVERIFY(line.identifier().isEmpty());
    QVERIFY(line.group().isEmpty());
    QVERIFY(line.parameterList().isEmpty());
    QVERIFY(line.value().isNull());
}

void VCardLineTest::shouldAssignValues()
{
    KContacts::VCardLine line;
    const QString identifier(QStringLiteral("foo"));
    const QString group(QStringLiteral("bla"));
    line.setIdentifier(identifier);
    line.setGroup(group);
    QMap<QString, QString> map;
    for(int i = 0; i < 5; ++i) {
        map.insert(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
        line.addParameter(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
    }
    const QVariant valueVariant = QVariant(QStringLiteral("a"));
    line.setValue(valueVariant);

    QVERIFY(!line.parameterList().isEmpty());
    QCOMPARE(line.parameterList(), QStringList() << map.keys());

    QCOMPARE(line.identifier(), identifier);

    QCOMPARE(line.group(), group);
    QCOMPARE(line.value(), valueVariant);
}

void VCardLineTest::shouldCopyValue()
{
    KContacts::VCardLine line;
    const QString identifier(QStringLiteral("foo"));
    const QString group(QStringLiteral("bla"));
    line.setIdentifier(identifier);
    line.setGroup(group);
    QMap<QString, QString> map;
    for(int i = 0; i < 5; ++i) {
        map.insert(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
        line.addParameter(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
    }
    const QVariant valueVariant = QVariant(QStringLiteral("a"));
    line.setValue(valueVariant);

    QVERIFY(!line.parameterList().isEmpty());
    QCOMPARE(line.parameterList(), QStringList() << map.keys());

    QCOMPARE(line.identifier(), identifier);

    QCOMPARE(line.group(), group);
    QCOMPARE(line.value(), valueVariant);


    KContacts::VCardLine copyLine(line);
    QVERIFY(!copyLine.parameterList().isEmpty());
    QCOMPARE(copyLine.parameterList(), QStringList() << map.keys());

    QCOMPARE(copyLine.identifier(), identifier);

    QCOMPARE(copyLine.group(), group);
    QCOMPARE(copyLine.value(), valueVariant);
}

void VCardLineTest::shouldEqualValue()
{
    KContacts::VCardLine line;
    const QString identifier(QStringLiteral("foo"));
    const QString group(QStringLiteral("bla"));
    line.setIdentifier(identifier);
    line.setGroup(group);
    QMap<QString, QString> map;
    for(int i = 0; i < 5; ++i) {
        map.insert(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
        line.addParameter(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
    }
    const QVariant valueVariant = QVariant(QStringLiteral("a"));
    line.setValue(valueVariant);

    QVERIFY(!line.parameterList().isEmpty());
    QCOMPARE(line.parameterList(), QStringList() << map.keys());

    QCOMPARE(line.identifier(), identifier);

    QCOMPARE(line.group(), group);
    QCOMPARE(line.value(), valueVariant);


    KContacts::VCardLine copyLine(line);
    QVERIFY(line == copyLine);
}

QTEST_MAIN(VCardLineTest)
