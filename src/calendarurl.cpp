/*
    This file is part of the KContacts framework.
    Copyright (c) 2015 Laurent Montel <montel@kde.org>

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

#include "calendarurl.h"

#include <QMap>
#include <QUrl>
#include <QDataStream>
#include <qstringlist.h>

using namespace KContacts;

class Q_DECL_HIDDEN CalendarUrl::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        type = other.type;
        url = other.url;
    }
    static QString typeToString(CalendarUrl::CalendarType type);
    QMap<QString, QStringList> parameters;
    QUrl url;
    CalendarUrl::CalendarType type;

};

QString CalendarUrl::Private::typeToString(CalendarUrl::CalendarType type)
{
    QString ret;
    switch (type) {
    case Unknown:
    case EndCalendarType:
        ret = QStringLiteral("Unknown");
        break;
    case FBUrl:
        ret = QStringLiteral("FreeBusy");
        break;
    case CALUri:
        ret = QStringLiteral("CalUri");
        break;
    case CALADRUri:
        ret = QStringLiteral("Caladruri");
        break;
    }
    return ret;
}

CalendarUrl::CalendarUrl()
    : d(new Private)
{
    d->type = Unknown;
}

CalendarUrl::CalendarUrl(CalendarUrl::CalendarType type)
    : d(new Private)
{
    d->type = type;
}

CalendarUrl::CalendarUrl(const CalendarUrl &other)
    : d(other.d)
{
}

CalendarUrl::~CalendarUrl()
{

}

QMap<QString, QStringList> CalendarUrl::parameters() const
{
    return d->parameters;
}

bool CalendarUrl::operator==(const CalendarUrl &other) const
{
    return (d->parameters == other.parameters()) && (d->type == other.type()) && (d->url == other.url());
}

bool CalendarUrl::operator!=(const CalendarUrl &other) const
{
    return !(other == *this);
}

CalendarUrl &CalendarUrl::operator=(const CalendarUrl &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString CalendarUrl::toString() const
{
    QString str;
    str += QLatin1String("CalendarUrl {\n");
    str += QStringLiteral("    url: %1\n").arg(d->url.toString());
    str += QStringLiteral("    type: %1\n").arg(CalendarUrl::Private::typeToString(d->type));
    if (!d->parameters.isEmpty()) {
        QMapIterator<QString, QStringList> i(d->parameters);
        QString param;
        while (i.hasNext()) {
            i.next();
            param += QStringLiteral("%1 %2").arg(i.key(), i.value().join(QStringLiteral(",")));
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

void CalendarUrl::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

bool CalendarUrl::isValid() const
{
    return (d->type != Unknown) && (d->url.isValid());
}

void CalendarUrl::setType(CalendarUrl::CalendarType type)
{
    d->type = type;
}

CalendarUrl::CalendarType CalendarUrl::type() const
{
    return d->type;
}

void CalendarUrl::setUrl(const QUrl &url)
{
    d->url = url;
}

QUrl CalendarUrl::url() const
{
    return d->url;
}

QDataStream &KContacts::operator<<(QDataStream &s, const CalendarUrl &calUrl)
{
    return s << calUrl.d->parameters << (uint)calUrl.d->type << calUrl.d->url;
}

QDataStream &KContacts::operator>>(QDataStream &s, CalendarUrl &calUrl)
{
    uint type;
    s >> calUrl.d->parameters >> type >> calUrl.d->url;
    calUrl.d->type = static_cast<CalendarUrl::CalendarType>(type);
    return s;
}

