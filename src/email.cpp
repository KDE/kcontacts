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

#include "email.h"

#include <QMap>
#include <qstringlist.h>

using namespace KContacts;

class Email::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private( const Private &other )
        : QSharedData( other )
    {
        parameters = other.parameters;
        mail = other.mail;
    }
    QMap<QString, QStringList> parameters;
    QString mail;
};

Email::Email()
    : d( new Private )
{

}

Email::Email(const QString &mail)
    : d( new Private )
{
    d->mail = mail;
}

Email::Email( const Email &other )
  : d( other.d )
{
}

Email::~Email()
{

}

QMap<QString, QStringList> Email::parameters() const
{
    return d->parameters;
}

bool Email::operator==(const Email &other) const
{
    return (d->parameters == other.parameters()) && (d->mail == other.mail());
}

bool Email::operator!=(const Email &other) const
{
    return !( other == *this );
}

Email &Email::operator=(const Email &other)
{
    if ( this != &other ) {
      d = other.d;
    }

    return *this;
}

QString Email::toString() const
{
    QString str;
    str += QString::fromLatin1( "Email {\n" );
    str += QString::fromLatin1( "    mail: %1\n" ).arg( d->mail );
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

void Email::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

void Email::setEmail(const QString &mail)
{
    d->mail = mail;
}

QString Email::mail() const
{
    return d->mail;
}

bool Email::isValid() const
{
  return !d->mail.isEmpty();
}

QDataStream &KContacts::operator<<(QDataStream &s, const Email &email)
{
    return s << email.d->parameters << email.d->mail;
}

QDataStream &KContacts::operator>>(QDataStream &s, Email &email)
{
    s >> email.d->parameters >> email.d->mail;
    return s;
}

