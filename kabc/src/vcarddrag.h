/*
  This file is part of libkabc.

  Copyright (c) 2002-2010 Tobias Koenig <tokoe@kde.org>

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

#ifndef KABC_VCARDDRAG_H
#define KABC_VCARDDRAG_H

#include "kabc_export.h"

#include <addressee.h>

namespace KABC {

/**
  @brief Utility function for implementing drag&drop for vCards

  See the Qt drag'n'drop documentation.

  @since 4.5
*/
namespace VCardDrag {

  /**
    Adds the vCard representation as data of the drag object.

    @param md the object to set the data on
    @param content the vCard data to set

    @return Always @c true
  */
  KABC_EXPORT bool populateMimeData( QMimeData *md, const QByteArray &content );

  /**
    Adds the vCard representation as data of the drag object.

    @param md the object to set the data on
    @param contacts list of Addressee objects to serialize to vCard

    @return @c true if serializing of the given list worked, otherwise @c false

    @see VCardConverter::createVCards()
  */
  KABC_EXPORT bool populateMimeData( QMimeData *md, const KABC::Addressee::List &contacts );

  /**
    Returns if drag&drop object can be decoded to vCard.

    @param md the object to check for vCard data

    @return @c true if the given data object contains a vCard MIME type, otherwise @c false
  */
  KABC_EXPORT bool canDecode( const QMimeData *md );

  /**
    Decodes the drag&drop object to vCard component @p content.

    @param md the object to check for vCard data
    @param content where to put the vCard data from @p md

    @return @c true if there was data for the vCard MIME type, otherwise @c false

    @see canDecode()
  */
  KABC_EXPORT bool fromMimeData( const QMimeData *md, QByteArray &content );

  /**
    Decodes the MIME data @p md and puts the resulting vCard into @p contactss.

    @param md the object to check for vCard data
    @param contacts where to put the parsed vCards from @p md

    @return @c true if there was data for the vCard MIME type and it could be parsed successfully,
            otherwise @c false

    @see canDecode()
  */
  KABC_EXPORT bool fromMimeData( const QMimeData *md, KABC::Addressee::List &contacts );
}

}

#endif // VCARDDRAG_H
