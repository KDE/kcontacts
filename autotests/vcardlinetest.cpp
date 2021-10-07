/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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

static void compareBuffers(const KContacts::VCardLine &line,
                           const QString &identifier,
                           const QString &group,
                           const QMap<QString, QString> &parametersMap,
                           const QVariant &valueVariant)
{
    QStringList parameterList = line.parameterList();
    QVERIFY(!parameterList.isEmpty());
    std::sort(parameterList.begin(), parameterList.end());
    QCOMPARE(parameterList, parametersMap.keys());

    QCOMPARE(line.identifier(), identifier);

    QCOMPARE(line.group(), group);
    QCOMPARE(line.value(), valueVariant);
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
    for (int i = 0; i < 5; ++i) {
        map.insert(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
        line.addParameter(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
    }
    const QVariant valueVariant = QVariant(QStringLiteral("a"));
    line.setValue(valueVariant);

    compareBuffers(line, identifier, group, map, valueVariant);
}

void VCardLineTest::shouldCopyValue()
{
    KContacts::VCardLine line;
    const QString identifier(QStringLiteral("foo"));
    const QString group(QStringLiteral("bla"));
    line.setIdentifier(identifier);
    line.setGroup(group);
    QMap<QString, QString> map;
    for (int i = 0; i < 5; ++i) {
        map.insert(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
        line.addParameter(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
    }
    const QVariant valueVariant = QVariant(QStringLiteral("a"));
    line.setValue(valueVariant);

    compareBuffers(line, identifier, group, map, valueVariant);

    KContacts::VCardLine copyLine(line);
    compareBuffers(copyLine, identifier, group, map, valueVariant);
}

void VCardLineTest::shouldEqualValue()
{
    KContacts::VCardLine line;
    const QString identifier(QStringLiteral("foo"));
    const QString group(QStringLiteral("bla"));
    line.setIdentifier(identifier);
    line.setGroup(group);
    QMap<QString, QString> map;
    for (int i = 0; i < 5; ++i) {
        map.insert(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
        line.addParameter(QStringLiteral("foo%1").arg(i), QStringLiteral("bla%1").arg(i));
    }
    const QVariant valueVariant = QVariant(QStringLiteral("a"));
    line.setValue(valueVariant);

    compareBuffers(line, identifier, group, map, valueVariant);

    KContacts::VCardLine copyLine(line);
    QCOMPARE(line, copyLine);
}

QTEST_MAIN(VCardLineTest)
