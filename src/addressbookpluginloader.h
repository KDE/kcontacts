/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_ADRESSBOOKPLUGINLOADER_H
#define KCONTACTS_ADRESSBOOKPLUGINLOADER_H

#include "kcontacts_export.h"

#include <KContacts/AddressBookPlugin>

namespace KContacts
{

/**
 * Provides access to a KContact::AddressBookPlugin instance, if available.
 * @since 6.0
 */
class KCONTACTS_EXPORT AddressBookPluginLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasPlugin READ hasPlugin)
    Q_PROPERTY(QList<KContacts::AddressBookPlugin *> plugins READ plugins)

public:
    /**
     * Returns @c true if there is a platform address book is available.
     */
    static bool hasPlugin();

    /**
     * Returns the platform address book plugins.
     */
    static QList<KContacts::AddressBookPlugin *> plugins();
};

}

#endif // KCONTACTS_CONTACTPLUGINLOADER_H
