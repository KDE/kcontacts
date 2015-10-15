/*
    This file is part of the KContacts framework.
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

#include "vcardconverter.h"
#include "vcardtool.h"

using namespace KContacts;

VCardConverter::VCardConverter()
    : d(Q_NULLPTR)
{
}

VCardConverter::~VCardConverter()
{
}

QByteArray VCardConverter::exportVCard(const Addressee &addr, Version version) const
{
    Addressee::List list;
    list.append(addr);

    return exportVCards(list, version);
}

QByteArray VCardConverter::exportVCards(const Addressee::List &list, Version version) const
{
    VCardTool tool;
    QByteArray returnValue;
    switch (version) {
    case v2_1:
        returnValue = tool.exportVCards(list, VCard::v2_1);
        break;
    case v3_0:
        returnValue = tool.exportVCards(list, VCard::v3_0);
        break;
    case v4_0:
        returnValue = tool.exportVCards(list, VCard::v4_0);
        break;
    }

    return returnValue;
}

QByteArray VCardConverter::createVCard(const Addressee &addr, Version version) const
{
    Addressee::List list;
    list.append(addr);

    return createVCards(list, version);
}

QByteArray VCardConverter::createVCards(const Addressee::List &list, Version version) const
{
    VCardTool tool;
    QByteArray returnValue;
    switch (version) {
    case v2_1:
        returnValue = tool.createVCards(list, VCard::v2_1);
        break;
    case v3_0:
        returnValue = tool.createVCards(list, VCard::v3_0);
        break;
    case v4_0:
        returnValue = tool.createVCards(list, VCard::v4_0);
        break;
    }

    return returnValue;
}

Addressee VCardConverter::parseVCard(const QByteArray &vcard) const
{
    Addressee::List list = parseVCards(vcard);

    return list.isEmpty() ? Addressee() : list[ 0 ];
}

Addressee::List VCardConverter::parseVCards(const QByteArray &vcard) const
{
    VCardTool tool;

    return tool.parseVCards(vcard);
}

/* Helper functions */

QString KContacts::dateToVCardString(const QDateTime &dateTime)
{
    return dateTime.toString(QStringLiteral("yyyyMMddThhmmssZ"));
}

QString KContacts::dateToVCardString(const QDate &date)
{
    return date.toString(QStringLiteral("yyyyMMdd"));
}

QDateTime KContacts::VCardStringToDate(const QString &dateString)
{
    QDate date;
    QTime time;
    QString d(dateString);

    d = d.remove(QLatin1Char('-')).remove(QLatin1Char(':'));

    if (d.length() >= 8) {
        date = QDate(d.midRef(0, 4).toUInt(), d.midRef(4, 2).toUInt(), d.midRef(6, 2).toUInt());
    }

    if (d.length() > 9 && d[ 8 ].toUpper() == QLatin1Char('T')) {
        time = QTime(d.midRef(9, 2).toUInt(), d.midRef(11, 2).toUInt(), d.midRef(13, 2).toUInt());
    }

    return QDateTime(date, time);
}

void KContacts::adaptIMAttributes(QByteArray &data)
{
    data.replace("X-messaging/aim-All", ("X-AIM"));
    data.replace("X-messaging/icq-All", ("X-ICQ"));
    data.replace("X-messaging/xmpp-All", ("X-JABBER"));
    data.replace("X-messaging/msn-All", ("X-MSN"));
    data.replace("X-messaging/yahoo-All", ("X-YAHOO"));
    data.replace("X-messaging/gadu-All", ("X-GADUGADU"));
    data.replace("X-messaging/skype-All", ("X-SKYPE"));
    data.replace("X-messaging/groupwise-All", ("X-GROUPWISE"));
    data.replace("X-messaging/sms-All", ("X-SMS"));
    data.replace("X-messaging/meanwhile-All", ("X-MEANWHILE"));
    data.replace("X-messaging/irc-All", ("X-IRC"));
    data.replace("X-messaging/googletalk-All", ("X-GTALK"));
}
