/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resourcelocatorurl.h"
#include <QDataStream>
#include <QMap>
#include <QStringList>

using namespace KContacts;

class Q_DECL_HIDDEN ResourceLocatorUrl::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        url = other.url;
    }

    QMap<QString, QStringList> parameters;
    QUrl url;
};

ResourceLocatorUrl::ResourceLocatorUrl()
    : d(new Private)
{
}

ResourceLocatorUrl::ResourceLocatorUrl(const ResourceLocatorUrl &other)
    : d(other.d)
{
}

ResourceLocatorUrl::~ResourceLocatorUrl()
{
}

struct url_type_name {
    const char *name;
    ResourceLocatorUrl::Type type;
};

static const url_type_name url_type_names[] = {
    {"HOME", ResourceLocatorUrl::Home},
    {"WORK", ResourceLocatorUrl::Work},
    {"OTHER", ResourceLocatorUrl::Other},
    {"PROFILE", ResourceLocatorUrl::Profile},
};

ResourceLocatorUrl::Type ResourceLocatorUrl::type() const
{
    const auto it = d->parameters.constFind(QLatin1String("type"));
    if (it == d->parameters.end()) {
        return Unknown;
    }

    Type type = Unknown;
    for (const auto &s : it.value()) {
        const auto it = std::find_if(std::begin(url_type_names), std::end(url_type_names), [s](const url_type_name &t) {
            return QLatin1String(t.name) == s;
        });
        if (it != std::end(url_type_names)) {
            type |= (*it).type;
        }
    }
    return type;
}

void ResourceLocatorUrl::setType(ResourceLocatorUrl::Type type)
{
    const auto oldType = this->type();

    auto types = d->parameters.value(QLatin1String("type"));
    for (const auto &t : url_type_names) {
        if (((type ^ oldType) & t.type) == 0) {
            continue; // no change
        }

        if (type & t.type) {
            types.push_back(QLatin1String(t.name));
        } else {
            types.removeAll(QLatin1String(t.name));
        }
    }

    d->parameters.insert(QLatin1String("type"), types);
}

bool ResourceLocatorUrl::isPreferred() const
{
    auto it = d->parameters.constFind(QLatin1String("pref"));
    if (it != d->parameters.end() && !it.value().isEmpty()) {
        return it.value().at(0) == QLatin1Char('1');
    }

    it = d->parameters.constFind(QLatin1String("type"));
    if (it != d->parameters.end()) {
        return it.value().contains(QLatin1String("PREF"), Qt::CaseInsensitive);
    }

    return false;
}

void ResourceLocatorUrl::setPreferred(bool preferred)
{
    if (preferred == isPreferred()) {
        return;
    }

    auto types = d->parameters.value(QLatin1String("type"));
    if (!preferred) {
        d->parameters.remove(QLatin1String("pref"));
        types.removeAll(QLatin1String("PREF"));
    } else {
        types.push_back(QLatin1String("PREF"));
    }
    d->parameters.insert(QLatin1String("type"), types);
}

QMap<QString, QStringList> ResourceLocatorUrl::parameters() const
{
    return d->parameters;
}

bool ResourceLocatorUrl::operator==(const ResourceLocatorUrl &other) const
{
    return (d->parameters == other.parameters()) && (d->url == other.url());
}

bool ResourceLocatorUrl::operator!=(const ResourceLocatorUrl &other) const
{
    return !(other == *this);
}

ResourceLocatorUrl &ResourceLocatorUrl::operator=(const ResourceLocatorUrl &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString ResourceLocatorUrl::toString() const
{
    QString str = QLatin1String("ResourceLocatorUrl {\n");
    str += QStringLiteral("    url: %1\n").arg(d->url.toString());
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

void ResourceLocatorUrl::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

bool ResourceLocatorUrl::isValid() const
{
    return d->url.isValid();
}

void ResourceLocatorUrl::setUrl(const QUrl &url)
{
    d->url = url;
}

QUrl ResourceLocatorUrl::url() const
{
    return d->url;
}

QDataStream &KContacts::operator<<(QDataStream &s, const ResourceLocatorUrl &calUrl)
{
    return s << calUrl.d->parameters << calUrl.d->url;
}

QDataStream &KContacts::operator>>(QDataStream &s, ResourceLocatorUrl &calUrl)
{
    s >> calUrl.d->parameters >> calUrl.d->url;
    return s;
}
