/*
    This file is part of the KContacts framework.
    Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

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

#include "title.h"
#include <QMap>
#include <QStringList>
#include <QDataStream>

using namespace KContacts;

class Q_DECL_HIDDEN Title::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        title = other.title;
    }

    QMap<QString, QStringList> parameters;
    QString title;
};

Title::Title()
    : d(new Private)
{
}

Title::Title(const Title &other)
    : d(other.d)
{
}

Title::Title(const QString &title)
    : d(new Private)
{
    d->title = title;
}

Title::~Title()
{
}

void Title::setTitle(const QString &title)
{
    d->title = title;
}

QString Title::title() const
{
    return d->title;
}

bool Title::isValid() const
{
    return !d->title.isEmpty();
}

void Title::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> Title::parameters() const
{
    return d->parameters;
}

bool Title::operator==(const Title &other) const
{
    return (d->parameters == other.parameters()) && (d->title == other.title());
}

bool Title::operator!=(const Title &other) const
{
    return !(other == *this);
}

Title &Title::operator=(const Title &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Title::toString() const
{
    QString str = QLatin1String("Title {\n");
    str += QStringLiteral("    title: %1\n").arg(d->title);
    if (!d->parameters.isEmpty()) {
        QString param;
        QMap<QString, QStringList>::const_iterator it = d->parameters.constBegin();
        const QMap<QString, QStringList>::const_iterator end = d->parameters.constEnd();
        while (it != end) {
            param += QStringLiteral("%1 %2").arg(it.key(), it.value().join(QLatin1Char(',')));
            ++it;
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Title &title)
{
    return s << title.d->parameters << title.d->title;
}

QDataStream &KContacts::operator>>(QDataStream &s, Title &title)
{
    s >> title.d->parameters >> title.d->title;
    return s;
}
