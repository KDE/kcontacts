/*
  This file is part of the kcalcore library.

  SPDX-FileCopyrightText: 2020 Nicolas Fella <nicolas.fella@gmx.de>
  SPDX-FileCopyrightText: 2023 g10Code GmbH
  SPDX-FileContributor: Carl Schwan <carlschwan@gnupg.com>
  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addressbookplugin.h"

using namespace KContacts;

AddressBookPlugin::AddressBookPlugin(QObject *parent, const QVariantList &args)
    : QObject(parent)
{
    Q_UNUSED(args)
}

#include "moc_addressbookplugin.cpp"
