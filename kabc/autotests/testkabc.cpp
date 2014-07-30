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

#include "kabc/geo.h"
#include "kabc/secrecy.h"
#include "kabc/stdaddressbook.h"
#include "kabc/timezone.h"
#include "kabc/key.h"
#include "kabc/vcardconverter.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <qdebug.h>
#include <klocalizedstring.h>
#include <kcmdlineargs.h>

#include <QtCore/QFile>
#include <QImage>

using namespace KABC;

int main(int argc, char **argv)
{
    KAboutData aboutData("testkabc", 0, ki18n("TestKabc"), "0.1");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app(false);
    AddressBook *ab = StdAddressBook::self();

#define READ

#ifdef READ
    AddressBook::Iterator it;
    for (it = ab->begin(); it != ab->end(); ++it) {
        VCardConverter converter;
        const QByteArray vcard = converter.createVCard(*it);
        qDebug() << "card=" << vcard;
    }
#else
    Addressee addr;

    addr.setGivenName("Tobias");
    addr.setFamilyName("Koenig");

    Picture pic;
    QImage img;
    img.load("/home/tobias/test.png");
    /*
      pic.setData( img );
      pic.setType( QImage::imageFormat( "/home/tobias/test.png" ) );
    */
    pic.setUrl("http://www.mypict.de");
    addr.setLogo(pic);

    ab->insertAddressee(addr);

    StdAddressBook::save();
#endif

    return 0;
}
