/*
    This file is part of libkabc.
    Copyright (c) 2002 Cornelius Schumacher <schumacher@kde.org>

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

#include <klocale.h>
#include <klocalizedstring.h>
#include <kconfig.h>

#include <kconfiggroup.h>

#include "field.h"

using namespace KABC;

class Field::Private
{
public:
    Private(int fieldId, int category = 0,
            const QString &label = QString(),
            const QString &key = QString(),
            const QString &app = QString())
        : mFieldId(fieldId), mCategory(category), mLabel(label),
          mKey(key), mApp(app) {}

    enum FieldId {
        CustomField,
        FormattedName,
        FamilyName,
        GivenName,
        AdditionalName,
        Prefix,
        Suffix,
        NickName,
        Birthday,
        HomeAddressStreet,
        HomeAddressPostOfficeBox,
        HomeAddressLocality,
        HomeAddressRegion,
        HomeAddressPostalCode,
        HomeAddressCountry,
        HomeAddressLabel,
        BusinessAddressStreet,
        BusinessAddressPostOfficeBox,
        BusinessAddressLocality,
        BusinessAddressRegion,
        BusinessAddressPostalCode,
        BusinessAddressCountry,
        BusinessAddressLabel,
        HomePhone,
        BusinessPhone,
        MobilePhone,
        HomeFax,
        BusinessFax,
        CarPhone,
        Isdn,
        Pager,
        Email,
        Mailer,
        Title,
        Role,
        Organization,
        Department,
        Note,
        Url
    };

    int fieldId()
    {
        return mFieldId;
    }
    int category()
    {
        return mCategory;
    }

    QString label()
    {
        return mLabel;
    }
    QString key()
    {
        return mKey;
    }
    QString app()
    {
        return mApp;
    }

    static Field::List mAllFields;
    static Field::List mDefaultFields;
    static Field::List mCustomFields;

private:
    int mFieldId;
    int mCategory;

    QString mLabel;
    QString mKey;
    QString mApp;
};

Field::List Field::Private::mAllFields;
Field::List Field::Private::mDefaultFields;
Field::List Field::Private::mCustomFields;

Field::Field(Private *p)
    : d(p)
{
}

Field::~Field()
{
    delete d;
}

QString Field::label()
{
    switch (d->fieldId()) {
    case Private::FormattedName:
        return Addressee::formattedNameLabel();
    case Private::FamilyName:
        return Addressee::familyNameLabel();
    case Private::GivenName:
        return Addressee::givenNameLabel();
    case Private::AdditionalName:
        return Addressee::additionalNameLabel();
    case Private::Prefix:
        return Addressee::prefixLabel();
    case Private::Suffix:
        return Addressee::suffixLabel();
    case Private::NickName:
        return Addressee::nickNameLabel();
    case Private::Birthday:
        return Addressee::birthdayLabel();
    case Private::HomeAddressStreet:
        return Addressee::homeAddressStreetLabel();
    case Private::HomeAddressPostOfficeBox:
        return Addressee::homeAddressPostOfficeBoxLabel();
    case Private::HomeAddressLocality:
        return Addressee::homeAddressLocalityLabel();
    case Private::HomeAddressRegion:
        return Addressee::homeAddressRegionLabel();
    case Private::HomeAddressPostalCode:
        return Addressee::homeAddressPostalCodeLabel();
    case Private::HomeAddressCountry:
        return Addressee::homeAddressCountryLabel();
    case Private::HomeAddressLabel:
        return Addressee::homeAddressLabelLabel();
    case Private::BusinessAddressStreet:
        return Addressee::businessAddressStreetLabel();
    case Private::BusinessAddressPostOfficeBox:
        return Addressee::businessAddressPostOfficeBoxLabel();
    case Private::BusinessAddressLocality:
        return Addressee::businessAddressLocalityLabel();
    case Private::BusinessAddressRegion:
        return Addressee::businessAddressRegionLabel();
    case Private::BusinessAddressPostalCode:
        return Addressee::businessAddressPostalCodeLabel();
    case Private::BusinessAddressCountry:
        return Addressee::businessAddressCountryLabel();
    case Private::BusinessAddressLabel:
        return Addressee::businessAddressLabelLabel();
    case Private::HomePhone:
        return Addressee::homePhoneLabel();
    case Private::BusinessPhone:
        return Addressee::businessPhoneLabel();
    case Private::MobilePhone:
        return Addressee::mobilePhoneLabel();
    case Private::HomeFax:
        return Addressee::homeFaxLabel();
    case Private::BusinessFax:
        return Addressee::businessFaxLabel();
    case Private::CarPhone:
        return Addressee::carPhoneLabel();
    case Private::Isdn:
        return Addressee::isdnLabel();
    case Private::Pager:
        return Addressee::pagerLabel();
    case Private::Email:
        return Addressee::emailLabel();
    case Private::Mailer:
        return Addressee::mailerLabel();
    case Private::Title:
        return Addressee::titleLabel();
    case Private::Role:
        return Addressee::roleLabel();
    case Private::Organization:
        return Addressee::organizationLabel();
    case Private::Department:
        return Addressee::departmentLabel();
    case Private::Note:
        return Addressee::noteLabel();
    case Private::Url:
        return Addressee::urlLabel();
    case Private::CustomField:
        return d->label();
    default:
        return i18n("Unknown Field");
    }
}

int Field::category()
{
    return d->category();
}

QString Field::categoryLabel(int category)
{
    switch (category) {
    case All:
        return i18n("All");
    case Frequent:
        return i18n("Frequent");
    case Address:
        return i18nc("street/postal", "Address");
    case Email:
        return i18n("Email");
    case Personal:
        return i18n("Personal");
    case Organization:
        return i18n("Organization");
    case CustomCategory:
        return i18n("Custom");
    default:
        return i18n("Undefined");
    }
}

QString Field::value(const KABC::Addressee &a)
{
    switch (d->fieldId()) {
    case Private::FormattedName:
        return a.formattedName();
    case Private::FamilyName:
        return a.familyName();
    case Private::GivenName:
        return a.givenName();
    case Private::AdditionalName:
        return a.additionalName();
    case Private::Prefix:
        return a.prefix();
    case Private::Suffix:
        return a.suffix();
    case Private::NickName:
        return a.nickName();
    case Private::Mailer:
        return a.mailer();
    case Private::Title:
        return a.title();
    case Private::Role:
        return a.role();
    case Private::Organization:
        return a.organization();
    case Private::Department:
        return a.department();
    case Private::Note:
        return a.note();
    case Private::Email:
        return a.preferredEmail();
    case Private::Birthday:
        if (a.birthday().isValid()) {
            return a.birthday().date().toString(Qt::ISODate);
        } else {
            return QString();
        }
    case Private::Url:
        return a.url().toDisplayString();
    case Private::HomePhone: {
        PhoneNumber::List::ConstIterator it;

        {
            // check for preferred number
            const PhoneNumber::List list = a.phoneNumbers(PhoneNumber::Home | PhoneNumber::Pref);
            for (it = list.begin(); it != list.end(); ++it) {
                if (((*it).type() & ~(PhoneNumber::Pref)) == PhoneNumber::Home) {
                    return (*it).number();
                }
            }
        }

        {
            // check for normal home number
            const PhoneNumber::List list = a.phoneNumbers(PhoneNumber::Home);
            for (it = list.begin(); it != list.end(); ++it) {
                if (((*it).type() & ~(PhoneNumber::Pref)) == PhoneNumber::Home) {
                    return (*it).number();
                }
            }
        }

        return QString();
    }
    case Private::BusinessPhone: {
        PhoneNumber::List::ConstIterator it;

        {
            // check for preferred number
            const PhoneNumber::List list = a.phoneNumbers(PhoneNumber::Work | PhoneNumber::Pref);
            for (it = list.begin(); it != list.end(); ++it) {
                if (((*it).type() & ~(PhoneNumber::Pref)) == PhoneNumber::Work) {
                    return (*it).number();
                }
            }
        }

        {
            // check for normal work number
            const PhoneNumber::List list = a.phoneNumbers(PhoneNumber::Work);
            for (it = list.begin(); it != list.end(); ++it) {
                if (((*it).type() & ~(PhoneNumber::Pref)) == PhoneNumber::Work) {
                    return (*it).number();
                }
            }
        }

        return QString();
    }
    case Private::MobilePhone:
        return a.phoneNumber(PhoneNumber::Cell).number();
    case Private::HomeFax:
        return a.phoneNumber(PhoneNumber::Home | PhoneNumber::Fax).number();
    case Private::BusinessFax:
        return a.phoneNumber(PhoneNumber::Work | PhoneNumber::Fax).number();
    case Private::CarPhone:
        return a.phoneNumber(PhoneNumber::Car).number();
    case Private::Isdn:
        return a.phoneNumber(PhoneNumber::Isdn).number();
    case Private::Pager:
        return a.phoneNumber(PhoneNumber::Pager).number();
    case Private::HomeAddressStreet:
        return a.address(Address::Home).street();
    case Private::HomeAddressPostOfficeBox:
        return a.address(Address::Home).postOfficeBox();
    case Private::HomeAddressLocality:
        return a.address(Address::Home).locality();
    case Private::HomeAddressRegion:
        return a.address(Address::Home).region();
    case Private::HomeAddressPostalCode:
        return a.address(Address::Home).postalCode();
    case Private::HomeAddressCountry:
        return a.address(Address::Home).country();
    case Private::BusinessAddressStreet:
        return a.address(Address::Work).street();
    case Private::BusinessAddressPostOfficeBox:
        return a.address(Address::Work).postOfficeBox();
    case Private::BusinessAddressLocality:
        return a.address(Address::Work).locality();
    case Private::BusinessAddressRegion:
        return a.address(Address::Work).region();
    case Private::BusinessAddressPostalCode:
        return a.address(Address::Work).postalCode();
    case Private::BusinessAddressCountry:
        return a.address(Address::Work).country();
    case Private::CustomField:
        return a.custom(d->app(), d->key());
    default:
        return QString();
    }
}

bool Field::setValue(KABC::Addressee &a, const QString &value)
{
    switch (d->fieldId()) {
    case Private::FormattedName:
        a.setFormattedName(value);
        return true;
    case Private::FamilyName:
        a.setFamilyName(value);
        return true;
    case Private::GivenName:
        a.setGivenName(value);
        return true;
    case Private::AdditionalName:
        a.setAdditionalName(value);
        return true;
    case Private::Prefix:
        a.setPrefix(value);
        return true;
    case Private::Suffix:
        a.setSuffix(value);
        return true;
    case Private::NickName:
        a.setNickName(value);
        return true;
    case Private::Mailer:
        a.setMailer(value);
        return true;
    case Private::Title:
        a.setTitle(value);
        return true;
    case Private::Role:
        a.setRole(value);
        return true;
    case Private::Organization:
        a.setOrganization(value);
        return true;
    case Private::Department:
        a.setDepartment(value);
        return true;
    case Private::Note:
        a.setNote(value);
        return true;
    case Private::Birthday:
        a.setBirthday(QDateTime::fromString(value, Qt::ISODate));
        return true;
    case Private::CustomField:
        a.insertCustom(d->app(), d->key(), value);
        return true;
    default:
        return false;
    }
}

QString Field::sortKey(const KABC::Addressee &a)
{
    switch (d->fieldId()) {
    case Private::FormattedName:
        return a.formattedName();
    case Private::FamilyName:
        return a.familyName();
    case Private::GivenName:
        return a.givenName();
    case Private::AdditionalName:
        return a.additionalName();
    case Private::Prefix:
        return a.prefix();
    case Private::Suffix:
        return a.suffix();
    case Private::NickName:
        return a.nickName();
    case Private::Mailer:
        return a.mailer();
    case Private::Title:
        return a.title();
    case Private::Role:
        return a.role();
    case Private::Organization:
        return a.organization();
    case Private::Department:
        return a.department();
    case Private::Note:
        return a.note();
    case Private::Birthday:
        if (a.birthday().isValid()) {
            QDate date = a.birthday().date();
            QString key;
            key.sprintf("%02d-%02d", date.month(), date.day());
            return key;
        } else {
            return QLatin1String("00-00");
        }
    default:
        return value(a).toLower();
    }
}

bool Field::isCustom()
{
    return d->fieldId() == Private::CustomField;
}

Field::List Field::allFields()
{
    if (Private::mAllFields.isEmpty()) {
        createField(Private::FormattedName, Frequent);
        createField(Private::FamilyName, Frequent);
        createField(Private::GivenName, Frequent);
        createField(Private::AdditionalName);
        createField(Private::Prefix);
        createField(Private::Suffix);
        createField(Private::NickName, Personal);
        createField(Private::Birthday, Personal);
        createField(Private::HomeAddressStreet, Address | Personal);
        createField(Private::HomeAddressPostOfficeBox, Address | Personal);
        createField(Private::HomeAddressLocality, Address | Personal);
        createField(Private::HomeAddressRegion, Address | Personal);
        createField(Private::HomeAddressPostalCode, Address | Personal);
        createField(Private::HomeAddressCountry, Address | Personal);
        createField(Private::HomeAddressLabel, Address | Personal);
        createField(Private::BusinessAddressStreet, Address | Organization);
        createField(Private::BusinessAddressPostOfficeBox, Address | Organization);
        createField(Private::BusinessAddressLocality, Address | Organization);
        createField(Private::BusinessAddressRegion, Address | Organization);
        createField(Private::BusinessAddressPostalCode, Address | Organization);
        createField(Private::BusinessAddressCountry, Address | Organization);
        createField(Private::BusinessAddressLabel, Address | Organization);
        createField(Private::HomePhone, Personal | Frequent);
        createField(Private::BusinessPhone, Organization | Frequent);
        createField(Private::MobilePhone, Frequent);
        createField(Private::HomeFax);
        createField(Private::BusinessFax);
        createField(Private::CarPhone);
        createField(Private::Isdn);
        createField(Private::Pager);
        createField(Private::Email, Email | Frequent);
        createField(Private::Mailer, Email);
        createField(Private::Title, Organization);
        createField(Private::Role, Organization);
        createField(Private::Organization, Organization);
        createField(Private::Department, Organization);
        createField(Private::Note);
        createField(Private::Url);
    }

    return Private::mAllFields;
}

Field::List Field::defaultFields()
{
    if (Private::mDefaultFields.isEmpty()) {
        createDefaultField(Private::FormattedName);
        createDefaultField(Private::Email);
    }

    return Private::mDefaultFields;
}

void Field::createField(int id, int category)
{
    Private::mAllFields.append(new Field(new Private(id, category)));
}

void Field::createDefaultField(int id, int category)
{
    Private::mDefaultFields.append(new Field(new Private(id, category)));
}

void Field::deleteFields()
{
    Field::List::ConstIterator it;

    for (it = Private::mAllFields.constBegin(); it != Private::mAllFields.constEnd(); ++it) {
        delete(*it);
    }
    Private::mAllFields.clear();

    for (it = Private::mDefaultFields.constBegin(); it != Private::mDefaultFields.constEnd(); ++it) {
        delete(*it);
    }
    Private::mDefaultFields.clear();

    for (it = Private::mCustomFields.constBegin(); it != Private::mCustomFields.constEnd(); ++it) {
        delete(*it);
    }
    Private::mCustomFields.clear();
}

void Field::saveFields(const QString &identifier,
                       const Field::List &fields)
{
    KConfigGroup cg(KSharedConfig::openConfig(), "KABCFields");

    saveFields(cg, identifier, fields);
}

void Field::saveFields(KConfigGroup &cfg, const QString &identifier,
                       const Field::List &fields)
{
    QList<int> fieldIds;

    int custom = 0;
    Field::List::ConstIterator it;
    for (it = fields.begin(); it != fields.end(); ++it) {
        fieldIds.append((*it)->d->fieldId());
        if ((*it)->isCustom()) {
            QStringList customEntry;
            customEntry << (*it)->d->label();
            customEntry << (*it)->d->key();
            customEntry << (*it)->d->app();
            cfg.writeEntry(QLatin1String("KABC_CustomEntry_") + identifier + QLatin1Char('_') +
                           QString::number(custom++), customEntry);
        }
    }

    cfg.writeEntry(identifier, fieldIds);
}

Field::List Field::restoreFields(const QString &identifier)
{
    KConfigGroup cg(KSharedConfig::openConfig(), "KABCFields");

    return restoreFields(cg, identifier);
}

Field::List Field::restoreFields(const KConfigGroup &cfg, const QString &identifier)
{
    const QList<int> fieldIds = cfg.readEntry(identifier, QList<int>());

    Field::List fields;

    int custom = 0;
    QList<int>::ConstIterator it;
    for (it = fieldIds.begin(); it != fieldIds.end(); ++it) {
        Private *f = 0;
        if ((*it) == Private::CustomField) {
            QStringList customEntry = cfg.readEntry(QLatin1String("KABC_CustomEntry_") +
                                                    identifier + QLatin1Char('_') +
                                                    QString::number(custom++), QStringList());
            f = new Private(*it, CustomCategory, customEntry[ 0 ],
                            customEntry[ 1 ], customEntry[ 2 ]);
        } else {
            f = new Private(*it);
        }
        fields.append(new Field(f));
    }

    return fields;
}

bool Field::equals(Field *field)
{
    bool sameId = (d->fieldId() == field->d->fieldId());

    if (!sameId) {
        return false;
    }

    if (d->fieldId() != Private::CustomField) {
        return true;
    }

    return d->key() == field->d->key();
}

Field *Field::createCustomField(const QString &label, int category,
                                const QString &key, const QString &app)
{
    Field *field = new Field(new Private(Private::CustomField,
                                         category | CustomCategory,
                                         label, key, app));
    Private::mCustomFields.append(field);

    return field;
}
