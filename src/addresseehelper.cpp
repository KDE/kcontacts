/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2003 Carsten Pfeiffer <pfeiffer@kde.org>

    This SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addresseehelper.h"

#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>

using namespace KContacts;

class Q_DECL_HIDDEN KContacts::AddresseeHelperPrivate
{
public:
    QSet<QString> mTitles;
    QSet<QString> mPrefixes;
    QSet<QString> mSuffixes;
    bool mTreatAsFamilyName;
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
}

AddresseeHelper::~AddresseeHelper()
{
}

static void addToSet(const QStringList &list, QSet<QString> &container)
{
    for (const auto &str : list) {
        if (!str.isEmpty()) {
            container.insert(str);
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

    d->mPrefixes.insert(QStringLiteral("van"));
    d->mPrefixes.insert(QStringLiteral("von"));
    d->mPrefixes.insert(QStringLiteral("de"));

    KConfig _config(QStringLiteral("kabcrc"), KConfig::NoGlobals);
    KConfigGroup config(&_config, "General");

    addToSet(config.readEntry("Prefixes", QStringList()), d->mTitles);
    addToSet(config.readEntry("Inclusions", QStringList()), d->mPrefixes);
    addToSet(config.readEntry("Suffixes", QStringList()), d->mSuffixes);
    d->mTreatAsFamilyName = config.readEntry("TreatAsFamilyName", true);
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

bool AddresseeHelper::treatAsFamilyName() const
{
    return d->mTreatAsFamilyName;
}

#include "moc_addresseehelper.cpp"
