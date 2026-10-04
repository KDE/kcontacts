/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pronouns.h"
#include "parametermap_p.h"

#include <QDataStream>
#include <QStringList>

using namespace KContacts;
using namespace Qt::StringLiterals;

class Q_DECL_HIDDEN Pronouns::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mParamMap = other.mParamMap;
        pronouns = other.pronouns;
    }

    ParameterMap mParamMap;
    QString pronouns;
};

Pronouns::Pronouns()
    : d(new Private)
{
}

Pronouns::Pronouns(const Pronouns &other)
    : d(other.d)
{
}

Pronouns::Pronouns(const QString &pronouns)
    : d(new Private)
{
    d->pronouns = pronouns;
}

Pronouns::~Pronouns() = default;

void Pronouns::setPronouns(const QString &pronouns)
{
    d->pronouns = pronouns;
}

QString Pronouns::pronouns() const
{
    return d->pronouns;
}

bool Pronouns::isValid() const
{
    return !d->pronouns.isEmpty();
}

void Pronouns::setParams(const ParameterMap &params)
{
    d->mParamMap = params;
}

ParameterMap Pronouns::params() const
{
    return d->mParamMap;
}

bool Pronouns::operator==(const Pronouns &other) const
{
    return (d->mParamMap == other.d->mParamMap) && (d->pronouns == other.pronouns());
}

bool Pronouns::operator!=(const Pronouns &other) const
{
    return !(other == *this);
}

Pronouns &Pronouns::operator=(const Pronouns &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Pronouns::toString() const
{
    QString str = "Pronouns {\n"_L1;
    str += u"    pronouns: %1\n"_s.arg(d->pronouns);
    str += d->mParamMap.toString();
    str += "}\n"_L1;
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Pronouns &object)
{
    return s << object.d->mParamMap << object.d->pronouns;
}

QDataStream &KContacts::operator>>(QDataStream &s, Pronouns &object)
{
    s >> object.d->mParamMap >> object.d->pronouns;
    return s;
}

QMap<QString, QStringList> Pronouns::parameters() const
{
    return d->mParamMap.toQMap();
}

void Pronouns::setParameters(const QMap<QString, QStringList> &parameters)
{
    QMap<QString, QStringList> normalized;
    for (auto it = parameters.cbegin(); it != parameters.cend(); ++it) {
        normalized.insert(it.key().toLower(), it.value());
    }
    d->mParamMap = ParameterMap::fromQMap(normalized);
}

#include "moc_pronouns.cpp"
