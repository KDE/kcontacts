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

#include "resourcedirconfig.h"
#include "resourcedir.h"

#include "kabc/formatfactory.h"
#include "kabc/stdaddressbook.h"

#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kdialog.h>

#include <QtGui/QLabel>
#include <QtGui/QLayout>

using namespace KABC;

ResourceDirConfig::ResourceDirConfig( QWidget *parent )
    : KRES::ConfigWidget( parent )
{
  QGridLayout *mainLayout = new QGridLayout( this );
  mainLayout->setMargin( 0 );
  mainLayout->setSpacing( KDialog::spacingHint() );

  QLabel *label = new QLabel( i18n( "Format:" ), this );
  mFormatBox = new KComboBox( this );

  mainLayout->addWidget( label, 0, 0 );
  mainLayout->addWidget( mFormatBox, 0, 1 );

  label = new QLabel( i18n( "Location:" ), this );
  mFileNameEdit = new KUrlRequester( this );
  mFileNameEdit->setMode( KFile::Directory );

  mainLayout->addWidget( label, 1, 0 );
  mainLayout->addWidget( mFileNameEdit, 1, 1 );

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

void ResourceDirConfig::setEditMode( bool value )
{
  mFormatBox->setEnabled( !value );
  mInEditMode = value;
}

void ResourceDirConfig::loadSettings( KRES::Resource *res )
{
  ResourceDir *resource = dynamic_cast<ResourceDir*>( res );

  if ( !resource ) {
    kDebug() << "cast failed";
    return;
  }

  mFormatBox->setCurrentIndex( mFormatTypes.indexOf( resource->format() ) );

  mFileNameEdit->setUrl( resource->path() );
  if ( mFileNameEdit->url().isEmpty() ) {
    mFileNameEdit->setUrl( KABC::StdAddressBook::directoryName() );
  }
}

void ResourceDirConfig::saveSettings( KRES::Resource *res )
{
  ResourceDir *resource = dynamic_cast<ResourceDir*>( res );

  if ( !resource ) {
    kDebug() << "cast failed";
    return;
  }

  if ( mInEditMode ) {
    resource->setFormat( mFormatTypes[ mFormatBox->currentIndex() ] );
  }

  resource->setPath( mFileNameEdit->url().path() );
}

#include "resourcedirconfig.moc"
