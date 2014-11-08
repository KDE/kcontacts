/*
    This file is part of the KContacts framework.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>
                  2013 Tobias Koenig <tokoe@kde.org>

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

#ifndef KCONTACTS_ADDRESSEE_H
#define KCONTACTS_ADDRESSEE_H

#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QUrl>
#include <QtCore/QMap>

#include "address.h"
#include "geo.h"
#include "key.h"
#include "phonenumber.h"
#include "picture.h"
#include "secrecy.h"
#include "sound.h"
#include "timezone.h"
#include "addresseelist.h"  // for typedef QList<Addressee> List;

namespace KContacts
{

class SortMode;

/**
  @short address book entry

  This class represents an entry in the address book.

  The data of this class is implicitly shared. You can pass this class by value.

  If you need the name of a field for presenting it to the user you should use
  the functions ending in Label(). They return a translated string which can be
  used as label for the corresponding field.

  About the name fields:

  givenName() is the first name and familyName() the last name. In some
  countries the family name comes first, that's the reason for the
  naming. formattedName() is the full name with the correct formatting.
  It is used as an override, when the correct formatting can't be generated
  from the other name fields automatically.

  realName() returns a fully formatted name(). It uses formattedName, if set,
  otherwise it constucts the name from the name fields. As fallback, if
  nothing else is set it uses name().

  name() is the NAME type of RFC2426. It can be used as internal name for the
  data enty, but shouldn't be used for displaying the data to the user.
 */
class KCONTACTS_EXPORT Addressee
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Addressee &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Addressee &);

public:
    /**
      A list of addressee objects
     */
    typedef AddresseeList List;
    /**
      A map from unique identifier to addressee.

      @see uid()
     */
    typedef QMap<QString, Addressee> Map;

    /**
      Construct an empty address book entry.
     */
    Addressee();

    /**
      Destroys the address book entry.
     */
    ~Addressee();

    /**
      Copy constructor.
     */
    Addressee(const Addressee &);

    /**
      Assignment operator.

      @return a reference to @c this
    */
    Addressee &operator=(const Addressee &);

    /**
      Equality operator.

      @return @c true if @c this and the given addressee are equal,
              otherwise @c false
    */
    bool operator==(const Addressee &) const;

    /**
      Not-equal operator.

      @return @c true if @c this and the given addressee are not equal,
              otherwise @c false
    */
    bool operator!=(const Addressee &) const;

    /**
      Return, if the address book entry is empty.
     */
    bool isEmpty() const;

    /**
      Set unique identifier.

      @param uid the KABC unique identifier
     */
    void setUid(const QString &uid);

    /**
      Return unique identifier.
     */
    QString uid() const;

    /**
      Return translated label for uid field.
     */
    static QString uidLabel();

    /**
      Set name.
     */
    void setName(const QString &name);

    /**
      Return name.
     */
    QString name() const;

    /**
      Return translated label for name field.
     */
    static QString nameLabel();

    /**
      Set formatted name.
     */
    void setFormattedName(const QString &formattedName);

    /**
      Return formatted name.
     */
    QString formattedName() const;

    /**
      Return translated label for formattedName field.
     */
    static QString formattedNameLabel();

    /**
      Set family name.
     */
    void setFamilyName(const QString &familyName);

    /**
      Return family name.
     */
    QString familyName() const;

    /**
      Return translated label for familyName field.
     */
    static QString familyNameLabel();

    /**
      Set given name.
     */
    void setGivenName(const QString &givenName);

    /**
      Return given name.
     */
    QString givenName() const;

    /**
      Return translated label for givenName field.
     */
    static QString givenNameLabel();

    /**
      Set additional names.
     */
    void setAdditionalName(const QString &additionalName);

    /**
      Return additional names.
     */
    QString additionalName() const;

    /**
      Return translated label for additionalName field.
     */
    static QString additionalNameLabel();

    /**
      Set honorific prefixes.
     */
    void setPrefix(const QString &prefix);

    /**
      Return honorific prefixes.
     */
    QString prefix() const;

    /**
      Return translated label for prefix field.
     */
    static QString prefixLabel();

    /**
      Set honorific suffixes.
     */
    void setSuffix(const QString &suffix);

    /**
      Return honorific suffixes.
     */
    QString suffix() const;

    /**
      Return translated label for suffix field.
     */
    static QString suffixLabel();

    /**
      Set nick name.
     */
    void setNickName(const QString &nickName);

    /**
      Return nick name.
     */
    QString nickName() const;

    /**
      Return translated label for nickName field.
     */
    static QString nickNameLabel();

    /**
      Set birthday.
     */
    void setBirthday(const QDateTime &birthday);

    /**
      Return birthday.
     */
    QDateTime birthday() const;

    /**
      Return translated label for birthday field.
     */
    static QString birthdayLabel();

    /**
      Return translated label for homeAddressStreet field.
     */
    static QString homeAddressStreetLabel();

    /**
      Return translated label for homeAddressPostOfficeBox field.
     */
    static QString homeAddressPostOfficeBoxLabel();

    /**
      Return translated label for homeAddressLocality field.
     */
    static QString homeAddressLocalityLabel();

    /**
      Return translated label for homeAddressRegion field.
     */
    static QString homeAddressRegionLabel();

    /**
      Return translated label for homeAddressPostalCode field.
     */
    static QString homeAddressPostalCodeLabel();

    /**
      Return translated label for homeAddressCountry field.
     */
    static QString homeAddressCountryLabel();

    /**
      Return translated label for homeAddressLabel field.
     */
    static QString homeAddressLabelLabel();

    /**
      Return translated label for businessAddressStreet field.
     */
    static QString businessAddressStreetLabel();

    /**
      Return translated label for businessAddressPostOfficeBox field.
     */
    static QString businessAddressPostOfficeBoxLabel();

    /**
      Return translated label for businessAddressLocality field.
     */
    static QString businessAddressLocalityLabel();

    /**
      Return translated label for businessAddressRegion field.
     */
    static QString businessAddressRegionLabel();

    /**
      Return translated label for businessAddressPostalCode field.
     */
    static QString businessAddressPostalCodeLabel();

    /**
      Return translated label for businessAddressCountry field.
     */
    static QString businessAddressCountryLabel();

    /**
      Return translated label for businessAddressLabel field.
     */
    static QString businessAddressLabelLabel();

    /**
      Return translated label for homePhone field.
     */
    static QString homePhoneLabel();

    /**
      Return translated label for businessPhone field.
     */
    static QString businessPhoneLabel();

    /**
      Return translated label for mobilePhone field.
     */
    static QString mobilePhoneLabel();

    /**
      Return translated label for homeFax field.
     */
    static QString homeFaxLabel();

    /**
      Return translated label for businessFax field.
     */
    static QString businessFaxLabel();

    /**
      Return translated label for carPhone field.
     */
    static QString carPhoneLabel();

    /**
      Return translated label for isdn field.
     */
    static QString isdnLabel();

    /**
      Return translated label for pager field.
     */
    static QString pagerLabel();

    /**
      Return translated label for email field.
     */
    static QString emailLabel();

    /**
      Set mail client.
     */
    void setMailer(const QString &mailer);

    /**
      Return mail client.
     */
    QString mailer() const;

    /**
      Return translated label for mailer field.
     */
    static QString mailerLabel();

    /**
      Set time zone.
     */
    void setTimeZone(const TimeZone &timeZone);

    /**
      Return time zone.
     */
    TimeZone timeZone() const;

    /**
      Return translated label for timeZone field.
     */
    static QString timeZoneLabel();

    /**
      Set geographic position.
     */
    void setGeo(const Geo &geo);

    /**
      Return geographic position.
     */
    Geo geo() const;

    /**
      Return translated label for geo field.
     */
    static QString geoLabel();

    /**
      Set title.
     */
    void setTitle(const QString &title);

    /**
      Return title.
     */
    QString title() const;

    /**
      Return translated label for title field.
     */
    static QString titleLabel();

    /**
      Set role.
     */
    void setRole(const QString &role);

    /**
      Return role.
     */
    QString role() const;

    /**
      Return translated label for role field.
     */
    static QString roleLabel();

    /**
      Set organization.
     */
    void setOrganization(const QString &organization);

    /**
      Return organization.
     */
    QString organization() const;

    /**
      Return translated label for organization field.
     */
    static QString organizationLabel();

    /**
      Set department.
     */
    void setDepartment(const QString &department);

    /**
      Return department.
     */
    QString department() const;

    /**
      Return translated label for department field.
     */
    static QString departmentLabel();

    /**
      Set note.
     */
    void setNote(const QString &note);

    /**
      Return note.
     */
    QString note() const;

    /**
      Return translated label for note field.
     */
    static QString noteLabel();

    /**
      Set product identifier.
     */
    void setProductId(const QString &productId);

    /**
      Return product identifier.
     */
    QString productId() const;

    /**
      Return translated label for productId field.
     */
    static QString productIdLabel();

    /**
      Set revision date.
     */
    void setRevision(const QDateTime &revision);

    /**
      Return revision date.
     */
    QDateTime revision() const;

    /**
      Return translated label for revision field.
     */
    static QString revisionLabel();

    /**
      Set sort string.
     */
    void setSortString(const QString &sortString);

    /**
      Return sort string.
     */
    QString sortString() const;

    /**
      Return translated label for sortString field.
     */
    static QString sortStringLabel();

    /**
      Set homepage.
     */
    void setUrl(const QUrl &url);

    /**
      Return homepage.
     */
    QUrl url() const;

    /**
      Return translated label for url field.
     */
    static QString urlLabel();

    /**
      Set security class.
     */
    void setSecrecy(const Secrecy &secrecy);

    /**
      Return security class.
     */
    Secrecy secrecy() const;

    /**
      Return translated label for secrecy field.
     */
    static QString secrecyLabel();

    /**
      Set logo.
     */
    void setLogo(const Picture &logo);

    /**
      Return logo.
     */
    Picture logo() const;

    /**
      Return translated label for logo field.
     */
    static QString logoLabel();

    /**
      Set photo.
     */
    void setPhoto(const Picture &photo);

    /**
      Return photo.
     */
    Picture photo() const;

    /**
      Return translated label for photo field.
     */
    static QString photoLabel();

    /**
      Set sound.
     */
    void setSound(const Sound &sound);

    /**
      Return sound.
     */
    Sound sound() const;

    /**
      Return translated label for sound field.
     */
    static QString soundLabel();

    /**
      Set name fields by parsing the given string and trying to associate the
      parts of the string with according fields. This function should probably
      be a bit more clever.
     */
    void setNameFromString(const QString &);

    /**
      Return the name of the addressee. This is calculated from all the name
      fields.
     */
    QString realName() const;

    /**
      Return the name that consists of all name parts.
     */
    QString assembledName() const;

    /**
      Return email address including real name.

      @param email Email address to be used to construct the full email string.
                   If this is QString() the preferred email address is used.
     */
    QString fullEmail(const QString &email = QString()) const;

    /**
      Insert an email address. If the email address already exists in this
      addressee it is not duplicated.

      @param email Email address
      @param preferred Set to true, if this is the preferred email address of
                       the addressee.
     */
    void insertEmail(const QString &email, bool preferred = false);

    /**
      Remove email address. If the email address doesn't exist, nothing happens.

      @param email Email address to remove
     */
    void removeEmail(const QString &email);

    /**
      Return preferred email address. This is the first email address or the
      last one added with insertEmail() with a set preferred parameter.
     */
    QString preferredEmail() const;

    /**
      Return list of all email addresses.
     */
    QStringList emails() const;

    /**
       Set the emails to @p list.
       The first email address gets the preferred one!
       @param list The list of email addresses.
     */
    void setEmails(const QStringList &list);

    /**
      Insert a phone number. If a phone number with the same id already exists
      in this addressee it is not duplicated.

      @param phoneNumber The telephone number to insert to the addressee
     */
    void insertPhoneNumber(const PhoneNumber &phoneNumber);

    /**
      Remove phone number. If no phone number with the given id exists for this
      addresse nothing happens.

      @param phoneNumber The telephone number to remove from the addressee
     */
    void removePhoneNumber(const PhoneNumber &phoneNumber);

    /**
      Return phone number, which matches the given type.

      @param type The type of phone number to get
     */
    PhoneNumber phoneNumber(PhoneNumber::Type type) const;

    /**
      Return list of all phone numbers.
     */
    PhoneNumber::List phoneNumbers() const;

    /**
      Return list of phone numbers with a special type.

      @param type The type of phone number to get
     */
    PhoneNumber::List phoneNumbers(PhoneNumber::Type type) const;

    /**
      Return phone number with the given id.

      @param id The identifier of the phone number to look for.
                See PhoneNumber::id()
     */
    PhoneNumber findPhoneNumber(const QString &id) const;

    /**
      Insert a key. If a key with the same id already exists
      in this addressee it is not duplicated.

      @param key The key to insert
     */
    void insertKey(const Key &key);

    /**
      Remove a key. If no key with the given id exists for this
      addresse nothing happens.

      @param key The key to remove
     */
    void removeKey(const Key &key);

    /**
      Return key, which matches the given type.
      If @p type == Key::Custom you can specify a string
      that should match. If you leave the string empty, the first
      key with a custom value is returned.

      @param type The type of key to look for
      @param customTypeString A string to match custom keys against when
             @p type is @c Key::Custom
     */
    Key key(Key::Type type, const QString &customTypeString = QString()) const;

    /**
      Return list of all keys.
     */
    Key::List keys() const;

    /**
       Set the list of keys
       @param keys The keys to be set.
     */
    void setKeys(const Key::List &keys);

    /**
      Return list of keys with a special type.
      If @p type == Key::Custom you can specify a string
      that should match. If you leave the string empty, all custom
      keys will be returned.

      @param type The type of key to look for
      @param customTypeString A string to match custom keys against when
             @p type is @c Key::Custom
     */
    Key::List keys(Key::Type type, const QString &customTypeString = QString()) const;

    /**
      Return key with the given id.

      @param id The identifier of the key to look for. See Key::id()
     */
    Key findKey(const QString &id) const;

    /**
      Insert an address. If an address with the same id already exists
      in this addressee it is not duplicated.

      @param address The address to insert
     */
    void insertAddress(const Address &address);

    /**
      Remove address. If no address with the given id exists for this
      addresse nothing happens.

      @param address The address to remove
     */
    void removeAddress(const Address &address);

    /**
      Return address, which matches the given type.

      @param type The type of address to look for
     */
    Address address(Address::Type type) const;

    /**
      Return list of all addresses.
     */
    Address::List addresses() const;

    /**
      Return list of addresses with a special type.

      @param type The type of addresses to look for
     */
    Address::List addresses(Address::Type type) const;

    /**
      Return address with the given id.

      @param id The identifier of the address to look for. See Address::id()
     */
    Address findAddress(const QString &id) const;

    /**
      Insert category. If the category already exists it is not duplicated.
     */
    void insertCategory(const QString &);

    /**
      Remove category.
     */
    void removeCategory(const QString &);

    /**
      Return, if addressee has the given category.
     */
    bool hasCategory(const QString &) const;

    /**
      Set categories to given value.
     */
    void setCategories(const QStringList &);

    /**
      Return list of all set categories.
     */
    QStringList categories() const;

    /**
      Insert custom entry. The entry is identified by the name of the inserting
      application and a unique name. If an entry with the given app and name
      already exists its value is replaced with the new given value.

      An empty value isn't allowed (nothing happens if this is called with
      any of the three arguments being empty)

      @param app Name of the application inserting this custom entry
      @param name Name of this application specific custom entry
      @param value Value of this application specific custom entry
     */
    void insertCustom(const QString &app, const QString &name,
                      const QString &value);

    /**
      Remove custom entry.

      @param app Name of the application which has inserted this custom entry
      @param name Name of this application specific custom entry
     */
    void removeCustom(const QString &app, const QString &name);

    /**
      Return value of custom entry, identified by app and entry name.

      @param app Name of the application which has inserted this custom entry
      @param name Name of this application specific custom entry
     */
    QString custom(const QString &app, const QString &name) const;

    /**
      Set all custom entries.
     */
    void setCustoms(const QStringList &);

    /**
      Return list of all custom entries.

      The format of the custom entries is 'app-key:value' and the list is sorted
      alphabetically by 'app-key'.
     */
    QStringList customs() const;

    /**
      Parse full email address. The result is given back in fullName and email.

      @param rawEmail The input string to parse for name and email
      @param fullName The name part of the @p rawEmail input, if it contained one
      @param email The email part of the @p rawEmail input, if it contained one
     */
    static void parseEmailAddress(const QString &rawEmail, QString &fullName,
                                  QString &email);

    /**
      Returns string representation of the addressee.
     */
    QString toString() const;

    /**
      Mark addressee as changed.

      @param value Sets the status indicating changed data
     */
    void setChanged(bool value);

    /**
      Return whether the addressee is changed.
     */
    bool changed() const;

    /**
      Sets the sort mode implementation.

      Comparing to addressee objects is delegated to an instance
      of a subclass of KContacts::SortMode.

      @param mode The sort mode implementator to use
     */
    static void setSortMode(KContacts::SortMode *mode);

    /**
      Return whether this addressee is "less-than" a given one

      @param addr The addressee object to compare with

      @see setSortMode()
     */
    bool operator< (const Addressee &addr) const;

    /**
      Returns the MIME type used for Addressees
     */
    static QString mimeType();

private:
    class Private;
    QSharedDataPointer<Private> d;
};
#ifdef kabc_EXPORTS
KDE_DUMMY_QHASH_FUNCTION(Addressee)
#endif

KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Addressee &);
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Addressee &);

}

#define KCONTACTS_ADDRESSEE_METATYPE_DEFINED 1
Q_DECLARE_METATYPE(KContacts::Addressee)

#endif
