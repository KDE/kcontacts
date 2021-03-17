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

namespace KContacts
{
/** @short Class that holds a Calendar Url (FBURL/CALADRURI/CALURI)
 *  @since 4.14.6
 */
class KCONTACTS_EXPORT CalendarUrl
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const CalendarUrl &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, CalendarUrl &);

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

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const CalendarUrl &other) const;
    Q_REQUIRED_RESULT bool operator!=(const CalendarUrl &other) const;

    CalendarUrl &operator=(const CalendarUrl &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const CalendarUrl &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, CalendarUrl &object);
}
Q_DECLARE_TYPEINFO(KContacts::CalendarUrl, Q_MOVABLE_TYPE);
#endif // CALENDARURL_H
