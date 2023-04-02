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

class TitleTest;

namespace KContacts
{
class ParameterMap;

/** @short Class that holds a Title for a contact.
 *  @since 5.3
 */
class KCONTACTS_EXPORT Title
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Title &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Title &);
    friend class VCardTool;
    friend class ::TitleTest;

public:
    Title();
    Title(const Title &other);
    Title(const QString &title);

    ~Title();

    typedef QVector<Title> List;

    void setTitle(const QString &title);
    Q_REQUIRED_RESULT QString title() const;

    Q_REQUIRED_RESULT bool isValid() const;

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

    Q_REQUIRED_RESULT bool operator==(const Title &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Title &other) const;

    Title &operator=(const Title &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    // exported for TitleTest
    void setParams(const ParameterMap &params);
    Q_REQUIRED_RESULT ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Title &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Title &object);
}
Q_DECLARE_TYPEINFO(KContacts::Title, Q_MOVABLE_TYPE);
#endif // TITLE_H
