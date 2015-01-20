/*
    This file is part of libkabc.
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

#include "langtest.h"
#include "lang.h"
#include <qtest.h>

LangTest::LangTest(QObject *parent)
    : QObject(parent)
{

}

LangTest::~LangTest()
{

}

void LangTest::shouldHaveDefaultValue()
{
    KContacts::Lang language;
    QVERIFY(!language.isValid());
    QVERIFY(language.language().isEmpty());
    QVERIFY(language.parameters().isEmpty());
}

void LangTest::shouldAssignValue()
{
    const QString lang(QLatin1String("fr"));
    QMap<QString, QStringList> params;
    params.insert(QLatin1String("Foo1"), QStringList()<< QLatin1String("bla1") <<QLatin1String("blo1"));
    params.insert(QLatin1String("Foo2"), QStringList()<< QLatin1String("bla2") <<QLatin1String("blo2"));
    KContacts::Lang language(lang);
    language.setParameters(params);
    QVERIFY(language.isValid());
    QVERIFY(!language.language().isEmpty());
    QCOMPARE(language.language(), lang);
    QVERIFY(!language.parameters().isEmpty());
    QCOMPARE(language.parameters(), params);

}

void LangTest::shouldAssignExternal()
{
    KContacts::Lang language;
    const QString lang(QLatin1String("fr"));
    language.setLanguage(lang);
    QVERIFY(language.isValid());
    QCOMPARE(language.language(), lang);
}

void LangTest::shouldSerialized()
{
    KContacts::Lang language;
    KContacts::Lang result;
    const QString lang(QLatin1String("fr"));
    language.setLanguage(lang);
    QMap<QString, QStringList> params;
    params.insert(QLatin1String("Foo1"), QStringList()<< QLatin1String("bla1") <<QLatin1String("blo1"));
    params.insert(QLatin1String("Foo2"), QStringList()<< QLatin1String("bla2") <<QLatin1String("blo2"));
    language.setParameters(params);


    QByteArray data;
    QDataStream s( &data, QIODevice::WriteOnly );
    s << language;

    QDataStream t( &data, QIODevice::ReadOnly );
    t >> result;

    QVERIFY( language == result );

}

void LangTest::shouldEqualLanguage()
{
    KContacts::Lang language;
    KContacts::Lang result;
    const QString lang(QLatin1String("fr"));
    language.setLanguage(lang);
    QMap<QString, QStringList> params;
    params.insert(QLatin1String("Foo1"), QStringList()<< QLatin1String("bla1") <<QLatin1String("blo1"));
    params.insert(QLatin1String("Foo2"), QStringList()<< QLatin1String("bla2") <<QLatin1String("blo2"));
    language.setParameters(params);

    result = language;
    QVERIFY( language == result );
}

QTEST_MAIN(LangTest)
