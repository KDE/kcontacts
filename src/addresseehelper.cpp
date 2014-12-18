/*
    This file is part of the KDE libraries
    Copyright (C) 2003 Carsten Pfeiffer <pfeiffer@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation, version 2.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "addresseehelper.h"

#include <kconfig.h>
#include <klocalizedstring.h>
#include <kconfiggroup.h>

#include <QCoreApplication>
#include <QtDBus/QtDBus>

using namespace KContacts;

class KContacts::AddresseeHelperPrivate
{
public:
    QSet<QString> mTitles;
    QSet<QString> mPrefixes;
    QSet<QString> mSuffixes;
    bool mTradeAsFamilyName;
};

Q_GLOBAL_STATIC(AddresseeHelper, s_self)

// static
AddresseeHelper *AddresseeHelper::self()
{
    return s_self;
}

AddresseeHelper::AddresseeHelper()
    : QObject()
    , d(new AddresseeHelperPrivate)
{
    initSettings();

    QDBusConnection::sessionBus().connect(QString(), QLatin1String("/KABC"),
                                          QLatin1String("org.kde.kabc.AddressBookConfig"),
                                          QLatin1String("changed"),
                                          this, SLOT(initSettings()));
}

AddresseeHelper::~AddresseeHelper()
{
}

static void addToSet(const QStringList &list, QSet<QString> &container)
{
    QStringList::ConstIterator it;
    QStringList::ConstIterator end(list.end());
    for (it = list.begin(); it != end; ++it) {
        if (!(*it).isEmpty()) {
            container.insert(*it);
        }
    }
}

void AddresseeHelper::initSettings()
{
    d->mTitles.clear();
    d->mSuffixes.clear();
    d->mPrefixes.clear();

    d->mTitles.insert(i18n("Dr."));
    d->mTitles.insert(i18n("Miss"));
    d->mTitles.insert(i18n("Mr."));
    d->mTitles.insert(i18n("Mrs."));
    d->mTitles.insert(i18n("Ms."));
    d->mTitles.insert(i18n("Prof."));

    d->mSuffixes.insert(i18n("I"));
    d->mSuffixes.insert(i18n("II"));
    d->mSuffixes.insert(i18n("III"));
    d->mSuffixes.insert(i18n("Jr."));
    d->mSuffixes.insert(i18n("Sr."));

    d->mPrefixes.insert(QLatin1String("van"));
    d->mPrefixes.insert(QLatin1String("von"));
    d->mPrefixes.insert(QLatin1String("de"));

    KConfig _config(QLatin1String("kabcrc"), KConfig::NoGlobals);
    KConfigGroup config(&_config, "General");

    addToSet(config.readEntry("Prefixes", QStringList()), d->mTitles);
    addToSet(config.readEntry("Inclusions", QStringList()), d->mPrefixes);
    addToSet(config.readEntry("Suffixes", QStringList()), d->mSuffixes);
    d->mTradeAsFamilyName = config.readEntry("TradeAsFamilyName", true);
}

bool AddresseeHelper::containsTitle(const QString &title) const
{
    return d->mTitles.contains(title);
}

bool AddresseeHelper::containsPrefix(const QString &prefix) const
{
    return d->mPrefixes.contains(prefix);
}

bool AddresseeHelper::containsSuffix(const QString &suffix) const
{
    return d->mSuffixes.contains(suffix);
}

bool AddresseeHelper::tradeAsFamilyName() const
{
    return d->mTradeAsFamilyName;
}

