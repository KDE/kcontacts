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

#ifndef KABC_ADDRESSBOOK_H
#define KABC_ADDRESSBOOK_H

#include "addressee.h"
#include "field.h"

#include "kresources/manager.h"

#include <QtCore/QObject>
#include <QtCore/QList>

namespace KABC {

class DistributionList;
class ErrorHandler;
class Resource;
class Ticket;

/**
  @short Address Book

  This class provides access to a collection of address book entries.
 */
class KABC_EXPORT AddressBook : public QObject
{
  Q_OBJECT

  friend KABC_EXPORT QDataStream &operator<<( QDataStream &lhs,
                                              const AddressBook &rhs );
  friend KABC_EXPORT QDataStream &operator>>( QDataStream &lhs,
                                              AddressBook &rhs );
  friend class StdAddressBook;

  public:
    class ConstIterator;

    /**
      @short Address Book Iterator

      This class provides an iterator for address book entries.
     */
    class KABC_EXPORT Iterator  //krazy:exclude=dpointer
    {
      friend class AddressBook;
      friend class ConstIterator;
      public:
        /**
          Default constructor
        */
        Iterator();

        /**
          Copy constructor
        */
        Iterator( const Iterator & );
        ~Iterator();

        /**
          Assignment operator. Assignes the given iterator to @c *this.

          @return this iterator, @c *this
        */
        Iterator &operator=( const Iterator & );

        /**
          Constant Dereference operator.
          @note For invalid iterators, the result is undefined.

          @return the @c const Addressee object the iterator points to.
        */
        const Addressee &operator*() const;

        /**
          Dereference operator.
          @note For invalid iterators, the result is undefined.

          @return the Addressee object the iterator points to.
        */
        Addressee &operator*();

        /**
          Arrow Dereference operator, provided for convenience.
          @note For invalid iterators, the result is undefined.

          @return the Addressee object the iterator points to.
        */
        Addressee *operator->();

        /**
          Preincrement operator. Advances the iterator by one.

          @return this iterator, @c *this
        */
        Iterator &operator++();

        /**
          Postincrement operator. Advances the iterator by one.
          @note This function does not copy the iterator object.

          @return this iterator, @c *this
        */
        Iterator &operator++(int);

        /**
          Predecrement operator. Decreases the iterator by one.

          @return this iterator, @c *this
        */
        Iterator &operator--();

        /**
          Postdecrement operator. Decreases the iterator by one.
          @note This function does not copy the iterator object.

          @return this iterator, @c *this
        */
        Iterator &operator--(int);

        /**
          Equality operator. Compares this iterator to @p it

          @param it the iterator to compare this iterator to
          @return @c true if both iterators are equal,
                  @c false otherwise
        */
        bool operator==( const Iterator &it ) const;

        /**
          Inequality operator. Compares this iterator to @p it

          @param it the iterator to compare this iterator to
          @return @c true if the iterators are not equal,
                  @c false otherwise
        */
        bool operator!=( const Iterator &it ) const;

      private:
        struct IteratorData;
        IteratorData *const d;
    };

    /**
      @short Address Book Const Iterator

      This class provides a const iterator for address book entries.
     */
    class KABC_EXPORT ConstIterator //krazy:exclude=dpointer
    {
      friend class AddressBook;
      public:
        /**
         * Default constructor
         */
        ConstIterator();

        /**
          Copy constructor
        */
        ConstIterator( const ConstIterator & );

#ifndef QT_STRICT_ITERATORS
        /**
          Copy constructor. Constructs a ConstIterator from
          an non-@c const Iterator
        */
        ConstIterator( const Iterator & );
#endif

        ~ConstIterator();

        /**
          Assignment operator. Assignes the given iterator to @c *this.

          @return this iterator, @c *this
        */
        ConstIterator &operator=( const ConstIterator & );

        /**
          Constant Dereference operator.
          @note For invalid iterators, the result is undefined.
          @note Unlike in Iterator, there is no non-constant
                dereference operator.

          @return the @c const Addressee object the iterator points to.
        */
        const Addressee &operator*() const;

        /**
          Arrow Dereference operator, provided for convenience.
          @note For invalid iterators, the result is undefined.

          @return the Addressee object the iterator points to.
        */
        const Addressee *operator->() const;

        /**
          Preincrement operator. Advances the iterator by one.

          @return this iterator, @c *this
        */
        ConstIterator &operator++();

        /**
          Postincrement operator. Advances the iterator by one.
          @note This function does not copy the iterator object.

          @return this iterator, @c *this
        */
        ConstIterator &operator++(int);

        /**
          Predecrement operator. Decreases the iterator by one.

          @return this iterator, @c *this
        */
        ConstIterator &operator--();

        /**
          Postdecrement operator. Decreases the iterator by one.
          @note This function does not copy the iterator object.

          @return this iterator, @c *this
        */
        ConstIterator &operator--(int);

        /**
          Equality operator. Compares this iterator to @p it

          @param it the iterator to compare this iterator to
          @return @c true if both iterators are equal,
                  @c false otherwise
        */
        bool operator==( const ConstIterator &it ) const;

        /**
          Inequality operator. Compares this iterator to @p it

          @param it the iterator to compare this iterator to
          @return @c true if the iterators are not equal,
                  @c false otherwise
        */
        bool operator!=( const ConstIterator &it ) const;

      private:
        struct ConstIteratorData;
        ConstIteratorData *const d;
    };

    /**
      Typedef for STL style iterator
     */
    typedef Iterator iterator;

    /**
      Typedef for STL style iterator
     */
    typedef ConstIterator const_iterator;

    /**
      Constructs an address book object.
      You have to add the resources manually before calling load().
     */
    AddressBook();

    /**
      Constructs an address book object.
      The resources are loaded automatically.

      @param config The config file which contains the resource settings.
     */
    AddressBook( const QString &config );

    /**
      Destructor.
     */
    virtual ~AddressBook();

    /**
      Requests a ticket for saving the addressbook. Calling this function locks
      the addressbook for all other processes. You need the returned ticket
      object for calling the save() function.

      @param resource A pointer to the resource which shall be locked. If 0,
                      the default resource is locked.
      @return 0 if the resource is already locked or a valid save ticket
              otherwise.
      @see save()
     */
    Ticket *requestSaveTicket( Resource *resource = 0 );

    /**
      Releases the ticket requested previously with requestSaveTicket().
      Call this function, if you want to release a ticket without saving.

      @param ticket the save ticket acquired with requestSaveTicket()
     */
    void releaseSaveTicket( Ticket *ticket );

    /**
      Loads all addressees synchronously.

      @return Whether the loading was successfully.
     */
    bool load();

    /**
      Loads all addressees asynchronously. This function returns immediately
      and emits the addressBookChanged() signal as soon as the loading has
      finished.

      @return Whether the synchronous part of loading was successfully.
     */
    bool asyncLoad();

    /**
      Saves all addressees of one resource synchronously. If the save is
      successful the ticket is deleted.

      @param ticket The ticket returned by requestSaveTicket().
      @return Whether the saving was successfully.
     */
    bool save( Ticket *ticket );

    /**
      Saves all addressees of one resource asynchronously. If the save is
      successful the ticket is deleted.

      @param ticket The ticket returned by requestSaveTicket().
      @return Whether the synchronous part of saving was successfully.
     */
    bool asyncSave( Ticket *ticket );

    /**
      Returns an iterator pointing to the first addressee of address book.
      This iterator equals end() if the address book is empty.
     */
    ConstIterator begin() const;
    ConstIterator constBegin() const { return begin(); }

    /**
      This is an overloaded member function, provided for convenience. It
      behaves essentially like the above function.
     */
    Iterator begin();

    /**
      Returns an iterator pointing to the last addressee of address book.
      This iterator equals begin() if the address book is empty.
     */
    ConstIterator end() const;
    ConstIterator constEnd() const { return end(); }

    /**
      This is an overloaded member function, provided for convenience. It
      behaves essentially like the above function.
     */
    Iterator end();

    /**
      Removes all addressees from the address book.
     */
    void clear();

    /**
      Insert an addressee into the address book. If an addressee with the same
      unique id already exists, it is replaced by the new one, otherwise it is
      appended.

      @param addr The addressee which shall be insert.
     */
    void insertAddressee( const Addressee &addr );

    /**
      Removes an addressee from the address book.

      @param addr The addressee which shall be removed.
     */
    void removeAddressee( const Addressee &addr );

    /**
      This is an overloaded member function, provided for convenience. It
      behaves essentially like the above function.

      @param it An iterator pointing to the addressee which shall be removed.
     */
    void removeAddressee( const Iterator &it );

    /**
      Returns an iterator pointing to the specified addressee. It will return
      end() if no addressee matched.

      @param addr The addressee you are looking for.
     */
    Iterator find( const Addressee &addr );

    /**
      Returns an iterator pointing to the specified addressee. It will return
      end() if no addressee matched.

      @param addr The addressee you are looking for.
     */
    ConstIterator find( const Addressee &addr ) const;

    /**
      Searches an addressee with the specified unique identifier.

      @param uid The unique identifier you are looking for.
      @return The addressee with the specified unique identifier or an
              empty addressee.
     */
    Addressee findByUid( const QString &uid ) const;

    /**
      Returns a list of all addressees in the address book.
     */
    Addressee::List allAddressees() const;

    /**
      Searches all addressees which match the specified name.

      @param name The name you are looking for.
      @return A list of all matching addressees.
     */
    Addressee::List findByName( const QString &name ) const;

    /**
      Searches all addressees which match the specified email address.

      @param email The email address you are looking for.
      @return A list of all matching addressees.
     */
    Addressee::List findByEmail( const QString &email ) const;

    /**
      Searches all addressees which belongs to the specified category.

      @param category The category you are looking for.
      @return A list of all matching addressees.
     */
    Addressee::List findByCategory( const QString &category ) const;

    /**
      Creates a distribution list with a given @p name storing it in a
      given @p resource

      @note The newly created list will be added to the addressbook
            automatically on creation.

      @param name The localized name of the new distribution list.
      @param resource The resource which should save the list. If @c 0
            (default) the addressbook's standard resource will be used.

      @see standardResource()
     */
    DistributionList *createDistributionList( const QString &name, Resource *resource = 0 );

    /**
      Removes a distribution @p list from its associated resource.

      @param list The list to remove.
     */
    void removeDistributionList( DistributionList *list );

    /**
      Returns a distribution list for the given @p identifier or @c 0

      @param identifier The ID of the list for look for.
     */
    DistributionList *findDistributionListByIdentifier( const QString &identifier );

    /**
      Returns a distribution list with the given @p name or @c 0

      @param name The localized name of the list for look for.
      @param caseSensitivity Whether to do string matching case sensitive or
             case insensitive. Default is @c Qt::CaseSensitive
     */
    DistributionList *findDistributionListByName(
      const QString &name,
      Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive );

    /**
      Returns a list of all distribution lists of all resources of this
      address book
     */
    QList<DistributionList*> allDistributionLists();

    /**
      Returns a list of names of all distribution lists of all resources
      of this address book.

      Convenience function, equal to iterate over the list returned
      by allDistributionLists()
     */
    QStringList allDistributionListNames() const;

    /**
      Returns a string identifying this addressbook. The identifier is
      created by concatenation of the resource identifiers.
     */
    virtual QString identifier() const;

    /**
      Returns a list of all Fields known to the address book which are
      associated with the given field category.
     */
    Field::List fields( int category = Field::All ) const;

    /**
      Add custom field to address book.

      @param label    User visible label of the field.
      @param category Ored list of field categories.
      @param key      Identifier used as key for reading and writing the field.
      @param app      String used as application key for reading and writing
                      the field.
     */
    bool addCustomField( const QString &label, int category = Field::All,
                         const QString &key = QString(),
                         const QString &app = QString() ) const;

    /**
      Adds a resource to the address book.

      @param resource The resource you want to add.
      @return Whether opening the resource was successfully.
     */
    bool addResource( Resource *resource );

    /**
      Removes a resource from the address book.

      @param resource The resource you want to remove.
      @return Whether closing the resource was successfully.
     */
    bool removeResource( Resource *resource );

    /**
      Returns a list of all resources.
     */
    QList<Resource*> resources() const;

    /**
      Sets the @p ErrorHandler, that is used by error() to
      provide GUI independent error messages.

      @param errorHandler The error handler you want to use.
     */
    void setErrorHandler( ErrorHandler *errorHandler );

    /**
      Shows GUI independent error messages.

      @param msg The error message that shall be displayed.
     */
    void error( const QString &msg );

    /**
      Used for debug output. This function prints out the list
      of all addressees to kDebug(5700).
     */
    void dump() const;

    /**
      Emits the signal addressBookLocked() using @c this as the parameter
     */
    void emitAddressBookLocked() { addressBookLocked( this ); }
    /**
      Emits the signal addressBookUnlocked() using @c this as the parameter
     */
    void emitAddressBookUnlocked() { addressBookUnlocked( this ); }
    /**
      Emits the signal addressBookChanged() using @c this as the parameter
     */
    void emitAddressBookChanged() { addressBookChanged( this ); }

    /**
      Returns true when the loading of the addressbook has finished,
      otherwise false.
     */
    bool loadingHasFinished() const;

  Q_SIGNALS:
    /**
      Emitted when one of the resources discovered a change in its backend
      or the asynchronous loading of all resources has finished.
      You should connect to this signal to update the presentation of
      the contact data in your application.

      @param addressBook The address book which emitted this signal.
     */
    void addressBookChanged( AddressBook *addressBook );

    /**
      Emitted when one of the resources has been locked for writing.

      @param addressBook The address book which emitted this signal.
     */
    void addressBookLocked( AddressBook *addressBook );

    /**
      Emitted when one of the resources has been unlocked.
      You should connect to this signal if you want to save your changes
      to a resource which is currently locked, and want to get notified when
      saving is possible again.

      @param addressBook The address book which emitted this signal.
     */
    void addressBookUnlocked( AddressBook *addressBook );

    /**
      Emitted when the asynchronous loading of one resource has finished
      after calling asyncLoad().

      @param resource The resource which emitted this signal.
     */
    void loadingFinished( Resource *resource );

    /**
      Emitted when the asynchronous saving of one resource has finished
      after calling asyncSave().

      @param resource The resource which emitted this signal.
     */
    void savingFinished( Resource *resource );

  protected Q_SLOTS:
    /**
      Handles loading success.

      Resource will be removed from the list of those pending for loading
      and signal loadingFinished() will be emitted.
      It this has been the last one in this list, signal addressBookChanged()
      is emitted as well.

      @param resource The resource which has been saved successfully.
     */
    void resourceLoadingFinished( Resource *resource );

    /**
      Handles saving success.

      Resource will be removed from the list of those pending for saving.

      @param resource The resource which has been saved successfully.
     */
    void resourceSavingFinished( Resource *resource );

    /**
      Handles loading errors.

      Resource will be removed from the list of those pending for loading.
      If this has been the last one in this list, signal addressBookChanged()
      is emitted.

      @param resource The resource which could not be loaded.
      @param errMsg The message describing the error. See error()
     */
    void resourceLoadingError( Resource *resource, const QString &errMsg );

    /**
      Handles saving errors.

      Resource will be removed from the list of those pending for saving.

      @param resource The resource which could not be saved.
      @param errMsg The message describing the error. See error()
     */
    void resourceSavingError( Resource *resource, const QString &errMsg );

  protected:
    /**
      Sets the resource manager's standard resource.

      Convenience method for resourceManager()->setStandardResource()

      @param resource The resource to use as the standard

      @see standardResource()
     */
    void setStandardResource( Resource *resource );

    /**
      Returns the addressbook resource manager's standard resource.

      Convenience method for resourceManager()->standardResource()

      @see setStandardResource()
     */
    Resource *standardResource();

    /**
      Returns the addressbook's resource manager.
     */
    KRES::Manager<Resource> *resourceManager();

  private:
    class Private;
    Private *const d;
};

KABC_EXPORT QDataStream &operator<<( QDataStream &lhs, const AddressBook &rhs );
KABC_EXPORT QDataStream &operator>>( QDataStream &lhs, AddressBook &rhs );

}

#endif
