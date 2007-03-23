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

#include <QtGui/QApplication>

#include <kconfig.h>
#include <klocale.h>

#include <QtDBus/QtDBus>

#include "addresseehelper.h"

using namespace KABC;

AddresseeHelper * AddresseeHelper::s_self;

// static
AddresseeHelper *AddresseeHelper::self()
{
  if ( !s_self )
    s_self = new AddresseeHelper();

  return s_self;
}

AddresseeHelper::AddresseeHelper()
  : QObject( qApp )
{
  initSettings();

  QDBusConnection::sessionBus().connect(QString(), QString(), "org.kde.kabc.AddressBookConfig", "changed",
          this, SLOT(initSettings()));
}

// static
void AddresseeHelper::addToSet( const QStringList& list,
                                QSet<QString>& container )
{
  QStringList::ConstIterator it;
  for ( it = list.begin(); it != list.end(); ++it ) {
    if ( !(*it).isEmpty() )
      container.insert( *it );
  }
}

void AddresseeHelper::initSettings()
{
  mTitles.clear();
  mSuffixes.clear();
  mPrefixes.clear();

  mTitles.insert( i18n( "Dr." ) );
  mTitles.insert( i18n( "Miss" ) );
  mTitles.insert( i18n( "Mr." ) );
  mTitles.insert( i18n( "Mrs." ) );
  mTitles.insert( i18n( "Ms." ) );
  mTitles.insert( i18n( "Prof." ) );

  mSuffixes.insert( i18n( "I" ) );
  mSuffixes.insert( i18n( "II" ) );
  mSuffixes.insert( i18n( "III" ) );
  mSuffixes.insert( i18n( "Jr." ) );
  mSuffixes.insert( i18n( "Sr." ) );

  mPrefixes.insert( "van" );
  mPrefixes.insert( "von" );
  mPrefixes.insert( "de" );

  KConfig _config( "kabcrc", KConfig::NoGlobals  );
  KConfigGroup config(&_config, "General" );

  addToSet( config.readEntry( "Prefixes", QStringList() ),   mTitles );
  addToSet( config.readEntry( "Inclusions", QStringList() ), mPrefixes );
  addToSet( config.readEntry( "Suffixes", QStringList() ),   mSuffixes );
  mTradeAsFamilyName = config.readEntry("TradeAsFamilyName", true);
}

bool AddresseeHelper::containsTitle( const QString& title ) const
{
  return mTitles.contains( title);
}

bool AddresseeHelper::containsPrefix( const QString& prefix ) const
{
  return mPrefixes.contains( prefix );
}

bool AddresseeHelper::containsSuffix( const QString& suffix ) const
{
  return mSuffixes.contains( suffix );
}

bool AddresseeHelper::tradeAsFamilyName() const
{
  return mTradeAsFamilyName;
}

#include "addresseehelper.moc"
