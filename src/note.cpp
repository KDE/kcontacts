/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "note.h"
#include <QDataStream>
#include <QMap>
#include <QStringList>

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
