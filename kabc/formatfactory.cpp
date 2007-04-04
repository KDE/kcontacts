/*
    This file is part of libkabc.
    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>

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

#include <QtCore/QFile>

#include <kdebug.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstandarddirs.h>
#include <kstaticdeleter.h>

#include "vcardformat.h"

#include "formatfactory.h"

using namespace KABC;

static KStaticDeleter<FormatFactory> factoryDeleter;

class FormatFactory::Private
{
  public:
    ~Private()
    {
      mFormatList.clear();
    }

    KLibrary *openLibrary( const QString& libName );

    static FormatFactory *mSelf;

    QHash<QString, FormatInfo> mFormatList;
};

KLibrary *FormatFactory::Private::openLibrary( const QString& libName )
{
  KLibrary *library = 0;

  QString path = KLibLoader::findLibrary( QFile::encodeName( libName ) );

  if ( path.isEmpty() ) {
    kDebug( 5700 ) << "No format plugin library was found!" << endl;
    return 0;
  }

  library = KLibLoader::self()->library( QFile::encodeName( path ) );

  if ( !library ) {
    kDebug( 5700 ) << "Could not load library '" << libName << "'" << endl;
    return 0;
  }

  return library;
}

FormatFactory *FormatFactory::Private::mSelf = 0;

FormatFactory *FormatFactory::self()
{
  kDebug(5700) << "FormatFactory::self()" << endl;

  if ( !Private::mSelf )
    factoryDeleter.setObject( Private::mSelf, new FormatFactory );

  return Private::mSelf;
}

FormatFactory::FormatFactory()
  : d( new Private )
{
  // dummy entry for default format
  FormatInfo info;
  info.library = "<NoLibrary>";
  info.nameLabel = i18n( "vCard" );
  info.descriptionLabel = i18n( "vCard Format" );
  d->mFormatList.insert( "vcard", info );

  const QStringList list = KGlobal::dirs()->findAllResources( "data" ,"kabc/formats/*.desktop",
                                                              KStandardDirs::Recursive |
                                                              KStandardDirs::NoDuplicates );
  for ( QStringList::ConstIterator it = list.begin(); it != list.end(); ++it ) {
    KConfig config( *it, KConfig::OnlyLocal);

    if ( !config.hasGroup( "Misc" ) || !config.hasGroup( "Plugin" ) )
      continue;

    KConfigGroup group = config.group( "Plugin" );
    QString type = group.readEntry( "Type" );
    info.library = group.readEntry( "X-KDE-Library" );

    group = config.group( "Misc" );
    info.nameLabel = group.readEntry( "Name" );
    info.descriptionLabel = group.readEntry( "Comment", i18n( "No description available." ) );

    d->mFormatList.insert( type, info );
  }
}

FormatFactory::~FormatFactory()
{
  delete d;
}

QStringList FormatFactory::formats()
{
  QStringList retval;

  // make sure 'vcard' is the first entry
  retval << "vcard";

  QHashIterator<QString, FormatInfo> it( d->mFormatList );
  while ( it.hasNext() ) {
    it.next();
    if ( it.key() != "vcard" )
      retval << it.key();
  }

  return retval;
}

FormatInfo FormatFactory::info( const QString &type )
{
  if ( type.isEmpty() || !d->mFormatList.contains( type ) )
    return FormatInfo();
  else
    return d->mFormatList[ type ];
}

Format *FormatFactory::format( const QString& type )
{
  Format *format = 0;

  if ( type.isEmpty() )
    return 0;

  if ( type == "vcard" ) {
    format = new VCardFormat;
    format->setType( type );
    format->setNameLabel( i18n( "vCard" ) );
    format->setDescriptionLabel( i18n( "vCard Format" ) );
    return format;
  }

  if ( !d->mFormatList.contains( type ) )
    return 0;

  FormatInfo fi = d->mFormatList[ type ];
  QString libName = fi.library;

  KLibrary *library = d->openLibrary( libName );
  if ( !library )
    return 0;

  KLibrary::void_function_ptr format_func = library->resolveFunction( "format" );

  if ( format_func ) {
    format = ((Format* (*)())format_func)();
    format->setType( type );
    format->setNameLabel( fi.nameLabel );
    format->setDescriptionLabel( fi.descriptionLabel );
  } else {
    kDebug( 5700 ) << "'" << libName << "' is not a format plugin." << endl;
    return 0;
  }

  return format;
}
