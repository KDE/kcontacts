// This file is part of the KContacts framework.
// SPDX-FileCopyrightText: 2023 g10Code GmbH
// SPDX-FileContributor: Carl Schwan <carlschwan@gnupg.com>
// SPDX-License-Identifier: LGPL-2.0-or-later

#include "addressbook.h"

#include <QIcon>

using namespace KContacts;

class AddressBook::Private
{
public:
    QString name;
    QIcon icon;
    AccessMode accessMode;
    bool isLoading;
};

AddressBook::AddressBook(QObject *parent)
    : QObject(parent)
{
}

AddressBook::~AddressBook() = default;

QString AddressBook::name() const
{
    return d->name;
}

void AddressBook::setName(const QString &name)
{
    if (d->name == name) {
        return;
    }
    d->name = name;
}

QIcon AddressBook::icon() const
{
    return d->icon;
}

void AddressBook::setIcon(const QIcon &icon)
{
    if (d->icon.name() == icon.name()) {
        return;
    }
    d->icon = icon;
    Q_EMIT iconChanged();
}

AccessMode AddressBook::accessMode() const
{
    return d->accessMode;
}

void AddressBook::setAccessMode(const AccessMode mode)
{
    if (d->accessMode == mode) {
        return;
    }
    d->accessMode = mode;
    Q_EMIT accessModeChanged();
}

bool AddressBook::isLoading() const
{
    return d->isLoading;
}

void AddressBook::setIsLoading(const bool isLoading)
{
    if (d->isLoading == isLoading) {
        return;
    }
    d->isLoading = isLoading;
    Q_EMIT isLoadingChanged();
}
