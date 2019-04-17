/*
    This file is part of the KContacts framework.
    Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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

#ifndef RESOURCELOCATORURL_H
#define RESOURCELOCATORURL_H

#include "kcontacts_export.h"

#include <QMap>
#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>
#include <QUrl>

namespace KContacts {

/** @short Class that holds a Resource Locator
 *  @since 5.0
 */
class KCONTACTS_EXPORT ResourceLocatorUrl
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const ResourceLocatorUrl &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, ResourceLocatorUrl &);

    Q_GADGET
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(bool isValid READ isValid)

public:

    ResourceLocatorUrl();
    ResourceLocatorUrl(const ResourceLocatorUrl &other);

    ~ResourceLocatorUrl();

    typedef QVector<ResourceLocatorUrl> List;

    Q_REQUIRED_RESULT bool isValid() const;

    void setUrl(const QUrl &url);
    Q_REQUIRED_RESULT QUrl url() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const ResourceLocatorUrl &other) const;
    Q_REQUIRED_RESULT bool operator!=(const ResourceLocatorUrl &other) const;

    ResourceLocatorUrl &operator=(const ResourceLocatorUrl &other);

    Q_REQUIRED_RESULT QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const ResourceLocatorUrl &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, ResourceLocatorUrl &object);
}
Q_DECLARE_METATYPE(KContacts::ResourceLocatorUrl)
Q_DECLARE_TYPEINFO(KContacts::ResourceLocatorUrl, Q_MOVABLE_TYPE);

#endif // RESOURCELOCATORURL_H
