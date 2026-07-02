/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2002 Jost Schenck <jost@schenck.de>
    SPDX-FileCopyrightText: 2003 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_ADDRESSEELIST_H
#define KCONTACTS_ADDRESSEELIST_H

// All KF6 public headers should provide the version macros.
// Include version header explicitly because the export header
// (which usually takes care of that) isn't used here
#include <kcontacts_version.h> // IWYU pragma: export

#include <QList>

namespace KContacts
{
class Addressee;

/*!
 * \typedef KContacts::AddresseeList
 * A QList of Addressee.
 */
typedef QList<Addressee> AddresseeList;
}

#endif
