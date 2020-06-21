/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "related.h"
#include <QMap>
#include <QStringList>
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
    QString str = QLatin1String("Related {\n");
    str += QStringLiteral("    relatedTo: %1\n").arg(d->relatedTo);
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

QDataStream &KContacts::operator<<(QDataStream &s, const Related &related)
{
    return s << related.d->parameters << related.d->relatedTo;
}

QDataStream &KContacts::operator>>(QDataStream &s, Related &related)
{
    s >> related.d->parameters >> related.d->relatedTo;
    return s;
}
