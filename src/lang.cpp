/*
    This file is part of the KContacts framework.
    Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#include "lang.h"
#include <QMap>
#include <qstringlist.h>
#include <QDataStream>

using namespace KContacts;

class Q_DECL_HIDDEN Lang::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        language = other.language;
    }
    QMap<QString, QStringList> parameters;
    QString language;
};

Lang::Lang()
    : d(new Private)
{

}

Lang::Lang(const Lang &other)
    : d(other.d)
{

}

Lang::Lang(const QString &language)
    : d(new Private)
{
    d->language = language;
}

Lang::~Lang()
{

}

void Lang::setLanguage(const QString &lang)
{
    d->language = lang;
}

QString Lang::language() const
{
    return d->language;
}

bool Lang::isValid() const
{
    return !d->language.isEmpty();
}

void Lang::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> Lang::parameters() const
{
    return d->parameters;
}

bool Lang::operator==(const Lang &other) const
{
    return (d->parameters == other.parameters()) && (d->language == other.language());
}

bool Lang::operator!=(const Lang &other) const
{
    return !(other == *this);
}

Lang &Lang::operator=(const Lang &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Lang::toString() const
{
    QString str;
    str += QLatin1String("Lang {\n");
    str += QStringLiteral("    language: %1\n").arg(d->language);
    if (!d->parameters.isEmpty()) {
        QString param;
        QMap<QString, QStringList>::const_iterator it = d->parameters.constBegin();
        const QMap<QString, QStringList>::const_iterator end = d->parameters.constEnd();
        while (it != end) {
            param += QStringLiteral("%1 %2").arg(it.key(), it.value().join(QLatin1Char(',')));
            it++;
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Lang &lang)
{
    return s << lang.d->parameters << lang.d->language;
}

QDataStream &KContacts::operator>>(QDataStream &s, Lang &lang)
{
    s >> lang.d->parameters >> lang.d->language;
    return s;
}
