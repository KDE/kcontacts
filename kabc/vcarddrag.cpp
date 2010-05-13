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

#include "vcarddrag.h"

#include "vcardconverter.h"

#include <KMimeType>

#include <QMimeData>

using namespace KABC;

static QString findCompatibleMimeType( const QMimeData *md )
{
  // check the canonical MIME type first
  if ( md->hasFormat( KABC::Addressee::mimeType() ) ) {
    return KABC::Addressee::mimeType();
  }

  const QStringList mimeTypeOffers = md->formats();
  Q_FOREACH( const QString &mimeType, mimeTypeOffers ) {
   const KMimeType::Ptr mimeTypePtr = KMimeType::mimeType( mimeType, KMimeType::ResolveAliases );
    if ( !mimeTypePtr.isNull() ) {
      if ( mimeTypePtr->is( KABC::Addressee::mimeType() ) ) {
        return mimeType;
      }
    }
  }

  return QString();
}

bool VCardDrag::populateMimeData( QMimeData *md, const QByteArray &content )
{
  md->setData( KABC::Addressee::mimeType(), content );
  return true;
}

bool VCardDrag::populateMimeData( QMimeData *md, const KABC::Addressee::List &addressees )
{
  KABC::VCardConverter converter;
  const QByteArray vcards = converter.createVCards( addressees );
  if ( !vcards.isEmpty() ) {
    return populateMimeData( md, vcards );
  } else {
    return false;
  }
}

bool VCardDrag::canDecode( const QMimeData *md )
{
  return !findCompatibleMimeType( md ).isEmpty();
}

bool VCardDrag::fromMimeData( const QMimeData *md, QByteArray &content )
{
  const QString mimeOffer = findCompatibleMimeType( md );
  if ( mimeOffer.isEmpty() ) {
    return false;
  }
  content = md->data( mimeOffer );
  return !content.isEmpty();
}

bool VCardDrag::fromMimeData( const QMimeData *md, KABC::Addressee::List &addressees )
{
  const QString mimeOffer = findCompatibleMimeType( md );
  if ( mimeOffer.isEmpty() ) {
    return false;
  }
  addressees = KABC::VCardConverter().parseVCards( md->data( mimeOffer ) );
  return !addressees.isEmpty();
}

// kate: space-indent on; indent-width 2; replace-tabs on;
