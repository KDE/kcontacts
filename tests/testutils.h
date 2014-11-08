/*
    This file is part of the KContacts framework.
    Copyright (c) 2007 KDE-PIM team <kde-pim@kde.org>

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

#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QtCore/QByteArray>
#include <QtCore/QString>

#include <kcontacts/addressee.h>

KContacts::Addressee vcard1();
KContacts::Addressee vcard2();
KContacts::Addressee vcard3();
KContacts::Addressee::List vCardsAsAddresseeList();
QByteArray vCardAsText(const QString &location);
QByteArray vCardsAsText();

#endif
