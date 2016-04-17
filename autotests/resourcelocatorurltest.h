/*
    This file is part of the KContacts framework.
    Copyright (c) 2015-2016 Laurent Montel <montel@kde.org>

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

#ifndef ResourceLocatorUrlTest_H
#define ResourceLocatorUrlTest_H

#include <QObject>

class ResourceLocatorUrlTest : public QObject
{
    Q_OBJECT
public:
    explicit ResourceLocatorUrlTest(QObject *parent = Q_NULLPTR);
    ~ResourceLocatorUrlTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldAssignExternal();
    void shouldSerialized();
    void shouldEqualResourceLocatorUrl();
    void shouldParseResourceLocatorUrl();
    void shouldGenerateVCard4();
    void shouldGenerateVCardWithParameter();
    void shouldGenerateVCard3();
};

#endif // ResourceLocatorUrlTest_H
