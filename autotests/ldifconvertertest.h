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

#ifndef LDIFCONVERTERTEST_H
#define LDIFCONVERTERTEST_H

#include <QObject>

class LDifConverterTest : public QObject
{
    Q_OBJECT
public:
    explicit LDifConverterTest(QObject *parent = Q_NULLPTR);
    ~LDifConverterTest();

private Q_SLOTS:
    //Import
    void shouldImportEmail();
    void shouldImportMultiEmails();
    void shouldImportStandardBirthday();
    void shouldImportTheBatsBirthday();
    void shouldImportTitle();
    void shouldImportWorkStreet();
    void shouldImportContactGroup();
    void shouldImportMultiEntries();
    void shouldImportGroupAndAddress();

    //Export
    void shouldExportEmail();
    void shouldExportBirthday();
    void shouldExportTitle();
    void shouldExportMultiEntries();
    void shouldExportGroup();
    void shouldExportWorkStreet();
};

#endif // LDIFCONVERTERTEST_H
