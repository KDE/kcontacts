/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2024 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_QML_TYPES
#define KCONTACTS_QML_TYPES

#include <KContacts/Address>
#include <KContacts/AddressFormat>
#include <KContacts/Addressee>
#include <KContacts/GrammaticalGender>
#include <KContacts/Pronouns>
#include <KContacts/SocialProfile>

#include <QQmlEngine>

struct AddressForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::Address)
    QML_VALUE_TYPE(address)
    QML_STRUCTURED_VALUE
};

struct AddresseeForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::Addressee)
    QML_VALUE_TYPE(addressee)
    QML_STRUCTURED_VALUE
};

struct EmailForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::Email)
    QML_VALUE_TYPE(email)
    QML_CONSTRUCTIBLE_VALUE
};

struct PhoneNumberForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::PhoneNumber)
    QML_VALUE_TYPE(phoneNumber)
    QML_STRUCTURED_VALUE
};

struct ImppForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::Impp)
    QML_VALUE_TYPE(impp)
    QML_STRUCTURED_VALUE
};

struct PictureForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::Picture)
    QML_VALUE_TYPE(picture)
    QML_STRUCTURED_VALUE
};

struct GeoForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::Geo)
    QML_VALUE_TYPE(geo)
    QML_STRUCTURED_VALUE
};

struct GrammaticalGenderForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::GrammaticalGender)
    QML_VALUE_TYPE(grammaticalGender)
    QML_STRUCTURED_VALUE
};

struct PronounsForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::Pronouns)
    QML_VALUE_TYPE(pronouns)
    QML_STRUCTURED_VALUE
};

struct SocialProfileForeign {
    Q_GADGET
    QML_FOREIGN(KContacts::SocialProfile)
    QML_VALUE_TYPE(socialProfile)
    QML_STRUCTURED_VALUE
};

namespace KContactForeign
{
Q_NAMESPACE
QML_NAMED_ELEMENT(KContacts)
QML_FOREIGN_NAMESPACE(KContacts)
};

#endif
