/*
    This file is part of libkabc.
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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef VCARDLINE_H
#define VCARDLINE_H

#include <qstring.h>
#include <qstringlist.h>
#include <qvariant.h>

class VCardLine
{
  public:
    typedef QValueList<VCardLine> List;

    VCardLine();
    VCardLine( const QString &identifier, const QString &value = QString::null );

    ~VCardLine();

    /**
     * Sets the identifier of this line e.g. UID, FN, CLASS
     */
    void setIdentifier( const QString& identifier );

    /**
     * Returns the identifier of this line.
     */
    QString identifier() const;

    /**
     * Sets the value of of this line.
     */
    void setValue( const QString& value );

    /**
     * Returns the value of this line.
     */
    QString value() const;

    /**
     * Returns all parameters.
     */
    QStringList parameterList() const;

    /**
     * Add a new parameter to the line.
     */
    void addParameter( const QString& param, const QString& value );

    /**
     * Returns the values of a special parameter.
     * You can get a list of all parameters with @ref paramList().
     */
    QStringList parameters( const QString& param ) const;

    /**
     * Returns only the first value of a special parameter.
     * You can get a list of all parameters with @ref paramList().
     */
    QString parameter( const QString& param ) const;

  private:
    QMap< QString, QValueList<QString> > mParamMap;
    QString mIdentifier;
    QString mValue;
};

#endif
