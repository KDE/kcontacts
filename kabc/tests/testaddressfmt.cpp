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

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kstandarddirs.h>

#include "kabc/addressbook.h"
#include "kabc/address.h"

using namespace KABC;

static const KCmdLineOptions options[] =
{
  { "save", "", 0 },
  { "number", "", 0 },
  KCmdLineLastOption
};

int main(int argc,char **argv)
{
  KAboutData aboutData("testaddressfmt","TestAddressFormat","0.1");
  KCmdLineArgs::init(argc, argv, &aboutData);
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app;

  Address a;
  a.setStreet("Lummerlandstr. 1");
  a.setPostalCode("12345");
  a.setLocality("Lummerstadt");
  a.setCountry ("Germany");

  Address b;
  b.setStreet("457 Foobar Ave");
  b.setPostalCode("1A2B3C");
  b.setLocality("Nervousbreaktown");
  b.setRegion("DC");
  b.setCountry("United States of America");

  Address c;
  c.setStreet("Lummerlandstr. 1");
  c.setPostalCode("12345");
  c.setLocality("Lummerstadt");
  c.setCountry ("Deutschland");

  Address d;
  d.setStreet("Lummerlandstr. 1");
  d.setPostalCode("12345");
  d.setLocality("Lummerstadt");
  d.setCountry ("");

  qDebug( "-------------------------------------\nShould have german address formatting, local country formatting\n" );
  qDebug( "%s", a.formattedAddress("Jim Knopf").toLatin1().constData() );
  qDebug( "-------------------------------------\nShould have US address formatting, local country formatting\n" );
  qDebug( "%s", b.formattedAddress("Huck Finn").toLatin1().constData() );
  qDebug( "-------------------------------------\nShould have german address formatting, local country formatting\n" );
  qDebug( "%s", c.formattedAddress("Jim Knopf").toLatin1().constData() );
  qDebug( "-------------------------------------\nShould have local address formatting, local country formatting\n" );
  qDebug( "%s", d.formattedAddress("Jim Knopf").toLatin1().constData() );
}


