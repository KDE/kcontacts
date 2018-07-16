/*
    This file is part of the KContacts framework.
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

#include "address.h"
#include "geo.h"

#include "kcontacts_debug.h"
#include <krandom.h>
#include <klocalizedstring.h>
#include <kconfig.h>

#include <kconfiggroup.h>

#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QSharedData>
#include <QStandardPaths>
#include <QLocale>
#include <QDataStream>

using namespace KContacts;

// template tags for address formatting localization
#define KCONTACTS_FMTTAG_realname   QStringLiteral("%n")
#define KCONTACTS_FMTTAG_REALNAME   QStringLiteral("%N")
#define KCONTACTS_FMTTAG_company    QStringLiteral("%cm")
#define KCONTACTS_FMTTAG_COMPANY    QStringLiteral("%CM")
#define KCONTACTS_FMTTAG_pobox      QStringLiteral("%p")
#define KCONTACTS_FMTTAG_street     QStringLiteral("%s")
#define KCONTACTS_FMTTAG_STREET     QStringLiteral("%S")
#define KCONTACTS_FMTTAG_zipcode    QStringLiteral("%z")
#define KCONTACTS_FMTTAG_location   QStringLiteral("%l")
#define KCONTACTS_FMTTAG_LOCATION   QStringLiteral("%L")
#define KCONTACTS_FMTTAG_region     QStringLiteral("%r")
#define KCONTACTS_FMTTAG_REGION     QStringLiteral("%R")
#define KCONTACTS_FMTTAG_newline    QStringLiteral("\\n")
#define KCONTACTS_FMTTAG_condcomma  QStringLiteral("%,")
#define KCONTACTS_FMTTAG_condwhite  QStringLiteral("%w")
#define KCONTACTS_FMTTAG_purgeempty QStringLiteral("%0")

/**
  Finds the balanced closing bracket starting from the opening bracket at
  pos in tsection.
  @return  position of closing bracket, -1 for unbalanced brackets
*/
static int findBalancedBracket(const QString &tsection, int pos)
{
    int balancecounter = 0;
    for (int i = pos + 1; i < tsection.length(); ++i) {
        if (QLatin1Char(')') == tsection[i] && 0 == balancecounter) {
            // found end of brackets
            return i;
        } else {
            if (QLatin1Char('(') == tsection[i]) {
                // nested brackets
                balancecounter++;
            }
        }
    }
    return -1;
}

/**
  Parses a snippet of an address template
  @param tsection   the template string to be parsed
  @param result     QString reference in which the result will be stored
  @return           true if at least one tag evaluated positively, else false
*/
static bool parseAddressTemplateSection(const QString &tsection, QString &result, const QString &realName, const QString &orgaName, const KContacts::Address &address)
{
    // This method first parses and substitutes any bracketed sections and
    // after that replaces any tags with their values. If a bracketed section
    // or a tag evaluate to zero, they are not just removed but replaced
    // with a placeholder. This is because in the last step conditionals are
    // resolved which depend on information about zero-evaluations.
    result = tsection;
    int stpos = 0;
    bool ret = false;

    // first check for brackets that have to be evaluated first
    int fpos = result.indexOf(KCONTACTS_FMTTAG_purgeempty, stpos);
    while (-1 != fpos) {
        int bpos1 = fpos + KCONTACTS_FMTTAG_purgeempty.length();
        // expect opening bracket and find next balanced closing bracket. If
        // next char is no opening bracket, continue parsing (no valid tag)
        if (QLatin1Char('(') == result[bpos1]) {
            int bpos2 = findBalancedBracket(result, bpos1);
            if (-1 != bpos2) {
                // we have balanced brackets, recursively parse:
                QString rplstr;
                bool purge = !parseAddressTemplateSection(result.mid(bpos1 + 1,
                                                                     bpos2 - bpos1 - 1), rplstr,
                                                          realName, orgaName, address);
                if (purge) {
                    // purge -> remove all
                    // replace with !_P_!, so conditional tags work later
                    result.replace(fpos, bpos2 - fpos + 1, QStringLiteral("!_P_!"));
                    // leave stpos as it is
                } else {
                    // no purge -> replace with recursively parsed string
                    result.replace(fpos, bpos2 - fpos + 1, rplstr);
                    ret = true;
                    stpos = fpos + rplstr.length();
                }
            } else {
                // unbalanced brackets:  keep on parsing (should not happen
                // and will result in bad formatting)
                stpos = bpos1;
            }
        }
        fpos = result.indexOf(KCONTACTS_FMTTAG_purgeempty, stpos);
    }

    // after sorting out all purge tags, we just search'n'replace the rest,
    // keeping track of whether at least one tag evaluates to something.
    // The following macro needs QString for R_FIELD
    // It substitutes !_P_! for empty fields so conditional tags work later
#define REPLTAG(R_TAG, R_FIELD) \
    if (result.contains(R_TAG)) { \
        QString rpl = R_FIELD.isEmpty() ? QStringLiteral("!_P_!") : R_FIELD; \
        result.replace(R_TAG, rpl); \
        if (!R_FIELD.isEmpty()) { \
            ret = true; \
        } \
    }
    REPLTAG(KCONTACTS_FMTTAG_realname, realName);
    REPLTAG(KCONTACTS_FMTTAG_REALNAME, realName.toUpper());
    REPLTAG(KCONTACTS_FMTTAG_company, orgaName);
    REPLTAG(KCONTACTS_FMTTAG_COMPANY, orgaName.toUpper());
    REPLTAG(KCONTACTS_FMTTAG_pobox, address.postOfficeBox());
    REPLTAG(KCONTACTS_FMTTAG_street, address.street());
    REPLTAG(KCONTACTS_FMTTAG_STREET, address.street().toUpper());
    REPLTAG(KCONTACTS_FMTTAG_zipcode, address.postalCode());
    REPLTAG(KCONTACTS_FMTTAG_location, address.locality());
    REPLTAG(KCONTACTS_FMTTAG_LOCATION, address.locality().toUpper());
    REPLTAG(KCONTACTS_FMTTAG_region, address.region());
    REPLTAG(KCONTACTS_FMTTAG_REGION, address.region().toUpper());
    result.replace(KCONTACTS_FMTTAG_newline, QLatin1String("\n"));
#undef REPLTAG

    // conditional comma
    fpos = result.indexOf(KCONTACTS_FMTTAG_condcomma, 0);
    while (-1 != fpos) {
        const QString str1 = result.mid(fpos - 5, 5);
        const QString str2 = result.mid(fpos + 2, 5);
        if (str1 != QLatin1String("!_P_!") && str2 != QLatin1String("!_P_!")) {
            result.replace(fpos, 2, QStringLiteral(", "));
        } else {
            result.remove(fpos, 2);
        }
        fpos = result.indexOf(KCONTACTS_FMTTAG_condcomma, fpos);
    }
    // conditional whitespace
    fpos = result.indexOf(KCONTACTS_FMTTAG_condwhite, 0);
    while (-1 != fpos) {
        const QString str1 = result.mid(fpos - 5, 5);
        const QString str2 = result.mid(fpos + 2, 5);
        if (str1 != QLatin1String("!_P_!") && str2 != QLatin1String("!_P_!")) {
            result.replace(fpos, 2, QLatin1Char(' '));
        } else {
            result.remove(fpos, 2);
        }
        fpos = result.indexOf(KCONTACTS_FMTTAG_condwhite, fpos);
    }

    // remove purged:
    result.remove(QStringLiteral("!_P_!"));

    return ret;
}

class Q_DECL_HIDDEN Address::Private : public QSharedData
{
public:
    Private()
        : mEmpty(true)
        , mType(nullptr)
    {
        mId = KRandom::randomString(10);
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
    bool first = true;
    const TypeList list = typeList();

    TypeList::ConstIterator it;
    TypeList::ConstIterator end(list.end());
    for (it = list.begin(); it != end; ++it) {
        // these are actually flags
        const TypeFlag flag = static_cast<TypeFlag>(static_cast<int>(*it));
        if (type & flag) {
            if (!first) {
                label.append(QLatin1Char('/'));
            }

            label.append(typeFlagLabel(flag));

            if (first) {
                first = false;
            }
        }
    }
    return label;
}

QString Address::typeLabel() const
{
    QString label;
    bool first = true;

    const TypeList list = typeList();

    TypeList::ConstIterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        if ((type() & (*it)) && ((*it) != Pref)) {
            if (!first) {
                label.append(QLatin1Char('/'));
            }
            label.append(typeLabel(*it));
            if (first) {
                first = false;
            }
        }
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
    return d->mStreet;
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

Address::TypeList Address::typeList()
{
    static TypeList list;

    if (list.isEmpty()) {
        list << Dom << Intl << Postal << Parcel << Home << Work << Pref;
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
    str += QStringLiteral("  IsEmpty: %1\n").
           arg(d->mEmpty ? QStringLiteral("true") : QStringLiteral("false"));
    str += QStringLiteral("  Id: %1\n").arg(d->mId);
    str += QStringLiteral("  Type: %1\n").arg(typeLabel(d->mType));
    str += QStringLiteral("  Post office box: %1\n").arg(d->mPostOfficeBox);
    str += QStringLiteral("  Extended: %1\n").arg(d->mExtended);
    str += QStringLiteral("  Street: %1\n").arg(d->mStreet);
    str += QStringLiteral("  Locality: %1\n").arg(d->mLocality);
    str += QStringLiteral("  Region: %1\n").arg(d->mRegion);
    str += QStringLiteral("  Postal code: %1\n").arg(d->mPostalCode);
    str += QStringLiteral("  Country: %1\n").arg(d->mCountry);
    str += QStringLiteral("  Label: %1\n").arg(d->mLabel);
    str += QStringLiteral("  Geo: %1\n").arg(d->mGeo.toString());
    str += QLatin1String("}\n");

    return str;
}

static QString countryCodeFromLocale()
{
    const auto n = QLocale().name(); // this is in the form <lang>_<COUNTRY>, with the exception of 'C'
    const auto idx = n.indexOf(QLatin1Char('_'));
    if (idx > 0) {
        return n.mid(idx + 1).toLower();
    }
    return {};
}

QString Address::formattedAddress(const QString &realName, const QString &orgaName) const
{
    QString ciso;
    QString addrTemplate;
    QString ret;

    // FIXME: first check for iso-country-field and prefer that one
    if (!country().isEmpty()) {
        ciso = countryToISO(country());
    } else {
        // fall back to our own country
        ciso = countryCodeFromLocale();
    }
    static KConfig entry(QStringLiteral(":/org.kde.kcontacts/addressformatrc"));

    KConfigGroup group = entry.group(ciso);
    // decide whether this needs special business address formatting
    if (orgaName.isEmpty()) {
        addrTemplate = group.readEntry("AddressFormat");
    } else {
        addrTemplate = group.readEntry("BusinessAddressFormat");
        if (addrTemplate.isEmpty()) {
            addrTemplate = group.readEntry("AddressFormat");
        }
    }

    // in the case there's no format found at all, default to what we've always
    // used:
    if (addrTemplate.isEmpty()) {
        qCWarning(KCONTACTS_LOG) << "address format database incomplete"
                                 << "(no format for locale" << ciso
                                 << "found). Using default address formatting.";
        addrTemplate = QStringLiteral("%0(%n\\n)%0(%cm\\n)%0(%s\\n)%0(PO BOX %p\\n)%0(%l%w%r)%,%z");
    }

    // scan
    parseAddressTemplateSection(addrTemplate, ret, realName, orgaName, *this);

    // now add the country line if needed (formatting this time according to
    // the rules of our own system country )
    if (!country().isEmpty()) {
        KConfigGroup group = entry.group(countryCodeFromLocale());
        QString cpos = group.readEntry("AddressCountryPosition");
        if (QLatin1String("BELOW") == cpos || cpos.isEmpty()) {
            ret = ret + QLatin1String("\n\n") + country().toUpper();
        } else if (QLatin1String("below") == cpos) {
            ret = ret + QLatin1String("\n\n") + country();
        } else if (QLatin1String("ABOVE") == cpos) {
            ret = country().toUpper() + QLatin1String("\n\n") + ret;
        } else if (QLatin1String("above") == cpos) {
            ret = country() + QLatin1String("\n\n") + ret;
        }
    }

    return ret;
}

struct IsoCache {
    QHash<QString, QString> countryToIso;
    QHash<QString, QString> isoToCountry;
};
Q_GLOBAL_STATIC(IsoCache, sIsoCache)

QString Address::countryToISO(const QString &cname)
{
    // we search a map file for translations from country names to
    // iso codes, storing caching things in a QHash for faster future
    // access.

    const auto it = sIsoCache->countryToIso.constFind(cname);
    if (it != sIsoCache->countryToIso.constEnd()) {
        return it.value();
    }

    QFile file(QStringLiteral(":/org.kde.kcontacts/countrytransl.map"));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream s(&file);
        QString strbuf = s.readLine();
        while (!strbuf.isEmpty()) {
            const auto countryInfo = strbuf.splitRef(QLatin1Char('\t'), QString::KeepEmptyParts);
            if (countryInfo[0] == cname) {
                file.close();
                const auto iso = countryInfo[1].toString();
                sIsoCache->countryToIso.insert(cname, iso);
                return iso;
            }
            strbuf = s.readLine();
        }
        file.close();
    }

    return {};
}

QString Address::ISOtoCountry(const QString &ISOname)
{
    // get country name from ISO country code (e.g. "no" -> i18n("Norway"))
    const auto iso = ISOname.simplified().toLower();
    if (iso.simplified().isEmpty()) {
        return QString();
    }

    const auto it = sIsoCache->isoToCountry.constFind(iso);
    if (it != sIsoCache->isoToCountry.constEnd()) {
        return it.value();
    }

    QFile file(QStringLiteral(":/org.kde.kcontacts/countrytransl.map"));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream s(&file);
        QString searchStr = QLatin1Char('\t') + iso;
        QString strbuf = s.readLine();
        while (!strbuf.isEmpty()) {
            int pos;
            if ((pos = strbuf.indexOf(searchStr)) != -1) {
                file.close();
                const auto country = i18nd("iso_3166-1", strbuf.leftRef(pos).toUtf8().constData());
                sIsoCache->isoToCountry.insert(iso, country);
                sIsoCache->countryToIso.insert(country, iso);
                return country;
            }
            strbuf = s.readLine();
        }
        file.close();
    }

    return ISOname;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Address &addr)
{
    return s << addr.d->mId << (uint)addr.d->mType << addr.d->mPostOfficeBox
             << addr.d->mExtended << addr.d->mStreet << addr.d->mLocality
             << addr.d->mRegion << addr.d->mPostalCode << addr.d->mCountry
             << addr.d->mLabel << addr.d->mEmpty << addr.d->mGeo;
}

QDataStream &KContacts::operator>>(QDataStream &s, Address &addr)
{
    uint type;
    s >> addr.d->mId >> type >> addr.d->mPostOfficeBox >> addr.d->mExtended
    >> addr.d->mStreet >> addr.d->mLocality >> addr.d->mRegion
    >> addr.d->mPostalCode >> addr.d->mCountry >> addr.d->mLabel
    >> addr.d->mEmpty >> addr.d->mGeo;

    addr.d->mType = Address::Type(type);

    return s;
}
