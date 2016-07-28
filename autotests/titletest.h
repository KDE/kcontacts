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

#ifndef TITLETEST_H
#define TITLETEST_H

#include <QObject>

class TitleTest : public QObject
{
    Q_OBJECT
public:
    explicit TitleTest(QObject *parent = Q_NULLPTR);
    ~TitleTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldAssignExternal();
    void shouldSerialized();
    void shouldEqualTitle();
    void shouldParseTitle();
    void shouldParseWithoutTitle();
    void shouldCreateVCard();
    void shouldCreateVCardWithTwoTitle();
    void shouldCreateVCardWithParameters();
    void shouldGenerateTitleForVCard3();
};

#endif // TITLETEST_H
