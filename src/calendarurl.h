/*
    This file is part of the KContacts framework.
    Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#ifndef CALENDARURL_H
#define CALENDARURL_H
#include "kcontacts_export.h"

#include <QSharedDataPointer>
#include <QString>
#include <QMap>
class QUrl;
/** @short Class that holds a Calendar Url (FBURL/CALADRURI/CALURI)
 *  @since 4.14.6
 */

namespace KContacts
{

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
        EndCalendarType
    };

    CalendarUrl();
    CalendarUrl(CalendarUrl::CalendarType type);
    CalendarUrl(const CalendarUrl &other);

    ~CalendarUrl();

    typedef QVector<CalendarUrl> List;

    bool isValid() const;

    void setType(CalendarUrl::CalendarType type);
    CalendarUrl::CalendarType type() const;

    void setUrl(const QUrl &url);
    QUrl url() const;

    void setParameters(const QMap<QString, QStringList> &params);
    QMap<QString, QStringList> parameters() const;

    bool operator==(const CalendarUrl &other) const;
    bool operator!=(const CalendarUrl &other) const;

    CalendarUrl &operator=(const CalendarUrl &other);

    QString toString() const;
private:
    class Private;
    QSharedDataPointer<Private> d;
};

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const CalendarUrl &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, CalendarUrl &object);

}
Q_DECLARE_TYPEINFO(KContacts::CalendarUrl, Q_MOVABLE_TYPE);
#endif // CALENDARURL_H
