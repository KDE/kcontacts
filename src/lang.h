/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LANG_H
#define LANG_H

#include "kcontacts_export.h"
#include <QMap>
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
/** @short Class that holds a Language for a contact.
 *  @since 4.14.5
 */
class KCONTACTS_EXPORT Lang
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Lang &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Lang &);

public:
    Lang();
    Lang(const Lang &other);
    Lang(const QString &language);

    ~Lang();

    typedef QVector<Lang> List;

    void setLanguage(const QString &lang);
    Q_REQUIRED_RESULT QString language() const;

    Q_REQUIRED_RESULT bool isValid() const;

    void setParameters(const QMap<QString, QStringList> &params);
    Q_REQUIRED_RESULT QMap<QString, QStringList> parameters() const;

    Q_REQUIRED_RESULT bool operator==(const Lang &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Lang &other) const;

    Lang &operator=(const Lang &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Lang &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Lang &object);
}
Q_DECLARE_TYPEINFO(KContacts::Lang, Q_MOVABLE_TYPE);
#endif // LANG_H
