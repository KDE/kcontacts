/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "address.h"
#include "addressformat.h"
#include "addressformatter_p.h"

#include "kcontacts_debug.h"
#include <KConfig>
#include <KCountry>
#include <KLocalizedString>

#include <KConfigGroup>

#include <QDataStream>
#include <QSharedData>
#include <QStringList>
#include <QUrlQuery>
#include <QUuid>

using namespace KContacts;
using namespace Qt::StringLiterals;

class Q_DECL_HIDDEN Address::Private : public QSharedData
{
public:
    Private()
        : mEmpty(true)
    {
        mId = QUuid::createUuid().toString(QUuid::Id128);
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        mEmpty = other.mEmpty;
        mId = other.mId;
        mType = other.mType;

        mPostOfficeBox = other.mPostOfficeBox;
        mExtended = other.mExtended;
        mStreet = other.mStreet;
        mLocality = other.mLocality;
        mRegion = other.mRegion;
        mPostalCode = other.mPostalCode;
        mCountry = other.mCountry;
        mLabel = other.mLabel;
        mGeo = other.mGeo;
        mRoom = other.mRoom;
        mApartment = other.mApartment;
        mFloor = other.mFloor;
        mStreetNumber = other.mStreetNumber;
        mStreetName = other.mStreetName;
        mBuilding = other.mBuilding;
        mBlock = other.mBlock;
        mSubdistrict = other.mSubdistrict;
        mDistrict = other.mDistrict;
        mLandmark = other.mLandmark;
        mDirection = other.mDirection;
    }

    bool mEmpty;
    QString mId;
    Type mType;
    Geo mGeo;

    QString mPostOfficeBox;
    QString mExtended;
    QString mStreet;
    QString mLocality;
    QString mRegion;
    QString mPostalCode;
    QString mCountry;
    QString mLabel;
    QString mRoom;
    QString mApartment;
    QString mFloor;
    QString mStreetNumber;
    QString mStreetName;
    QString mBuilding;
    QString mBlock;
    QString mSubdistrict;
    QString mDistrict;
    QString mLandmark;
    QString mDirection;
};

Address::Address()
    : d(new Private)
{
}

Address::Address(Type type)
    : d(new Private)
{
    d->mType = type;
}

Address::Address(const Address &other)
    : d(other.d)
{
}

Address::~Address()
{
}

Address &Address::operator=(const Address &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

bool Address::operator==(const Address &other) const
{
    if (d->mId != other.d->mId) {
        return false;
    }
    if (d->mType != other.d->mType) {
        return false;
    }
    if (d->mPostOfficeBox != other.d->mPostOfficeBox) {
        return false;
    }
    if (d->mExtended != other.d->mExtended) {
        return false;
    }
    if (d->mStreet != other.d->mStreet) {
        return false;
    }
    if (d->mLocality != other.d->mLocality) {
        return false;
    }
    if (d->mRegion != other.d->mRegion) {
        return false;
    }
    if (d->mPostalCode != other.d->mPostalCode) {
        return false;
    }
    if (d->mCountry != other.d->mCountry) {
        return false;
    }
    if (d->mLabel != other.d->mLabel) {
        return false;
    }

    if (d->mRoom != other.d->mRoom) {
        return false;
    }
    if (d->mApartment != other.d->mApartment) {
        return false;
    }
    if (d->mFloor != other.d->mFloor) {
        return false;
    }
    if (d->mStreetNumber != other.d->mStreetNumber) {
        return false;
    }
    if (d->mStreetName != other.d->mStreetName) {
        return false;
    }
    if (d->mBuilding != other.d->mBuilding) {
        return false;
    }
    if (d->mBlock != other.d->mBlock) {
        return false;
    }
    if (d->mSubdistrict != other.d->mSubdistrict) {
        return false;
    }
    if (d->mDistrict != other.d->mDistrict) {
        return false;
    }
    if (d->mLandmark != other.d->mLandmark) {
        return false;
    }
    if (d->mDirection != other.d->mDirection) {
        return false;
    }

    if (d->mGeo != other.d->mGeo) {
        return false;
    }

    return true;
}

bool Address::operator!=(const Address &a) const
{
    return !(a == *this);
}

bool Address::isEmpty() const
{
    return d->mEmpty;
}

void Address::clear()
{
    *this = Address();
}

void Address::setId(const QString &id)
{
    d->mEmpty = false;
    d->mId = id;
}

QString Address::id() const
{
    return d->mId;
}

void Address::setType(Type type)
{
    d->mEmpty = false;
    d->mType = type;
}

Address::Type Address::type() const
{
    return d->mType;
}

QString Address::typeLabel(Type type)
{
    QString label;
    const TypeList list = typeList();

    for (const auto typeFlag : list) {
        // these are actually flags
        const TypeFlag flag = static_cast<TypeFlag>(static_cast<int>(typeFlag));
        if (type & flag) {
            label.append(QLatin1Char('/') + typeFlagLabel(flag));
        }
    }

    // Remove the first '/'
    if (!label.isEmpty()) {
        label.remove(0, 1);
    }

    return label;
}

QString Address::typeLabel() const
{
    QString label;
    const TypeList list = typeList();

    for (const auto f : list) {
        if ((type() & f) && (f != Pref)) {
            label.append(QLatin1Char('/') + typeLabel(f));
        }
    }
    // Remove the first '/'
    if (!label.isEmpty()) {
        label.remove(0, 1);
    }
    return label;
}

void Address::setPostOfficeBox(const QString &postOfficeBox)
{
    d->mEmpty = false;
    d->mPostOfficeBox = postOfficeBox;
}

QString Address::postOfficeBox() const
{
    return d->mPostOfficeBox;
}

QString Address::postOfficeBoxLabel()
{
    return i18n("Post Office Box");
}

void Address::setExtended(const QString &extended)
{
    d->mEmpty = false;
    d->mExtended = extended;
}

QString Address::extended() const
{
    return d->mExtended;
}

QString Address::extendedLabel()
{
    return i18n("Extended Address Information");
}

void Address::setStreet(const QString &street)
{
    d->mEmpty = false;
    d->mStreet = street;
}

QString Address::street() const
{
    QStringList components{d->mStreetNumber,
                           d->mStreetName,
                           d->mRoom,
                           d->mApartment,
                           d->mFloor,
                           d->mBuilding,
                           d->mBlock,
                           d->mSubdistrict,
                           d->mDistrict,
                           d->mLandmark,
                           d->mDirection};
    components.removeAll(QString());
    return components.isEmpty() ? d->mStreet : components.join(u' ');
}

QString Address::streetLabel()
{
    return i18n("Street");
}

void Address::setLocality(const QString &locality)
{
    d->mEmpty = false;
    d->mLocality = locality;
}

QString Address::locality() const
{
    return d->mLocality;
}

QString Address::localityLabel()
{
    return i18n("Locality");
}

void Address::setRegion(const QString &region)
{
    d->mEmpty = false;
    d->mRegion = region;
}

QString Address::region() const
{
    return d->mRegion;
}

QString Address::regionLabel()
{
    return i18n("Region");
}

void Address::setPostalCode(const QString &postalCode)
{
    d->mEmpty = false;
    d->mPostalCode = postalCode;
}

QString Address::postalCode() const
{
    return d->mPostalCode;
}

QString Address::postalCodeLabel()
{
    return i18n("Postal Code");
}

void Address::setCountry(const QString &country)
{
    d->mEmpty = false;
    d->mCountry = country;
}

QString Address::country() const
{
    return d->mCountry;
}

QString Address::countryLabel()
{
    return i18n("Country");
}

void Address::setLabel(const QString &label)
{
    d->mEmpty = false;
    d->mLabel = label;
}

QString Address::label() const
{
    return d->mLabel;
}

QString Address::labelLabel()
{
    return i18n("Delivery Label");
}

void Address::setRoom(const QString &room)
{
    d->mEmpty = false;
    d->mRoom = room;
}

QString Address::room() const
{
    return d->mRoom;
}

QString Address::roomLabel()
{
    return i18nc("Address component", "Room");
}

void Address::setApartment(const QString &apartment)
{
    d->mEmpty = false;
    d->mApartment = apartment;
}

QString Address::apartment() const
{
    return d->mApartment;
}

QString Address::apartmentLabel()
{
    return i18nc("Address component", "Apartment");
}

void Address::setFloor(const QString &floor)
{
    d->mEmpty = false;
    d->mFloor = floor;
}

QString Address::floor() const
{
    return d->mFloor;
}

QString Address::floorLabel()
{
    return i18nc("Address component", "Floor");
}

void Address::setStreetNumber(const QString &streetNumber)
{
    d->mEmpty = false;
    d->mStreetNumber = streetNumber;
}

QString Address::streetNumber() const
{
    return d->mStreetNumber;
}

QString Address::streetNumberLabel()
{
    return i18nc("Address component", "Street number");
}

void Address::setStreetName(const QString &streetName)
{
    d->mEmpty = false;
    d->mStreetName = streetName;
}

QString Address::streetName() const
{
    return d->mStreetName;
}

QString Address::streetNameLabel()
{
    return i18nc("Address component", "Street name");
}

void Address::setBuilding(const QString &building)
{
    d->mEmpty = false;
    d->mBuilding = building;
}

QString Address::building() const
{
    return d->mBuilding;
}

QString Address::buildingLabel()
{
    return i18nc("Address component", "Building");
}

void Address::setBlock(const QString &block)
{
    d->mEmpty = false;
    d->mBlock = block;
}

QString Address::block() const
{
    return d->mBlock;
}

QString Address::blockLabel()
{
    return i18nc("Address component", "Block");
}

void Address::setSubdistrict(const QString &subdistrict)
{
    d->mEmpty = false;
    d->mSubdistrict = subdistrict;
}

QString Address::subdistrict() const
{
    return d->mSubdistrict;
}

QString Address::subdistrictLabel()
{
    return i18nc("Address component", "Subdistrict");
}

void Address::setDistrict(const QString &district)
{
    d->mEmpty = false;
    d->mDistrict = district;
}

QString Address::district() const
{
    return d->mDistrict;
}

QString Address::districtLabel()
{
    return i18nc("Address component", "District");
}

void Address::setLandmark(const QString &landmark)
{
    d->mEmpty = false;
    d->mLandmark = landmark;
}

QString Address::landmark() const
{
    return d->mLandmark;
}

QString Address::landmarkLabel()
{
    return i18nc("Address component", "Landmark");
}

void Address::setDirection(const QString &direction)
{
    d->mEmpty = false;
    d->mDirection = direction;
}

QString Address::direction() const
{
    return d->mDirection;
}

QString Address::directionLabel()
{
    return i18nc("Address component", "Direction");
}

Address::TypeList Address::typeList()
{
    static TypeList list;

    if (list.isEmpty()) {
        list << Dom << Intl << Postal << Parcel << Home << Work << Pref << Billing << Delivery;
    }

    return list;
}

QString Address::typeFlagLabel(TypeFlag type)
{
    switch (type) {
    case Dom:
        return i18nc("Address is in home country", "Domestic");
    case Intl:
        return i18nc("Address is not in home country", "International");
    case Postal:
        return i18nc("Address for delivering letters", "Postal");
    case Parcel:
        return i18nc("Address for delivering packages", "Parcel");
    case Home:
        return i18nc("Home Address", "Home");
    case Work:
        return i18nc("Work Address", "Work");
    case Pref:
        return i18n("Preferred Address");
    case Billing:
        return i18nc("Address for billing", "Billing");
    case Delivery:
        return i18nc("Address for delivering goods", "Delivery");
    }
    return i18nc("another type of address", "Other");
}

void Address::setGeo(const Geo &geo)
{
    d->mEmpty = false;
    d->mGeo = geo;
}

Geo Address::geo() const
{
    return d->mGeo;
}

QString Address::toString() const
{
    QString str = QLatin1String("Address {\n");
    str += QStringLiteral("  IsEmpty: %1\n").arg(d->mEmpty ? QStringLiteral("true") : QStringLiteral("false"));
    str += QStringLiteral("  Id: %1\n").arg(d->mId);
    str += QStringLiteral("  Type: %1\n").arg(typeLabel(d->mType));
    str += QStringLiteral("  Post office box: %1\n").arg(d->mPostOfficeBox);
    str += QStringLiteral("  Extended: %1\n").arg(d->mExtended);
    str += QStringLiteral("  Street: %1\n").arg(d->mStreet);
    str += QStringLiteral("  Locality: %1\n").arg(d->mLocality);
    str += QStringLiteral("  Region: %1\n").arg(d->mRegion);
    str += QStringLiteral("  Postal code: %1\n").arg(d->mPostalCode);
    str += QStringLiteral("  Country: %1\n").arg(d->mCountry);
    str += u"  Room: %1\n"_s.arg(d->mRoom);
    str += u"  Apartment: %1\n"_s.arg(d->mApartment);
    str += u"  Floor: %1\n"_s.arg(d->mFloor);
    str += u"  StreetNumber: %1\n"_s.arg(d->mStreetNumber);
    str += u"  StreetName: %1\n"_s.arg(d->mStreetName);
    str += u"  Building: %1\n"_s.arg(d->mBuilding);
    str += u"  Block: %1\n"_s.arg(d->mBlock);
    str += u"  Subdistrict: %1\n"_s.arg(d->mSubdistrict);
    str += u"  District: %1\n"_s.arg(d->mDistrict);
    str += u"  Landmark: %1\n"_s.arg(d->mLandmark);
    str += u"  Direction: %1\n"_s.arg(d->mDirection);
    str += QStringLiteral("  Label: %1\n").arg(d->mLabel);
    str += QStringLiteral("  Geo: %1\n").arg(d->mGeo.toString());
    str += QLatin1String("}\n");

    return str;
}

QString Address::formatted(AddressFormatStyle style, const QString &realName, const QString &orgaName) const
{
    const auto formatPref = (orgaName.isEmpty() || style != AddressFormatStyle::Postal) ? AddressFormatPreference::Generic : AddressFormatPreference::Business;
    const auto format = AddressFormatRepository::formatForAddress(*this, formatPref);
    return AddressFormatter::format(*this, realName, orgaName, format, style);
}

QString Address::formattedPostalAddress() const
{
    return formatted(AddressFormatStyle::Postal);
}

QUrl Address::geoUri() const
{
    QUrl url;
    url.setScheme(QStringLiteral("geo"));

    if (geo().isValid()) {
        url.setPath(QString::number(geo().latitude()) + QLatin1Char(',') + QString::number(geo().longitude()));
        return url;
    }

    if (!isEmpty()) {
        url.setPath(QStringLiteral("0,0"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("q"), formatted(KContacts::AddressFormatStyle::GeoUriQuery));
        url.setQuery(query);
        return url;
    }

    return {};
}

// clang-format off
QDataStream &KContacts::operator<<(QDataStream &s, const Address &addr)
{
    return s << addr.d->mId << (uint)addr.d->mType << addr.d->mPostOfficeBox
             << addr.d->mExtended << addr.d->mStreet << addr.d->mLocality
             << addr.d->mRegion << addr.d->mPostalCode << addr.d->mCountry
             << addr.d->mLabel << addr.d->mEmpty << addr.d->mGeo
             << addr.d->mRoom << addr.d->mApartment << addr.d->mFloor
             << addr.d->mStreetNumber << addr.d->mStreetName << addr.d->mBuilding
             << addr.d->mBlock << addr.d->mSubdistrict << addr.d->mDistrict
             << addr.d->mLandmark << addr.d->mDirection;
}

QDataStream &KContacts::operator>>(QDataStream &s, Address &addr)
{
    uint type;
    s >> addr.d->mId >> type >> addr.d->mPostOfficeBox >> addr.d->mExtended
    >> addr.d->mStreet >> addr.d->mLocality >> addr.d->mRegion
    >> addr.d->mPostalCode >> addr.d->mCountry >> addr.d->mLabel
    >> addr.d->mEmpty >> addr.d->mGeo
    >> addr.d->mRoom >> addr.d->mApartment >> addr.d->mFloor
    >> addr.d->mStreetNumber >> addr.d->mStreetName >> addr.d->mBuilding
    >> addr.d->mBlock >> addr.d->mSubdistrict >> addr.d->mDistrict
    >> addr.d->mLandmark >> addr.d->mDirection;

    addr.d->mType = Address::Type(type);

    return s;
}
// clang-format on

#include "moc_address.cpp"
