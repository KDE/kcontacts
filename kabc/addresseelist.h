/*
    This file is part of libkabc.
    Copyright (c) 2002 Jost Schenck <jost@schenck.de>
                  2003 Tobias Koenig <tokoe@kde.org>

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

#ifndef KABC_ADDRESSEELIST_H
#define KABC_ADDRESSEELIST_H

#include "kabc_export.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QList>

namespace KABC {

class Field;
class SortMode;
class Addressee;

/**
 * Each trait must implement one static function for equality, one for "less
 * than". Class name should be the field name. A trait does not necessarily
 * have to stick to just one field: a trait sorting by family name can e.g.
 * sort addressees with equal family name by given name.
 *
 * If you want to implement reverse sorting, you do not have to write another
 * trait, as AddresseeList takes care of that.
 */
namespace SortingTraits
{

class KABC_EXPORT Uid
{
  public:
    /**
     * Creates an instance.
     */
    Uid();

    /**
     * Destroys the instance.
     */
    ~Uid();

    /**
     * "Equal" compare method
     *
     * @return @c true if the first parameter is equal to the second
     *         when comparing the uid attribute.
     *
     * @see Addressee::uid()
     * @see QString::compare()
     */
    static bool eq( const Addressee &, const Addressee & );

    /**
     * "Less-Than" compare method
     *
     * @return @c true if the first parameter is "less-than" the second
     *         when comparing the uid attribute.
     *
     * @see Addressee::uid()
     * @see QString::compare()
     */
    static bool lt( const Addressee &, const Addressee & );

  private:
    class Private;
    Private *const d;
};

class KABC_EXPORT Name
{
  public:
    /**
     * Creates an instance.
     */
    Name();

    /**
     * Destroys the instance.
     */
    ~Name();

    /**
     * "Equal" compare method
     *
     * @return @c true if the first parameter is equal to the second
     *         when comparing the name attribute.
     *
     * @see Addressee::name()
     * @see QString::localeAwareCompare()
     */
    static bool eq( const Addressee &, const Addressee & );

    /**
     * "Less-Than" compare method
     *
     * @return @c true if the first parameter is "less-than" the second
     *         when comparing the name attribute.
     *
     * @see Addressee::name()
     * @see QString::localeAwareCompare()
     */
    static bool lt( const Addressee &, const Addressee & );

  private:
    class Private;
    Private *const d;
};

class KABC_EXPORT FormattedName
{
  public:
    /**
     * Creates an instance.
     */
    FormattedName();

    /**
     * Destroys the instance.
     */
    ~FormattedName();

    /**
     * "Equal" compare method
     *
     * @return @c true if the first parameter is equal to the second
     *         when comparing the formatted name attribute.
     *
     * @see Addressee::formattedName()
     * @see QString::localeAwareCompare()
     */
    static bool eq( const Addressee &, const Addressee & );

    /**
     * "Less-Than" compare method
     *
     * @return @c true if the first parameter is "less-than" the second
     *         when comparing the formatted name attribute.
     *
     * @see Addressee::formattedName()
     * @see QString::localeAwareCompare()
     */
    static bool lt( const Addressee &, const Addressee & );

  private:
    class Private;
    Private *const d;
};

class KABC_EXPORT FamilyName // fallback to given name
{
  public:
    /**
     * Creates an instance.
     */
    FamilyName();

    /**
     * Destroys the instance.
     */
    ~FamilyName();

    /**
     * "Equal" compare method
     *
     * @return @c true if the first parameter is equal to the second
     *         when comparing the family name and given name attributes.
     *
     * @see Addressee::familyName()
     * @see Addressee::givenName()
     * @see QString::localeAwareCompare()
     */
    static bool eq( const Addressee &, const Addressee & );

    /**
     * "Less-Than" compare method
     *
     * Falls back to comparing given name if equal
     *
     * @return @c true if the first parameter is "less-than" the second
     *         when comparing the family name attribute.
     *
     * @see Addressee::familyName()
     * @see QString::localeAwareCompare()
     */
    static bool lt( const Addressee &, const Addressee & );

  private:
    class Private;
    Private *const d;
};

class KABC_EXPORT GivenName  // fallback to family name
{
  public:
    /**
     * Creates an instance.
     */
    GivenName();

    /**
     * Destroys the instance.
     */
    ~GivenName();

    /**
     * "Equal" compare method
     *
     * @return @c true if the first parameter is equal to the second
     *         when comparing the given name and family name attributes.
     *
     * @see Addressee::givenName()
     * @see Addressee::familyName()
     * @see QString::localeAwareCompare()
     */
    static bool eq( const Addressee &, const Addressee & );

    /**
     * "Less-Than" compare method
     *
     * Falls back to comparing family name if equal
     *
     * @return @c true if the first parameter is "less-than" the second
     *         when comparing the given name attribute.
     *
     * @see Addressee::givenName()
     * @see QString::localeAwareCompare()
     */
    static bool lt( const Addressee &, const Addressee & );

  private:
    class Private;
    Private *const d;
};

}

/**
 * Addressee attribute used for sorting.
 */
typedef enum {
  Uid,
  Name,
  FormattedName,
  FamilyName,
  GivenName
} SortingCriterion;

/**
 * @short  a QValueList of Addressee, with sorting functionality
 *
 * This class extends the functionality of QValueList with
 * sorting methods specific to the Addressee class. It can be used
 * just like any other QValueList but is no template class.
 *
 * An AddresseeList does not automatically keep sorted when addressees
 * are added or removed or the sorting order is changed, as this would
 * slow down larger operations by sorting after every step. So after
 * such operations you have to call {@link #sort} or {@link #sortBy} to
 * create a defined order again.
 *
 * Iterator usage is inherited from QList and extensively documented
 * there. Please remember that the state of an iterator is undefined
 * after any sorting operation.
 *
 * For the enumeration Type SortingCriterion, which specifies the
 * field by the collection will be sorted, the following values exist:
 * Uid, Name, FormattedName, FamilyName, GivenName.
 *
 * @author Jost Schenck jost@schenck.de
 */
class KABC_EXPORT AddresseeList : public QList<Addressee>
{
  public:
    /**
     * Creates a new addressee list.
     */
    AddresseeList();

    /**
     * Creates a new addressee list.
     */
    AddresseeList( const AddresseeList & );

    /**
     * Creates a new addressee list.
     */
    AddresseeList( const QList<Addressee> & );

    /**
     * Destroys the addressee list.
     */
    ~AddresseeList();

    /**
     * Assignment operator.
     *
     * @param other the list to assign from
     * @return a reference to @c this
     */
    AddresseeList &operator=( const AddresseeList &other );

    /**
     * Determines the direction of sorting. On change, the list
     * will <em>not</em> automatically be resorted.
     * @param reverseSorting <tt>true</tt> if sorting should be done reverse,
     *                       <tt>false</tt> otherwise
     */
    void setReverseSorting( bool reverseSorting = true );

    /**
     * Returns the direction of sorting.
     * @return    <tt>true</tt> if sorting is done reverse, <tt>false</tt> otherwise
     */
    bool reverseSorting() const;

    /**
     * Sorts this list by a specific criterion.
     * @param c    the criterion by which should be sorted
     */
    void sortBy( SortingCriterion c );

    /**
     * Sorts this list by a specific field. If no parameter is given, the
     * last used Field object will be used.
     * @param field    pointer to the Field object to be sorted by
     */
    void sortByField( Field *field = 0 );

    /**
     * Sorts this list by a specific sorting mode.
     * @param mode    pointer to the sorting mode object to be sorted by
    */
    void sortByMode( SortMode *mode = 0 );

    /**
     * Sorts this list by its active sorting criterion. This normally is the
     * criterion of the last sortBy operation or <tt>FormattedName</tt> if up
     * to now there has been no sortBy operation.
     *
     * Please note that the sorting trait of the last {@link #sortByTrait}
     * method call is not remembered and thus the action can not be repeated
     * by this method.
     */
    void sort();

    /**
     * Templated sort function. You normally will not want to use this but
     * {@link #sortBy} and {@link #sort} instead as the existing sorting
     * criteria completely suffice for most cases.
     *
     * However, if you do want to use some special sorting criterion, you can
     * write a trait class that will be provided to this templated method.
     * This trait class has to have a class declaration like the following:
     * \code
     * class MySortingTrait {
     *   public:
     *     // eq returns true if a1 and a2 are equal
     *     static bool eq(KABC::Addressee a1, KABC::Addressee a2);
     *     // lt returns true is a1 is "less than" a2
     *     static bool lt(KABC::Addressee a1, KABC::Addressee a2);
     * };
     * \endcode
     * You can then pass this class to the sortByTrait method like this:
     * \code
     * myAddresseelist.sortByTrait&lt;MySortingTrait&gt;();
     * \endcode
     * Please note that the {@link #sort} method can not be used to repeat the
     * sorting of the last <tt>sortByTrait</tt> action.
     *
     * Right now this method uses the bubble sort algorithm. This should be
     * replaced for a better one when I have time.
     */
    template<class Trait> void sortByTrait();

    /**
     * Returns the active sorting criterion, ie the sorting criterion that
     * will be used by a {@link #sort} call.
     */
    SortingCriterion sortingCriterion() const;

    /**
     * Returns the active sorting field, ie a pointer to the Field object
     * which was used for the last {@link #sortByField} operation.
     * This function returns the last GLOBAL sorting field, not
     * the class specific one.
     * You're a lot better off by keeping track of this locally.
     */
    Field *sortingField() const;

    /**
     * Returns a string representation of the addressee list.
     */
    QString toString() const;

  private:
    class Private;
    QSharedDataPointer<Private> d;
};

}

#endif
