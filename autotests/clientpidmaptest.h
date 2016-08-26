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

#ifndef ROLETEST_H
#define ROLETEST_H

#include <QObject>

class ClientPidMapTest : public QObject
{
    Q_OBJECT
public:
    explicit ClientPidMapTest(QObject *parent = Q_NULLPTR);
    ~ClientPidMapTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldAssignExternal();
    void shouldSerialized();
    void shouldEqualClientPidMap();
#if 0
    void shouldParseClientPidMap();
    void shouldParseWithoutClientPidMap();
    void shouldCreateVCard();
    void shouldCreateVCardWithTwoClientPidMap();
    void shouldCreateVCardWithParameters();
    void shouldGenerateClientPidMapForVCard3();
#endif
};

#endif // ROLETEST_H
