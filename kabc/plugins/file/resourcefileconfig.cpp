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

#include "resourcefileconfig.h"
#include "resourcefile.h"

#include "kabc/formatfactory.h"
#include "kabc/stdaddressbook.h"

#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kdialog.h>

#include <QtGui/QFormLayout>

#include <unistd.h>

using namespace KABC;

ResourceFileConfig::ResourceFileConfig( QWidget *parent )
    : ConfigWidget( parent )
{
  QFormLayout *mainLayout = new QFormLayout( this );
  mainLayout->setMargin( 0 );

  mFormatBox = new KComboBox( this );

  mainLayout->addRow( i18n( "Format:" ), mFormatBox );

  mFileNameEdit = new KUrlRequester( this );
  mFileNameEdit->setMode( KFile::File | KFile::LocalOnly );

  mainLayout->addRow( i18n( "Location:" ), mFileNameEdit );

  connect( mFileNameEdit, SIGNAL(textChanged(QString)),
           SLOT(checkFilePermissions(QString)) );

  FormatFactory *factory = FormatFactory::self();
  QStringList formats = factory->formats();
  QStringList::Iterator it;
  for ( it = formats.begin(); it != formats.end(); ++it ) {
    FormatInfo info = factory->info( *it );
    if ( !info.isNull() ) {
      mFormatTypes << (*it);
      mFormatBox->addItem( info.nameLabel );
    }
  }

  mInEditMode = false;
}

void ResourceFileConfig::setEditMode( bool value )
{
  mFormatBox->setEnabled( !value );
  mInEditMode = value;
}

void ResourceFileConfig::loadSettings( KRES::Resource *res )
{
  ResourceFile *resource = dynamic_cast<ResourceFile*>( res );

  if ( !resource ) {
    kDebug() << "cast failed";
    return;
  }

  mFormatBox->setCurrentIndex( mFormatTypes.indexOf( resource->format() ) );

  mFileNameEdit->setUrl( KUrl::fromPath( resource->fileName() ) );
  if ( mFileNameEdit->url().isEmpty() ) {
    mFileNameEdit->setUrl( KUrl::fromPath( KABC::StdAddressBook::fileName() ) );
  }
}

void ResourceFileConfig::saveSettings( KRES::Resource *res )
{
  ResourceFile *resource = dynamic_cast<ResourceFile*>( res );

  if ( !resource ) {
    kDebug() << "cast failed";
    return;
  }

  if ( !mInEditMode ) {
    resource->setFormat( mFormatTypes[ mFormatBox->currentIndex() ] );
  }

  resource->setFileName( mFileNameEdit->url().path() );
}

void ResourceFileConfig::checkFilePermissions( const QString &fileName )
{
  // If file exist but is not writeable...
  if ( access( QFile::encodeName( fileName ), F_OK ) == 0 ) {
    emit setReadOnly( access( QFile::encodeName( fileName ), W_OK ) < 0 );
  }
}

#include "resourcefileconfig.moc"
