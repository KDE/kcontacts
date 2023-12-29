/*
  This file is part of the kcontact library.

  SPDX-FileCopyrightText: 2020 Nicolas Fella <nicolas.fella@gmx.de>
  SPDX-FileCopyrightText: 2023 g10Code GmbH
  SPDX-FileContributor: Carl Schwan <carlschwan@gnupg.com>
  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCALCORE_CALENDARPLUGIN_H
#define KCALCORE_CALENDARPLUGIN_H

#include "kcontacts_export.h"

#include <KContacts/AddressBook>

namespace KContacts
{
/**
  @brief
  A plugin that provides address book data.

  It allows addresseee book applications to consume data provided by multiple
  sources, e.g. local vcard files or remote address books.

  @since 6.0
*/
class KCONTACTS_EXPORT AddressBookPlugin : public QObject
{
    Q_OBJECT
public:
    AddressBookPlugin(QObject *parent, const QVariantList &args);

    /**
     * The set of address books defined by this plugin.
     *
     * @return QList of AddressBook.
     */
    virtual QList<AddressBook::Ptr> addressBooks() const = 0;

Q_SIGNALS:
    /**
     * Emitted when the set of address books changed.
     */
    void addressBooksChanged();

private:
    void *d;
};

}

#endif
