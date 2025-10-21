/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef RESOURCELOCATORURL_H
#define RESOURCELOCATORURL_H

#include "kcontacts_export.h"

#include <QMap>
#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>
#include <QUrl>

class ResourceLocatorUrlTest;

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::ResourceLocatorUrl
 * \inheaderfile KContacts/ResourceLocatorUrl
 * \inmodule KContacts
 *
 * \brief Class that holds a Resource Locator.
 * \since 5.0
 */
class KCONTACTS_EXPORT ResourceLocatorUrl
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const ResourceLocatorUrl &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, ResourceLocatorUrl &);
    friend class VCardTool;
    friend class ::ResourceLocatorUrlTest;

    Q_GADGET

    /*!
     * \property KContacts::ResourceLocatorUrl::url
     */
    Q_PROPERTY(QUrl url READ url WRITE setUrl)

    /*!
     * \property KContacts::ResourceLocatorUrl::isValid
     */
    Q_PROPERTY(bool isValid READ isValid)

    /*!
     * \property KContacts::ResourceLocatorUrl::type
     */
    Q_PROPERTY(Type type READ type WRITE setType)

    /*!
     * \property KContacts::ResourceLocatorUrl::isPreferred
     */
    Q_PROPERTY(bool isPreferred READ isPreferred WRITE setPreferred)

    /*!
     * \property KContacts::Impp::resourceLabel
     */
    Q_PROPERTY(QString resourceLabel READ resourceLabel)

public:
    /*!
     */
    ResourceLocatorUrl();

    ResourceLocatorUrl(const ResourceLocatorUrl &other);

    ~ResourceLocatorUrl();

    /*!
     */
    typedef QList<ResourceLocatorUrl> List;

    /*!
     * URL types.
     * \since 5.12
     *
     * \value Unknown No or unknown URL type is set
     * \value Home Personal website
     * \value Work Work website
     * \value Profile Profile website
     * \value[since 6.0] Ftp Ftp website
     * \value[since 6.0] Reservation Reservation website
     * \value[since 6.0] AppInstallPage Application installation website
     * \value Other Other website
     */
    enum TypeFlag {
        Unknown = 0,
        Home = 1,
        Work = 2,
        Profile = 4,
        Ftp = 8,
        Reservation = 16,
        AppInstallPage = 32,
        Other = 64,
    };

    Q_DECLARE_FLAGS(Type, TypeFlag)
    Q_FLAG(Type)

    /*!
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    void setUrl(const QUrl &url);

    /*!
     */
    [[nodiscard]] QUrl url() const;

    /*!
     * Returns the type of the URL.
     * \since 5.12
     */
    Type type() const;

    /*!
     * Sets the URL type.
     * \since 5.12
     */
    void setType(Type type);

    /*!
     * Returns whether this is the preferred website.
     * \since 5.12
     */
    bool isPreferred() const;

    /*!
     * Sets that this is the preferred website.
     * \since 5.12
     */
    void setPreferred(bool preferred);

    /*!
     */
    [[nodiscard]] bool operator==(const ResourceLocatorUrl &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const ResourceLocatorUrl &other) const;

    ResourceLocatorUrl &operator=(const ResourceLocatorUrl &other);

    /*!
     */
    [[nodiscard]] QString toString() const;

    /*!
     * Returns a translated label for the type.
     * \since 6.20
     */
    [[nodiscard]] QString resourceLabel() const;

private:
    // exported for ResourceLocatorUrlTest
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ResourceLocatorUrl::Type)

/*!
 * \relates KContacts::ResourceLocatorUrl
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const ResourceLocatorUrl &object);

/*!
 * \relates KContacts::ResourceLocatorUrl
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, ResourceLocatorUrl &object);
}

Q_DECLARE_TYPEINFO(KContacts::ResourceLocatorUrl, Q_RELOCATABLE_TYPE);

#endif // RESOURCELOCATORURL_H
