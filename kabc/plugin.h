/*
    This file is part of libkabc.
    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>

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

#ifndef KABC_PLUGIN_H
#define KABC_PLUGIN_H

#include "kabc_export.h"
#include <QtCore/QString>

namespace KABC {

/**
 * Base class for Plugins. Defines the interface for the basic
 * plugin properties.
 *
 * @see FormatInfo
 */
class KABC_EXPORT Plugin
{
  public:
    /**
     * Creates the Plugin instance.
     */
    Plugin();

    /**
     * Destroys the plugin instance
     */
    virtual ~Plugin();

    /**
     * Sets the plugin's type.
     *
     * @param type The type of the plugin
     */
    virtual void setType( const QString &type );

    /**
     * Returns the plugin's type.
     */
    virtual QString type() const;

    /**
     * Sets the plugin's name.
     *
     * @param label The localized string to display as the name
     */
    virtual void setNameLabel( const QString &label );

    /**
     * Returns the plugin's name.
     */
    virtual QString nameLabel() const;

    /**
     * Sets the plugin's description.
     *
     * @param label The localized string to display as a description
     */
    virtual void setDescriptionLabel( const QString &label );

    /**
     * Returns the plugin's description.
     */
    virtual QString descriptionLabel() const;

  private:
    class Private;
    Private *const d;

    Q_DISABLE_COPY( Plugin )
};

}
#endif
