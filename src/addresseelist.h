/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2002 Jost Schenck <jost@schenck.de>
    SPDX-FileCopyrightText: 2003 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_ADDRESSEELIST_H
#define KCONTACTS_ADDRESSEELIST_H

#include <QVector>

namespace KContacts {
class Addressee;

/**
 * @short  a QVector of Addressee.
 */
typedef QVector<Addressee> AddresseeList;

}

#endif
