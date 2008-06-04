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

#ifndef KABC_DISTRIBUTIONLIST_H
#define KABC_DISTRIBUTIONLIST_H

#include "addressbook.h"
#include <QtCore/QList>

namespace KABC {

//class DistributionListManager;
class Resource;

/**
  @short Distribution list of email addresses

  This class represents a list of email addresses. Each email address is
  associated with an address book entry. If the address book entry changes, the
  entry in the distribution list is automatically updated.
*/
class KABC_EXPORT DistributionList
{
  public:
    /**
      @short Distribution List Entry

      This class represents an entry of a distribution list. It consists of an
      addressee and an email address. If the email address is null, the
      preferred email address of the addressee is used.
    */
    class KABC_EXPORT Entry
    {
      public:
        /**
          A list of Entry instances
        */
        typedef QList<Entry> List;

        /**
          Creates an empty Entry instance
        */
        Entry();

        /**
          Copy constructor.

          @param other The Entry to copy from
        */
        Entry( const Entry &other );

        /**
          Creates an Entry instance.

          @param addressee The addressee of the list entry.
          @param email The email address. If @c QString() the preferred email
                 of the @p addressee will be used instead
        */
        Entry( const Addressee &addressee, const QString &email );

        /**
          Destroys the Entry instance.
        */
        ~Entry();

        /**
          Assignment operator.

          @param other The Entry to assign to @c this
        */
        Entry &operator=( const Entry &other );

        /**
          Returns the addressee of the list entry.
        */
        Addressee addressee() const;

        /**
          Returns the email address of the list entry.

          @return @c QString() if no specific email address has been set
        */
        QString email() const;

      private:
        class Private;
        Private *const d;
    };

    /**
       Create distribution list object.

      @param resource The resource the list belongs to.
      @param name    Name of this list.
    */
    DistributionList( Resource *resource, const QString &name );

    /**
       Create distribution list object.

      @param resource    The resource the list belongs to.
      @param identifier Identifier of this list.
      @param name       Name of this list.
    */
    DistributionList( Resource *resource, const QString &identifier,
                      const QString &name );

    /**
      Destructor.
    */
    ~DistributionList();

    /**
      Sets the @p identifier of this list which is used as key by resources

      @param identifier A unique identifier of the distribution list
    */
    void setIdentifier( const QString &identifier );

    /**
      Returns the distribution list's identifier
    */
    QString identifier() const;

    /**
      Set name of this list.

      This is a i18n string for display to the user
    */
    void setName( const QString & );

    /**
      Get name of this list.
    */
    QString name() const;

    /**
      Insert an entry into this distribution list. If the entry already exists
      nothing happens.

      @param email Email address to use for comparison with already inserted
                   entries. If the same addressee is already in the list but
                   the @p email is not the same, insert it again, otherwise
                   update the already existing entry
    */
    void insertEntry( const Addressee &, const QString &email=QString() );

    /**
      Remove an entry from this distribution list. If the entry doesn't exist
      nothing happens.

      @param email Email address to use as an additional check, since the
                   same addressee entry can be in the list multiple times
                   but with different emails
    */
    void removeEntry( const Addressee &, const QString &email=QString() );

    /**
      Return list of email addresses, which belong to this distributon list.
      These addresses can be directly used by e.g. a mail client.
    */
    QStringList emails() const;

    /**
      Return list of entries belonging to this distribution list. This function
      is mainly useful for a distribution list editor.
    */
    Entry::List entries() const;

    Resource *resource() const;

  private:
    class Private;
    Private *const d;

    Q_DISABLE_COPY( DistributionList )
};

/**
  Typedef for map from IDs to respective DistribtionList
*/
typedef QMap<QString, DistributionList*> DistributionListMap;

}
#endif
