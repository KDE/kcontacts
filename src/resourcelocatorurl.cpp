/*
    This file is part of the KContacts framework.
    Copyright (c) 2015-2016 Laurent Montel <montel@kde.org>

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

#include "resourcelocatorurl.h"
#include <QMap>
#include <QUrl>
#include <QDataStream>
#include <qstringlist.h>

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
    QString str;
    str += QLatin1String("ResourceLocatorUrl {\n");
    str += QStringLiteral("    url: %1\n").arg(d->url.toString());
    if (!d->parameters.isEmpty()) {
        QMapIterator<QString, QStringList> i(d->parameters);
        QString param;
        while (i.hasNext()) {
            i.next();
            param += QStringLiteral("%1 %2").arg(i.key(), i.value().join(QLatin1Char(',')));
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
    return (d->url.isValid());
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

