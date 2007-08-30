/*
    This file is part of kabc.
    Copyright (c) 2004 Tobias Koenig <tokoe@kde.org>

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

#ifndef KABC_RESOURCECACHED_H
#define KABC_RESOURCECACHED_H

#include <kabc/kabc_export.h>
#include <kabc/resource.h>

#include <kresources/idmapper.h>

namespace KABC {

class KABC_EXPORT ResourceCached : public Resource
{
  Q_OBJECT

  public:
    ResourceCached();
    ResourceCached( const KConfigGroup &group );
    ~ResourceCached();

    /**
      Writes the resource specific config to file.

      @param group The config section to write into
     */
    virtual void writeConfig( KConfigGroup &group );

    /**
      Insert an addressee into the resource.

      @param addr The addressee to add
     */
    virtual void insertAddressee( const Addressee &addr );

    /**
      Removes an addressee from resource.

      @param addr The addressee to remove
     */
    virtual void removeAddressee( const Addressee &addr );

    /**
      Loads the change cache from a file.

      @return @c false if the file cannot be opened for reading,
              otherwise @c true

      @see cacheFile()
     */
    bool loadFromCache();

    /**
      Saves the change cache to a file.

      @see cacheFile()
     */
    void saveToCache();

    /**
      Removes all entries from the cache that are not available in @p list

      @param list The list of addressees to keep in the cache if cached
     */
    void cleanUpCache( const KABC::Addressee::List &list );

    /**
      Returns a reference to the id mapper.
     */
    KRES::IdMapper &idMapper();

    /**
      Returns whether there are any changes cached.
     */
    bool hasChanges() const;

    /**
      Clears all cached changes.
     */
    void clearChanges();

    /**
      Clears the change caches for a given addressee

      Removes the identifier addressee from all three "type" caches.

      @param addr The addressee to remove from the caches
     */
    void clearChange( const KABC::Addressee &addr );

    /**
      Clears the change caches for a given @p uid

      Removes the identifier addressee from all three "type" caches.

      @param uid The unique identifier of an addressee to remove from the
                 caches. See KABC::Addressee::uid()
     */
    void clearChange( const QString &uid );

    /**
       Returns a list of the @c "added" addressees
     */
    KABC::Addressee::List addedAddressees() const;

    /**
       Returns a list of the @c "changed" addressees
     */
    KABC::Addressee::List changedAddressees() const;

    /**
       Returns a list of the @c "deleted" addressees
     */
    KABC::Addressee::List deletedAddressees() const;

  protected:
    /**
      Returns the file path of the cache file.

      Cache file contents will be in vCard format, see
      KABC::VCardConverter::parseVCards() and
      KABC::VCardConverter::createVCards()

      @see loadFromCache()
      @see saveFromCache()
     */
    virtual QString cacheFile() const;

    /**
      Determines the file path for saving caches of a given @p type.

      Will be used by loadChangesCache and saveChangesCache().

      @param type The type of change, i.e. @c "added", @c "delete"
                  or @c "changed"
     */
    virtual QString changesCacheFile( const QString &type ) const;

    /**
      Loads change caches (added, deleted, changed addressees) from
      respective cache files.

      Subclasses can override the filename for each type by re-implementing
      changesCacheFile().

      File contents are expected to be in vCard format.
      See KABC::VCardConverter::parseVCards()
     */
    void loadChangesCache();

    /**
      Saves cached changes (added, deleted, changed addressees) to
      respective cache files.

      Subclasses can override the filename for each type by re-implementing
      changesCacheFile().

      File contents will be in vCard format.
      See KABC::VCardConverter::createVCards()
     */
    void saveChangesCache();

    /**
      Sets the identifier of the instances IdMapper.

      @see idMapper()
      @see IdMapper::setIdentifier()
     */
    void setIdMapperIdentifier();

  private:
    class Private;
    Private *const d;
};

}

#endif
