/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "lang.h"
#include <QDataStream>
#include <QMap>
#include <QStringList>

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
    QString str = QLatin1String("Lang {\n");
    str += QStringLiteral("    language: %1\n").arg(d->language);
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

QDataStream &KContacts::operator<<(QDataStream &s, const Lang &lang)
{
    return s << lang.d->parameters << lang.d->language;
}

QDataStream &KContacts::operator>>(QDataStream &s, Lang &lang)
{
    s >> lang.d->parameters >> lang.d->language;
    return s;
}
