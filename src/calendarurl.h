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

/** @short Class that holds a Calendar Url (FBURL/CALADRURI/CALURI)
 *  @since 4.14.6
 */
class KCONTACTS_EXPORT CalendarUrl
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const CalendarUrl &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, CalendarUrl &);
    friend class VCardTool;
    friend class ::CalendarUrlTest;

public:
    enum CalendarType {
        Unknown = 0,
        FBUrl,
        CALUri,
        CALADRUri,
        EndCalendarType,
    };

    CalendarUrl();
    CalendarUrl(CalendarUrl::CalendarType type);
    CalendarUrl(const CalendarUrl &other);

    ~CalendarUrl();

    typedef QVector<CalendarUrl> List;

    Q_REQUIRED_RESULT bool isValid() const;

    void setType(CalendarUrl::CalendarType type);
    Q_REQUIRED_RESULT CalendarUrl::CalendarType type() const;

    void setUrl(const QUrl &url);
    QUrl url() const;

#if KCONTACTS_ENABLE_DEPRECATED_SINCE(5, 88)
    /**
     * @deprecated Since 5.88 for lack of usage
     */
    KCONTACTS_DEPRECATED_VERSION(5, 88, "For lack of usage.")
    void setParameters(const QMap<QString, QStringList> &params);
#endif

#if KCONTACTS_ENABLE_DEPRECATED_SINCE(5, 88)
    /**
     * @deprecated Since 5.88 for lack of usage
     */
    Q_REQUIRED_RESULT
    KCONTACTS_DEPRECATED_VERSION(5, 88, "For lack of usage.")
    QMap<QString, QStringList> parameters() const;
#endif

    Q_REQUIRED_RESULT bool operator==(const CalendarUrl &other) const;
    Q_REQUIRED_RESULT bool operator!=(const CalendarUrl &other) const;

    CalendarUrl &operator=(const CalendarUrl &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    void setParams(const ParameterMap &params);
    Q_REQUIRED_RESULT ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const CalendarUrl &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, CalendarUrl &object);
}
Q_DECLARE_TYPEINFO(KContacts::CalendarUrl, Q_MOVABLE_TYPE);
#endif // CALENDARURL_H
