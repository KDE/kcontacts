/*
    This file is part of the KContacts framework.
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>

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

#ifndef KCONTACTS_VCARDTOOL_H
#define KCONTACTS_VCARDTOOL_H

#include "kcontacts_export.h"
#include "addressee.h"
#include "vcardparser/vcardparser.h"

class QDateTime;

namespace KContacts
{

class Key;
class Picture;
class Secrecy;
class Sound;

class KCONTACTS_EXPORT VCardTool
{
public:
    VCardTool();
    ~VCardTool();

    /**
      Creates a string that contains the addressees from the list in
      the vCard format.
     */
    QByteArray createVCards(const Addressee::List &list,
                            VCard::Version version = VCard::v3_0) const;

    /**
     * since 4.9.1
     */
    QByteArray exportVCards(const Addressee::List &list,
                            VCard::Version version = VCard::v3_0) const;
    /**
      Parses the string and returns a list of addressee objects.
     */
    Addressee::List parseVCards(const QByteArray &vcard) const;

private:

    QByteArray createVCards(const Addressee::List &list,
                            VCard::Version version, bool exportVcard) const;

    /**
      Split a string and replaces escaped separators on the fly with
      unescaped ones.
     */
    QStringList splitString(const QChar &sep, const QString &value) const;

    QDateTime parseDateTime(const QString &str) const;
    QString createDateTime(const QDateTime &dateTime) const;

    Picture parsePicture(const VCardLine &line) const;
    VCardLine createPicture(const QString &identifier, const Picture &pic) const;

    Sound parseSound(const VCardLine &line) const;
    VCardLine createSound(const Sound &snd) const;

    Key parseKey(const VCardLine &line) const;
    VCardLine createKey(const Key &key) const;

    Secrecy parseSecrecy(const VCardLine &line) const;
    VCardLine createSecrecy(const Secrecy &secrecy) const;

    QMap<QString, Address::TypeFlag> mAddressTypeMap;
    QMap<QString, PhoneNumber::TypeFlag> mPhoneTypeMap;

    class VCardToolPrivate;
    VCardToolPrivate *d;
};

}

#endif
