/*
    This file is part of the KContacts framework.
    Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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

#include "note.h"
#include <QMap>
#include <qstringlist.h>
#include <QDataStream>

using namespace KContacts;

class Q_DECL_HIDDEN Note::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        note = other.note;
    }
    QMap<QString, QStringList> parameters;
    QString note;
};

Note::Note()
    : d(new Private)
{

}

Note::Note(const Note &other)
    : d(other.d)
{

}

Note::Note(const QString &note)
    : d(new Private)
{
    d->note = note;
}

Note::~Note()
{

}

void Note::setNote(const QString &note)
{
    d->note = note;
}

QString Note::note() const
{
    return d->note;
}

bool Note::isValid() const
{
    return !d->note.isEmpty();
}

void Note::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

QMap<QString, QStringList> Note::parameters() const
{
    return d->parameters;
}

bool Note::operator==(const Note &other) const
{
    return (d->parameters == other.parameters()) && (d->note == other.note());
}

bool Note::operator!=(const Note &other) const
{
    return !(other == *this);
}

Note &Note::operator=(const Note &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Note::toString() const
{
    QString str = QLatin1String("Note {\n");
    str += QStringLiteral("    note: %1\n").arg(d->note);
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

QDataStream &KContacts::operator<<(QDataStream &s, const Note &note)
{
    return s << note.d->parameters << note.d->note;
}

QDataStream &KContacts::operator>>(QDataStream &s, Note &note)
{
    s >> note.d->parameters >> note.d->note;
    return s;
}
