/*
    This file is part of the KContacts framework.
    Copyright (c) 2015-2016 Laurent Montel <montel@kde.org>

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

#include "related.h"
#include <QMap>
#include <qstringlist.h>
#include <QDataStream>

using namespace KContacts;

class Q_DECL_HIDDEN Related::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        relatedTo = other.relatedTo;
    }
    QMap<QString, QStringList> parameters;
    QString relatedTo;
};

Related::Related()
    : d(new Private)
{

}

Related::Related(const Related &other)
    : d(other.d)
{

}

Related::Related(const QString &relatedTo)
    : d(new Private)
{
    d->relatedTo = relatedTo;
}

Related::~Related()
{

}

void Related::setRelated(const QString &relatedTo)
{
    d->relatedTo = relatedTo;
}

QString Related::related() const
{
    return d->relatedTo;
}

bool Related::isValid() const
{
    return !d->relatedTo.isEmpty();
}

void Related::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> Related::parameters() const
{
    return d->parameters;
}

bool Related::operator==(const Related &other) const
{
    return (d->parameters == other.parameters()) && (d->relatedTo == other.related());
}

bool Related::operator!=(const Related &other) const
{
    return !(other == *this);
}

Related &Related::operator=(const Related &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Related::toString() const
{
    QString str;
    str += QLatin1String("Related {\n");
    str += QStringLiteral("    relatedTo: %1\n").arg(d->relatedTo);
    if (!d->parameters.isEmpty()) {
        QMapIterator<QString, QStringList> i(d->parameters);
        QString param;
        while (i.hasNext()) {
            i.next();
            param += QStringLiteral("%1 %2").arg(i.key(), i.value().join(QLatin1Char(',')));
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Related &related)
{
    return s << related.d->parameters << related.d->relatedTo;
}

QDataStream &KContacts::operator>>(QDataStream &s, Related &related)
{
    s >> related.d->parameters >> related.d->relatedTo;
    return s;
}
