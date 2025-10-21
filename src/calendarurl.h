/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef CALENDARURL_H
#define CALENDARURL_H
#include "kcontacts_export.h"

#include <QMap>
#include <QSharedDataPointer>
#include <QString>
class QUrl;

class CalendarUrlTest;

namespace KContacts
{
class ParameterMap;

/*!
 * \class KContacts::CalendarUrl
 * \inheaderfile KContacts/CalendarUrl
 * \inmodule KContacts
 *
 * \brief Class that holds a Calendar Url (FBURL/CALADRURI/CALURI).
 *
 * \sa https://datatracker.ietf.org/doc/html/rfc6350#section-6.9
 * \since 4.14.6
 */
class KCONTACTS_EXPORT CalendarUrl
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const CalendarUrl &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, CalendarUrl &);
    friend class VCardTool;
    friend class ::CalendarUrlTest;

public:
    /*!
     * \value Unknown Unknow calendar type
     * \value FBUrl Specify the calendar containing the FreeBusy time information
     * \value CALUri Specify the calendar associated with the contact
     * \value CALADRUri Specify the calendar which should received the sheduling requests
     * \omitvalue EndCalendarType
     */
    enum CalendarType {
        Unknown = 0,
        FBUrl,
        CALUri,
        CALADRUri,
        EndCalendarType,
    };

    /*!
     *
     */
    CalendarUrl();

    /*!
     *
     */
    CalendarUrl(CalendarUrl::CalendarType type);

    CalendarUrl(const CalendarUrl &other);

    ~CalendarUrl();

    /*!
     * \typedef KContacts::CalendarUrl::List
     */
    typedef QList<CalendarUrl> List;

    /*!
     *
     */
    [[nodiscard]] bool isValid() const;

    /*!
     *
     */
    void setType(CalendarUrl::CalendarType type);

    /*!
     *
     */
    [[nodiscard]] CalendarUrl::CalendarType type() const;

    /*!
     *
     */
    void setUrl(const QUrl &url);

    /*!
     *
     */
    [[nodiscard]] QUrl url() const;

    /*!
     *
     */
    [[nodiscard]] bool operator==(const CalendarUrl &other) const;

    /*!
     *
     */
    [[nodiscard]] bool operator!=(const CalendarUrl &other) const;

    CalendarUrl &operator=(const CalendarUrl &other);

    /*!
     *
     */
    [[nodiscard]] QString toString() const;

private:
    // exported for CalendarUrlTest
    void setParams(const ParameterMap &params);
    [[nodiscard]] ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::CalendarUrl
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const CalendarUrl &object);

/*!
 * \relates KContacts::CalendarUrl
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, CalendarUrl &object);
}
Q_DECLARE_TYPEINFO(KContacts::CalendarUrl, Q_RELOCATABLE_TYPE);
#endif // CALENDARURL_H
