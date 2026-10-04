/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grammaticalgender.h"
#include "parametermap_p.h"

#include <QDataStream>
#include <QStringList>

using namespace KContacts;
using namespace Qt::StringLiterals;

class Q_DECL_HIDDEN GrammaticalGender::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mParamMap = other.mParamMap;
        gender = other.gender;
    }

    ParameterMap mParamMap;
    QString gender;
};

GrammaticalGender::GrammaticalGender()
    : d(new Private)
{
}

GrammaticalGender::GrammaticalGender(const GrammaticalGender &other)
    : d(other.d)
{
}

GrammaticalGender::GrammaticalGender(const QString &gender)
    : d(new Private)
{
    d->gender = gender;
}

GrammaticalGender::~GrammaticalGender() = default;

void GrammaticalGender::setGender(const QString &gender)
{
    d->gender = gender;
}

QString GrammaticalGender::gender() const
{
    return d->gender;
}

QString GrammaticalGender::language() const
{
    const auto it = d->mParamMap.findParam(u"language"_s);
    if (it != d->mParamMap.cend() && !it->paramValues.isEmpty()) {
        return it->paramValues.first();
    }
    return {};
}

void GrammaticalGender::setLanguage(const QString &language)
{
    auto it = d->mParamMap.findParam(u"language"_s);
    if (language.isEmpty()) {
        if (it != d->mParamMap.end()) {
            d->mParamMap.erase(it);
        }
    } else if (it != d->mParamMap.end()) {
        it->paramValues = {language};
    } else {
        d->mParamMap.insertParam({u"language"_s, {language}});
    }
}

bool GrammaticalGender::isValid() const
{
    return !d->gender.isEmpty();
}

void GrammaticalGender::setParams(const ParameterMap &params)
{
    d->mParamMap = params;
}

ParameterMap GrammaticalGender::params() const
{
    return d->mParamMap;
}

bool GrammaticalGender::operator==(const GrammaticalGender &other) const
{
    return (d->mParamMap == other.d->mParamMap) && (d->gender == other.gender());
}

bool GrammaticalGender::operator!=(const GrammaticalGender &other) const
{
    return !(other == *this);
}

GrammaticalGender &GrammaticalGender::operator=(const GrammaticalGender &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString GrammaticalGender::toString() const
{
    QString str = "GrammaticalGender {\n"_L1;
    str += u"    gender: %1\n"_s.arg(d->gender);
    str += d->mParamMap.toString();
    str += "}\n"_L1;
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const GrammaticalGender &object)
{
    return s << object.d->mParamMap << object.d->gender;
}

QDataStream &KContacts::operator>>(QDataStream &s, GrammaticalGender &object)
{
    s >> object.d->mParamMap >> object.d->gender;
    return s;
}

QMap<QString, QStringList> GrammaticalGender::parameters() const
{
    return d->mParamMap.toQMap();
}

void GrammaticalGender::setParameters(const QMap<QString, QStringList> &parameters)
{
    QMap<QString, QStringList> normalized;
    for (auto it = parameters.cbegin(); it != parameters.cend(); ++it) {
        normalized.insert(it.key().toLower(), it.value());
    }
    d->mParamMap = ParameterMap::fromQMap(normalized);
}

#include "moc_grammaticalgender.cpp"
