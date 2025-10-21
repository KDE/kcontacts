/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_TIMEZONE_H
#define KCONTACTS_TIMEZONE_H

#include "kcontacts_export.h"
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
/*!
 * \class KContacts::TimeZone
 * \inheaderfile KContacts/TimeZone
 * \inmodule KContacts
 *
 * \brief Time zone information.
 *
 * This class stores information about a time zone.
 */
class KCONTACTS_EXPORT TimeZone
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const TimeZone &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, TimeZone &);

public:
    /*!
     * Construct invalid time zone.
     */
    TimeZone();

    /*!
     * Construct time zone.
     *
     * \a offset Offset in minutes relative to UTC.
     */
    TimeZone(int offset);

    TimeZone(const TimeZone &other);

    TimeZone(const QByteArray &name);

    ~TimeZone();

    /*!
     * Set time zone offset relative to UTC.
     *
     * \a offset Offset in minutes.
     */
    void setOffset(int offset);

    /*!
     * Return offset in minutes relative to UTC.
     */
    [[nodiscard]] int offset() const;

    /*!
     * Return, if this time zone object is valid.
     */
    [[nodiscard]] bool isValid() const;

    /*!
     */
    [[nodiscard]] bool operator==(const TimeZone &other) const;

    /*!
     */
    [[nodiscard]] bool operator!=(const TimeZone &other) const;
    TimeZone &operator=(const TimeZone &other);

    /*!
     * Return string representation of time zone offset.
     */
    [[nodiscard]] QString toString() const;

    [[nodiscard]] QByteArray timeZoneName() const;
    void setTimeZoneName(const QByteArray &name);

private:
    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::TimeZone
 *
 * Serializes the \a timeZone object into the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const TimeZone &timeZone);

/*!
 * \relates KContacts::TimeZone
 *
 * Initializes the \a timeZone object from the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, TimeZone &timeZone);
}
Q_DECLARE_TYPEINFO(KContacts::TimeZone, Q_RELOCATABLE_TYPE);

#endif
