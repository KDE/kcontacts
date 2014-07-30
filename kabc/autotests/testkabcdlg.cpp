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

#include "kabc/addresseedialog.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <qdebug.h>
#include <klocalizedstring.h>
#include <kcmdlineargs.h>

using namespace KABC;

int main(int argc, char **argv)
{
    KAboutData aboutData("testkabcdlg", 0, ki18n("TestKabc"), "0.1");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("multiple", ki18n("Allow selection of multiple addressees"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->isSet("multiple")) {
        Addressee::List al = AddresseeDialog::getAddressees(0);
        Addressee::List::ConstIterator it;
        qDebug() << "Selected Addressees:";
        for (it = al.constBegin(); it != al.constEnd(); ++it) {
            qDebug() << "  " << (*it).fullEmail();
        }
    } else {
        Addressee a = AddresseeDialog::getAddressee(0);

        if (!a.isEmpty()) {
            qDebug() << "Selected Addressee:";
            qDebug() << a.toString();
        } else {
            qDebug() << "No Addressee selected.";
        }
    }
}
