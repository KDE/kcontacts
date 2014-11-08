/*
    This file is part of the KContacts framework.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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

#include "kcontacts/stdaddressbook.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <qdebug.h>
#include <kcmdlineargs.h>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QRegExp>

using namespace KContacts;

int main(int argc, char **argv)
{
    KAboutData aboutData("kabcargl", 0, ki18n("Fix broken pre3.0rc3 format"), "0.1");
    aboutData.addAuthor(ki18n("Cornelius Schumacher"), KLocalizedString(), "schumacher@kde.org");

    KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app;

    QString filename = StdAddressBook::fileName();

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file '" << filename << "' for reading.";
        return 1;
    }

    QTextStream t(&f);
    t.setCodec(QTextCodec::codecForName("UTF-8"));
    QString text = t.readAll();
    f.close();

    text = QString::fromUtf8(text.toLocal8Bit());
    text.replace(QLatin1String("\n"), QLatin1String("\r\n"));

    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Error opening file '" << filename << "' for writing.";
        return 1;
    }

    QTextStream t2(&f);
    t2.setCodec(QTextCodec::codecForName("UTF-8"));
    t2 << text;
    f.close();
}
