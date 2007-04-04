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
#include "kabc/vcardformat.h"
#include "kabc/phonenumber.h"
#include "kabc/plugins/file/resourcefile.h"
#include "kabc/formats/binaryformat.h"

using namespace KABC;

static const KCmdLineOptions options[] =
{
  { "save", "", 0 },
  { "number", "", 0 },
  KCmdLineLastOption
};

int main(int argc,char **argv)
{
  KAboutData aboutData("testaddressee","TestAddressee","0.1");
  KCmdLineArgs::init(argc, argv, &aboutData);
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app;
//   KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

  kDebug() << "Creating a" << endl;
  Addressee a;

  kDebug() << "tick1" << endl;
  a.setGivenName("Hans");
  kDebug() << "tick2" << endl;
  a.setPrefix("Dr.");

  kDebug() << "Creating b" << endl;
  Addressee b( a );

  kDebug() << "tack1" << endl;
  a.setFamilyName("Wurst");
  kDebug() << "tack2" << endl;
  a.setNickName("hansi");

  kDebug() << "Creating c" << endl;
  Addressee c = a;

  kDebug() << "tock1" << endl;
  c.setGivenName("Eberhard");

  a.dump();
  b.dump();
  c.dump();
}
