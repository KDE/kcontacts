/*
    This file is part of libkabc.

    Copyright (c) 2004 Bo Thorsen <bo@sonofthor.dk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef RESOURCEABC_H
#define RESOURCEABC_H

#include "kabc_export.h"
#include "resource.h"
#include <QtCore/QMap>

// This is here because it can't go to kdelibs/kabc yet, but ultimately
// it should probably go there (maybe redesigned to have a real object
// for subresources).

namespace KABC {

/**
 * This class is the implementation of subfolder resources for KABC.
 * More methods will be added to give KAddressBook the possibility to
 * handle subresources.
 */

class KABC_EXPORT ResourceABC : public Resource
{
  Q_OBJECT

  public:
    ResourceABC();
    ResourceABC( const KConfigGroup &group );
    virtual ~ResourceABC();

    /**
     * Get the UID to subresource map. This is necessary to implement
     * the search order.
     * The returned map has the UID as key and the resource it's in as
     * the data.
     */
    virtual QMap<QString, QString> uidToResourceMap() const = 0;

    /**
     * If this resource has subresources, return a QStringList of them.
     * In most cases, resources do not have subresources, so this is
     * by default just empty.
     */
    virtual QStringList subresources() const;

    /**
     * Is the given subresource writable?
     */
    virtual bool subresourceWritable( const QString & ) const = 0;

    /**
     * Is this subresource active or not?
     */
    virtual bool subresourceActive( const QString & ) const;

    /**
     * Completion weight for a given subresource
     */
    virtual int subresourceCompletionWeight( const QString &subResource ) const = 0;

    /**
     * Label for a given subresource
     */
    virtual QString subresourceLabel( const QString &subResource ) const = 0;

  public Q_SLOTS:
    /**
     * (De-)activate a subresource.
     */
    virtual void setSubresourceActive( const QString &subResource, bool active ) = 0;

    /**
     * Set completion weight for a given subresource
     */
    virtual void setSubresourceCompletionWeight( const QString &subResource, int weight ) = 0;

  Q_SIGNALS:
    /**
     * This signal is emitted when a subresource is added.
     */
    void signalSubresourceAdded( KABC::ResourceABC *, const QString &type,
                                 const QString &subResource );

    /**
     * This signal is emitted when a subresource is removed.
     */
    void signalSubresourceRemoved( KABC::ResourceABC *, const QString &type,
                                   const QString &subResource );

    /**
     * This signal is emitted when subresource properties have changed.
     */
    void signalSubresourceChanged( KABC::ResourceABC *, const QString &type,
                                   const QString &subResource );
};

}

#endif // RESOURCEABC_H
