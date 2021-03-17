/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2016-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TITLE_H
#define TITLE_H

#include "kcontacts_export.h"
#include <QMap>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
/** @short Class that holds a Title for a contact.
 *  @since 5.3
 */
class KCONTACTS_EXPORT Title
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Title &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Title &);

public:
    Title();
    Title(const Title &other);
    Title(const QString &title);

    ~Title();

    typedef QVector<Title> List;

    void setTitle(const QString &title);
    Q_REQUIRED_RESULT QString title() const;

    Q_REQUIRED_RESULT bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Title &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Title &other) const;

    Title &operator=(const Title &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Title &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Title &object);
}
Q_DECLARE_TYPEINFO(KContacts::Title, Q_MOVABLE_TYPE);
#endif // TITLE_H
