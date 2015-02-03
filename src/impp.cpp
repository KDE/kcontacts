/*
    This file is part of libkabc.
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

#include "impp.h"

#include <QMap>
#include <qstringlist.h>

using namespace KABC;

class Impp::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private( const Private &other )
        : QSharedData( other )
    {
        parameters = other.parameters;
    }
    QMap<QString, QStringList> parameters;
    QString address;
};

Impp::Impp()
    : d(new Private)
{

}

Impp::Impp(const Impp &other)
    : d( other.d )
{
}

Impp::Impp(const QString &address)
    : d(new Private)
{
    d->address = address;
}

Impp::~Impp()
{

}

bool Impp::isValid() const
{
    return !d->address.isEmpty();
}

void Impp::setAddress(const QString &address)
{
    d->address = address;
}

QString Impp::address() const
{
    return d->address;
}

void Impp::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> Impp::parameters() const
{
    return d->parameters;
}

bool Impp::operator==(const Impp &other) const
{
    return (d->parameters == other.parameters()) && (d->address == other.address());
}

bool Impp::operator!=(const Impp &other) const
{
    return !( other == *this );
}

Impp &Impp::operator=(const Impp &other)
{
    if ( this != &other ) {
      d = other.d;
    }

    return *this;
}

QString Impp::toString() const
{
    QString str;
    str += QString::fromLatin1( "Impp {\n" );
    str += QString::fromLatin1( "    address: %1\n" ).arg( d->address );
    if (!d->parameters.isEmpty()) {
        QMapIterator<QString, QStringList> i(d->parameters);
        QString param;
        while (i.hasNext()) {
            i.next();
            param += QString::fromLatin1("%1 %2").arg(i.key()).arg(i.value().join(QLatin1String(",")));
        }
        str += QString::fromLatin1( "    parameters: %1\n" ).arg( param );
    }
    str += QString::fromLatin1( "}\n" );
    return str;
}


QDataStream &KABC::operator<<(QDataStream &s, const Impp &impp)
{
    return s << impp.d->parameters << impp.d->address;
}

QDataStream &KABC::operator>>(QDataStream &s, Impp &impp)
{
    s >> impp.d->parameters >> impp.d->address;
    return s;
}
