/*
    This file is part of the KContacts framework.

    SPDX-FileCopyrightText: 2002-2010 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_VCARDDRAG_H
#define KCONTACTS_VCARDDRAG_H

#include "kcontacts_export.h"

#include <QMimeData>
#include <kcontacts/addressee.h>

namespace KContacts
{
/*!
  \namespace KContacts::VCardDrag
  \inheaderfile KContacts/VCardDrag
  \inmodule KContacts

  \brief Utility function for implementing drag&drop for vCards.

  See the Qt drag'n'drop documentation.

  \since 4.5
*/
namespace VCardDrag
{
/*!
  Adds the vCard representation as data of the drag object.

  \a md the object to set the data on

  \a content the vCard data to set

  Always returns true
*/
KCONTACTS_EXPORT bool populateMimeData(QMimeData *md, const QByteArray &content);

/*!
  Adds the vCard representation as data of the drag object.

  \a md the object to set the data on

  \a contacts list of Addressee objects to serialize to vCard

  Returns \c true if serializing of the given list worked, otherwise \c false

  \sa VCardConverter::createVCards()
*/
KCONTACTS_EXPORT bool populateMimeData(QMimeData *md, const KContacts::Addressee::List &contacts);

/*!
  Returns if drag&drop object can be decoded to vCard.

  \a md the object to check for vCard data

  Returns \c true if the given data object contains a vCard MIME type, otherwise \c false
*/
KCONTACTS_EXPORT bool canDecode(const QMimeData *md);

/*!
  Decodes the drag&drop object to vCard component \a content.

  \a md the object to check for vCard data

  \a content where to put the vCard data from \a md

  Returns \c true if there was data for the vCard MIME type, otherwise \c false

  \sa canDecode()
*/
KCONTACTS_EXPORT bool fromMimeData(const QMimeData *md, QByteArray &content);

/*!
  Decodes the MIME data \a md and puts the resulting vCard into \a contacts.

  \a md the object to check for vCard data

  \a contacts where to put the parsed vCards from @p md

  Returns \c true if there was data for the vCard MIME type and it could be parsed successfully,
          otherwise \c false

  \sa canDecode()
*/
KCONTACTS_EXPORT bool fromMimeData(const QMimeData *md, KContacts::Addressee::List &contacts);
}
}

#endif // VCARDDRAG_H
