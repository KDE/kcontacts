/*
    This file is part of libkabc.
    Copyright (c) 2007 KDE-PIM team <kde-pim@kde.org>

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

#include "testutils.h"
#include <kabc/addressee.h>
#include <vcardconverter.h>
#include <kdebug.h>

using namespace KABC;

int main()
{
  Addressee::List l = vCardsAsAddresseeList();
  QByteArray vcards = vCardsAsText();

  VCardConverter vct;

  Addressee::List parsed = vct.parseVCards( vcards );

  if ( l.size() != parsed.size() ) {
    kDebug() << "\tSize - FAILED :" << l.size() << "vs. parsed" << parsed.size();
  } else {
    kDebug() << "\tSize - PASSED";
  }

  Addressee::List::iterator itr1;
  Addressee::List::iterator itr2;
  for ( itr1 = l.begin(), itr2 = parsed.begin();
        itr1 != l.end() && itr2 != parsed.end(); ++itr1, ++itr2 ) {
    if ( ( *itr1 ).fullEmail() == ( *itr2 ).fullEmail() &&
         ( *itr1 ).organization() == ( *itr2 ).organization() &&
         ( *itr1 ).phoneNumbers() == ( *itr2 ).phoneNumbers()  &&
         ( *itr1 ).emails() == ( *itr2 ).emails() &&
         ( *itr1 ).role() == ( *itr2 ).role() ) {
      kDebug() << "\tAddressee  - PASSED";
      kDebug() << "\t\t" << ( *itr1 ).fullEmail() << "VS." << ( *itr2 ).fullEmail();
    } else {
      kDebug() << "\tAddressee  - FAILED";
      kDebug() << ( *itr1 ).toString();
      kDebug() << ( *itr2 ).toString();
      //kDebug()<<"\t\t"<< (*itr1).fullEmail() << "VS." << (*itr2).fullEmail();
    }
  }
}
