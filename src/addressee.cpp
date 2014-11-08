/*
    This file is part of the KContacts framework.
    Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2003 Carsten Pfeiffer <pfeiffer@kde.org>
    Copyright (c) 2005 Ingo Kloecker <kloecker@kde.org>

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

#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QSharedData>
#include <QtCore/QUuid>

#include "kabc_debug.h"
#include <QDebug>
#include <klocalizedstring.h>

#include "addresseehelper.h"
#include "field.h"
#include "sortmode.h"

#include "addressee.h"

using namespace KContacts;

static bool matchBinaryPattern(int value, int pattern);

template <class L>
static bool listEquals(const QList<L> &, const QList<L> &);
static bool listEquals(const QStringList &, const QStringList &);
static bool emailsEquals(const QStringList &, const QStringList &);

class Addressee::Private : public QSharedData
{
public:
    Private()
        : mUid(QUuid::createUuid().toString().mid(1, 36)),   //We avoid the curly braces so the string is RFC4122 compliant and can be used as urn
          mEmpty(true), mChanged(false)
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mUid = other.mUid;
        mName = other.mName;
        mFormattedName = other.mFormattedName;
        mFamilyName = other.mFamilyName;
        mGivenName = other.mGivenName;
        mAdditionalName = other.mAdditionalName;
        mPrefix = other.mPrefix;
        mSuffix = other.mSuffix;
        mNickName = other.mNickName;
        mBirthday = other.mBirthday;
        mMailer = other.mMailer;
        mTimeZone = other.mTimeZone;
        mGeo = other.mGeo;
        mTitle = other.mTitle;
        mRole = other.mRole;
        mOrganization = other.mOrganization;
        mDepartment = other.mDepartment;
        mNote = other.mNote;
        mProductId = other.mProductId;
        mRevision = other.mRevision;
        mSortString = other.mSortString;
        mUrl = other.mUrl;
        mSecrecy = other.mSecrecy;
        mLogo = other.mLogo;
        mPhoto = other.mPhoto;
        mSound = other.mSound;

        mPhoneNumbers = other.mPhoneNumbers;
        mAddresses = other.mAddresses;
        mKeys = other.mKeys;
        mEmails = other.mEmails;
        mCategories = other.mCategories;
        mCustomFields = other.mCustomFields;
        mEmpty = other.mEmpty;
        mChanged = other.mChanged;
    }

    ~Private()
    {
    }

    QString mUid;
    QString mName;
    QString mFormattedName;
    QString mFamilyName;
    QString mGivenName;
    QString mAdditionalName;
    QString mPrefix;
    QString mSuffix;
    QString mNickName;
    QDateTime mBirthday;
    QString mMailer;
    TimeZone mTimeZone;
    Geo mGeo;
    QString mTitle;
    QString mRole;
    QString mOrganization;
    QString mDepartment;
    QString mNote;
    QString mProductId;
    QDateTime mRevision;
    QString mSortString;
    QUrl mUrl;
    Secrecy mSecrecy;
    Picture mLogo;
    Picture mPhoto;
    Sound mSound;

    PhoneNumber::List mPhoneNumbers;
    Address::List mAddresses;
    Key::List mKeys;
    QStringList mEmails;
    QStringList mCategories;
    QMap<QString, QString> mCustomFields;

    bool mEmpty    : 1;
    bool mChanged  : 1;

    static KContacts::SortMode *mSortMode;
};

KContacts::SortMode *Addressee::Private::mSortMode = 0;

Addressee::Addressee()
    : d(new Private)
{
}

Addressee::~Addressee()
{
}

Addressee::Addressee(const Addressee &other)
    : d(other.d)
{
}

Addressee &Addressee::operator=(const Addressee &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

bool Addressee::operator==(const Addressee &addressee) const
{
    if (d->mUid != addressee.d->mUid) {
        qDebug() << "uid differs";
        return false;
    }

    if (d->mName != addressee.d->mName &&
            !(d->mName.isEmpty() && addressee.d->mName.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "name differs";
        return false;
    }

    if (d->mFormattedName != addressee.d->mFormattedName &&
            !(d->mFormattedName.isEmpty() && addressee.d->mFormattedName.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "formattedName differs";
        return false;
    }

    if (d->mFamilyName != addressee.d->mFamilyName &&
            !(d->mFamilyName.isEmpty() && addressee.d->mFamilyName.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "familyName differs";
        return false;
    }

    if (d->mGivenName != addressee.d->mGivenName &&
            !(d->mGivenName.isEmpty() && addressee.d->mGivenName.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "givenName differs";
        return false;
    }

    if (d->mAdditionalName != addressee.d->mAdditionalName &&
            !(d->mAdditionalName.isEmpty() && addressee.d->mAdditionalName.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "additionalName differs";
        return false;
    }

    if (d->mPrefix != addressee.d->mPrefix &&
            !(d->mPrefix.isEmpty() && addressee.d->mPrefix.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "prefix differs";
        return false;
    }

    if (d->mSuffix != addressee.d->mSuffix &&
            !(d->mSuffix.isEmpty() && addressee.d->mSuffix.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "suffix differs";
        return false;
    }

    if (d->mNickName != addressee.d->mNickName &&
            !(d->mNickName.isEmpty() && addressee.d->mNickName.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "nickName differs";
        return false;
    }

    if (d->mBirthday != addressee.d->mBirthday) {
        qCDebug(KCONTACTS_LOG) << "birthday differs";
        return false;
    }

    if (d->mMailer != addressee.d->mMailer &&
            !(d->mMailer.isEmpty() && addressee.d->mMailer.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "mailer differs";
        return false;
    }

    if (d->mTimeZone != addressee.d->mTimeZone) {
        qCDebug(KCONTACTS_LOG) << "timeZone differs";
        return false;
    }

    if (d->mGeo != addressee.d->mGeo) {
        qCDebug(KCONTACTS_LOG) << "geo differs";
        return false;
    }

    if (d->mTitle != addressee.d->mTitle &&
            !(d->mTitle.isEmpty() && addressee.d->mTitle.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "title differs";
        return false;
    }

    if (d->mRole != addressee.d->mRole &&
            !(d->mRole.isEmpty() && addressee.d->mRole.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "role differs";
        return false;
    }

    if (d->mOrganization != addressee.d->mOrganization &&
            !(d->mOrganization.isEmpty() && addressee.d->mOrganization.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "organization differs";
        return false;
    }

    if (d->mDepartment != addressee.d->mDepartment &&
            !(d->mDepartment.isEmpty() && addressee.d->mDepartment.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "department differs";
        return false;
    }

    if (d->mNote != addressee.d->mNote &&
            !(d->mNote.isEmpty() && addressee.d->mNote.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "note differs";
        return false;
    }

    if (d->mProductId != addressee.d->mProductId &&
            !(d->mProductId.isEmpty() && addressee.d->mProductId.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "productId differs";
        return false;
    }

    if (d->mSortString != addressee.d->mSortString &&
            !(d->mSortString.isEmpty() && addressee.d->mSortString.isEmpty())) {
        qCDebug(KCONTACTS_LOG) << "sortString differs";
        return false;
    }

    if (d->mSecrecy != addressee.d->mSecrecy) {
        qCDebug(KCONTACTS_LOG) << "secrecy differs";
        return false;
    }

    if (d->mLogo != addressee.d->mLogo) {
        qCDebug(KCONTACTS_LOG) << "logo differs";
        return false;
    }

    if (d->mPhoto != addressee.d->mPhoto) {
        qCDebug(KCONTACTS_LOG) << "photo differs";
        return false;
    }

    if (d->mSound != addressee.d->mSound) {
        qCDebug(KCONTACTS_LOG) << "sound differs";
        return false;
    }

    if ((d->mUrl.isValid() || addressee.d->mUrl.isValid()) &&
            (d->mUrl != addressee.d->mUrl)) {
        qDebug() << "url differs";
        return false;
    }

    if (!listEquals(d->mPhoneNumbers, addressee.d->mPhoneNumbers)) {
        qDebug() << "phoneNumbers differs";
        return false;
    }

    if (!listEquals(d->mAddresses, addressee.d->mAddresses)) {
        qDebug() << "addresses differs";
        return false;
    }

    if (!listEquals(d->mKeys, addressee.d->mKeys)) {
        qDebug() << "keys differs";
        return false;
    }

    if (!emailsEquals(d->mEmails, addressee.d->mEmails)) {
        qDebug() << "emails differs";
        return false;
    }

    if (!listEquals(d->mCategories, addressee.d->mCategories)) {
        qDebug() << "categories differs";
        return false;
    }

    if (d->mCustomFields != addressee.d->mCustomFields) {
        qDebug() << "custom differs";
        return false;
    }

    return true;
}

bool Addressee::operator!=(const Addressee &a) const
{
    return !(a == *this);
}

bool Addressee::isEmpty() const
{
    return d->mEmpty;
}

void Addressee::setUid(const QString &id)
{
    if (id == d->mUid) {
        return;
    }

    d->mEmpty = false;
    d->mUid = id;
}

QString Addressee::uid() const
{
    return d->mUid;
}

QString Addressee::uidLabel()
{
    return i18n("Unique Identifier");
}

void Addressee::setName(const QString &name)
{
    if (name == d->mName) {
        return;
    }

    d->mEmpty = false;
    d->mName = name;
}

QString Addressee::name() const
{
    return d->mName;
}

QString Addressee::nameLabel()
{
    return i18n("Name");
}

void Addressee::setFormattedName(const QString &formattedName)
{
    if (formattedName == d->mFormattedName) {
        return;
    }

    d->mEmpty = false;
    d->mFormattedName = formattedName;
}

QString Addressee::formattedName() const
{
    return d->mFormattedName;
}

QString Addressee::formattedNameLabel()
{
    return i18n("Formatted Name");
}

void Addressee::setFamilyName(const QString &familyName)
{
    if (familyName == d->mFamilyName) {
        return;
    }

    d->mEmpty = false;
    d->mFamilyName = familyName;
}

QString Addressee::familyName() const
{
    return d->mFamilyName;
}

QString Addressee::familyNameLabel()
{
    return i18n("Family Name");
}

void Addressee::setGivenName(const QString &givenName)
{
    if (givenName == d->mGivenName) {
        return;
    }

    d->mEmpty = false;
    d->mGivenName = givenName;
}

QString Addressee::givenName() const
{
    return d->mGivenName;
}

QString Addressee::givenNameLabel()
{
    return i18n("Given Name");
}

void Addressee::setAdditionalName(const QString &additionalName)
{
    if (additionalName == d->mAdditionalName) {
        return;
    }

    d->mEmpty = false;
    d->mAdditionalName = additionalName;
}

QString Addressee::additionalName() const
{
    return d->mAdditionalName;
}

QString Addressee::additionalNameLabel()
{
    return i18n("Additional Names");
}

void Addressee::setPrefix(const QString &prefix)
{
    if (prefix == d->mPrefix) {
        return;
    }

    d->mEmpty = false;
    d->mPrefix = prefix;
}

QString Addressee::prefix() const
{
    return d->mPrefix;
}

QString Addressee::prefixLabel()
{
    return i18n("Honorific Prefixes");
}

void Addressee::setSuffix(const QString &suffix)
{
    if (suffix == d->mSuffix) {
        return;
    }

    d->mEmpty = false;
    d->mSuffix = suffix;
}

QString Addressee::suffix() const
{
    return d->mSuffix;
}

QString Addressee::suffixLabel()
{
    return i18n("Honorific Suffixes");
}

void Addressee::setNickName(const QString &nickName)
{
    if (nickName == d->mNickName) {
        return;
    }

    d->mEmpty = false;
    d->mNickName = nickName;
}

QString Addressee::nickName() const
{
    return d->mNickName;
}

QString Addressee::nickNameLabel()
{
    return i18n("Nick Name");
}

void Addressee::setBirthday(const QDateTime &birthday)
{
    if (birthday == d->mBirthday) {
        return;
    }

    d->mEmpty = false;
    d->mBirthday = birthday;
}

QDateTime Addressee::birthday() const
{
    return d->mBirthday;
}

QString Addressee::birthdayLabel()
{
    return i18n("Birthday");
}

QString Addressee::homeAddressStreetLabel()
{
    return i18n("Home Address Street");
}

QString Addressee::homeAddressPostOfficeBoxLabel()
{
    return i18n("Home Address Post Office Box");
}

QString Addressee::homeAddressLocalityLabel()
{
    return i18n("Home Address City");
}

QString Addressee::homeAddressRegionLabel()
{
    return i18n("Home Address State");
}

QString Addressee::homeAddressPostalCodeLabel()
{
    return i18n("Home Address Zip Code");
}

QString Addressee::homeAddressCountryLabel()
{
    return i18n("Home Address Country");
}

QString Addressee::homeAddressLabelLabel()
{
    return i18n("Home Address Label");
}

QString Addressee::businessAddressStreetLabel()
{
    return i18n("Business Address Street");
}

QString Addressee::businessAddressPostOfficeBoxLabel()
{
    return i18n("Business Address Post Office Box");
}

QString Addressee::businessAddressLocalityLabel()
{
    return i18n("Business Address City");
}

QString Addressee::businessAddressRegionLabel()
{
    return i18n("Business Address State");
}

QString Addressee::businessAddressPostalCodeLabel()
{
    return i18n("Business Address Zip Code");
}

QString Addressee::businessAddressCountryLabel()
{
    return i18n("Business Address Country");
}

QString Addressee::businessAddressLabelLabel()
{
    return i18n("Business Address Label");
}

QString Addressee::homePhoneLabel()
{
    return i18n("Home Phone");
}

QString Addressee::businessPhoneLabel()
{
    return i18n("Business Phone");
}

QString Addressee::mobilePhoneLabel()
{
    return i18n("Mobile Phone");
}

QString Addressee::homeFaxLabel()
{
    return i18n("Home Fax");
}

QString Addressee::businessFaxLabel()
{
    return i18n("Business Fax");
}

QString Addressee::carPhoneLabel()
{
    return i18n("Car Phone");
}

QString Addressee::isdnLabel()
{
    return i18n("ISDN");
}

QString Addressee::pagerLabel()
{
    return i18n("Pager");
}

QString Addressee::emailLabel()
{
    return i18n("Email Address");
}

void Addressee::setMailer(const QString &mailer)
{
    if (mailer == d->mMailer) {
        return;
    }

    d->mEmpty = false;
    d->mMailer = mailer;
}

QString Addressee::mailer() const
{
    return d->mMailer;
}

QString Addressee::mailerLabel()
{
    return i18n("Mail Client");
}

void Addressee::setTimeZone(const TimeZone &timeZone)
{
    if (timeZone == d->mTimeZone) {
        return;
    }

    d->mEmpty = false;
    d->mTimeZone = timeZone;
}

TimeZone Addressee::timeZone() const
{
    return d->mTimeZone;
}

QString Addressee::timeZoneLabel()
{
    return i18n("Time Zone");
}

void Addressee::setGeo(const Geo &geo)
{
    if (geo == d->mGeo) {
        return;
    }

    d->mEmpty = false;
    d->mGeo = geo;
}

Geo Addressee::geo() const
{
    return d->mGeo;
}

QString Addressee::geoLabel()
{
    return i18n("Geographic Position");
}

void Addressee::setTitle(const QString &title)
{
    if (title == d->mTitle) {
        return;
    }

    d->mEmpty = false;
    d->mTitle = title;
}

QString Addressee::title() const
{
    return d->mTitle;
}

QString Addressee::titleLabel()
{
    return i18nc("a person's title", "Title");
}

void Addressee::setRole(const QString &role)
{
    if (role == d->mRole) {
        return;
    }

    d->mEmpty = false;
    d->mRole = role;
}

QString Addressee::role() const
{
    return d->mRole;
}

QString Addressee::roleLabel()
{
    return i18nc("of a person in an organization", "Role");
}

void Addressee::setOrganization(const QString &organization)
{
    if (organization == d->mOrganization) {
        return;
    }

    d->mEmpty = false;
    d->mOrganization = organization;
}

QString Addressee::organization() const
{
    return d->mOrganization;
}

QString Addressee::organizationLabel()
{
    return i18n("Organization");
}

void Addressee::setDepartment(const QString &department)
{
    if (department == d->mDepartment) {
        return;
    }

    d->mEmpty = false;
    d->mDepartment = department;
}

QString Addressee::department() const
{
    return d->mDepartment;
}

QString Addressee::departmentLabel()
{
    return i18n("Department");
}

void Addressee::setNote(const QString &note)
{
    if (note == d->mNote) {
        return;
    }

    d->mEmpty = false;
    d->mNote = note;
}

QString Addressee::note() const
{
    return d->mNote;
}

QString Addressee::noteLabel()
{
    return i18n("Note");
}

void Addressee::setProductId(const QString &productId)
{
    if (productId == d->mProductId) {
        return;
    }

    d->mEmpty = false;
    d->mProductId = productId;
}

QString Addressee::productId() const
{
    return d->mProductId;
}

QString Addressee::productIdLabel()
{
    return i18n("Product Identifier");
}

void Addressee::setRevision(const QDateTime &revision)
{
    if (revision == d->mRevision) {
        return;
    }

    d->mEmpty = false;
    d->mRevision = revision;
}

QDateTime Addressee::revision() const
{
    return d->mRevision;
}

QString Addressee::revisionLabel()
{
    return i18n("Revision Date");
}

void Addressee::setSortString(const QString &sortString)
{
    if (sortString == d->mSortString) {
        return;
    }

    d->mEmpty = false;
    d->mSortString = sortString;
}

QString Addressee::sortString() const
{
    return d->mSortString;
}

QString Addressee::sortStringLabel()
{
    return i18n("Sort String");
}

void Addressee::setUrl(const QUrl &url)
{
    if (url == d->mUrl) {
        return;
    }

    d->mEmpty = false;
    d->mUrl = url;
}

QUrl Addressee::url() const
{
    return d->mUrl;
}

QString Addressee::urlLabel()
{
    return i18n("Homepage");
}

void Addressee::setSecrecy(const Secrecy &secrecy)
{
    if (secrecy == d->mSecrecy) {
        return;
    }

    d->mEmpty = false;
    d->mSecrecy = secrecy;
}

Secrecy Addressee::secrecy() const
{
    return d->mSecrecy;
}

QString Addressee::secrecyLabel()
{
    return i18n("Security Class");
}

void Addressee::setLogo(const Picture &logo)
{
    if (logo == d->mLogo) {
        return;
    }

    d->mEmpty = false;
    d->mLogo = logo;
}

Picture Addressee::logo() const
{
    return d->mLogo;
}

QString Addressee::logoLabel()
{
    return i18n("Logo");
}

void Addressee::setPhoto(const Picture &photo)
{
    if (photo == d->mPhoto) {
        return;
    }

    d->mEmpty = false;
    d->mPhoto = photo;
}

Picture Addressee::photo() const
{
    return d->mPhoto;
}

QString Addressee::photoLabel()
{
    return i18n("Photo");
}

void Addressee::setSound(const Sound &sound)
{
    if (sound == d->mSound) {
        return;
    }

    d->mEmpty = false;
    d->mSound = sound;
}

Sound Addressee::sound() const
{
    return d->mSound;
}

QString Addressee::soundLabel()
{
    return i18n("Sound");
}

void Addressee::setNameFromString(const QString &s)
{
    QString str = s;
    //remove enclosing quotes from string
    if (str.length() > 1  &&
            s[ 0 ] == QLatin1Char('"') &&
            s[ s.length() - 1 ] == QLatin1Char('"')) {
        str = s.mid(1, s.length() - 2);
    }

    setFormattedName(str);
    setName(str);

    // clear all name parts
    setPrefix(QString());
    setGivenName(QString());
    setAdditionalName(QString());
    setFamilyName(QString());
    setSuffix(QString());

    if (str.isEmpty()) {
        return;
    }

    static QString spaceStr = QString::fromLatin1(" ");
    static QString emptyStr = QString::fromLatin1("");
    AddresseeHelper *helper = AddresseeHelper::self();

    int i = str.indexOf(QLatin1Char(','));
    if (i < 0) {
        QStringList parts = str.split(spaceStr);
        int leftOffset = 0;
        int rightOffset = parts.count() - 1;

        QString suffix;
        while (rightOffset >= 0) {
            if (helper->containsSuffix(parts[ rightOffset ])) {
                suffix.prepend(parts[ rightOffset ] + (suffix.isEmpty() ? emptyStr : spaceStr));
                rightOffset--;
            } else {
                break;
            }
        }
        setSuffix(suffix);

        if (rightOffset < 0) {
            return;
        }

        if (rightOffset - 1 >= 0 && helper->containsPrefix(parts[ rightOffset - 1 ].toLower())) {
            setFamilyName(parts[ rightOffset - 1 ] + spaceStr + parts[ rightOffset ]);
            rightOffset--;
        } else {
            if (helper->tradeAsFamilyName()) {
                setFamilyName(parts[ rightOffset ]);
            } else {
                setGivenName(parts[ rightOffset ]);
            }
        }

        QString prefix;
        while (leftOffset < rightOffset) {
            if (helper->containsTitle(parts[ leftOffset ])) {
                prefix.append((prefix.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ]);
                leftOffset++;
            } else {
                break;
            }
        }
        setPrefix(prefix);

        if (leftOffset < rightOffset) {
            setGivenName(parts[ leftOffset ]);
            leftOffset++;
        }

        QString additionalName;
        while (leftOffset < rightOffset) {
            additionalName.append((additionalName.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ]);
            leftOffset++;
        }
        setAdditionalName(additionalName);
    } else {
        QString part1 = str.left(i);
        QString part2 = str.mid(i + 1);

        QStringList parts = part1.split(spaceStr);
        int leftOffset = 0;
        int rightOffset = parts.count() - 1;

        if (parts.count() > 0) {

            QString suffix;
            while (rightOffset >= 0) {
                if (helper->containsSuffix(parts[ rightOffset ])) {
                    suffix.prepend(parts[ rightOffset ] + (suffix.isEmpty() ? emptyStr : spaceStr));
                    rightOffset--;
                } else {
                    break;
                }
            }
            setSuffix(suffix);

            if (rightOffset - 1 >= 0 && helper->containsPrefix(parts[ rightOffset - 1 ].toLower())) {
                setFamilyName(parts[ rightOffset - 1 ] + spaceStr + parts[ rightOffset ]);
                rightOffset--;
            } else {
                setFamilyName(parts[ rightOffset ]);
            }

            QString prefix;
            while (leftOffset < rightOffset) {
                if (helper->containsTitle(parts[ leftOffset ])) {
                    prefix.append((prefix.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ]);
                    leftOffset++;
                } else {
                    break;
                }
            }
        } else {
            setPrefix(QString());
            setFamilyName(QString());
            setSuffix(QString());
        }

        parts = part2.split(spaceStr);

        leftOffset = 0;
        rightOffset = parts.count();

        if (parts.count() > 0) {

            QString prefix;
            while (leftOffset < rightOffset) {
                if (helper->containsTitle(parts[ leftOffset ])) {
                    prefix.append((prefix.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ]);
                    leftOffset++;
                } else {
                    break;
                }
            }
            setPrefix(prefix);

            if (leftOffset < rightOffset) {
                setGivenName(parts[ leftOffset ]);
                leftOffset++;
            }

            QString additionalName;
            while (leftOffset < rightOffset) {
                additionalName.append((additionalName.isEmpty() ? emptyStr : spaceStr) + parts[ leftOffset ]);
                leftOffset++;
            }
            setAdditionalName(additionalName);
        } else {
            setGivenName(QString());
            setAdditionalName(QString());
        }
    }
}

QString Addressee::realName() const
{
    QString n(formattedName());
    if (!n.isEmpty()) {
        return n;
    }

    n = assembledName();
    if (!n.isEmpty()) {
        return n;
    }

    n = name();
    if (!n.isEmpty()) {
        return n;
    }

    return organization();
}

QString Addressee::assembledName() const
{
    const QString name = prefix() + QLatin1Char(' ') +
                         givenName() + QLatin1Char(' ') +
                         additionalName() + QLatin1Char(' ') +
                         familyName() + QLatin1Char(' ') +
                         suffix();

    return name.simplified();
}

QString Addressee::fullEmail(const QString &email) const
{
    QString e;
    if (email.isNull()) {
        e = preferredEmail();
    } else {
        e = email;
    }
    if (e.isEmpty()) {
        return QString();
    }

    QString text;
    if (realName().isEmpty()) {
        text = e;
    } else {
        QRegExp needQuotes(QLatin1String("[^ 0-9A-Za-z\\x0080-\\xFFFF]"));
        if (realName().indexOf(needQuotes) != -1) {
            QString name = realName();
            name.replace(QLatin1String("\""), QLatin1String("\\\""));
            text = QLatin1String("\"") + name + QLatin1String("\" <") + e + QLatin1Char('>');
        } else {
            text = realName() + QLatin1String(" <") + e + QLatin1Char('>');
        }
    }

    return text;
}

void Addressee::insertEmail(const QString &email, bool preferred)
{
    if (email.simplified().isEmpty()) {
        return;
    }

    if (d->mEmails.contains(email)) {
        if (!preferred || d->mEmails.first() == email) {
            return;
        }

        d->mEmails.removeAll(email);
        d->mEmails.prepend(email);
    } else {
        d->mEmpty = false;
        if (preferred) {
            d->mEmails.prepend(email);
        } else {
            d->mEmails.append(email);
        }
    }
}

void Addressee::removeEmail(const QString &email)
{
    if (d->mEmails.contains(email)) {
        d->mEmails.removeAll(email);
    }
}

QString Addressee::preferredEmail() const
{
    if (d->mEmails.count() == 0) {
        return QString();
    } else {
        return d->mEmails.first();
    }
}

QStringList Addressee::emails() const
{
    return d->mEmails;
}

void Addressee::setEmails(const QStringList &emails)
{
    d->mEmails = emails;
}
void Addressee::insertPhoneNumber(const PhoneNumber &phoneNumber)
{
    d->mEmpty = false;

    PhoneNumber::List::Iterator it;
    for (it = d->mPhoneNumbers.begin(); it != d->mPhoneNumbers.end(); ++it) {
        if ((*it).id() == phoneNumber.id()) {
            *it = phoneNumber;
            return;
        }
    }
    if (!phoneNumber.number().simplified().isEmpty()) {
        d->mPhoneNumbers.append(phoneNumber);
    }
}

void Addressee::removePhoneNumber(const PhoneNumber &phoneNumber)
{
    PhoneNumber::List::Iterator it;
    for (it = d->mPhoneNumbers.begin(); it != d->mPhoneNumbers.end(); ++it) {
        if ((*it).id() == phoneNumber.id()) {
            d->mPhoneNumbers.erase(it);
            return;
        }
    }
}

PhoneNumber Addressee::phoneNumber(PhoneNumber::Type type) const
{
    PhoneNumber phoneNumber(QString(), type);
    PhoneNumber::List::ConstIterator it;
    for (it = d->mPhoneNumbers.constBegin(); it != d->mPhoneNumbers.constEnd(); ++it) {
        if (matchBinaryPattern((*it).type(), type)) {
            if ((*it).type() & PhoneNumber::Pref) {
                return (*it);
            } else if (phoneNumber.number().isEmpty()) {
                phoneNumber = (*it);
            }
        }
    }

    return phoneNumber;
}

PhoneNumber::List Addressee::phoneNumbers() const
{
    return d->mPhoneNumbers;
}

PhoneNumber::List Addressee::phoneNumbers(PhoneNumber::Type type) const
{
    PhoneNumber::List list;

    PhoneNumber::List::ConstIterator it;
    PhoneNumber::List::ConstIterator end(d->mPhoneNumbers.constEnd());
    for (it = d->mPhoneNumbers.constBegin(); it != end; ++it) {
        if (matchBinaryPattern((*it).type(), type)) {
            list.append(*it);
        }
    }
    return list;
}

PhoneNumber Addressee::findPhoneNumber(const QString &id) const
{
    PhoneNumber::List::ConstIterator it;
    PhoneNumber::List::ConstIterator end(d->mPhoneNumbers.constEnd());
    for (it = d->mPhoneNumbers.constBegin(); it != end; ++it) {
        if ((*it).id() == id) {
            return *it;
        }
    }
    return PhoneNumber();
}

void Addressee::insertKey(const Key &key)
{
    d->mEmpty = false;

    Key::List::Iterator it;
    for (it = d->mKeys.begin(); it != d->mKeys.end(); ++it) {
        if ((*it).id() == key.id()) {
            *it = key;
            return;
        }
    }
    d->mKeys.append(key);
}

void Addressee::removeKey(const Key &key)
{
    Key::List::Iterator it;
    for (it = d->mKeys.begin(); it != d->mKeys.end(); ++it) {
        if ((*it).id() == key.id()) {
            d->mKeys.removeAll(key);
            return;
        }
    }
}

Key Addressee::key(Key::Type type, const QString &customTypeString) const
{
    Key::List::ConstIterator it;
    Key::List::ConstIterator end(d->mKeys.constEnd());
    for (it = d->mKeys.constBegin(); it != end; ++it) {
        if ((*it).type() == type) {
            if (type == Key::Custom) {
                if (customTypeString.isEmpty()) {
                    return *it;
                } else {
                    if ((*it).customTypeString() == customTypeString) {
                        return (*it);
                    }
                }
            } else {
                return *it;
            }
        }
    }
    return Key(QString(), type);
}

void Addressee::setKeys(const Key::List &list)
{
    d->mKeys = list;
}

Key::List Addressee::keys() const
{
    return d->mKeys;
}

Key::List Addressee::keys(Key::Type type, const QString &customTypeString) const
{
    Key::List list;

    Key::List::ConstIterator it;
    Key::List::ConstIterator end(d->mKeys.constEnd());
    for (it = d->mKeys.constBegin(); it != end; ++it) {
        if ((*it).type() == type) {
            if (type == Key::Custom) {
                if (customTypeString.isEmpty()) {
                    list.append(*it);
                } else {
                    if ((*it).customTypeString() == customTypeString) {
                        list.append(*it);
                    }
                }
            } else {
                list.append(*it);
            }
        }
    }
    return list;
}

Key Addressee::findKey(const QString &id) const
{
    Key::List::ConstIterator it;
    Key::List::ConstIterator end(d->mKeys.constEnd());
    for (it = d->mKeys.constBegin(); it != end; ++it) {
        if ((*it).id() == id) {
            return *it;
        }
    }
    return Key();
}

QString Addressee::toString() const
{
    QString str;

    str += QLatin1String("Addressee {\n");
    str += QString::fromLatin1("  Uid: %1\n").arg(uid());

    str += QString::fromLatin1("  Name: %1\n").arg(name());
    str += QString::fromLatin1("  FormattedName: %1\n").arg(formattedName());
    str += QString::fromLatin1("  FamilyName: %1\n").arg(familyName());
    str += QString::fromLatin1("  GivenName: %1\n").arg(givenName());
    str += QString::fromLatin1("  AdditionalName: %1\n").arg(additionalName());
    str += QString::fromLatin1("  Prefix: %1\n").arg(prefix());
    str += QString::fromLatin1("  Suffix: %1\n").arg(suffix());
    str += QString::fromLatin1("  NickName: %1\n").arg(nickName());
    str += QString::fromLatin1("  Birthday: %1\n").arg(birthday().toString());
    str += QString::fromLatin1("  Mailer: %1\n").arg(mailer());
    str += QString::fromLatin1("  TimeZone: %1\n").arg(timeZone().toString());
    str += QString::fromLatin1("  Geo: %1\n").arg(geo().toString());
    str += QString::fromLatin1("  Title: %1\n").arg(title());
    str += QString::fromLatin1("  Role: %1\n").arg(role());
    str += QString::fromLatin1("  Organization: %1\n").arg(organization());
    str += QString::fromLatin1("  Department: %1\n").arg(department());
    str += QString::fromLatin1("  Note: %1\n").arg(note());
    str += QString::fromLatin1("  ProductId: %1\n").arg(productId());
    str += QString::fromLatin1("  Revision: %1\n").arg(revision().toString());
    str += QString::fromLatin1("  SortString: %1\n").arg(sortString());
    str += QString::fromLatin1("  Url: %1\n").arg(url().url());
    str += QString::fromLatin1("  Secrecy: %1\n").arg(secrecy().toString());
    str += QString::fromLatin1("  Logo: %1\n").arg(logo().toString());
    str += QString::fromLatin1("  Photo: %1\n").arg(photo().toString());
    str += QString::fromLatin1("  Sound: %1\n").arg(sound().toString());

    str += QLatin1String("  Emails {\n");
    const QStringList e = emails();
    QStringList::ConstIterator it;
    for (it = e.begin(); it != e.end(); ++it) {
        str += QString::fromLatin1("    %1\n").arg(*it);
    }
    str += QLatin1String("  }\n");

    str += QLatin1String("  PhoneNumbers {\n");
    const PhoneNumber::List p = phoneNumbers();
    PhoneNumber::List::ConstIterator it2;
    for (it2 = p.begin(); it2 != p.end(); ++it2) {
        str += (*it2).toString();
    }
    str += QLatin1String("  }\n");

    str += QLatin1String("  Addresses {\n");
    const Address::List a = addresses();
    Address::List::ConstIterator it3;
    for (it3 = a.begin(); it3 != a.end(); ++it3) {
        str += (*it3).toString();
    }
    str += QLatin1String("  }\n");

    str += QLatin1String("  Keys {\n");
    const Key::List k = keys();
    Key::List::ConstIterator it4;
    for (it4 = k.begin(); it4 != k.end(); ++it4) {
        str += (*it4).toString();
    }
    str += QLatin1String("  }\n");

    str += QLatin1String("}\n");

    return str;
}

void Addressee::insertAddress(const Address &address)
{
    if (address.isEmpty()) {
        return;
    }

    d->mEmpty = false;

    Address::List::Iterator it;
    for (it = d->mAddresses.begin(); it != d->mAddresses.end(); ++it) {
        if ((*it).id() == address.id()) {
            *it = address;
            return;
        }
    }

    d->mAddresses.append(address);
}

void Addressee::removeAddress(const Address &address)
{
    Address::List::Iterator it;
    for (it = d->mAddresses.begin(); it != d->mAddresses.end(); ++it) {
        if ((*it).id() == address.id()) {
            d->mAddresses.erase(it);
            return;
        }
    }
}

Address Addressee::address(Address::Type type) const
{
    Address address(type);
    Address::List::ConstIterator it;
    Address::List::ConstIterator end(d->mAddresses.constEnd());
    for (it = d->mAddresses.constBegin(); it != end; ++it) {
        if (matchBinaryPattern((*it).type(), type)) {
            if ((*it).type() & Address::Pref) {
                return (*it);
            } else if (address.isEmpty()) {
                address = (*it);
            }
        }
    }

    return address;
}

Address::List Addressee::addresses() const
{
    return d->mAddresses;
}

Address::List Addressee::addresses(Address::Type type) const
{
    Address::List list;

    Address::List::ConstIterator it;
    Address::List::ConstIterator end(d->mAddresses.constEnd());
    for (it = d->mAddresses.constBegin(); it != end; ++it) {
        if (matchBinaryPattern((*it).type(), type)) {
            list.append(*it);
        }
    }

    return list;
}

Address Addressee::findAddress(const QString &id) const
{
    Address::List::ConstIterator it;
    Address::List::ConstIterator end(d->mAddresses.constEnd());
    for (it = d->mAddresses.constBegin(); it != end; ++it) {
        if ((*it).id() == id) {
            return *it;
        }
    }
    return Address();
}

void Addressee::insertCategory(const QString &c)
{
    d->mEmpty = false;

    if (d->mCategories.contains(c)) {
        return;
    }

    d->mCategories.append(c);
}

void Addressee::removeCategory(const QString &category)
{
    if (d->mCategories.contains(category)) {
        d->mCategories.removeAll(category);
    }
}

bool Addressee::hasCategory(const QString &category) const
{
    return d->mCategories.contains(category);
}

void Addressee::setCategories(const QStringList &c)
{
    d->mEmpty = false;

    d->mCategories = c;
}

QStringList Addressee::categories() const
{
    return d->mCategories;
}

void Addressee::insertCustom(const QString &app, const QString &name,
                             const QString &value)
{
    if (value.isEmpty() || name.isEmpty() || app.isEmpty()) {
        return;
    }

    d->mEmpty = false;

    const QString qualifiedName = app + QLatin1Char('-') + name;

    d->mCustomFields.insert(qualifiedName, value);
}

void Addressee::removeCustom(const QString &app, const QString &name)
{
    const QString qualifiedName = app + QLatin1Char('-') + name;

    d->mCustomFields.remove(qualifiedName);
}

QString Addressee::custom(const QString &app, const QString &name) const
{
    const QString qualifiedName = app + QLatin1Char('-') + name;

    return d->mCustomFields.value(qualifiedName);
}

void Addressee::setCustoms(const QStringList &customs)
{
    d->mEmpty = false;

    d->mCustomFields.clear();

    foreach (const QString &custom, customs) {
        const int index = custom.indexOf(QLatin1Char(':'));
        if (index == -1) {
            continue;
        }

        const QString qualifiedName = custom.left(index);
        const QString value = custom.mid(index + 1);

        d->mCustomFields.insert(qualifiedName, value);
    }
}

QStringList Addressee::customs() const
{
    QStringList result;

    QMapIterator<QString, QString> it(d->mCustomFields);
    while (it.hasNext()) {
        it.next();
        result << it.key() + QLatin1Char(':') + it.value();
    }

    return result;
}

void Addressee::parseEmailAddress(const QString &rawEmail, QString &fullName,
                                  QString &email)
{
    // This is a simplified version of KPIM::splitAddress().

    fullName.clear();
    email.clear();
    if (rawEmail.isEmpty()) {
        return;    // KPIM::AddressEmpty;
    }

    // The code works on 8-bit strings, so convert the input to UTF-8.
    QByteArray address = rawEmail.toUtf8();

    QByteArray displayName;
    QByteArray addrSpec;
    QByteArray comment;

    // The following is a primitive parser for a mailbox-list (cf. RFC 2822).
    // The purpose is to extract a displayable string from the mailboxes.
    // Comments in the addr-spec are not handled. No error checking is done.

    enum { TopLevel, InComment, InAngleAddress } context = TopLevel;
    bool inQuotedString = false;
    int commentLevel = 0;
    bool stop = false;

    for (char *p = address.data(); *p && !stop; ++p) {
        switch (context) {
        case TopLevel : {
            switch (*p) {
            case '"' : inQuotedString = !inQuotedString;
                displayName += *p;
                break;
            case '(' : if (!inQuotedString) {
                    context = InComment;
                    commentLevel = 1;
                } else {
                    displayName += *p;
                }
                break;
            case '<' : if (!inQuotedString) {
                    context = InAngleAddress;
                } else {
                    displayName += *p;
                }
                break;
            case '\\' : // quoted character
                displayName += *p;
                ++p; // skip the '\'
                if (*p) {
                    displayName += *p;
                } else
                    //return KPIM::UnexpectedEnd;
                {
                    goto ABORT_PARSING;
                }
                break;
            case ',' : if (!inQuotedString) {
                    //if ( allowMultipleAddresses )
                    //  stop = true;
                    //else
                    //  return KPIM::UnexpectedComma;
                    goto ABORT_PARSING;
                } else {
                    displayName += *p;
                }
                break;
            default :  displayName += *p;
            }
            break;
        }
        case InComment : {
            switch (*p) {
            case '(' : ++commentLevel;
                comment += *p;
                break;
            case ')' : --commentLevel;
                if (commentLevel == 0) {
                    context = TopLevel;
                    comment += ' '; // separate the text of several comments
                } else {
                    comment += *p;
                }
                break;
            case '\\' : // quoted character
                comment += *p;
                ++p; // skip the '\'
                if (*p) {
                    comment += *p;
                } else
                    //return KPIM::UnexpectedEnd;
                {
                    goto ABORT_PARSING;
                }
                break;
            default :  comment += *p;
            }
            break;
        }
        case InAngleAddress : {
            switch (*p) {
            case '"' : inQuotedString = !inQuotedString;
                addrSpec += *p;
                break;
            case '>' : if (!inQuotedString) {
                    context = TopLevel;
                } else {
                    addrSpec += *p;
                }
                break;
            case '\\' : // quoted character
                addrSpec += *p;
                ++p; // skip the '\'
                if (*p) {
                    addrSpec += *p;
                } else
                    //return KPIM::UnexpectedEnd;
                {
                    goto ABORT_PARSING;
                }
                break;
            default :  addrSpec += *p;
            }
            break;
        }
        } // switch ( context )
    }

ABORT_PARSING:
    displayName = displayName.trimmed();
    comment = comment.trimmed();
    addrSpec = addrSpec.trimmed();

    fullName = QString::fromUtf8(displayName);
    email = QString::fromUtf8(addrSpec);

    // check for errors
    if (inQuotedString) {
        return;    // KPIM::UnbalancedQuote;
    }
    if (context == InComment) {
        return;    // KPIM::UnbalancedParens;
    }
    if (context == InAngleAddress) {
        return;    // KPIM::UnclosedAngleAddr;
    }

    if (addrSpec.isEmpty()) {
        if (displayName.isEmpty()) {
            return;    // KPIM::NoAddressSpec;
        } else {
            //addrSpec = displayName;
            //displayName.truncate( 0 );
            // Address of the form "foo@bar" or "foo@bar (Name)".
            email = fullName;
            fullName = QString::fromUtf8(comment);
        }
    }

    email = email.toLower();
    // Check that we do not have any extra characters on the end of the
    // strings
    unsigned int len = fullName.length();
    if (fullName[ 0 ] == QLatin1Char('"') && fullName[ len - 1 ] == QLatin1Char('"')) {
        fullName = fullName.mid(1, len - 2);
    }
}

void Addressee::setChanged(bool value)
{
    d->mChanged = value;
}

bool Addressee::changed() const
{
    return d->mChanged;
}

void Addressee::setSortMode(KContacts::SortMode *mode)
{
    Private::mSortMode = mode;
}

bool Addressee::operator< (const Addressee &addr) const
{
    if (!Private::mSortMode) {
        return false;
    } else {
        return Private::mSortMode->lesser(*this, addr);
    }
}

QString Addressee::mimeType()
{
    return QLatin1String("text/directory");
}

QDataStream &KContacts::operator<<(QDataStream &s, const Addressee &a)
{
    s << a.d->mUid;

    s << a.d->mName;
    s << a.d->mFormattedName;
    s << a.d->mFamilyName;
    s << a.d->mGivenName;
    s << a.d->mAdditionalName;
    s << a.d->mPrefix;
    s << a.d->mSuffix;
    s << a.d->mNickName;
    s << a.d->mBirthday;
    s << a.d->mMailer;
    s << a.d->mTimeZone;
    s << a.d->mGeo;
    s << a.d->mTitle;
    s << a.d->mRole;
    s << a.d->mOrganization;
    s << a.d->mDepartment;
    s << a.d->mNote;
    s << a.d->mProductId;
    s << a.d->mRevision;
    s << a.d->mSortString;
    s << a.d->mUrl;
    s << a.d->mSecrecy;
    s << a.d->mLogo;
    s << a.d->mPhoto;
    s << a.d->mSound;
    s << a.d->mPhoneNumbers;
    s << a.d->mAddresses;
    s << a.d->mEmails;
    s << a.d->mCategories;
    s << a.customs();
    s << a.d->mKeys;
    return s;
}

QDataStream &KContacts::operator>>(QDataStream &s, Addressee &a)
{
    s >> a.d->mUid;

    s >> a.d->mName;
    s >> a.d->mFormattedName;
    s >> a.d->mFamilyName;
    s >> a.d->mGivenName;
    s >> a.d->mAdditionalName;
    s >> a.d->mPrefix;
    s >> a.d->mSuffix;
    s >> a.d->mNickName;
    s >> a.d->mBirthday;
    s >> a.d->mMailer;
    s >> a.d->mTimeZone;
    s >> a.d->mGeo;
    s >> a.d->mTitle;
    s >> a.d->mRole;
    s >> a.d->mOrganization;
    s >> a.d->mDepartment;
    s >> a.d->mNote;
    s >> a.d->mProductId;
    s >> a.d->mRevision;
    s >> a.d->mSortString;
    s >> a.d->mUrl;
    s >> a.d->mSecrecy;
    s >> a.d->mLogo;
    s >> a.d->mPhoto;
    s >> a.d->mSound;
    s >> a.d->mPhoneNumbers;
    s >> a.d->mAddresses;
    s >> a.d->mEmails;
    s >> a.d->mCategories;
    QStringList customFields;
    s >> customFields;
    a.setCustoms(customFields);
    s >> a.d->mKeys;

    a.d->mEmpty = false;

    return s;
}

bool matchBinaryPattern(int value, int pattern)
{
    /**
      We want to match all telephonnumbers/addresses which have the bits in the
      pattern set. More are allowed.
      if pattern == 0 we have a special handling, then we want only those with
      exactly no bit set.
     */
    if (pattern == 0) {
        return (value == 0);
    } else {
        return (pattern == (pattern & value));
    }
}

template <class L>
bool listEquals(const QList<L> &list, const QList<L> &pattern)
{
    if (list.count() != pattern.count()) {
        return false;
    }
    const int numberOfElement(list.count());
    for (int i = 0; i < numberOfElement; ++i) {
        if (!pattern.contains(list[ i ])) {
            return false;
        }
    }

    return true;
}

bool listEquals(const QStringList &list, const QStringList &pattern)
{
    if (list.count() != pattern.count()) {
        return false;
    }

    const int numberOfElement(list.count());
    for (int i = 0; i < numberOfElement; ++i) {
        if (!pattern.contains(list[ i ])) {
            return false;
        }
    }

    return true;
}

bool emailsEquals(const QStringList &list, const QStringList &pattern)
{
    if (list.count() != pattern.count()) {
        return false;
    }

    if (list.isEmpty()) {
        return true;
    }

    if (list.first() != pattern.first()) {
        return false;
    }

    QStringList::ConstIterator it;
    QStringList::ConstIterator end(list.constEnd());
    for (it = list.constBegin(); it != end; ++it) {
        if (!pattern.contains(*it)) {
            return false;
        }
    }

    return true;
}
