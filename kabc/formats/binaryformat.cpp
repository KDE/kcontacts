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

#include <QDataStream>
#include <QImage>

#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include "addressbook.h"
#include "addressee.h"
#include "picture.h"
#include "sound.h"

#include "binaryformat.h"

#define BINARY_FORMAT_VERSION 1

using namespace KABC;

extern "C"
{
  KDE_EXPORT Format *format()
  {
    return new BinaryFormat;
  }
}

bool BinaryFormat::load( Addressee &addressee, QFile *file )
{
  kDebug(5700) << "BinaryFormat::load()" << endl;
  QDataStream stream( file );

  if ( !checkHeader( stream ) )
    return false;

  loadAddressee( addressee, stream );

  return true;
}

bool BinaryFormat::loadAll( AddressBook*, Resource *resource, QFile *file )
{
  kDebug(5700) << "BinaryFormat::loadAll()" << endl;

  QDataStream stream( file );

  if ( !checkHeader( stream ) )
    return false;

  quint32 entries;

  stream >> entries;

  for ( uint i = 0; i < entries; ++i ) {
    Addressee addressee;
    loadAddressee( addressee, stream );
    addressee.setResource( resource );
    addressee.setChanged( false );
    resource->insertAddressee( addressee );
  }

  return true;
}

void BinaryFormat::save( const Addressee &addressee, QFile *file )
{
  kDebug(5700) << "BinaryFormat::save()" << endl;

  QDataStream stream( file );

  writeHeader( stream );

  quint32 entries = 1;
  stream << entries;
  saveAddressee( addressee, stream );
}

void BinaryFormat::saveAll( AddressBook*, Resource *resource, QFile *file )
{
  kDebug(5700) << "BinaryFormat::saveAll()" << endl;

  quint32 counter = 0;
  QDataStream stream( file );

  writeHeader( stream );
  // set dummy number of entries
  stream << counter;

  Resource::Iterator it;
  for ( it = resource->begin(); it != resource->end(); ++it ) {
    saveAddressee( (*it), stream );
    counter++;
    (*it).setChanged( false );
  }

  // set real number of entries
  stream.device()->seek( 2 * sizeof( quint32 ) );
  stream << counter;
}

bool BinaryFormat::checkFormat( QFile *file ) const
{
  kDebug(5700) << "BinaryFormat::checkFormat()" << endl;

  QDataStream stream( file );

  return checkHeader( stream );
}

bool BinaryFormat::checkHeader( QDataStream &stream ) const
{
  quint32 magic, version;
    
  stream >> magic >> version;

  QFile *file = dynamic_cast<QFile*>( stream.device() );

  if ( !file ) {
    kError() << i18n("Not a file?") << endl;
    return false;
  }

  if ( magic != 0x2e93e ) {
    kError() << i18n("File '%1' is not binary format.",  file->fileName() ) << endl;
    return false;
  }

  if ( version != BINARY_FORMAT_VERSION ) {
    kError() << i18n("File '%1' is the wrong version.",  file->fileName() ) << endl;
    return false;
  }

  return true;
}

void BinaryFormat::writeHeader( QDataStream &stream )
{
  quint32 magic, version;
    
  magic = 0x2e93e;
  version = BINARY_FORMAT_VERSION;

  stream << magic << version;
}

void BinaryFormat::loadAddressee( Addressee &addressee, QDataStream &stream )
{
  stream >> addressee;
/*
  // load pictures
  Picture photo = addressee.photo();
  Picture logo = addressee.logo();

  if ( photo.isIntern() ) {
    QImage img;
    if ( !img.load( locateLocal( "data", "kabc/photos/" ) + addressee.uid() ) )
      kDebug(5700) << "No photo available for '" << addressee.uid() << "'." << endl;

    addressee.setPhoto( img );
  }

  if ( logo.isIntern() ) {
    QImage img;
    if ( !img.load( locateLocal( "data", "kabc/logos/" ) + addressee.uid() ) )
      kDebug(5700) << "No logo available for '" << addressee.uid() << "'." << endl;

    addressee.setLogo( img );
  }

  // load sound
  // TODO: load sound data from file
*/
}

void BinaryFormat::saveAddressee( const Addressee &addressee, QDataStream &stream )
{
  stream << addressee;
/*
  // load pictures
  Picture photo = addressee.photo();
  Picture logo = addressee.logo();

  if ( photo.isIntern() ) {
    QImage img = photo.data();
    QString fileName = locateLocal( "data", "kabc/photos/" ) + addressee.uid();

    if ( !img.save( fileName, "PNG" ) )
      kDebug(5700) << "Unable to save photo for '" << addressee.uid() << "'." << endl;
  }

  if ( logo.isIntern() ) {
    QImage img = logo.data();
    QString fileName = locateLocal( "data", "kabc/logos/" ) + addressee.uid();

    if ( !img.save( fileName, "PNG" ) )
      kDebug(5700) << "Unable to save logo for '" << addressee.uid() << "'." << endl;
  }

  // save sound
  // TODO: save the sound data to file
*/
}
