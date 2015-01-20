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
#include "emailtest.h"
#include "email.h"
#include <qtest.h>

EmailTest::EmailTest(QObject *parent)
    : QObject(parent)
{

}

EmailTest::~EmailTest()
{

}

void EmailTest::shouldHaveDefaultValue()
{
    KContacts::Email email;
    QVERIFY(!email.isValid());
    QVERIFY(email.mail().isEmpty());
    QVERIFY(email.parameters().isEmpty());
}

void EmailTest::shouldAssignValue()
{
    const QString mail(QLatin1String("foo@kde.org"));
    QMap<QString, QStringList> params;
    params.insert(QLatin1String("Foo1"), QStringList() << QLatin1String("bla1") << QLatin1String("blo1"));
    params.insert(QLatin1String("Foo2"), QStringList() << QLatin1String("bla2") << QLatin1String("blo2"));
    KContacts::Email email(mail);
    email.setParameters(params);
    QVERIFY(email.isValid());
    QVERIFY(!email.mail().isEmpty());
    QCOMPARE(email.mail(), mail);
    QVERIFY(!email.parameters().isEmpty());
    QCOMPARE(email.parameters(), params);
}

void EmailTest::shouldAssignExternal()
{
    KContacts::Email email;
    const QString mail(QLatin1String("foo@kde.org"));
    email.setEmail(mail);
    QVERIFY(email.isValid());
    QVERIFY(!email.mail().isEmpty());
    QCOMPARE(email.mail(), mail);
}

void EmailTest::shouldSerialized()
{
    KContacts::Email email;
    KContacts::Email result;
    const QString mail(QLatin1String("foo@kde.org"));
    email.setEmail(mail);
    QMap<QString, QStringList> params;
    params.insert(QLatin1String("Foo1"), QStringList() << QLatin1String("bla1") << QLatin1String("blo1"));
    params.insert(QLatin1String("Foo2"), QStringList() << QLatin1String("bla2") << QLatin1String("blo2"));
    email.setParameters(params);

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << email;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(email == result);
}

void EmailTest::shouldEqualEmail()
{
    KContacts::Email email;
    KContacts::Email result;
    const QString mail(QLatin1String("foo@kde.org"));
    email.setEmail(mail);
    QMap<QString, QStringList> params;
    params.insert(QLatin1String("Foo1"), QStringList() << QLatin1String("bla1") << QLatin1String("blo1"));
    params.insert(QLatin1String("Foo2"), QStringList() << QLatin1String("bla2") << QLatin1String("blo2"));
    email.setParameters(params);

    result = email;
    QVERIFY(email == result);
}

QTEST_MAIN(EmailTest)
