/*
    This file is part of the KContacts framework..
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>

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

#ifndef VCARDLINE_H
#define VCARDLINE_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace KContacts
{

class VCardLine
{
public:
    typedef QList<VCardLine> List;
    typedef QMap<QString, QStringList> ParamMap;

    VCardLine();
    VCardLine(const QString &identifier);
    VCardLine(const QString &identifier, const QVariant &value);
    VCardLine(const VCardLine &line);

    ~VCardLine();

    VCardLine &operator=(const VCardLine &line);

    /**
     * Sets the identifier of this line e.g. UID, FN, CLASS
     *
     * @param identifier The VCard identifier of this line
     */
    void setIdentifier(const QString &identifier);

    /**
     * Returns the identifier of this line.
     */
    QString identifier() const;

    /**
     * Sets the value of of this line.
     */
    void setValue(const QVariant &value);

    /**
     * Returns the value of this line.
     */
    QVariant value() const;

    /**
     * Sets the group the line belongs to.
     */
    void setGroup(const QString &group);

    /**
     * Returns the group the line belongs to.
     */
    QString group() const;

    /**
     * Returns whether the line belongs to a group.
     */
    bool hasGroup() const;

    /**
     * Returns all parameters.
     */
    QStringList parameterList() const;

    /**
     * Add a new parameter to the line.
     *
     * @param param Name of the parameter to insert
     * @param value Value of the parameter to insert
     */
    void addParameter(const QString &param, const QString &value);

    /**
     * Returns the values of a special parameter.
     * You can get a list of all parameters with paramList().
     *
     * @param param Name of the parameter to look for
     */
    QStringList parameters(const QString &param) const;

    /**
     * Returns only the first value of a special parameter.
     * You can get a list of all parameters with paramList().
     *
     * @param param Name of the parameter to look for
     */
    QString parameter(const QString &param) const;

private:
    ParamMap mParamMap;
    QString mIdentifier;
    QString mGroup;
    QVariant mValue;

    class VCardLinePrivate;
    VCardLinePrivate *d;
};

}

#endif
