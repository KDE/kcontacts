/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef RELATED_H
#define RELATED_H

#include "kcontacts_export.h"

#include <QMap>
#include <QSharedDataPointer>
#include <QString>

class RelatedTest;

namespace KContacts
{
class ParameterMap;

/** Describes a relationship of an Addressee. */
class KCONTACTS_EXPORT Related
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Related &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Related &);
    friend class VCardTool;
    friend class ::RelatedTest;

public:
    Related();
    Related(const Related &other);
    Related(const QString &related);

    ~Related();

    typedef QVector<Related> List;

    void setRelated(const QString &relatedTo);
    Q_REQUIRED_RESULT QString related() const;

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

    Q_REQUIRED_RESULT bool operator==(const Related &other) const;
    Q_REQUIRED_RESULT bool operator!=(const Related &other) const;

    Related &operator=(const Related &other);

    Q_REQUIRED_RESULT QString toString() const;

private:
    void setParams(const ParameterMap &params);
    Q_REQUIRED_RESULT ParameterMap params() const;

    class Private;
    QSharedDataPointer<Private> d;
};
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Related &object);

KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Related &object);
}
Q_DECLARE_TYPEINFO(KContacts::Related, Q_MOVABLE_TYPE);
#endif // LANG_H
