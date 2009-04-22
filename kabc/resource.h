/*
    This file is part of libkabc.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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

#ifndef KABC_RESOURCE_H
#define KABC_RESOURCE_H

#include "addressbook.h"
#include "distributionlist.h"
#include "plugin.h"
#include "kresources/resource.h"

namespace KABC {

/**
 * @short Helper class for handling coordinated save of address books.
 *
 * This class is used as helper class for saving address book.
 * @see requestSaveTicket(), save().
 */
class KABC_EXPORT Ticket
{
    friend class Resource;

  public:
    /**
      Destroys the Ticket instance.
    */
    ~Ticket();

    /**
      Returns the resource for which this ticket has been created.

      @see Resource::createTicket()
    */
    Resource *resource();

  private:
    Ticket( Resource *resource );

    class Private;
    Private *const d;
};

/**
 * @internal
 */
class KABC_EXPORT Resource : public KRES::Resource
{
  Q_OBJECT

  public:

    /**
      @short Resource Iterator

      This class provides an iterator for resource entries.
    */
    class ConstIterator;
    class KABC_EXPORT Iterator
    {
      friend class KABC::Resource::ConstIterator;
      friend class KABC::Resource;
      public:
        /**
         * Default constructor
         */
        Iterator();
        /**
         * Copy constructor
         */
        Iterator( const Iterator & );
        virtual ~Iterator();

        /**
         * Assignment operator. Assignes the given iterator to
         * @c *this.
         *
         * @return this iterator, @c *this
         */
        virtual Iterator &operator=( const Iterator & );
        /**
         * Constant Dereference operator.
         * @note For invalid iterators, the result is undefined.
         *
         * @return the @c const Addressee object the iterator points to.
         */
        virtual const Addressee &operator*() const;
        /**
         * Dereference operator.
         * @note For invalid iterators, the result is undefined.
         *
         * @return the Addressee object the iterator points to.
         */
        virtual Addressee &operator*();
        /**
         * Preincrement operator. Advances the iterator by one.
         *
         * @return this iterator, @c *this
         */
        virtual Iterator &operator++();
        /**
         * Postincrement operator. Advances the iterator by one.
         * @note This function does not copy the iterator object.
         *
         * @return this iterator, @c *this
         */
        virtual Iterator &operator++( int );
        /**
         * Predecrement operator. Decreases the iterator by one.
         *
         * @return this iterator, @c *this
         */
        virtual Iterator &operator--();
        /**
         * Postdecrement operator. Decreases the iterator by one.
         * @note This function does not copy the iterator object.
         *
         * @return this iterator, @c *this
         */
        virtual Iterator &operator--( int );
        /**
         * Equality operator. Compares this iterator to @p it
         *
         * @param it the iterator to compare this iterator to
         * @return @c true if both iterators are equal,
         *         @c false otherwise
         */
        virtual bool operator==( const Iterator &it ) const;
        /**
         * Inequality operator. Compares this iterator to @p it
         *
         * @param it the iterator to compare this iterator to
         * @return @c true if the iterators are not equal,
         *         @c false otherwise
         */
        virtual bool operator!=( const Iterator &it ) const;

      private:
        class Private;
        Private *const d;
    };

    /**
      @short Resource Const Iterator

      This class provides a const iterator for resource entries.
    */
    class KABC_EXPORT ConstIterator
    {
      friend class KABC::Resource;

      public:
        /**
         * Default constructor
         */
        ConstIterator();
        /**
         * Copy constructor
         */
        ConstIterator( const ConstIterator & );
#ifndef QT_STRICT_ITERATORS
        /**
         * Copy constructor. Constructs a ConstIterator from
         * an non-@c const Iterator
         */
        ConstIterator( const Iterator & );
#endif
        virtual ~ConstIterator();

        /**
         * Assignment operator. Assignes the given iterator to
         * @c *this.
         *
         * @return this iterator, @c *this
         */
        virtual ConstIterator &operator=( const ConstIterator & );
        /**
         * Constant Dereference operator.
         * @note For invalid iterators, the result is undefined.
         * @note Unlike in Iterator, there is no non-constant
         *       dereference operator.
         *
         * @return the @c const Addressee object the iterator points to.
         */
        virtual const Addressee &operator*() const;
        /**
         * Preincrement operator. Advances the iterator by one.
         *
         * @return this iterator, @c *this
         */
        virtual ConstIterator &operator++();
        /**
         * Postincrement operator. Advances the iterator by one.
         * @note This function does not copy the iterator object.
         *
         * @return this iterator, @c *this
         */
        virtual ConstIterator &operator++( int );
        /**
         * Predecrement operator. Decreases the iterator by one.
         *
         * @return this iterator, @c *this
         */
        virtual ConstIterator &operator--();
        /**
         * Postdecrement operator. Decreases the iterator by one.
         * @note This function does not copy the iterator object.
         *
         * @return this iterator, @c *this
         */
        virtual ConstIterator &operator--( int );
        /**
         * Equality operator. Compares this iterator to @p it
         *
         * @param it the iterator to compare this iterator to
         * @return @c true if both iterators are equal,
         *         @c false otherwise
         */
        virtual bool operator==( const ConstIterator &it ) const;
        /**
         * Inequality operator. Compares this iterator to @p it
         *
         * @param it the iterator to compare this iterator to
         * @return @c true if the iterators are not equal,
         *         @c false otherwise
         */
        virtual bool operator!=( const ConstIterator &it ) const;

      private:
        class Private;
        Private *const d;
    };

    /**
     * Typedef for STL style iterator
     */
    typedef Iterator iterator;

    /**
     * Typedef for STL style iterator
     */
    typedef ConstIterator const_iterator;

    /**
      Default constructor.
     */
    Resource();

    /**
      Constructor.

      @param group The configuration group where the derived classes can
                    read out their settings.
     */
    Resource( const KConfigGroup &group );

    /**
      Destructor.
     */
    virtual ~Resource();

    /**
      Returns an iterator pointing to the first addressee in the resource.
      This iterator equals end() if the resource is empty.
     */
    virtual ConstIterator begin() const;
    ConstIterator constBegin() const { return begin(); }

    /**
      This is an overloaded member function, provided for convenience. It
      behaves essentially like the above function.
     */
    virtual Iterator begin();

    /**
      Returns an iterator pointing to the last addressee in the resource.
      This iterator equals begin() if the resource is empty.
     */
    virtual ConstIterator end() const;
    ConstIterator constEnd() const { return end(); }

    /**
      This is an overloaded member function, provided for convenience. It
      behaves essentially like the above function.
     */
    virtual Iterator end();

    /**
      Returns a pointer to the addressbook.
     */
    AddressBook *addressBook();

    /**
      Writes the resource specific config to file.

      @param group The config section to write into
     */
    virtual void writeConfig( KConfigGroup &group );

    /**
      Request a ticket, you have to pass through save() to
      allow locking. The resource has to create its locks
      in this function.
    */
    virtual Ticket *requestSaveTicket() = 0;

    /**
      Releases the ticket previousely requested with requestSaveTicket().
      The resource has to remove its locks in this function.
      This function is also responsible for deleting the ticket.

      @param ticket the save ticket acquired with requestSaveTicket()
     */
    virtual void releaseSaveTicket( Ticket *ticket ) = 0;

    /**
      Loads all addressees synchronously.

      @returns Whether the loading was successfully.
     */
    virtual bool load() = 0;

    /**
      Loads all addressees asyncronously. You have to make sure that either
      the loadingFinished() or loadingError() signal is emitted from within
      this function.

      The default implementation simply calls the synchronous load.

      @return Whether the synchronous part of loading was successfully.
     */
    virtual bool asyncLoad();

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
      Saves all addressees synchronously.

      @param ticket You have to release the ticket later with
                    releaseSaveTicket() explicitly.
      @return Whether the saving was successfully.
     */
    virtual bool save( Ticket *ticket ) = 0;

    /**
      Saves all addressees asynchronously. You have to make sure that either
      the savingFinished() or savingError() signal is emitted from within
      this function.

      The default implementation simply calls the synchronous save.

      @param ticket You have to release the ticket later with
                    releaseSaveTicket() explicitly.
      @return Whether the saving was successfully.
     */
    virtual bool asyncSave( Ticket *ticket );

    /**
      Searches an addressee with the specified unique identifier.

      @param uid The unique identifier you are looking for.
      @return The addressee with the specified unique identifier or an
              empty addressee.
     */
    virtual Addressee findByUid( const QString &uid );

    /**
      Searches all addressees which match the specified name.

      @param name The name you are looking for.
      @return A list of all matching addressees.
     */
    virtual Addressee::List findByName( const QString &name );

    /**
      Searches all addressees which match the specified email address.

      @param email The email address you are looking for.
      @return A list of all matching addressees.
     */
    virtual Addressee::List findByEmail( const QString &email );

    /**
      Searches all addressees which belongs to the specified category.

      @param category The category you are looking for.
      @return A list of all matching addressees.
     */
    virtual Addressee::List findByCategory( const QString &category );

    /**
      Removes all addressees and distribution lists from the resource.
     */
    virtual void clear();

    /**
      Adds a distribution @p list into this resource.

      @param list The list to insert.
     */
    virtual void insertDistributionList( DistributionList *list );

    /**
      Removes a distribution @p list from this resource.

      @param list The list to remove.
     */
    virtual void removeDistributionList( DistributionList *list );

    /**
      Returns a distribution list for the given @p identifier or @c 0

      @param identifier The ID of the list for look for.
     */
    virtual DistributionList *findDistributionListByIdentifier( const QString &identifier );

    /**
      Returns a distribution list with the given @p name or @c 0

      @param name The localized name of the list for look for.
      @param caseSensitivity Whether to do string matching case sensitive or
             case insensitive. Default is @c Qt::CaseSensitive
     */
    virtual DistributionList *findDistributionListByName(
      const QString &name,
      Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive );

    /**
      Returns a list of all distribution lists of this resource.
     */
    virtual QList<DistributionList*> allDistributionLists();

    /**
      Returns a list of names of all distribution lists of this resource.

      Convenience function, equal to iterate over the list returned
      by allDistributionLists()
     */
    virtual QStringList allDistributionListNames() const;

    /**
      @internal

      Sets the address book of the resource.

      @param addr The address book to use
     */
    void setAddressBook( AddressBook *addr );

  Q_SIGNALS:
    /**
      This signal is emitted when the resource has finished the loading of all
      addressees from the backend to the internal cache.

      @param resource The pointer to the resource which emitted this signal.
     */
    void loadingFinished( Resource *resource );

    /**
      This signal is emitted when an error occurred during loading the
      addressees from the backend to the internal cache.

      @param resource The pointer to the resource which emitted this signal.
      @param msg A translated error message.
     */
    void loadingError( Resource *resource, const QString &msg );

    /**
      This signal is emitted when the resource has finished the saving of all
      addressees from the internal cache to the backend.

      @param resource The pointer to the resource which emitted this signal.
     */
    void savingFinished( Resource *resource );

    /**
      This signal is emitted when an error occurred during saving the
      addressees from the internal cache to the backend.

      @param resource The pointer to the resource which emitted this signal.
      @param msg A translated error message.
     */
    void savingError( Resource *resource, const QString &msg );

  protected:
    /**
      Factory method, just creates and returns a new Ticket for the given resource.

      Needed by subclasses since the constructor of Ticket is private and only
      this base class is a friend, effectively limiting "new Ticket(this)" to
      resource implementations.
    */
    Ticket *createTicket( Resource * );

    /**
      A mapping from KABC UIDs to the respective addressee.
    */
    Addressee::Map mAddrMap;

    /**
      A mapping from unique identifiers to the respective distribution list.
    */
    DistributionListMap mDistListMap;

  private:
    class Private;
    Private *const d;
};

}

#endif
