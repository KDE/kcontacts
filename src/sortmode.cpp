/*
    This file is part of the KContacts framework..
    Copyright (c) 2004 Tobias Koenig <tokoe@kde.org>

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

#include "sortmode.h"
#include "field.h"

using namespace KContacts;

SortMode::~SortMode()
{
}

class NameSortMode::Private
{
public:
    NameType mNameType;
    bool mAscendingOrder;
};

NameSortMode::NameSortMode()
    : d(new Private)
{
    d->mNameType = FormattedName;
    d->mAscendingOrder = true;
}

NameSortMode::NameSortMode(NameType type, bool ascending)
    : d(new Private)
{
    d->mNameType = type;
    d->mAscendingOrder = ascending;
}

NameSortMode::~NameSortMode()
{
    delete d;
}

bool NameSortMode::lesser(const KContacts::Addressee &first, const KContacts::Addressee &second) const
{
    bool lesser = false;

    switch (d->mNameType) {
    case FormattedName:
        lesser = QString::localeAwareCompare(first.formattedName(), second.formattedName()) < 0;
        break;
    case FamilyName:
        lesser = QString::localeAwareCompare(first.familyName(), second.familyName()) < 0;
        break;
    case GivenName:
        lesser = QString::localeAwareCompare(first.givenName(), second.givenName()) < 0;
        break;
    default:
        lesser = false;
        break;
    }

    if (!d->mAscendingOrder) {
        lesser = !lesser;
    }

    return lesser;
}

class FieldSortMode::Private
{
public:
    KContacts::Field *mField;
    bool mAscendingOrder;
};

FieldSortMode::FieldSortMode(KContacts::Field *field, bool ascending)
    : d(new Private)
{
    d->mField = field;
    d->mAscendingOrder = ascending;
}

FieldSortMode::~FieldSortMode()
{
    delete d;
}

bool FieldSortMode::lesser(const KContacts::Addressee &first, const KContacts::Addressee &second) const
{
    if (!d->mField) {
        return false;
    } else {
        bool lesser =
            QString::localeAwareCompare(d->mField->value(first), d->mField->value(second)) < 0;
        if (!d->mAscendingOrder) {
            lesser = !lesser;
        }

        return lesser;
    }
}
