/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "socialprofile.h"
#include "parametermap_p.h"

#include <QDataStream>
#include <QStringList>

using namespace KContacts;
using namespace Qt::StringLiterals;

class Q_DECL_HIDDEN SocialProfile::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mParamMap = other.mParamMap;
        profile = other.profile;
    }

    ParameterMap mParamMap;
    QString profile;
};

SocialProfile::SocialProfile()
    : d(new Private)
{
}

SocialProfile::SocialProfile(const SocialProfile &other)
    : d(other.d)
{
}

SocialProfile::SocialProfile(const QString &profile)
    : d(new Private)
{
    d->profile = profile;
}

SocialProfile::~SocialProfile() = default;

void SocialProfile::setProfile(const QString &profile)
{
    d->profile = profile;
}

QString SocialProfile::profile() const
{
    return d->profile;
}

bool SocialProfile::isValid() const
{
    return !d->profile.isEmpty();
}

void SocialProfile::setParams(const ParameterMap &params)
{
    d->mParamMap = params;
}

ParameterMap SocialProfile::params() const
{
    return d->mParamMap;
}

bool SocialProfile::operator==(const SocialProfile &other) const
{
    return (d->mParamMap == other.d->mParamMap) && (d->profile == other.profile());
}

bool SocialProfile::operator!=(const SocialProfile &other) const
{
    return !(other == *this);
}

SocialProfile &SocialProfile::operator=(const SocialProfile &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString SocialProfile::toString() const
{
    QString str = "SocialProfile {\n"_L1;
    str += u"    profile: %1\n"_s.arg(d->profile);
    str += d->mParamMap.toString();
    str += "}\n"_L1;
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const SocialProfile &object)
{
    return s << object.d->mParamMap << object.d->profile;
}

QDataStream &KContacts::operator>>(QDataStream &s, SocialProfile &object)
{
    s >> object.d->mParamMap >> object.d->profile;
    return s;
}

QMap<QString, QStringList> SocialProfile::parameters() const
{
    return d->mParamMap.toQMap();
}

void SocialProfile::setParameters(const QMap<QString, QStringList> &parameters)
{
    QMap<QString, QStringList> normalized;
    for (auto it = parameters.cbegin(); it != parameters.cend(); ++it) {
        normalized.insert(it.key().toLower(), it.value());
    }
    d->mParamMap = ParameterMap::fromQMap(normalized);
}

#include "moc_socialprofile.cpp"
