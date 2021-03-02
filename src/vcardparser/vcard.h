/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2003 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef VCARDPARSER_VCARD_H
#define VCARDPARSER_VCARD_H

#include <QMap>
#include <QStringList>
#include <QVector>
#include "vcardline.h"

namespace KContacts {
class VCard
{
public:
    typedef QVector<VCard> List;
    typedef QMap<QString, VCardLine::List> LineMap;

    enum Version {
        v2_1,
        v3_0,
        v4_0,
    };

    VCard();
    VCard(const VCard &card);

    ~VCard();

    VCard &operator=(const VCard &card);

    /**
     * Removes all lines from the vCard.
     */
    void clear();

    /**
     * Returns a list of all identifiers that exists in the vCard.
     */
    Q_REQUIRED_RESULT QStringList identifiers() const;

    /**
     * Adds a VCardLine to the VCard
     */
    void addLine(const VCardLine &line);

    /**
     * Returns all lines of the vcard with a special identifier.
     */
    Q_REQUIRED_RESULT VCardLine::List lines(const QString &identifier) const;

    /**
     * Returns only the first line of the vcard with a special identifier.
     */
    Q_REQUIRED_RESULT VCardLine line(const QString &identifier) const;

    /**
     * Set the version of the vCard.
     */
    void setVersion(Version version);

    /**
     * Returns the version of this vCard.
     */
    Q_REQUIRED_RESULT Version version() const;

private:
    LineMap mLineMap;
};
}

#endif
