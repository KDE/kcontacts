/*
    This file is part of libkabc.
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>

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

#include "vcardtool.h"
#include "key.h"
#include "picture.h"
#include "secrecy.h"
#include "sound.h"

#include <QtCore/QString>
#include <QtCore/QBuffer>

using namespace KABC;

static bool needsEncoding(const QString &value)
{
    uint length = value.length();
    for (uint i = 0; i < length; ++i) {
        char c = value.at(i).toLatin1();
        if ((c < 33 || c > 126) && c != ' ' && c != '=') {
            return true;
        }
    }

    return false;
}

VCardTool::VCardTool()
{
    mAddressTypeMap.insert(QLatin1String("dom"), Address::Dom);
    mAddressTypeMap.insert(QLatin1String("intl"), Address::Intl);
    mAddressTypeMap.insert(QLatin1String("postal"), Address::Postal);
    mAddressTypeMap.insert(QLatin1String("parcel"), Address::Parcel);
    mAddressTypeMap.insert(QLatin1String("home"), Address::Home);
    mAddressTypeMap.insert(QLatin1String("work"), Address::Work);
    mAddressTypeMap.insert(QLatin1String("pref"), Address::Pref);

    mPhoneTypeMap.insert(QLatin1String("HOME"), PhoneNumber::Home);
    mPhoneTypeMap.insert(QLatin1String("WORK"), PhoneNumber::Work);
    mPhoneTypeMap.insert(QLatin1String("MSG"), PhoneNumber::Msg);
    mPhoneTypeMap.insert(QLatin1String("PREF"), PhoneNumber::Pref);
    mPhoneTypeMap.insert(QLatin1String("VOICE"), PhoneNumber::Voice);
    mPhoneTypeMap.insert(QLatin1String("FAX"), PhoneNumber::Fax);
    mPhoneTypeMap.insert(QLatin1String("CELL"), PhoneNumber::Cell);
    mPhoneTypeMap.insert(QLatin1String("VIDEO"), PhoneNumber::Video);
    mPhoneTypeMap.insert(QLatin1String("BBS"), PhoneNumber::Bbs);
    mPhoneTypeMap.insert(QLatin1String("MODEM"), PhoneNumber::Modem);
    mPhoneTypeMap.insert(QLatin1String("CAR"), PhoneNumber::Car);
    mPhoneTypeMap.insert(QLatin1String("ISDN"), PhoneNumber::Isdn);
    mPhoneTypeMap.insert(QLatin1String("PCS"), PhoneNumber::Pcs);
    mPhoneTypeMap.insert(QLatin1String("PAGER"), PhoneNumber::Pager);
}

VCardTool::~VCardTool()
{
}

QByteArray VCardTool::exportVCards(const Addressee::List &list, VCard::Version version) const
{
    return createVCards(list, version, true /*export vcard*/);
}

QByteArray VCardTool::createVCards(const Addressee::List &list, VCard::Version version) const
{
    return createVCards(list, version, false /*don't export*/);
}

QByteArray VCardTool::createVCards(const Addressee::List &list,
                                   VCard::Version version, bool exportVcard) const
{
    VCard::List vCardList;

    Addressee::List::ConstIterator addrIt;
    Addressee::List::ConstIterator listEnd(list.constEnd());
    for (addrIt = list.constBegin(); addrIt != listEnd; ++addrIt) {
        VCard card;
        QStringList::ConstIterator strIt;

        // ADR + LABEL
        const Address::List addresses = (*addrIt).addresses();
        for (Address::List::ConstIterator it = addresses.begin(); it != addresses.end(); ++it) {
            QStringList address;

            const bool isEmpty = ((*it).postOfficeBox().isEmpty() &&
                                  (*it).extended().isEmpty() &&
                                  (*it).street().isEmpty() &&
                                  (*it).locality().isEmpty() &&
                                  (*it).region().isEmpty() &&
                                  (*it).postalCode().isEmpty() &&
                                  (*it).country().isEmpty());

            address.append((*it).postOfficeBox().replace(QLatin1Char(';'),
                           QLatin1String("\\;")));

            address.append((*it).extended().replace(QLatin1Char(';'),
                                                    QLatin1String("\\;")));

            address.append((*it).street().replace(QLatin1Char(';'),
                                                  QLatin1String("\\;")));

            address.append((*it).locality().replace(QLatin1Char(';'),
                                                    QLatin1String("\\;")));

            address.append((*it).region().replace(QLatin1Char(';'),
                                                  QLatin1String("\\;")));

            address.append((*it).postalCode().replace(QLatin1Char(';'),
                           QLatin1String("\\;")));

            address.append((*it).country().replace(QLatin1Char(';'),
                                                   QLatin1String("\\;")));

            VCardLine adrLine(QLatin1String("ADR"), address.join(QLatin1String(";")));
            if (version == VCard::v2_1 && needsEncoding(address.join(QLatin1String(";")))) {
                adrLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
                adrLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
            }

            VCardLine labelLine(QLatin1String("LABEL"), (*it).label());
            if (version == VCard::v2_1 && needsEncoding((*it).label())) {
                labelLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
                labelLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
            }

            const bool hasLabel = !(*it).label().isEmpty();
            QMap<QString, Address::TypeFlag>::ConstIterator typeIt;
            for (typeIt = mAddressTypeMap.constBegin();
                    typeIt != mAddressTypeMap.constEnd(); ++typeIt) {
                if (typeIt.value() & (*it).type()) {
                    adrLine.addParameter(QLatin1String("TYPE"), typeIt.key());
                    if (hasLabel) {
                        labelLine.addParameter(QLatin1String("TYPE"), typeIt.key());
                    }
                }
            }

            if (!isEmpty) {
                card.addLine(adrLine);
            }
            if (hasLabel) {
                card.addLine(labelLine);
            }
        }

        // BDAY
        card.addLine(VCardLine(QLatin1String("BDAY"), createDateTime((*addrIt).birthday())));

        // CATEGORIES only > 2.1
        if (version != VCard::v2_1) {
            QStringList categories = (*addrIt).categories();
            QStringList::Iterator catIt;
            QStringList::Iterator catEnd(categories.end());
            for (catIt = categories.begin(); catIt != catEnd; ++catIt) {
                (*catIt).replace(QLatin1Char(','), QLatin1String("\\,"));
            }

            VCardLine catLine(QLatin1String("CATEGORIES"), categories.join(QLatin1String(",")));
            card.addLine(catLine);
        }

        // CLASS only for version == 3.0
        if (version == VCard::v3_0) {
            card.addLine(createSecrecy((*addrIt).secrecy()));
        }

        // EMAIL
        const QStringList emails = (*addrIt).emails();
        bool pref = true;
        for (strIt = emails.begin(); strIt != emails.end(); ++strIt) {
            VCardLine line(QLatin1String("EMAIL"), *strIt);
            if (pref == true && emails.count() > 1) {
                line.addParameter(QLatin1String("TYPE"), QLatin1String("PREF"));
                pref = false;
            }
            card.addLine(line);
        }

        // FN required for only version > 2.1
        VCardLine fnLine(QLatin1String("FN"), (*addrIt).formattedName());
        if (version == VCard::v2_1 && needsEncoding((*addrIt).formattedName())) {
            fnLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
            fnLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
        }
        card.addLine(fnLine);

        // GEO
        const Geo geo = (*addrIt).geo();
        if (geo.isValid()) {
            QString str;
            str.sprintf("%.6f;%.6f", geo.latitude(), geo.longitude());
            card.addLine(VCardLine(QLatin1String("GEO"), str));
        }

        // KEY
        const Key::List keys = (*addrIt).keys();
        Key::List::ConstIterator keyIt;
        Key::List::ConstIterator keyEnd(keys.end());
        for (keyIt = keys.begin(); keyIt != keyEnd; ++keyIt) {
            card.addLine(createKey(*keyIt));
        }

        // LOGO
        card.addLine(createPicture(QLatin1String("LOGO"), (*addrIt).logo()));

        // MAILER only for version < 4.0
        if (version != VCard::v4_0) {
            VCardLine mailerLine(QLatin1String("MAILER"), (*addrIt).mailer());
            if (version == VCard::v2_1 && needsEncoding((*addrIt).mailer())) {
                mailerLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
                mailerLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
            }
            card.addLine(mailerLine);
        }

        // N required for only version < 4.0
        QStringList name;
        name.append((*addrIt).familyName().replace(QLatin1Char(';'),
                    QLatin1String("\\;")));

        name.append((*addrIt).givenName().replace(QLatin1Char(';'),
                    QLatin1String("\\;")));

        name.append((*addrIt).additionalName().replace(QLatin1Char(';'),
                    QLatin1String("\\;")));

        name.append((*addrIt).prefix().replace(QLatin1Char(';'),
                                               QLatin1String("\\;")));

        name.append((*addrIt).suffix().replace(QLatin1Char(';'),
                                               QLatin1String("\\;")));

        VCardLine nLine(QLatin1String("N"), name.join(QLatin1String(";")));
        if (version == VCard::v2_1 && needsEncoding(name.join(QLatin1String(";")))) {
            nLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
            nLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
        }
        if (version == VCard::v4_0 && !(*addrIt).sortString().isEmpty()) {
            nLine.addParameter(QLatin1String("SORT-AS"), (*addrIt).sortString());
        }

        card.addLine(nLine);

        // NAME only for version < 4.0
        if (version != VCard::v4_0) {
            VCardLine nameLine(QLatin1String("NAME"), (*addrIt).name());
            if (version == VCard::v2_1 && needsEncoding((*addrIt).name())) {
                nameLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
                nameLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
            }
            card.addLine(nameLine);
        }

        // NICKNAME only for version > 2.1
        if (version != VCard::v2_1) {
            card.addLine(VCardLine(QLatin1String("NICKNAME"), (*addrIt).nickName()));
        }

        // NOTE
        VCardLine noteLine(QLatin1String("NOTE"), (*addrIt).note());
        if (version == VCard::v2_1 && needsEncoding((*addrIt).note())) {
            noteLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
            noteLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
        }
        card.addLine(noteLine);

        // ORG
        QStringList organization;
        organization.append((*addrIt).organization().replace(QLatin1Char(';'),
                            QLatin1String("\\;")));
        if (!(*addrIt).department().isEmpty()) {
            organization.append((*addrIt).department().replace(QLatin1Char(';'),
                                QLatin1String("\\;")));
        }
        VCardLine orgLine(QLatin1String("ORG"), organization.join(QLatin1String(";")));
        if (version == VCard::v2_1 && needsEncoding(organization.join(QLatin1String(";")))) {
            orgLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
            orgLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
        }
        card.addLine(orgLine);

        // PHOTO
        card.addLine(createPicture(QLatin1String("PHOTO"), (*addrIt).photo()));

        // PROID only for version > 2.1
        if (version != VCard::v2_1) {
            card.addLine(VCardLine(QLatin1String("PRODID"), (*addrIt).productId()));
        }

        // REV
        card.addLine(VCardLine(QLatin1String("REV"), createDateTime((*addrIt).revision())));

        // ROLE
        VCardLine roleLine(QLatin1String("ROLE"), (*addrIt).role());
        if (version == VCard::v2_1 && needsEncoding((*addrIt).role())) {
            roleLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
            roleLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
        }
        card.addLine(roleLine);

        // SORT-STRING
        if (version == VCard::v3_0) {
            card.addLine(VCardLine(QLatin1String("SORT-STRING"), (*addrIt).sortString()));
        }

        // SOUND
        card.addLine(createSound((*addrIt).sound()));

        // TEL
        const PhoneNumber::List phoneNumbers = (*addrIt).phoneNumbers();
        PhoneNumber::List::ConstIterator phoneIt;
        PhoneNumber::List::ConstIterator phoneEnd(phoneNumbers.end());
        for (phoneIt = phoneNumbers.begin(); phoneIt != phoneEnd; ++phoneIt) {
            VCardLine line(QLatin1String("TEL"), (*phoneIt).number());

            QMap<QString, PhoneNumber::TypeFlag>::ConstIterator typeIt;
            QMap<QString, PhoneNumber::TypeFlag>::ConstIterator typeEnd(mPhoneTypeMap.constEnd());
            for (typeIt = mPhoneTypeMap.constBegin(); typeIt != typeEnd; ++typeIt) {
                if (typeIt.value() & (*phoneIt).type()) {
                    line.addParameter(QLatin1String("TYPE"), typeIt.key());
                }
            }

            card.addLine(line);
        }

        // TITLE
        VCardLine titleLine(QLatin1String("TITLE"), (*addrIt).title());
        if (version == VCard::v2_1 && needsEncoding((*addrIt).title())) {
            titleLine.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
            titleLine.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
        }
        card.addLine(titleLine);

        // TZ
        const TimeZone timeZone = (*addrIt).timeZone();
        if (timeZone.isValid()) {
            QString str;

            int neg = 1;
            if (timeZone.offset() < 0) {
                neg = -1;
            }

            str.sprintf("%c%02d:%02d", (timeZone.offset() >= 0 ? '+' : '-'),
                        (timeZone.offset() / 60) * neg,
                        (timeZone.offset() % 60) * neg);

            card.addLine(VCardLine(QLatin1String("TZ"), str));
        }

        // UID
        card.addLine(VCardLine(QLatin1String("UID"), (*addrIt).uid()));

        // URL
        card.addLine(VCardLine(QLatin1String("URL"), (*addrIt).url().url()));

        // VERSION
        if (version == VCard::v2_1) {
            card.addLine(VCardLine(QLatin1String("VERSION"), QLatin1String("2.1")));
        } else if (version == VCard::v3_0) {
            card.addLine(VCardLine(QLatin1String("VERSION"), QLatin1String("3.0")));
        } else if (version == VCard::v4_0) {
            card.addLine(VCardLine(QLatin1String("VERSION"), QLatin1String("4.0")));
        }

        // X-
        const QStringList customs = (*addrIt).customs();
        for (strIt = customs.begin(); strIt != customs.end(); ++strIt) {
            QString identifier = QLatin1String("X-") +
                                 (*strIt).left((*strIt).indexOf(QLatin1Char(':')));
            const QString value = (*strIt).mid((*strIt).indexOf(QLatin1Char(':')) + 1);
            if (value.isEmpty()) {
                continue;
            }
            //Convert to standard identifier
            if (exportVcard) {
                if (identifier == QLatin1String("X-messaging/aim-All")) {
                    identifier = QLatin1String("X-AIM");
                } else if (identifier == QLatin1String("X-messaging/icq-All")) {
                    identifier = QLatin1String("X-ICQ");
                } else if (identifier == QLatin1String("X-messaging/xmpp-All")) {
                    identifier = QLatin1String("X-JABBER");
                } else if (identifier == QLatin1String("X-messaging/msn-All")) {
                    identifier = QLatin1String("X-MSN");
                } else if (identifier == QLatin1String("X-messaging/yahoo-All")) {
                    identifier = QLatin1String("X-YAHOO");
                } else if (identifier == QLatin1String("X-messaging/gadu-All")) {
                    identifier = QLatin1String("X-GADUGADU");
                } else if (identifier == QLatin1String("X-messaging/skype-All")) {
                    identifier = QLatin1String("X-SKYPE");
                } else if (identifier == QLatin1String("X-messaging/groupwise-All")) {
                    identifier = QLatin1String("X-GROUPWISE");
                } else if (identifier == QLatin1String("X-messaging/sms-All")) {
                    identifier = QLatin1String("X-SMS");
                } else if (identifier == QLatin1String("X-messaging/meanwhile-All")) {
                    identifier = QLatin1String("X-MEANWHILE");
                } else if (identifier == QLatin1String("X-messaging/irc-All")) {
                    identifier = QLatin1String("X-IRC");   //Not defined by rfc but need for fixing #300869
                } else if (identifier == QLatin1String("X-messaging/googletalk-All")) {
                    //Not defined by rfc but need for fixing #300869
                    identifier = QLatin1String("X-GOOGLETALK");
                }
            }

            VCardLine line(identifier, value);
            if (version == VCard::v2_1 && needsEncoding(value)) {
                line.addParameter(QLatin1String("charset"), QLatin1String("UTF-8"));
                line.addParameter(QLatin1String("encoding"), QLatin1String("QUOTED-PRINTABLE"));
            }
            card.addLine(line);
        }

        vCardList.append(card);
    }

    return VCardParser::createVCards(vCardList);
}

Addressee::List VCardTool::parseVCards(const QByteArray &vcard) const
{
    static const QLatin1Char semicolonSep(';');
    static const QLatin1Char commaSep(',');
    QString identifier;

    Addressee::List addrList;
    const VCard::List vCardList = VCardParser::parseVCards(vcard);

    VCard::List::ConstIterator cardIt;
    VCard::List::ConstIterator listEnd(vCardList.end());
    for (cardIt = vCardList.begin(); cardIt != listEnd; ++cardIt) {
        Addressee addr;

        const QStringList idents = (*cardIt).identifiers();
        QStringList::ConstIterator identIt;
        QStringList::ConstIterator identEnd(idents.end());
        for (identIt = idents.begin(); identIt != identEnd; ++identIt) {
            const VCardLine::List lines = (*cardIt).lines((*identIt));
            VCardLine::List::ConstIterator lineIt;

            // iterate over the lines
            for (lineIt = lines.begin(); lineIt != lines.end(); ++lineIt) {
                identifier = (*lineIt).identifier().toLower();
                // ADR
                if (identifier == QLatin1String("adr")) {
                    Address address;
                    const QStringList addrParts = splitString(semicolonSep, (*lineIt).value().toString());
                    if (addrParts.count() > 0) {
                        address.setPostOfficeBox(addrParts.at(0));
                    }
                    if (addrParts.count() > 1) {
                        address.setExtended(addrParts.at(1));
                    }
                    if (addrParts.count() > 2) {
                        address.setStreet(addrParts.at(2));
                    }
                    if (addrParts.count() > 3) {
                        address.setLocality(addrParts.at(3));
                    }
                    if (addrParts.count() > 4) {
                        address.setRegion(addrParts.at(4));
                    }
                    if (addrParts.count() > 5) {
                        address.setPostalCode(addrParts.at(5));
                    }
                    if (addrParts.count() > 6) {
                        address.setCountry(addrParts.at(6));
                    }

                    Address::Type type;

                    const QStringList types = (*lineIt).parameters(QLatin1String("type"));
                    QStringList::ConstIterator end(types.end());
                    for (QStringList::ConstIterator it = types.begin(); it != end; ++it) {
                        type |= mAddressTypeMap[(*it).toLower() ];
                    }

                    address.setType(type);
                    if (!(*lineIt).parameter(QLatin1String("label")).isEmpty()) {
                        address.setLabel((*lineIt).parameter(QLatin1String("label")));
                    }
                    addr.insertAddress(address);
                }

                // BDAY
                else if (identifier == QLatin1String("bday")) {
                    addr.setBirthday(parseDateTime((*lineIt).value().toString()));
                }

                // CATEGORIES
                else if (identifier == QLatin1String("categories")) {
                    const QStringList categories = splitString(commaSep, (*lineIt).value().toString());
                    addr.setCategories(categories);
                }

                // CLASS
                else if (identifier == QLatin1String("class")) {
                    addr.setSecrecy(parseSecrecy(*lineIt));
                }

                // EMAIL
                else if (identifier == QLatin1String("email")) {
                    const QStringList types = (*lineIt).parameters(QLatin1String("type"));
                    addr.insertEmail((*lineIt).value().toString(),
                                     types.contains(QLatin1String("PREF")));
                }

                // FN
                else if (identifier == QLatin1String("fn")) {
                    addr.setFormattedName((*lineIt).value().toString());
                }

                // GEO
                else if (identifier == QLatin1String("geo")) {
                    Geo geo;

                    const QStringList geoParts =
                        (*lineIt).value().toString().split(QLatin1Char(';'), QString::KeepEmptyParts);
                    if (geoParts.size() >= 2) {
                        geo.setLatitude(geoParts.at(0).toFloat());
                        geo.setLongitude(geoParts.at(1).toFloat());
                        addr.setGeo(geo);
                    }
                }

                // KEY
                else if (identifier == QLatin1String("key")) {
                    addr.insertKey(parseKey(*lineIt));
                }

                // LABEL
                else if (identifier == QLatin1String("label")) {
                    Address::Type type;

                    const QStringList types = (*lineIt).parameters(QLatin1String("type"));
                    QStringList::ConstIterator end(types.end());
                    for (QStringList::ConstIterator it = types.begin(); it != end; ++it) {
                        type |= mAddressTypeMap[(*it).toLower() ];
                    }

                    bool available = false;
                    KABC::Address::List addressList = addr.addresses();
                    for (KABC::Address::List::Iterator it = addressList.begin();
                            it != addressList.end(); ++it) {
                        if ((*it).type() == type) {
                            (*it).setLabel((*lineIt).value().toString());
                            addr.insertAddress(*it);
                            available = true;
                            break;
                        }
                    }

                    if (!available) {   // a standalone LABEL tag
                        KABC::Address address(type);
                        address.setLabel((*lineIt).value().toString());
                        addr.insertAddress(address);
                    }
                }

                // LOGO
                else if (identifier == QLatin1String("logo")) {
                    addr.setLogo(parsePicture(*lineIt));
                }

                // MAILER
                else if (identifier == QLatin1String("mailer")) {
                    addr.setMailer((*lineIt).value().toString());
                }

                // N
                else if (identifier == QLatin1String("n")) {
                    const QStringList nameParts = splitString(semicolonSep, (*lineIt).value().toString());
                    const int numberOfParts(nameParts.count());
                    if (numberOfParts > 0) {
                        addr.setFamilyName(nameParts.at(0));
                    }
                    if (numberOfParts > 1) {
                        addr.setGivenName(nameParts.at(1));
                    }
                    if (numberOfParts > 2) {
                        addr.setAdditionalName(nameParts.at(2));
                    }
                    if (numberOfParts > 3) {
                        addr.setPrefix(nameParts.at(3));
                    }
                    if (numberOfParts > 4) {
                        addr.setSuffix(nameParts.at(4));
                    }
                    if (!(*lineIt).parameter(QLatin1String("sort-as")).isEmpty()) {
                        addr.setSortString((*lineIt).parameter(QLatin1String("sort-as")));
                    }
                }

                // NAME
                else if (identifier == QLatin1String("name")) {
                    addr.setName((*lineIt).value().toString());
                }

                // NICKNAME
                else if (identifier == QLatin1String("nickname")) {
                    addr.setNickName((*lineIt).value().toString());
                }

                // NOTE
                else if (identifier == QLatin1String("note")) {
                    addr.setNote((*lineIt).value().toString());
                }

                // ORGANIZATION
                else if (identifier == QLatin1String("org")) {
                    const QStringList orgParts = splitString(semicolonSep, (*lineIt).value().toString());
                    if (orgParts.count() > 0) {
                        addr.setOrganization(orgParts.at(0));
                    }
                    if (orgParts.count() > 1) {
                        addr.setDepartment(orgParts.at(1));
                    }
                    if (!(*lineIt).parameter(QLatin1String("sort-as")).isEmpty()) {
                        addr.setSortString((*lineIt).parameter(QLatin1String("sort-as")));
                    }
                }

                // PHOTO
                else if (identifier == QLatin1String("photo")) {
                    addr.setPhoto(parsePicture(*lineIt));
                }

                // PROID
                else if (identifier == QLatin1String("prodid")) {
                    addr.setProductId((*lineIt).value().toString());
                }

                // REV
                else if (identifier == QLatin1String("rev")) {
                    addr.setRevision(parseDateTime((*lineIt).value().toString()));
                }

                // ROLE
                else if (identifier == QLatin1String("role")) {
                    addr.setRole((*lineIt).value().toString());
                }

                // SORT-STRING
                else if (identifier == QLatin1String("sort-string")) {
                    addr.setSortString((*lineIt).value().toString());
                }

                // SOUND
                else if (identifier == QLatin1String("sound")) {
                    addr.setSound(parseSound(*lineIt));
                }

                // TEL
                else if (identifier == QLatin1String("tel")) {
                    PhoneNumber phone;
                    phone.setNumber((*lineIt).value().toString());

                    PhoneNumber::Type type;

                    const QStringList types = (*lineIt).parameters(QLatin1String("type"));
                    QStringList::ConstIterator typeEnd(types.end());
                    for (QStringList::ConstIterator it = types.begin(); it != typeEnd; ++it) {
                        type |= mPhoneTypeMap[(*it).toUpper()];
                    }

                    phone.setType(type);

                    addr.insertPhoneNumber(phone);
                }

                // TITLE
                else if (identifier == QLatin1String("title")) {
                    addr.setTitle((*lineIt).value().toString());
                }

                // TZ
                else if (identifier == QLatin1String("tz")) {
                    TimeZone tz;
                    const QString date = (*lineIt).value().toString();

                    if (!date.isEmpty()) {
                        int hours = date.mid(1, 2).toInt();
                        int minutes = date.mid(4, 2).toInt();
                        int offset = (hours * 60) + minutes;
                        offset = offset * (date[ 0 ] == QLatin1Char('+') ? 1 : -1);

                        tz.setOffset(offset);
                        addr.setTimeZone(tz);
                    }
                }

                // UID
                else if (identifier == QLatin1String("uid")) {
                    addr.setUid((*lineIt).value().toString());
                }

                // URL
                else if (identifier == QLatin1String("url")) {
                    addr.setUrl(QUrl((*lineIt).value().toString()));
                }

                // X-
                else if (identifier.startsWith(QLatin1String("x-"))) {
                    QString ident = (*lineIt).identifier();
                    //X-Evolution
                    if (identifier == QLatin1String("x-evolution-spouse") ||
                            identifier == QLatin1String("x-spouse")) {
                        ident = QLatin1String("X-KADDRESSBOOK-X-SpousesName");
                    } else if (identifier == QLatin1String("x-evolution-blog-url")) {
                        ident = QLatin1String("X-KADDRESSBOOK-BlogFeed");
                    } else if (identifier == QLatin1String("x-evolution-assistant") ||
                               identifier == QLatin1String("x-assistant")) {
                        ident = QLatin1String("X-KADDRESSBOOK-X-AssistantsName");
                    } else if (identifier == QLatin1String("x-evolution-anniversary") ||
                               identifier == QLatin1String("x-anniversary")) {
                        ident = QLatin1String("X-KADDRESSBOOK-X-Anniversary");
                    } else if (identifier == QLatin1String("x-evolution-manager") ||
                               identifier == QLatin1String("x-manager")) {
                        ident = QLatin1String("X-KADDRESSBOOK-X-ManagersName");
                    } else if (identifier == QLatin1String("x-aim")) {
                        ident = QLatin1String("X-messaging/aim-All");
                    } else if (identifier == QLatin1String("x-icq")) {
                        ident = QLatin1String("X-messaging/icq-All");
                    } else if (identifier == QLatin1String("x-jabber")) {
                        ident = QLatin1String("X-messaging/xmpp-All");
                    } else if (identifier == QLatin1String("x-jabber")) {
                        ident = QLatin1String("X-messaging/xmpp-All");
                    } else if (identifier == QLatin1String("x-msn")) {
                        ident = QLatin1String("X-messaging/msn-All");
                    } else if (identifier == QLatin1String("x-yahoo")) {
                        ident = QLatin1String("X-messaging/yahoo-All");
                    } else if (identifier == QLatin1String("x-gadugadu")) {
                        ident = QLatin1String("X-messaging/gadu-All");
                    } else if (identifier == QLatin1String("x-skype")) {
                        ident = QLatin1String("X-messaging/skype-All");
                    } else if (identifier == QLatin1String("x-groupwise")) {
                        ident = QLatin1String("X-messaging/groupwise-All");
                    } else if (identifier == QLatin1String("x-sms")) {
                        ident = QLatin1String("X-messaging/sms-All");
                    } else if (identifier == QLatin1String("x-meanwhile")) {
                        ident = QLatin1String("X-messaging/meanwhile-All");
                    } else if (identifier == QLatin1String("x-irc")) {
                        ident = QLatin1String("X-messaging/irc-All");
                    } else if (identifier == QLatin1String("x-googletalk")) {
                        ident = QLatin1String("X-messaging/googletalk-All");
                    }

                    const QString key = ident.mid(2);
                    const int dash = key.indexOf(QLatin1Char('-'));
                    addr.insertCustom(key.left(dash), key.mid(dash + 1),
                                      (*lineIt).value().toString());
                }
            }
        }

        addrList.append(addr);
    }

    return addrList;
}

QDateTime VCardTool::parseDateTime(const QString &str) const
{
    QDate date;
    QTime time;

    if (str.indexOf(QLatin1Char('-')) == -1) {       // is base format (yyyymmdd)
        date = QDate(str.left(4).toInt(), str.mid(4, 2).toInt(),
                     str.mid(6, 2).toInt());
    } else { // is extended format yyyy-mm-dd
        date = QDate(str.left(4).toInt(), str.mid(5, 2).toInt(),
                     str.mid(8, 2).toInt());
    }

    // does it also contain a time ? (Note: mm, ss are optional according ISO-8601)
    int timeStart = str.indexOf(QLatin1Char('T'));
    if (timeStart >= 0) {
        int hour = 0, minute = 0, second = 0;

        hour = str.mid(timeStart + 1, 2).toInt();    // hour must always be given

        if (str.indexOf(QLatin1Char(':'), timeStart + 1) > 0) {        // extended format (hh:mm:ss)
            if (str.length() >= (timeStart + 5)) {
                minute = str.mid(timeStart + 4, 2).toInt();
                if (str.length() >= (timeStart + 8)) {
                    second = str.mid(timeStart + 7, 2).toInt();
                }
            }
        } else {  // basic format (hhmmss)
            if (str.length() >= (timeStart + 4)) {
                minute = str.mid(timeStart + 3, 2).toInt();
                if (str.length() >= (timeStart + 6)) {
                    second = str.mid(timeStart + 5, 2).toInt();
                }
            }
        }

        time = QTime(hour, minute, second);
    }

    Qt::TimeSpec spec = (str.right(1) == QLatin1String("Z")) ? Qt::UTC : Qt::LocalTime;

    QDateTime dateTime(date);

    // explicitly set the time, which might be invalid, to keep the information
    // that the time is invalid. In createDateTime() the time/invalid flag is
    // checked which omits then to print the timestamp
    // This is needed to reproduce the given string in input
    // e.g. BDAY:2008-12-30
    // without time shall also result in a string without a time
    dateTime.setTime(time);

    dateTime.setTimeSpec(spec);
    return dateTime;
}

QString VCardTool::createDateTime(const QDateTime &dateTime) const
{
    QString str;

    if (dateTime.date().isValid()) {
        str.sprintf("%4d-%02d-%02d", dateTime.date().year(), dateTime.date().month(),
                    dateTime.date().day());
        if (dateTime.time().isValid()) {
            QString tmp;
            tmp.sprintf("T%02d:%02d:%02d", dateTime.time().hour(), dateTime.time().minute(),
                        dateTime.time().second());
            str += tmp;

            if (dateTime.timeSpec() == Qt::UTC) {
                str += QLatin1Char('Z');
            }
        }
    }

    return str;
}

Picture VCardTool::parsePicture(const VCardLine &line) const
{
    Picture pic;

    const QStringList params = line.parameterList();
    QString type;
    if (params.contains(QLatin1String("type"))) {
        type = line.parameter(QLatin1String("type"));
    }
    if (params.contains(QLatin1String("encoding"))) {
        pic.setRawData(line.value().toByteArray(), type);
    } else if (params.contains(QLatin1String("value"))) {
        if (line.parameter(QLatin1String("value")).toLower() == QLatin1String("uri")) {
            pic.setUrl(line.value().toString());
        }
    }

    return pic;
}

VCardLine VCardTool::createPicture(const QString &identifier, const Picture &pic) const
{
    VCardLine line(identifier);

    if (pic.isEmpty()) {
        return line;
    }

    if (pic.isIntern()) {
        line.setValue(pic.rawData());
        line.addParameter(QLatin1String("encoding"), QLatin1String("b"));
        line.addParameter(QLatin1String("type"), pic.type());
    } else {
        line.setValue(pic.url());
        line.addParameter(QLatin1String("value"), QLatin1String("URI"));
    }

    return line;
}

Sound VCardTool::parseSound(const VCardLine &line) const
{
    Sound snd;

    const QStringList params = line.parameterList();
    if (params.contains(QLatin1String("encoding"))) {
        snd.setData(line.value().toByteArray());
    } else if (params.contains(QLatin1String("value"))) {
        if (line.parameter(QLatin1String("value")).toLower() == QLatin1String("uri")) {
            snd.setUrl(line.value().toString());
        }
    }

    /* TODO: support sound types
      if ( params.contains( "type" ) )
        snd.setType( line.parameter( "type" ) );
    */

    return snd;
}

VCardLine VCardTool::createSound(const Sound &snd) const
{
    VCardLine line(QLatin1String("SOUND"));

    if (snd.isIntern()) {
        if (!snd.data().isEmpty()) {
            line.setValue(snd.data());
            line.addParameter(QLatin1String("encoding"), QLatin1String("b"));
            // TODO: need to store sound type!!!
        }
    } else if (!snd.url().isEmpty()) {
        line.setValue(snd.url());
        line.addParameter(QLatin1String("value"), QLatin1String("URI"));
    }

    return line;
}

Key VCardTool::parseKey(const VCardLine &line) const
{
    Key key;

    const QStringList params = line.parameterList();
    if (params.contains(QLatin1String("encoding"))) {
        key.setBinaryData(line.value().toByteArray());
    } else {
        key.setTextData(line.value().toString());
    }

    if (params.contains(QLatin1String("type"))) {
        if (line.parameter(QLatin1String("type")).toLower() == QLatin1String("x509")) {
            key.setType(Key::X509);
        } else if (line.parameter(QLatin1String("type")).toLower() == QLatin1String("pgp")) {
            key.setType(Key::PGP);
        } else {
            key.setType(Key::Custom);
            key.setCustomTypeString(line.parameter(QLatin1String("type")));
        }
    }

    return key;
}

VCardLine VCardTool::createKey(const Key &key) const
{
    VCardLine line(QLatin1String("KEY"));

    if (key.isBinary()) {
        if (!key.binaryData().isEmpty()) {
            line.setValue(key.binaryData());
            line.addParameter(QLatin1String("encoding"), QLatin1String("b"));
        }
    } else if (!key.textData().isEmpty()) {
        line.setValue(key.textData());
    }

    if (key.type() == Key::X509) {
        line.addParameter(QLatin1String("type"), QLatin1String("X509"));
    } else if (key.type() == Key::PGP) {
        line.addParameter(QLatin1String("type"), QLatin1String("PGP"));
    } else if (key.type() == Key::Custom) {
        line.addParameter(QLatin1String("type"), key.customTypeString());
    }

    return line;
}

Secrecy VCardTool::parseSecrecy(const VCardLine &line) const
{
    Secrecy secrecy;

    const QString value = line.value().toString().toLower();
    if (value == QLatin1String("public")) {
        secrecy.setType(Secrecy::Public);
    } else if (value == QLatin1String("private")) {
        secrecy.setType(Secrecy::Private);
    } else if (value == QLatin1String("confidential")) {
        secrecy.setType(Secrecy::Confidential);
    }

    return secrecy;
}

VCardLine VCardTool::createSecrecy(const Secrecy &secrecy) const
{
    VCardLine line(QLatin1String("CLASS"));

    int type = secrecy.type();

    if (type == Secrecy::Public) {
        line.setValue(QLatin1String("PUBLIC"));
    } else if (type == Secrecy::Private) {
        line.setValue(QLatin1String("PRIVATE"));
    } else if (type == Secrecy::Confidential) {
        line.setValue(QLatin1String("CONFIDENTIAL"));
    }

    return line;
}

QStringList VCardTool::splitString(const QChar &sep, const QString &str) const
{
    QStringList list;
    QString value(str);

    int start = 0;
    int pos = value.indexOf(sep, start);

    while (pos != -1) {
        if (pos == 0 || value[ pos - 1 ] != QLatin1Char('\\')) {
            if (pos > start && pos <= (int)value.length()) {
                list << value.mid(start, pos - start);
            } else {
                list << QString();
            }

            start = pos + 1;
            pos = value.indexOf(sep, start);
        } else {
            value.replace(pos - 1, 2, sep);
            pos = value.indexOf(sep, pos);
        }
    }

    int l = value.length() - 1;
    if (value.mid(start, l - start + 1).length() > 0) {
        list << value.mid(start, l - start + 1);
    } else {
        list << QString();
    }

    return list;
}
