/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>
    SPDX-FileCopyrightText: 2013 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_ADDRESSEE_H
#define KCONTACTS_ADDRESSEE_H

#include <QDateTime>
#include <QMap>
#include <QSharedDataPointer>
#include <QStringList>
#include <QUrl>

#include "address.h"
#include "addresseelist.h"
#include "calendarurl.h"
#include "clientpidmap.h"
#include "email.h"
#include "fieldgroup.h"
#include "gender.h"
#include "geo.h"
#include "impp.h"
#include "key.h"
#include "lang.h"
#include "nickname.h"
#include "org.h"
#include "phonenumber.h"
#include "picture.h"
#include "related.h"
#include "resourcelocatorurl.h"
#include "role.h"
#include "secrecy.h"
#include "sound.h"
#include "timezone.h"
#include "title.h"

namespace KContacts
{

/*!
 * \qmlvaluetype addressee
 * \inqmlmodule org.kde.contacts
 * \nativetype KContacts::Addressee
 * \brief Postal address information.
 *
 * \brief Address book entry.
 *
 * This class represents an entry in the address book.
 */

/*!
  \class KContacts::Addressee
  \inheaderfile KContacts/Addressee
  \inmodule KContacts

  \brief Address book entry.

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
  otherwise it constructs the name from the name fields. As fallback, if
  nothing else is set it uses name().

  name() is the NAME type of RFC2426. It can be used as internal name for the
  data entry, but shouldn't be used for displaying the data to the user.
 */
class KCONTACTS_EXPORT Addressee
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Addressee &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Addressee &);

    Q_GADGET

    /*!
     * \qmlproperty bool addressee::isEmpty
     */

    /*!
     * \property KContacts::Addressee::isEmpty
     */
    Q_PROPERTY(bool isEmpty READ isEmpty)

    /*!
     * \qmlproperty string addressee::uid
     */

    /*!
     * \property KContacts::Addressee::uid
     */
    Q_PROPERTY(QString uid READ uid WRITE setUid)

    /*!
     * \qmlproperty string addressee::name
     */

    /*!
     * \property KContacts::Addressee::name
     */
    Q_PROPERTY(QString name READ name WRITE setName)

    /*!
     * \qmlproperty string addressee::formattedName
     */

    /*!
     * \property KContacts::Addressee::formattedName
     */
    Q_PROPERTY(QString formattedName READ formattedName WRITE setFormattedName)

    /*!
     * \qmlproperty string addressee::familyName
     */

    /*!
     * \property KContacts::Addressee::familyName
     */
    Q_PROPERTY(QString familyName READ familyName WRITE setFamilyName)

    /*!
     * \qmlproperty string addressee::givenName
     */

    /*!
     * \property KContacts::Addressee::givenName
     */
    Q_PROPERTY(QString givenName READ givenName WRITE setGivenName)

    /*!
     * \qmlproperty string addressee::additionalName
     */

    /*!
     * \property KContacts::Addressee::additionalName
     */
    Q_PROPERTY(QString additionalName READ additionalName WRITE setAdditionalName)

    /*!
     * \qmlproperty string addressee::prefix
     */

    /*!
     * \property KContacts::Addressee::prefix
     */
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)

    /*!
     * \qmlproperty string addressee::suffix
     */

    /*!
     * \property KContacts::Addressee::suffix
     */
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)

    /*!
     * \qmlproperty string addressee::nickName
     */

    /*!
     * \property KContacts::Addressee::nickName
     */
    Q_PROPERTY(QString nickName READ nickName)

    /*!
     * \qmlproperty date addressee::birthday
     */

    /*!
     * \property KContacts::Addressee::birthday
     */
    Q_PROPERTY(QDateTime birthday READ birthday WRITE setBirthdayProperty) // special write method due to withTime argument

    /*!
     * \qmlproperty string addressee::birthdayHasTime
     */

    /*!
     * \property KContacts::Addressee::birthdayHasTime
     */
    Q_PROPERTY(bool birthdayHasTime READ birthdayHasTime)

    /*!
     * \qmlproperty string addressee::mailer
     */

    /*!
     * \property KContacts::Addressee::mailer
     */
    Q_PROPERTY(QString mailer READ mailer WRITE setMailer)

    /*!
     * \qmlproperty geo addressee::geo
     */

    /*!
     * \property KContacts::Addressee::geo
     */
    Q_PROPERTY(KContacts::Geo geo READ geo WRITE setGeo)

    /*!
     * \qmlproperty string addressee::title
     */

    /*!
     * \property KContacts::Addressee::title
     */
    Q_PROPERTY(QString title READ title)

    /*!
     * \qmlproperty string addressee::role
     */

    /*!
     * \property KContacts::Addressee::role
     */
    Q_PROPERTY(QString role READ role)

    /*!
     * \qmlproperty string addressee::organization
     */

    /*!
     * \property KContacts::Addressee::organization
     */
    Q_PROPERTY(QString organization READ organization)

    /*!
     * \qmlproperty string addressee::department
     */

    /*!
     * \property KContacts::Addressee::department
     */
    Q_PROPERTY(QString department READ department WRITE setDepartment)

    /*!
     * \qmlproperty string addressee::note
     */

    /*!
     * \property KContacts::Addressee::note
     */
    Q_PROPERTY(QString note READ note WRITE setNote)

    /*!
     * \qmlproperty string addressee::productId
     */

    /*!
     * \property KContacts::Addressee::productId
     */
    Q_PROPERTY(QString productId READ productId WRITE setProductId)

    /*!
     * \qmlproperty date addressee::revision
     */

    /*!
     * \property KContacts::Addressee::revision
     */
    Q_PROPERTY(QDateTime revision READ revision)

    /*!
     * \qmlproperty string addressee::sortString
     */

    /*!
     * \property KContacts::Addressee::sortString
     */
    Q_PROPERTY(QString sortString READ sortString WRITE setSortString)

    /*!
     * \qmlproperty string addressee::url
     */

    /*!
     * \property KContacts::Addressee::url
     */
    Q_PROPERTY(KContacts::ResourceLocatorUrl url READ url WRITE setUrl)

    /*!
     * \qmlproperty list<resourceLocatorUrl> addressee::extraUrls
     */

    /*!
     * \property KContacts::Addressee::extraUrls
     */
    Q_PROPERTY(QList<KContacts::ResourceLocatorUrl> extraUrls READ extraUrlList WRITE setExtraUrlList)

    /*!
     * \qmlproperty string addressee::realName
     */

    /*!
     * \property KContacts::Addressee::realName
     */
    Q_PROPERTY(QString realName READ realName)

    /*!
     * \qmlproperty string addressee::assembledName
     */

    /*!
     * \property KContacts::Addressee::assembledName
     */
    Q_PROPERTY(QString assembledName READ assembledName)

    /*!
     * \qmlproperty string addressee::preferredEmail
     */

    /*!
     * \property KContacts::Addressee::preferredEmail
     */
    Q_PROPERTY(QString preferredEmail READ preferredEmail)

    /*!
     * \qmlproperty list<email> addressee::emails
     */

    /*!
     * \property KContacts::Addressee::emails
     */
    Q_PROPERTY(QList<KContacts::Email> emails READ emailList WRITE setEmailList)

    /*!
     * \qmlproperty list<phoneNumber> addressee::phoneNumbers
     */

    /*!
     * \property KContacts::Addressee::phoneNumbers
     */
    Q_PROPERTY(QList<KContacts::PhoneNumber> phoneNumbers READ phoneNumbers WRITE setPhoneNumbers)

    /*!
     * \qmlproperty list<address> addressee::addresses
     */

    /*!
     * \property KContacts::Addressee::addresses
     */
    Q_PROPERTY(QList<KContacts::Address> addresses READ addresses WRITE setAddresses)

    /*!
     * \qmlproperty list<string> addressee::categories
     */

    /*!
     * \property KContacts::Addressee::categories
     */
    Q_PROPERTY(QStringList categories READ categories WRITE setCategories)

    /*!
     * \qmlproperty list<string> addressee::customs
     */

    /*!
     * \property KContacts::Addressee::customs
     */
    Q_PROPERTY(QStringList customs READ customs)

    /*!
     * \qmlproperty bool addressee::changed
     */

    /*!
     * \property KContacts::Addressee::changed
     */
    Q_PROPERTY(bool changed READ changed WRITE setChanged)

    /*!
     * \qmlproperty list<impp> addressee::impps
     */

    /*!
     * \property KContacts::Addressee::impps
     */
    Q_PROPERTY(QList<KContacts::Impp> impps READ imppList WRITE setImppList)

    /*!
     * \qmlproperty date addressee::anniversary
     */

    /*!
     * \property KContacts::Addressee::anniversary
     */
    Q_PROPERTY(QDate anniversary READ anniversary WRITE setAnniversary)

    /*!
     * \qmlproperty string addressee::assistantsName
     */

    /*!
     * \property KContacts::Addressee::assistantsName
     */
    Q_PROPERTY(QString assistantsName READ assistantsName WRITE setAssistantsName)

    /*!
     * \qmlproperty url addressee::blogFeed
     */

    /*!
     * \property KContacts::Addressee::blogFeed
     */
    Q_PROPERTY(QUrl blogFeed READ blogFeed WRITE setBlogFeed)

    /*!
     * \qmlproperty string addressee::managersName
     */

    /*!
     * \property KContacts::Addressee::managersName
     */
    Q_PROPERTY(QString managersName READ managersName WRITE setManagersName)

    /*!
     * \qmlproperty string addressee::office
     */

    /*!
     * \property KContacts::Addressee::office
     */
    Q_PROPERTY(QString office READ office WRITE setOffice)

    /*!
     * \qmlproperty string addressee::profession
     */

    /*!
     * \property KContacts::Addressee::profession
     */
    Q_PROPERTY(QString profession READ profession WRITE setProfession)

    /*!
     * \qmlproperty string addressee::spousesName
     */

    /*!
     * \property KContacts::Addressee::spousesName
     */
    Q_PROPERTY(QString spousesName READ spousesName WRITE setSpousesName)

    /*!
     * \qmlproperty picture addressee::photo
     */

    /*!
     * \property KContacts::Addressee::photo
     */
    Q_PROPERTY(KContacts::Picture photo READ photo WRITE setPhoto)

    // ### the following properties are still missing:
    // - logos, photos, sounds
    // - keys
    // - the list variants for nicks, titles, roles, orgs
    // - timezone, secrecy, gender, kind, members, relationships, language
    // - field groups, sourceUrls, calendarUrls

public:
    /*!
      A list of addressee objects
     */
    typedef AddresseeList List;

    /*!
      Construct an empty address book entry.
     */
    Addressee();

    /*!
      Destroys the address book entry.
     */
    ~Addressee();

    /*!
      Copy constructor.
     */
    Addressee(const Addressee &other);

    /*!
      Assignment operator.

      Returns a reference to \c this
    */
    Addressee &operator=(const Addressee &other);

    /*!
      Equality operator.

      Returns \c true if \c this and the given addressee are equal,
              otherwise \c false
    */
    bool operator==(const Addressee &other) const;

    /*!
      Not-equal operator.

      Returns \c true if \c this and the given addressee are not equal,
              otherwise \c false
    */
    bool operator!=(const Addressee &other) const;

    /*!
      Return if the address book entry is empty.
     */
    [[nodiscard]] bool isEmpty() const;

    /*!
      Set unique identifier.

      \a uid the KABC unique identifier
     */
    void setUid(const QString &uid);

    /*!
      Return unique identifier.
     */
    [[nodiscard]] QString uid() const;

    /*!
      Return translated label for uid field.
     */
    static QString uidLabel();

    /*!
      Set name.
     */
    void setName(const QString &name);

    /*!
      Return name.
     */
    [[nodiscard]] QString name() const;

    /*!
      Return translated label for name field.
     */
    static QString nameLabel();

    /*!
      Set formatted name.
     */
    void setFormattedName(const QString &formattedName);

    /*!
      Return formatted name.
     */
    [[nodiscard]] QString formattedName() const;

    /*!
      Return translated label for formattedName field.
     */
    [[nodiscard]] static QString formattedNameLabel();

    /*!
      Set family name.
     */
    void setFamilyName(const QString &familyName);

    /*!
      Return family name.
     */
    [[nodiscard]] QString familyName() const;

    /*!
      Return translated label for familyName field.
     */
    [[nodiscard]] static QString familyNameLabel();

    /*!
      Set given name.
     */
    void setGivenName(const QString &givenName);

    /*!
      Return given name.
     */
    [[nodiscard]] QString givenName() const;

    /*!
      Return translated label for givenName field.
     */
    [[nodiscard]] static QString givenNameLabel();

    /*!
      Set additional names.
     */
    void setAdditionalName(const QString &additionalName);

    /*!
      Return additional names.
     */
    [[nodiscard]] QString additionalName() const;

    /*!
      Return translated label for additionalName field.
     */
    [[nodiscard]] static QString additionalNameLabel();

    /*!
      Set honorific prefixes.
     */
    void setPrefix(const QString &prefix);

    /*!
      Return honorific prefixes.
     */
    [[nodiscard]] QString prefix() const;

    /*!
      Return translated label for prefix field.
     */
    [[nodiscard]] static QString prefixLabel();

    /*!
      Set honorific suffixes.
     */
    void setSuffix(const QString &suffix);

    /*!
      Return honorific suffixes.
     */
    [[nodiscard]] QString suffix() const;

    /*!
      Return translated label for suffix field.
     */
    [[nodiscard]] static QString suffixLabel();

    /*!
      Set nick name.
     */
    void setNickName(const QString &nickName);

    /*!
     */
    void setNickName(const NickName &nickName);

    /*!
     */
    void insertExtraNickName(const NickName &nickName);

    /*!
     */
    void setExtraNickNameList(const NickName::List &nickNameList);

    /*!
     */
    [[nodiscard]] NickName::List extraNickNameList() const;

    /*!
      Return nick name.
     */
    [[nodiscard]] QString nickName() const;

    /*!
      Return translated label for nickName field.
     */
    [[nodiscard]] static QString nickNameLabel();

    /*!
      Set birthday (date and time). If withTime is false the time will be set
      to midnight and birthdayHasTime() will return false afterwards.
      \since 5.4
     */
    void setBirthday(const QDateTime &birthday, bool withTime = true);

    /*!
      Set birthday (date only). birthdayHasTime() will return false afterwards.
     */
    void setBirthday(const QDate &birthday);

    /*!
      Return birthday. (If a valid date has been set, birthday().time() will
      always return a valid QTime!)
     */
    [[nodiscard]] QDateTime birthday() const;

    /*!
      Returns true if birthday has been set with a time. Returns false otherwise.
     */
    [[nodiscard]] bool birthdayHasTime() const;

    /*!
      Return translated label for birthday field.
     */
    [[nodiscard]] static QString birthdayLabel();

    /*!
      Return translated label for homeAddressStreet field.
     */
    [[nodiscard]] static QString homeAddressStreetLabel();

    /*!
      Return translated label for homeAddressPostOfficeBox field.
     */
    [[nodiscard]] static QString homeAddressPostOfficeBoxLabel();

    /*!
      Return translated label for homeAddressLocality field.
     */
    [[nodiscard]] static QString homeAddressLocalityLabel();

    /*!
      Return translated label for homeAddressRegion field.
     */
    [[nodiscard]] static QString homeAddressRegionLabel();

    /*!
      Return translated label for homeAddressPostalCode field.
     */
    [[nodiscard]] static QString homeAddressPostalCodeLabel();

    /*!
      Return translated label for homeAddressCountry field.
     */
    [[nodiscard]] static QString homeAddressCountryLabel();

    /*!
      Return translated label for homeAddressLabel field.
     */
    [[nodiscard]] static QString homeAddressLabelLabel();

    /*!
      Return translated label for businessAddressStreet field.
     */
    [[nodiscard]] static QString businessAddressStreetLabel();

    /*!
      Return translated label for businessAddressPostOfficeBox field.
     */
    [[nodiscard]] static QString businessAddressPostOfficeBoxLabel();

    /*!
      Return translated label for businessAddressLocality field.
     */
    [[nodiscard]] static QString businessAddressLocalityLabel();

    /*!
      Return translated label for businessAddressRegion field.
     */
    [[nodiscard]] static QString businessAddressRegionLabel();

    /*!
      Return translated label for businessAddressPostalCode field.
     */
    [[nodiscard]] static QString businessAddressPostalCodeLabel();

    /*!
      Return translated label for businessAddressCountry field.
     */
    [[nodiscard]] static QString businessAddressCountryLabel();

    /*!
      Return translated label for businessAddressLabel field.
     */
    [[nodiscard]] static QString businessAddressLabelLabel();

    /*!
      Return translated label for homePhone field.
     */
    [[nodiscard]] static QString homePhoneLabel();

    /*!
      Return translated label for businessPhone field.
     */
    [[nodiscard]] static QString businessPhoneLabel();

    /*!
      Return translated label for mobilePhone field.
     */
    [[nodiscard]] static QString mobilePhoneLabel();

    /*!
      Return translated label for homeFax field.
     */
    [[nodiscard]] static QString homeFaxLabel();

    /*!
      Return translated label for businessFax field.
     */
    [[nodiscard]] static QString businessFaxLabel();

    /*!
      Return translated label for carPhone field.
     */
    [[nodiscard]] static QString carPhoneLabel();

    /*!
      Return translated label for isdn field.
     */
    [[nodiscard]] static QString isdnLabel();

    /*!
      Return translated label for pager field.
     */
    [[nodiscard]] static QString pagerLabel();

    /*!
      Return translated label for email field.
     */
    [[nodiscard]] static QString emailLabel();

    /*!
      Set mail client.
     */
    void setMailer(const QString &mailer);

    /*!
      Return mail client.
     */
    [[nodiscard]] QString mailer() const;

    /*!
      Return translated label for mailer field.
     */
    [[nodiscard]] static QString mailerLabel();

    /*!
      Set time zone.
     */
    void setTimeZone(const TimeZone &timeZone);

    /*!
      Return time zone.
     */
    [[nodiscard]] TimeZone timeZone() const;

    /*!
      Return translated label for timeZone field.
     */
    [[nodiscard]] static QString timeZoneLabel();

    /*!
      Set geographic position.
     */
    void setGeo(const Geo &geo);

    /*!
      Return geographic position.
     */
    [[nodiscard]] Geo geo() const;

    /*!
      Return translated label for geo field.
     */
    [[nodiscard]] static QString geoLabel();

    // Remove in kf6
    /*!
      Set title.
     */
    void setTitle(const QString &title);

    /*!
     */
    void setTitle(const Title &title);

    /*!
     */
    void insertExtraTitle(const Title &title);

    /*!
     */
    void setExtraTitleList(const Title::List &urltitle);

    /*!
     */
    [[nodiscard]] Title::List extraTitleList() const;

    /*!
      Return title.
     */
    [[nodiscard]] QString title() const;

    /*!
      Return translated label for title field.
     */
    [[nodiscard]] static QString titleLabel();

    /*!
      Set role.
     */
    void setRole(const QString &role);

    /*!
     */
    void setRole(const Role &role);

    /*!
     */
    void insertExtraRole(const Role &role);

    /*!
     */
    void setExtraRoleList(const Role::List &roleList);

    /*!
     */
    [[nodiscard]] Role::List extraRoleList() const;

    /*!
      Return role.
     */
    [[nodiscard]] QString role() const;

    /*!
      Return translated label for role field.
     */
    [[nodiscard]] static QString roleLabel();

    // Remove in kf6
    /*!
      Set organization.
     */
    void setOrganization(const QString &organization);

    /*!
     */
    void setOrganization(const Org &organization);

    /*!
     */
    void insertExtraOrganization(const Org &organization);

    /*!
     */
    void setExtraOrganizationList(const Org::List &orgList);

    /*!
     */
    [[nodiscard]] Org::List extraOrganizationList() const;

    /*!
      Return organization.
     */
    [[nodiscard]] QString organization() const;

    /*!
      Return translated label for organization field.
     */
    [[nodiscard]] static QString organizationLabel();

    /*!
      Set department.
     */
    void setDepartment(const QString &department);

    /*!
      Return department.
     */
    [[nodiscard]] QString department() const;

    /*!
      Return translated label for department field.
     */
    [[nodiscard]] static QString departmentLabel();

    /*!
      Set note.
     */
    void setNote(const QString &note);

    /*!
      Return note.
     */
    [[nodiscard]] QString note() const;

    /*!
      Return translated label for note field.
     */
    [[nodiscard]] static QString noteLabel();

    /*!
      Set product identifier.
     */
    void setProductId(const QString &productId);

    /*!
      Return product identifier.
     */
    [[nodiscard]] QString productId() const;

    /*!
      Return translated label for productId field.
     */
    [[nodiscard]] static QString productIdLabel();

    /*!
      Set revision date.
     */
    void setRevision(const QDateTime &revision);

    /*!
      Return revision date.
     */
    [[nodiscard]] QDateTime revision() const;

    /*!
      Return translated label for revision field.
     */
    [[nodiscard]] static QString revisionLabel();

    /*!
      Set sort string.
     */
    void setSortString(const QString &sortString);

    /*!
      Return sort string.
     */
    [[nodiscard]] QString sortString() const;

    /*!
      Return translated label for sortString field.
     */
    [[nodiscard]] static QString sortStringLabel();

    /*!
      Set homepage.
     */
    void setUrl(const ResourceLocatorUrl &url);
    // kf6: remove it
    void setUrl(const QUrl &url);

    /*!
      Return homepage.
     */
    [[nodiscard]] ResourceLocatorUrl url() const;

    /*!
      Return translated label for url field.
     */
    [[nodiscard]] static QString urlLabel();

    /*!
      Set security class.
     */
    void setSecrecy(const Secrecy &secrecy);

    /*!
      Return security class.
     */
    [[nodiscard]] Secrecy secrecy() const;

    /*!
      Return translated label for secrecy field.
     */
    [[nodiscard]] static QString secrecyLabel();

    /*!
      Set logo.
     */
    void setLogo(const Picture &logo);

    /*!
      Return logo.
     */
    [[nodiscard]] Picture logo() const;

    /*!
      Return translated label for logo field.
     */
    [[nodiscard]] static QString logoLabel();

    /*!
      Set photo.
     */
    void setPhoto(const Picture &photo);

    /*!
      Return photo.
     */
    [[nodiscard]] Picture photo() const;

    /*!
      Return translated label for photo field.
     */
    [[nodiscard]] static QString photoLabel();

    /*!
      Set sound.
     */
    void setSound(const Sound &sound);

    /*!
      Return sound.
     */
    [[nodiscard]] Sound sound() const;

    /*!
      Return translated label for sound field.
     */
    [[nodiscard]] static QString soundLabel();

    /*!
      Set name fields by parsing the given string and trying to associate the
      parts of the string with according fields. This function should probably
      be a bit more clever.
     */
    void setNameFromString(const QString &s);

    /*!
      Return the name of the addressee. This is calculated from all the name
      fields.
     */
    [[nodiscard]] QString realName() const;

    /*!
      Return the name that consists of all name parts.
     */
    [[nodiscard]] QString assembledName() const;

    /*!
      Return email address including real name.

      \a email Email address to be used to construct the full email string.
                   If this is QString() the preferred email address is used.
     */
    [[nodiscard]] QString fullEmail(const QString &email = QString()) const;

    /*!
      Adds an email address. If the email address (i.e. email.mail()) already
      exists in this addressee it won't be duplicated, instead  email is assigned
      to it.

      \since 5.88
     */
    void addEmail(const Email &email);

    /*!
      Remove email address. If the email address doesn't exist, nothing happens.

      \a email Email address to remove
     */
    void removeEmail(const QString &email);

    /*!
      Return preferred email address. This is the first email address or the last
      one added with insertEmail() or addEmail() with a set preferred parameter.
     */
    [[nodiscard]] QString preferredEmail() const;

    /*!
      Return list of all email addresses.
     */
    [[nodiscard]] QStringList emails() const;

    /*!
       Set the emails to \a list.
       The first email address gets the preferred one!
     */
    void setEmails(const QStringList &list);

    /*!
      Insert a phone number. If a phone number with the same id already exists
      in this addressee it is not duplicated.

      \a phoneNumber The telephone number to insert to the addressee
     */
    void insertPhoneNumber(const PhoneNumber &phoneNumber);

    /*!
      Remove phone number. If no phone number with the given id exists for this
      addressee, nothing happens.

      \a phoneNumber The telephone number to remove from the addressee
     */
    void removePhoneNumber(const PhoneNumber &phoneNumber);

    /*!
      Return phone number, which matches the given type.

      \a type The type of phone number to get
     */
    [[nodiscard]] PhoneNumber phoneNumber(PhoneNumber::Type type) const;

    /*!
      Return list of all phone numbers.
     */
    [[nodiscard]] PhoneNumber::List phoneNumbers() const;

    void setPhoneNumbers(const PhoneNumber::List &phoneNumbers);

    /*!
      Return list of phone numbers with a special type.

      \a type The type of phone number to get
     */
    [[nodiscard]] PhoneNumber::List phoneNumbers(PhoneNumber::Type type) const;

    /*!
      Return phone number with the given id.

      \a id The identifier of the phone number to look for.
                See PhoneNumber::id()
     */
    [[nodiscard]] PhoneNumber findPhoneNumber(const QString &id) const;

    /*!
      Insert a key. If a key with the same id already exists
      in this addressee it is not duplicated.

      \a key The key to insert
     */
    void insertKey(const Key &key);

    /*!
      Remove a key. If no key with the given id exists for this
      addressee, nothing happens.

      \a key The key to remove
     */
    void removeKey(const Key &key);

    /*!
      Return key, which matches the given type.
      If \a type == Key::Custom you can specify a string
      that should match. If you leave the string empty, the first
      key with a custom value is returned.

      \a type The type of key to look for

      \a customTypeString A string to match custom keys against when
             \a type is \c Key::Custom
     */
    [[nodiscard]] Key key(Key::Type type, const QString &customTypeString = QString()) const;

    /*!
      Return list of all keys.
     */
    [[nodiscard]] Key::List keys() const;

    /*!
       Set the list of keys
       \a keys The keys to be set.
     */
    void setKeys(const Key::List &keys);

    /*!
      Return list of keys with a special type.
      If \a type == Key::Custom you can specify a string
      that should match. If you leave the string empty, all custom
      keys will be returned.

      \a type The type of key to look for

      \a customTypeString A string to match custom keys against when
             \a type is \c Key::Custom
     */
    [[nodiscard]] Key::List keys(Key::Type type, const QString &customTypeString = QString()) const;

    /*!
      Return key with the given id.

      \a id The identifier of the key to look for. See Key::id()
     */
    [[nodiscard]] Key findKey(const QString &id) const;

    /*!
      Insert an address. If an address with the same id already exists
      in this addressee it is not duplicated.

      \a address The address to insert
     */
    void insertAddress(const Address &address);

    /*!
      Remove address. If no address with the given id exists for this
      addressee, nothing happens.

      \a address The address to remove
     */
    void removeAddress(const Address &address);

    /*!
      Set the addressee

      \a addresses The new addresses
      \since 5.100
     */
    void setAddresses(const Address::List &addresses);

    /*!
      Return address, which matches the given type.

      \a type The type of address to look for
     */
    [[nodiscard]] Address address(Address::Type type) const;

    /*!
      Return list of all addresses.
     */
    [[nodiscard]] Address::List addresses() const;

    /*!
      Return list of addresses with a special type.

      \a type The type of addresses to look for
     */
    [[nodiscard]] Address::List addresses(Address::Type type) const;

    /*!
      Return address with the given id.

      \a id The identifier of the address to look for. See Address::id()
     */
    [[nodiscard]] Address findAddress(const QString &id) const;

    /*!
      Insert category. If the category already exists it is not duplicated.
     */
    void insertCategory(const QString &category);

    /*!
      Remove category.
     */
    void removeCategory(const QString &category);

    /*!
      Return, if addressee has the given category.
     */
    [[nodiscard]] bool hasCategory(const QString &category) const;

    /*!
      Set categories to given value.
     */
    void setCategories(const QStringList &category);

    /*!
      Return list of all set categories.
     */
    [[nodiscard]] QStringList categories() const;

    /*!
      Insert custom entry. The entry is identified by the name of the inserting
      application and a unique name. If an entry with the given app and name
      already exists its value is replaced with the new given value.

      An empty value isn't allowed (nothing happens if this is called with
      any of the three arguments being empty)

      \a app Name of the application inserting this custom entry

      \a name Name of this application specific custom entry

      \a value Value of this application specific custom entry
     */
    void insertCustom(const QString &app, const QString &name, const QString &value);

    /*!
      Remove custom entry.

      \a app Name of the application which has inserted this custom entry

      \a name Name of this application specific custom entry
     */
    void removeCustom(const QString &app, const QString &name);

    /*!
      Return value of custom entry, identified by app and entry name.

      \a app Name of the application which has inserted this custom entry

      \a name Name of this application specific custom entry
     */
    [[nodiscard]] QString custom(const QString &app, const QString &name) const;

    /*!
      Set all custom entries.
     */
    void setCustoms(const QStringList &customs);

    /*!
      Return list of all custom entries.

      The format of the custom entries is 'app-key:value' and the list is sorted
      alphabetically by 'app-key'.
     */
    [[nodiscard]] QStringList customs() const;

    /*!
      Parse full email address. The result is given back in fullName and email.

      \a rawEmail The input string to parse for name and email

      \a fullName The name part of the \a rawEmail input, if it contained one

      \a email The email part of the \a rawEmail input, if it contained one
     */
    static void parseEmailAddress(const QString &rawEmail, QString &fullName, QString &email);

    /*!
      Returns string representation of the addressee.
     */
    [[nodiscard]] QString toString() const;

    /*!
      Mark addressee as changed.

      \a value Sets the status indicating changed data
     */
    void setChanged(bool value);

    /*!
      Return whether the addressee is changed.
     */
    [[nodiscard]] bool changed() const;

    /*!
      Returns the MIME type used for Addressees
     */
    [[nodiscard]] static QString mimeType();

    [[nodiscard]] KContacts::Email::List emailList() const;
    void setEmailList(const Email::List &list);

    /*!
     * Remove Language
     */
    void removeLang(const QString &language);
    /*!
     * Insert Language
     */
    void insertLang(const Lang &language);
    /*!
     * Returns List of lang
     */
    [[nodiscard]] Lang::List langs() const;
    /*!
     */
    void setLangs(const Lang::List &langs);

    /*!
     */
    void setGender(const Gender &gender);

    /*!
     */
    [[nodiscard]] Gender gender() const;

    /*!
     */
    [[nodiscard]] QString kind() const;

    /*!
     */
    void setKind(const QString &kind);

    /*!
     */
    void insertCalendarUrl(const CalendarUrl &calendarUrl);

    /*!
     */
    [[nodiscard]] CalendarUrl::List calendarUrlList() const;

    /*!
     */
    void insertExtraSound(const Sound &sound);

    /*!
     */
    void setExtraSoundList(const Sound::List &soundList);

    /*!
     */
    [[nodiscard]] Sound::List extraSoundList() const;

    /*!
     */
    void insertExtraPhoto(const Picture &picture);

    /*!
     */
    void setExtraPhotoList(const Picture::List &pictureList);

    /*!
     */
    [[nodiscard]] Picture::List extraPhotoList() const;

    /*!
     */
    void insertExtraLogo(const Picture &logo);

    /*!
     */
    void setExtraLogoList(const Picture::List &logoList);

    /*!
     */
    [[nodiscard]] Picture::List extraLogoList() const;

    /*!
     */
    [[nodiscard]] ResourceLocatorUrl::List extraUrlList() const;

    /*!
     */
    void setExtraUrlList(const ResourceLocatorUrl::List &urlList);

    /*!
     */
    void insertExtraUrl(const ResourceLocatorUrl &url);

    // Member
    /*!
     */
    void insertMember(const QString &member);

    /*!
     */
    void setMembers(const QStringList &c);

    /*!
     */
    [[nodiscard]] QStringList members() const;

    // Relation
    /*!
     */
    void insertRelationship(const Related &related);

    /*!
     */
    void setRelationships(const Related::List &c);

    /*!
     */
    [[nodiscard]] Related::List relationships() const;

    // Source
    /*!
     */
    void insertSourceUrl(const QUrl &url);

    /*!
     */
    void setSourcesUrlList(const QList<QUrl> &urlList);

    /*!
     */
    [[nodiscard]] QList<QUrl> sourcesUrlList() const;

    // Impp
    /*!
     */
    [[nodiscard]] Impp::List imppList() const;

    /*!
     */
    void setImppList(const Impp::List &imppList);

    /*!
     */
    void insertImpp(const Impp &impp);

    // FieldGroup
    /*!
     */
    [[nodiscard]] FieldGroup::List fieldGroupList() const;

    /*!
     */
    void setFieldGroupList(const FieldGroup::List &fieldGroupList);

    /*!
     */
    void insertFieldGroup(const FieldGroup &fieldGroup);

    // ClientPidMap
    /*!
     */
    [[nodiscard]] ClientPidMap::List clientPidMapList() const;

    /*!
     */
    void setClientPidMapList(const ClientPidMap::List &clientpidmaplist);

    /*!
     */
    void insertClientPidMap(const ClientPidMap &clientpidmap);

    /*!
     * Returns the contact's anniversary date.
     *
     * \note This is a non-standard extension using the \c X-Anniversary field.
     *
     * \since 5.12
     */
    [[nodiscard]] QDate anniversary() const;
    /*!
     * Sets the contact's anniversary date.
     *
     * \note This is a non-standard extension using the \c X-Anniversary field.
     *
     * \since 5.12
     */
    void setAnniversary(const QDate &anniversary);

    /*!
     * Returns the contact's assistant's name.
     *
     * \note This is a non-standard extension using the \c X-AssistantsName field.
     *
     * \since 5.12
     */
    [[nodiscard]] QString assistantsName() const;
    /*!
     * Set the contact's assistant's name.
     *
     * \note This is a non-standard extension using the \c X-AssistantsName field.
     *
     * \since 5.12
     */
    void setAssistantsName(const QString &assistantsName);

    /*!
     * Returns the contact's blog feed.
     *
     * \note This is a non-standard extension using the \c BlogFeed field.
     *
     * \since 5.12
     */
    [[nodiscard]] QUrl blogFeed() const;
    /*!
     * Set the contact's blog feed.
     *
     * \note This is a non-standard extension using the \c BlogFeed field.
     *
     * \since 5.12
     */
    void setBlogFeed(const QUrl &blogFeed);

    /*!
     * Returns the contact's manager's name.
     *
     * \note This is a non-standard extension using the \c X-ManagersName field.
     *
     * \since 5.12
     */
    [[nodiscard]] QString managersName() const;
    /*!
     * Set the contact's manager's name.
     *
     * \note This is a non-standard extension using the \c X-ManagersName field.
     *
     * \since 5.12
     */
    void setManagersName(const QString &managersName);

    /*!
     * Returns the contact's office.
     *
     * \note This is a non-standard extension using the \c X-Office field.
     *
     * \since 5.12
     */
    [[nodiscard]] QString office() const;
    /*!
     * Set the contact's office.
     *
     * \note This is a non-standard extension using the \c X-Office field.
     *
     * \since 5.12
     */
    void setOffice(const QString &office);

    /*!
     * Returns the contact's profession.
     *
     * \note This is a non-standard extension using the \c X-Profession field.
     *
     * \since 5.12
     */
    [[nodiscard]] QString profession() const;
    /*!
     * Set the contact's profession.
     * @note This is a non-standard extension using the \c X-Profession field.
     * \since 5.12
     */
    void setProfession(const QString &profession);

    /*!
     * Returns the contact's spouse's name.
     *
     * \note This is a non-standard extension using the \c X-SpousesName field.
     *
     * \since 5.12
     */
    [[nodiscard]] QString spousesName() const;
    /*!
     * Set the contact's spouse's name.
     *
     * \note This is a non-standard extension using the \c X-SpousesName field.
     *
     * \since 5.12
     */
    void setSpousesName(const QString &spousesName);

private:
    KCONTACTS_NO_EXPORT void setBirthdayProperty(const QDateTime &birthday);

    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Addressee
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Addressee &);

/*!
 * \relates KContacts::Addressee
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Addressee &);
}

#endif
