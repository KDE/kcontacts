/*
    This file is part of the KContacts framework.
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>
    Copyright (c) 2015-2016 Laurent Montel <montel@kde.org>

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
#include "lang.h"
#include "gender.h"
#include "related.h"
#include "fieldgroup.h"
#include "kcontacts_debug.h"
#include <QtCore/QString>
#include <QtCore/QBuffer>
#include <QDebug>
#include <QRegularExpression>

using namespace KContacts;

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

static const struct {
    const char *addressType;
    Address::TypeFlag flag;
} s_addressTypes[] = {
    { "dom", Address::Dom },
    { "home", Address::Home },
    { "intl", Address::Intl },
    { "parcel", Address::Parcel },
    { "postal", Address::Postal },
    { "pref", Address::Pref },
    { "work", Address::Work },
};

static const unsigned int s_numAddressTypes
    = sizeof s_addressTypes / sizeof * s_addressTypes;

static Address::TypeFlag stringToAddressType(const QString &str)
{
    for (unsigned int i = 0; i < s_numAddressTypes; ++i)
        if (str == QLatin1String(s_addressTypes[i].addressType)) {
            return s_addressTypes[i].flag;
        }
    return {};
}

static const struct {
    const char *phoneType;
    PhoneNumber::TypeFlag flag;
} s_phoneTypes[] = {
    { "BBS", PhoneNumber::Bbs },
    { "CAR", PhoneNumber::Car },
    { "CELL", PhoneNumber::Cell },
    { "FAX", PhoneNumber::Fax },
    { "HOME", PhoneNumber::Home },
    { "ISDN", PhoneNumber::Isdn },
    { "MODEM", PhoneNumber::Modem },
    { "MSG", PhoneNumber::Msg },
    { "PAGER", PhoneNumber::Pager },
    { "PCS", PhoneNumber::Pcs },
    { "PREF", PhoneNumber::Pref },
    { "VIDEO", PhoneNumber::Video },
    { "VOICE", PhoneNumber::Voice },
    { "WORK", PhoneNumber::Work },
};

static const unsigned int s_numPhoneTypes
    = sizeof s_phoneTypes / sizeof * s_phoneTypes;

static PhoneNumber::TypeFlag stringToPhoneType(const QString &str)
{
    for (unsigned int i = 0; i < s_numPhoneTypes; ++i)
        if (str == QLatin1String(s_phoneTypes[i].phoneType)) {
            return s_phoneTypes[i].flag;
        }
    return {};
}

VCardTool::VCardTool()
{

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

void VCardTool::addParameters(VCardLine &line, const QMap<QString, QStringList> &params) const
{
    QMapIterator<QString, QStringList> i(params);
    while (i.hasNext()) {
        i.next();
        line.addParameter(i.key(), i.value().join(QLatin1Char(',')));
    }
}

void VCardTool::addParameter(VCardLine &line, VCard::Version version, const QString &key, const QStringList &valueStringList) const
{
    if (version == VCard::v2_1) {
        for (const QString &valueStr : valueStringList) {
            line.addParameter(valueStr, QString());
        }
    } else if (version == VCard::v3_0) {
        line.addParameter(key, valueStringList.join(QLatin1Char(',')));
    } else {
        if (valueStringList.count() < 2) {
            line.addParameter(key, valueStringList.join(QLatin1Char(',')));
        } else {
            line.addParameter(key, QLatin1Char('"') + valueStringList.join(QLatin1Char(',')) + QLatin1Char('"'));
        }
    }
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
        // VERSION
        if (version == VCard::v2_1) {
            card.addLine(VCardLine(QStringLiteral("VERSION"), QStringLiteral("2.1")));
        } else if (version == VCard::v3_0) {
            card.addLine(VCardLine(QStringLiteral("VERSION"), QStringLiteral("3.0")));
        } else if (version == VCard::v4_0) {
            card.addLine(VCardLine(QStringLiteral("VERSION"), QStringLiteral("4.0")));
        }

        // ADR + LABEL
        const Address::List addresses = (*addrIt).addresses();
        Address::List::ConstIterator end(addresses.end());
        for (Address::List::ConstIterator it = addresses.begin(); it != end; ++it) {
            QStringList address;

            const bool isEmpty = ((*it).postOfficeBox().isEmpty() &&
                                  (*it).extended().isEmpty() &&
                                  (*it).street().isEmpty() &&
                                  (*it).locality().isEmpty() &&
                                  (*it).region().isEmpty() &&
                                  (*it).postalCode().isEmpty() &&
                                  (*it).country().isEmpty());

            address.append((*it).postOfficeBox().replace(QLatin1Char(';'),
                           QStringLiteral("\\;")));

            address.append((*it).extended().replace(QLatin1Char(';'),
                                                    QStringLiteral("\\;")));

            address.append((*it).street().replace(QLatin1Char(';'),
                                                  QStringLiteral("\\;")));

            address.append((*it).locality().replace(QLatin1Char(';'),
                                                    QStringLiteral("\\;")));

            address.append((*it).region().replace(QLatin1Char(';'),
                                                  QStringLiteral("\\;")));

            address.append((*it).postalCode().replace(QLatin1Char(';'),
                           QStringLiteral("\\;")));

            address.append((*it).country().replace(QLatin1Char(';'),
                                                   QStringLiteral("\\;")));

            const QString addressJoined(address.join(QLatin1Char(';')));
            VCardLine adrLine(QStringLiteral("ADR"), addressJoined);
            if (version == VCard::v2_1 && needsEncoding(addressJoined)) {
                adrLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                adrLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
            }

            const bool hasLabel = !(*it).label().isEmpty();
            QStringList addreLineType;
            QStringList labelLineType;
            for (unsigned int i = 0; i < s_numAddressTypes; ++i) {
                if (s_addressTypes[i].flag & (*it).type()) {
                    const QString str = QString::fromLatin1(s_addressTypes[i].addressType);
                    addreLineType << str;
                    if (hasLabel) {
                        labelLineType << str;
                    }
                }
            }

            if (hasLabel) {
                if (version == VCard::v4_0) {
                    if (!(*it).label().isEmpty()) {
                        adrLine.addParameter(QStringLiteral("LABEL"), QStringLiteral("\"%1\"").arg((*it).label()));
                    }
                } else {
                    VCardLine labelLine(QStringLiteral("LABEL"), (*it).label());
                    if (version == VCard::v2_1 && needsEncoding((*it).label())) {
                        labelLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                        labelLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
                    }
                    addParameter(labelLine, version, QStringLiteral("TYPE"), labelLineType);
                    card.addLine(labelLine);
                }
            }
            if (version == VCard::v4_0) {
                Geo geo = (*it).geo();
                if (geo.isValid()) {
                    QString str;
                    str.sprintf("\"geo:%.6f,%.6f\"", geo.latitude(), geo.longitude());
                    adrLine.addParameter(QStringLiteral("GEO"), str);
                }
            }
            if (!isEmpty) {
                addParameter(adrLine, version, QStringLiteral("TYPE"), addreLineType);
                card.addLine(adrLine);
            }
        }

        // BDAY
        const bool withTime = (*addrIt).birthdayHasTime();
        const QString birthdayString = createDateTime((*addrIt).birthday(), version, withTime);
        card.addLine(VCardLine(QStringLiteral("BDAY"), birthdayString));

        //Laurent: 31 Jan 2015. Not necessary to export it. When Categories were changes as AkonadiTag nobody thought that it was break categorie support...
        //=> not necessary to export just tag...
        // CATEGORIES only > 2.1
        if (!exportVcard) {
            if (version != VCard::v2_1) {
                QStringList categories = (*addrIt).categories();
                QStringList::Iterator catIt;
                QStringList::Iterator catEnd(categories.end());
                for (catIt = categories.begin(); catIt != catEnd; ++catIt) {
                    (*catIt).replace(QLatin1Char(','), QStringLiteral("\\,"));
                }

                VCardLine catLine(QStringLiteral("CATEGORIES"), categories.join(QLatin1Char(',')));
                card.addLine(catLine);
            }
        }
        // MEMBER (only in 4.0)
        if (version == VCard::v4_0) {
            // The KIND property must be set to "group" in order to use this property.
            if ((*addrIt).kind().toLower() == QLatin1String("group")) {
                Q_FOREACH (const QString &member, (*addrIt).members()) {
                    VCardLine line(QStringLiteral("MEMBER"), member);
                    card.addLine(line);
                }
            }
        }
        // SOURCE
        Q_FOREACH (const QUrl &url, (*addrIt).sourcesUrlList()) {
            VCardLine line = VCardLine(QStringLiteral("SOURCE"), url.url());
            card.addLine(line);
        }

        const Related::List relatedList = (*addrIt).relationShips();
        Related::List::ConstIterator relatedIt;
        Related::List::ConstIterator relatedEnd(relatedList.end());
        for (relatedIt = relatedList.begin(); relatedIt != relatedEnd; ++relatedIt) {
            VCardLine line(QStringLiteral("RELATED"), (*relatedIt).related());
            addParameters(line, (*relatedIt).parameters());
            card.addLine(line);
        }
        // CLASS only for version == 3.0
        if (version == VCard::v3_0) {
            card.addLine(createSecrecy((*addrIt).secrecy()));
        }
        // LANG only for version == 4.0
        if (version == VCard::v4_0) {
            const Lang::List langList = (*addrIt).langs();
            Lang::List::ConstIterator langIt;
            Lang::List::ConstIterator langEnd(langList.end());
            for (langIt = langList.begin(); langIt != langEnd; ++langIt) {
                VCardLine line(QStringLiteral("LANG"), (*langIt).language());
                addParameters(line, (*langIt).parameters());
                card.addLine(line);
            }
        }
        // CLIENTPIDMAP
        if (version == VCard::v4_0) {
            const ClientPidMap::List clientpidmapList = (*addrIt).clientPidMapList();
            ClientPidMap::List::ConstIterator clientPidMapIt;
            ClientPidMap::List::ConstIterator clientPidMapEnd(clientpidmapList.end());
            for (clientPidMapIt = clientpidmapList.begin(); clientPidMapIt != clientPidMapEnd; ++clientPidMapIt) {
                VCardLine line(QStringLiteral("CLIENTPIDMAP"), (*clientPidMapIt).clientPidMap());
                addParameters(line, (*clientPidMapIt).parameters());
                card.addLine(line);
            }
        }
        // EMAIL
        const Email::List emailList = (*addrIt).emailList();
        Email::List::ConstIterator emailIt;
        Email::List::ConstIterator emailEnd(emailList.end());
        for (emailIt = emailList.begin(); emailIt != emailEnd; ++emailIt) {
            VCardLine line(QStringLiteral("EMAIL"), (*emailIt).mail());
            QMapIterator<QString, QStringList> i((*emailIt).parameters());
            while (i.hasNext()) {
                i.next();
                if (version == VCard::v2_1) {
                    if (i.key().toLower() == QLatin1String("type")) {
                        QStringList valueStringList = i.value();
                        bool hasPreferred = false;
                        if (valueStringList.contains(QStringLiteral("PREF"))) {
                            valueStringList.removeAll(QStringLiteral("PREF"));
                            hasPreferred = true;
                        }
                        if (!valueStringList.isEmpty()) {
                            addParameter(line, version, i.key(), valueStringList);
                        }
                        if (hasPreferred) {
                            line.addParameter(QStringLiteral("PREF"), QString());
                        }
                    } else {
                        line.addParameter(i.key(), i.value().join(QLatin1Char(',')));
                    }
                } else {
                    line.addParameter(i.key(), i.value().join(QLatin1Char(',')));
                }
            }
            card.addLine(line);
        }

        // FN required for only version > 2.1
        VCardLine fnLine(QStringLiteral("FN"), (*addrIt).formattedName());
        if (version == VCard::v2_1 && needsEncoding((*addrIt).formattedName())) {
            fnLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
            fnLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
        }
        card.addLine(fnLine);

        // GEO
        const Geo geo = (*addrIt).geo();
        if (geo.isValid()) {
            QString str;
            if (version == VCard::v4_0) {
                str.sprintf("geo:%.6f,%.6f", geo.latitude(), geo.longitude());
            } else {
                str.sprintf("%.6f;%.6f", geo.latitude(), geo.longitude());
            }
            card.addLine(VCardLine(QStringLiteral("GEO"), str));
        }

        // KEY
        const Key::List keys = (*addrIt).keys();
        Key::List::ConstIterator keyIt;
        Key::List::ConstIterator keyEnd(keys.end());
        for (keyIt = keys.begin(); keyIt != keyEnd; ++keyIt) {
            card.addLine(createKey(*keyIt, version));
        }

        // LOGO
        card.addLine(createPicture(QStringLiteral("LOGO"), (*addrIt).logo(), version));
        Q_FOREACH (const Picture &logo, (*addrIt).extraLogoList()) {
            card.addLine(createPicture(QStringLiteral("LOGO"), logo, version));
        }

        // MAILER only for version < 4.0
        if (version != VCard::v4_0) {
            VCardLine mailerLine(QStringLiteral("MAILER"), (*addrIt).mailer());
            if (version == VCard::v2_1 && needsEncoding((*addrIt).mailer())) {
                mailerLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                mailerLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
            }
            card.addLine(mailerLine);
        }

        // N required for only version < 4.0
        QStringList name;
        name.append((*addrIt).familyName().replace(QLatin1Char(';'),
                    QStringLiteral("\\;")));

        name.append((*addrIt).givenName().replace(QLatin1Char(';'),
                    QStringLiteral("\\;")));

        name.append((*addrIt).additionalName().replace(QLatin1Char(';'),
                    QStringLiteral("\\;")));

        name.append((*addrIt).prefix().replace(QLatin1Char(';'),
                                               QStringLiteral("\\;")));

        name.append((*addrIt).suffix().replace(QLatin1Char(';'),
                                               QStringLiteral("\\;")));

        VCardLine nLine(QStringLiteral("N"), name.join(QLatin1Char(';')));
        if (version == VCard::v2_1 && needsEncoding(name.join(QLatin1Char(';')))) {
            nLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
            nLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
        }
        if (version == VCard::v4_0 && !(*addrIt).sortString().isEmpty()) {
            nLine.addParameter(QStringLiteral("SORT-AS"), (*addrIt).sortString());
        }

        card.addLine(nLine);

        // NAME only for version < 4.0
        if (version != VCard::v4_0) {
            VCardLine nameLine(QStringLiteral("NAME"), (*addrIt).name());
            if (version == VCard::v2_1 && needsEncoding((*addrIt).name())) {
                nameLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                nameLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
            }
            card.addLine(nameLine);
        }

        // NICKNAME only for version > 2.1
        if (version != VCard::v2_1) {
            Q_FOREACH (const NickName &nickName, (*addrIt).extraNickNameList()) {
                VCardLine nickNameLine(QStringLiteral("NICKNAME"), nickName.nickname());
                addParameters(nickNameLine, nickName.parameters());

                card.addLine(nickNameLine);
            }
        }

        // NOTE
        VCardLine noteLine(QStringLiteral("NOTE"), (*addrIt).note());
        if (version == VCard::v2_1 && needsEncoding((*addrIt).note())) {
            noteLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
            noteLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
        }
        card.addLine(noteLine);

        // ORG
        Q_FOREACH (const Org &org, (*addrIt).extraOrganizationList()) {
            QStringList organization;
            organization.append(org.organization().replace(QLatin1Char(';'),
                                QStringLiteral("\\;")));
            if (!(*addrIt).department().isEmpty()) {
                organization.append((*addrIt).department().replace(QLatin1Char(';'),
                                    QStringLiteral("\\;")));
            }
            const QString orgStr = organization.join(QLatin1Char(';'));
            VCardLine orgLine(QStringLiteral("ORG"), orgStr);
            if (version == VCard::v2_1 && needsEncoding(orgStr)) {
                orgLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                orgLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
            }
            addParameters(orgLine, org.parameters());
            card.addLine(orgLine);
        }

        // PHOTO
        card.addLine(createPicture(QStringLiteral("PHOTO"), (*addrIt).photo(), version));
        Q_FOREACH (const Picture &photo, (*addrIt).extraPhotoList()) {
            card.addLine(createPicture(QStringLiteral("PHOTO"), photo, version));
        }

        // PROID only for version > 2.1
        if (version != VCard::v2_1) {
            card.addLine(VCardLine(QStringLiteral("PRODID"), (*addrIt).productId()));
        }

        // REV
        card.addLine(VCardLine(QStringLiteral("REV"), createDateTime((*addrIt).revision(), version)));

        // ROLE
        Q_FOREACH (const Role &role, (*addrIt).extraRoleList()) {
            VCardLine roleLine(QStringLiteral("ROLE"), role.role());
            if (version == VCard::v2_1 && needsEncoding(role.role())) {
                roleLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                roleLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
            }
            addParameters(roleLine, role.parameters());
            card.addLine(roleLine);
        }

        // SORT-STRING
        if (version == VCard::v3_0) {
            card.addLine(VCardLine(QStringLiteral("SORT-STRING"), (*addrIt).sortString()));
        }

        // SOUND
        card.addLine(createSound((*addrIt).sound(), version));
        Q_FOREACH (const Sound &sound, (*addrIt).extraSoundList()) {
            card.addLine(createSound(sound, version));
        }

        // TEL
        const PhoneNumber::List phoneNumbers = (*addrIt).phoneNumbers();
        PhoneNumber::List::ConstIterator phoneIt;
        PhoneNumber::List::ConstIterator phoneEnd(phoneNumbers.end());
        for (phoneIt = phoneNumbers.begin(); phoneIt != phoneEnd; ++phoneIt) {
            VCardLine line(QStringLiteral("TEL"), (*phoneIt).number());
            QMapIterator<QString, QStringList> i((*phoneIt).parameters());
            while (i.hasNext()) {
                i.next();
                if (i.key().toUpper() != QLatin1String("TYPE")) {
                    line.addParameter(i.key(), i.value().join(QLatin1Char(',')));
                }
            }

            QStringList lst;
            for (unsigned int i = 0; i < s_numPhoneTypes; ++i) {
                if (s_phoneTypes[i].flag & (*phoneIt).type()) {
                    const QString str = QString::fromLatin1(s_phoneTypes[i].phoneType);
                    if (version == VCard::v4_0) {
                        lst << str.toLower();
                    } else {
                        lst << str;
                    }
                }
            }
            if (!lst.isEmpty()) {
                addParameter(line, version, QStringLiteral("TYPE"), lst);
            }
            card.addLine(line);
        }

        // TITLE
        Q_FOREACH (const Title &title, (*addrIt).extraTitleList()) {
            VCardLine titleLine(QStringLiteral("TITLE"), title.title());
            if (version == VCard::v2_1 && needsEncoding(title.title())) {
                titleLine.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                titleLine.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
            }
            addParameters(titleLine, title.parameters());

            card.addLine(titleLine);
        }

        // TZ
        // TODO Add vcard4.0 support
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

            card.addLine(VCardLine(QStringLiteral("TZ"), str));
        }

        // UID
        card.addLine(VCardLine(QStringLiteral("UID"), (*addrIt).uid()));

        // URL
        Q_FOREACH (const ResourceLocatorUrl &url, (*addrIt).extraUrlList()) {
            VCardLine line(QStringLiteral("URL"), url.url());
            addParameters(line, url.parameters());
            card.addLine(line);
        }
        if (version == VCard::v4_0) {
            // GENDER
            const Gender gender = (*addrIt).gender();
            if (gender.isValid()) {
                QString genderStr;
                if (!gender.gender().isEmpty()) {
                    genderStr = gender.gender();
                }
                if (!gender.comment().isEmpty()) {
                    genderStr += QLatin1Char(';') + gender.comment();
                }
                VCardLine line(QStringLiteral("GENDER"), genderStr);
                card.addLine(line);
            }
            // KIND
            if (!(*addrIt).kind().isEmpty()) {
                VCardLine line(QStringLiteral("KIND"), (*addrIt).kind());
                card.addLine(line);
            }
        }
        // From vcard4.
        if (version == VCard::v4_0) {
            Q_FOREACH (const CalendarUrl &url, (*addrIt).calendarUrlList()) {
                if (url.isValid()) {
                    QString type;
                    switch (url.type()) {
                    case CalendarUrl::Unknown:
                    case CalendarUrl::EndCalendarType:
                        break;
                    case CalendarUrl::FBUrl:
                        type = QStringLiteral("FBURL");
                        break;
                    case CalendarUrl::CALUri:
                        type = QStringLiteral("CALURI");
                        break;
                    case CalendarUrl::CALADRUri:
                        type = QStringLiteral("CALADRURI");
                        break;

                    }
                    if (!type.isEmpty()) {
                        VCardLine line(type, url.url().toDisplayString());
                        addParameters(line, url.parameters());
                        card.addLine(line);
                    }
                }
            }
        }
        //FieldGroup
        Q_FOREACH (const FieldGroup &group, (*addrIt).fieldGroupList()) {
            VCardLine line(group.fieldGroupName(), group.value());
            addParameters(line, group.parameters());
            card.addLine(line);
        }

        // IMPP (supported in vcard 3 too)
        Q_FOREACH (const Impp &impp, (*addrIt).imppList()) {
            VCardLine line(QStringLiteral("IMPP"), impp.address());
            line.addParameter(QStringLiteral("X-SERVICE-TYPE"), Impp::typeToString(impp.type()));
            QMapIterator<QString, QStringList> i(impp.parameters());
            while (i.hasNext()) {
                i.next();
                if (i.key().toLower() != QStringLiteral("x-service-type")) {
                    line.addParameter(i.key(), i.value().join(QLatin1Char(',')));
                }
            }
            card.addLine(line);
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
                    identifier = QStringLiteral("X-AIM");
                } else if (identifier == QLatin1String("X-messaging/icq-All")) {
                    identifier = QStringLiteral("X-ICQ");
                } else if (identifier == QLatin1String("X-messaging/xmpp-All")) {
                    identifier = QStringLiteral("X-JABBER");
                } else if (identifier == QLatin1String("X-messaging/msn-All")) {
                    identifier = QStringLiteral("X-MSN");
                } else if (identifier == QLatin1String("X-messaging/yahoo-All")) {
                    identifier = QStringLiteral("X-YAHOO");
                } else if (identifier == QLatin1String("X-messaging/gadu-All")) {
                    identifier = QStringLiteral("X-GADUGADU");
                } else if (identifier == QLatin1String("X-messaging/skype-All")) {
                    identifier = QStringLiteral("X-SKYPE");
                } else if (identifier == QLatin1String("X-messaging/groupwise-All")) {
                    identifier = QStringLiteral("X-GROUPWISE");
                } else if (identifier == QLatin1String("X-messaging/sms-All")) {
                    identifier = QStringLiteral("X-SMS");
                } else if (identifier == QLatin1String("X-messaging/meanwhile-All")) {
                    identifier = QStringLiteral("X-MEANWHILE");
                } else if (identifier == QLatin1String("X-messaging/irc-All")) {
                    identifier = QStringLiteral("X-IRC");   //Not defined by rfc but need for fixing #300869
                } else if (identifier == QLatin1String("X-messaging/googletalk-All")) {
                    //Not defined by rfc but need for fixing #300869
                    identifier = QStringLiteral("X-GTALK");
                } else if (identifier == QLatin1String("X-messaging/twitter-All")) {
                    identifier = QStringLiteral("X-TWITTER");
                }
            }
            if (identifier.toLower() == QLatin1String("x-kaddressbook-x-anniversary") && version == VCard::v4_0) {
                // ANNIVERSARY
                if (!value.isEmpty()) {
                    const QDate date = QDate::fromString(value, Qt::ISODate);
                    QDateTime dt = QDateTime(date);
                    dt.setTime(QTime());
                    VCardLine line(QStringLiteral("ANNIVERSARY"), createDateTime(dt, version, false));
                    card.addLine(line);
                }
            } else if (identifier.toLower() == QLatin1String("x-kaddressbook-x-spousesname") && version == VCard::v4_0) {
                if (!value.isEmpty()) {
                    VCardLine line(QStringLiteral("RELATED"), QStringLiteral(";"));
                    line.addParameter(QStringLiteral("TYPE"), QStringLiteral("spouse"));
                    line.addParameter(QStringLiteral("VALUE"), value);
                    card.addLine(line);
                }
            } else {
                VCardLine line(identifier, value);
                if (version == VCard::v2_1 && needsEncoding(value)) {
                    line.addParameter(QStringLiteral("charset"), QStringLiteral("UTF-8"));
                    line.addParameter(QStringLiteral("encoding"), QStringLiteral("QUOTED-PRINTABLE"));
                }
                card.addLine(line);
            }
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
    QString group;

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
                group = (*lineIt).group();
                if (!group.isEmpty()) {
                    KContacts::FieldGroup groupField(group + QLatin1Char('.') + (*lineIt).identifier());
                    groupField.setParameters((*lineIt).parameterMap());
                    groupField.setValue((*lineIt).value().toString());
                    addr.insertFieldGroup(groupField);
                }
                // ADR
                else if (identifier == QLatin1String("adr")) {
                    Address address;
                    const QStringList addrParts = splitString(semicolonSep, (*lineIt).value().toString());
                    const int addrPartsCount(addrParts.count());
                    if (addrPartsCount > 0) {
                        address.setPostOfficeBox(addrParts.at(0));
                    }
                    if (addrPartsCount > 1) {
                        address.setExtended(addrParts.at(1));
                    }
                    if (addrPartsCount > 2) {
                        address.setStreet(addrParts.at(2));
                    }
                    if (addrPartsCount > 3) {
                        address.setLocality(addrParts.at(3));
                    }
                    if (addrPartsCount > 4) {
                        address.setRegion(addrParts.at(4));
                    }
                    if (addrPartsCount > 5) {
                        address.setPostalCode(addrParts.at(5));
                    }
                    if (addrPartsCount > 6) {
                        address.setCountry(addrParts.at(6));
                    }

                    Address::Type type;

                    const QStringList types = (*lineIt).parameters(QStringLiteral("type"));
                    QStringList::ConstIterator end(types.end());
                    for (QStringList::ConstIterator it = types.begin(); it != end; ++it) {
                        type |= stringToAddressType((*it).toLower());
                    }

                    address.setType(type);
                    QString label = (*lineIt).parameter(QStringLiteral("label"));
                    if (!label.isEmpty()) {
                        if (label.length() > 1) {
                            if (label.at(0) == QLatin1Char('"') && label.at(label.length() - 1) == QLatin1Char('"')) {
                                label = label.mid(1, label.length() - 2);
                            }
                        }
                        address.setLabel(label);
                    }
                    QString geoStr = (*lineIt).parameter(QStringLiteral("geo"));
                    if (!geoStr.isEmpty()) {
                        geoStr.remove(QLatin1Char('\"'));
                        geoStr.remove(QStringLiteral("geo:"));
                        if (geoStr.contains(QLatin1Char(','))) {
                            QStringList arguments = geoStr.split(QLatin1Char(','));
                            KContacts::Geo geo;
                            geo.setLatitude(arguments.at(0).toDouble());
                            geo.setLongitude(arguments.at(1).toDouble());
                            address.setGeo(geo);
                        }
                    }
                    addr.insertAddress(address);
                }
                // ANNIVERSARY
                else if (identifier == QLatin1String("anniversary")) {
                    const QString t = (*lineIt).value().toString();
                    const QDateTime dt(parseDateTime(t));
                    addr.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-Anniversary"), dt.date().toString(Qt::ISODate));
                }
                // BDAY
                else if (identifier == QLatin1String("bday")) {
                    bool withTime;
                    const QDateTime bday = parseDateTime((*lineIt).value().toString(), &withTime);
                    addr.setBirthday(bday, withTime);
                }

                // CATEGORIES
                else if (identifier == QLatin1String("categories")) {
                    const QStringList categories = splitString(commaSep, (*lineIt).value().toString());
                    addr.setCategories(categories);
                }
                // FBURL
                else if (identifier == QLatin1String("fburl")) {
                    CalendarUrl calurl;
                    calurl.setType(CalendarUrl::FBUrl);
                    const QUrl url = QUrl((*lineIt).value().toString());
                    calurl.setUrl(url);
                    calurl.setParameters((*lineIt).parameterMap());
                    addr.insertCalendarUrl(calurl);
                }
                // CALADRURI
                else if (identifier == QLatin1String("caladruri")) {
                    CalendarUrl calurl;
                    calurl.setType(CalendarUrl::CALADRUri);
                    const QUrl url = QUrl((*lineIt).value().toString());
                    calurl.setUrl(url);
                    calurl.setParameters((*lineIt).parameterMap());
                    addr.insertCalendarUrl(calurl);
                }
                // CALURI
                else if (identifier == QLatin1String("caluri")) {
                    CalendarUrl calurl;
                    calurl.setType(CalendarUrl::CALUri);
                    const QUrl url = QUrl((*lineIt).value().toString());
                    calurl.setUrl(url);
                    calurl.setParameters((*lineIt).parameterMap());
                    addr.insertCalendarUrl(calurl);
                }
                //IMPP
                else if (identifier == QLatin1String("impp")) {
                    QString imppStr = (*lineIt).value().toString();
                    Impp impp;
                    impp.setParameters((*lineIt).parameterMap());
                    if (!(*lineIt).parameter(QStringLiteral("x-service-type")).isEmpty()) {
                        impp.setAddress(imppStr);
                        const QString serviceType = (*lineIt).parameter(QStringLiteral("x-service-type")).toLower();
                        imppService(serviceType, impp);
                    } else {
                        const int pos = imppStr.indexOf(QLatin1Char(':'));
                        if (pos != -1) {
                            const QString serviceType = imppStr.left(pos);
                            const QString address = imppStr.right(imppStr.length() - pos - 1);
                            impp.setAddress(address);
                            imppService(serviceType, impp);
                        }
                    }
                    addr.insertImpp(impp);
                }
                // CLASS
                else if (identifier == QLatin1String("class")) {
                    addr.setSecrecy(parseSecrecy(*lineIt));
                }
                // GENDER
                else if (identifier == QLatin1String("gender")) {
                    QString genderStr = (*lineIt).value().toString();
                    if (!genderStr.isEmpty()) {
                        Gender gender;
                        if (genderStr.at(0) != QLatin1Char(';')) {
                            gender.setGender(genderStr.at(0));
                            if (genderStr.length() > 2 && (genderStr.at(1) == QLatin1Char(';'))) {
                                gender.setComment(genderStr.right(genderStr.length() - 2));
                            }
                        } else {
                            gender.setComment(genderStr.right(genderStr.length() - 1));
                        }
                        addr.setGender(gender);
                    }
                }
                // LANG
                else if (identifier == QLatin1String("lang")) {
                    Lang lang;
                    lang.setLanguage((*lineIt).value().toString());
                    lang.setParameters((*lineIt).parameterMap());
                    addr.insertLang(lang);
                }
                // EMAIL
                else if (identifier == QLatin1String("email")) {
                    const QStringList types = (*lineIt).parameters(QStringLiteral("type"));
                    addr.insertEmail((*lineIt).value().toString(),
                                     types.contains(QStringLiteral("PREF")), (*lineIt).parameterMap());
                }
                // KIND
                else if (identifier == QLatin1String("kind")) {
                    addr.setKind((*lineIt).value().toString());
                }
                // FN
                else if (identifier == QLatin1String("fn")) {
                    addr.setFormattedName((*lineIt).value().toString());
                }

                // GEO
                else if (identifier == QLatin1String("geo")) {
                    Geo geo;
                    QString lineStr = (*lineIt).value().toString();
                    if (lineStr.startsWith(QLatin1String("geo:"))) { //VCard 4.0
                        lineStr.remove(QStringLiteral("geo:"));
                        const QStringList geoParts =
                            lineStr.split(QLatin1Char(','), QString::KeepEmptyParts);
                        if (geoParts.size() >= 2) {
                            geo.setLatitude(geoParts.at(0).toFloat());
                            geo.setLongitude(geoParts.at(1).toFloat());
                            addr.setGeo(geo);
                        }
                    } else {
                        const QStringList geoParts =
                            lineStr.split(QLatin1Char(';'), QString::KeepEmptyParts);
                        if (geoParts.size() >= 2) {
                            geo.setLatitude(geoParts.at(0).toFloat());
                            geo.setLongitude(geoParts.at(1).toFloat());
                            addr.setGeo(geo);
                        }
                    }
                }

                // KEY
                else if (identifier == QLatin1String("key")) {
                    addr.insertKey(parseKey(*lineIt));
                }

                // LABEL
                else if (identifier == QLatin1String("label")) {
                    Address::Type type;

                    const QStringList types = (*lineIt).parameters(QStringLiteral("type"));
                    QStringList::ConstIterator end(types.end());
                    for (QStringList::ConstIterator it = types.begin(); it != end; ++it) {
                        type |= stringToAddressType((*it).toLower());
                    }

                    bool available = false;
                    KContacts::Address::List addressList = addr.addresses();
                    for (KContacts::Address::List::Iterator it = addressList.begin();
                            it != addressList.end(); ++it) {
                        if ((*it).type() == type) {
                            (*it).setLabel((*lineIt).value().toString());
                            addr.insertAddress(*it);
                            available = true;
                            break;
                        }
                    }

                    if (!available) {   // a standalone LABEL tag
                        KContacts::Address address(type);
                        address.setLabel((*lineIt).value().toString());
                        addr.insertAddress(address);
                    }
                }

                // LOGO
                else if (identifier == QLatin1String("logo")) {
                    Picture picture = parsePicture(*lineIt);
                    if (addr.logo().isEmpty()) {
                        addr.setLogo(picture);
                    } else {
                        addr.insertExtraLogo(picture);
                    }
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
                    if (!(*lineIt).parameter(QStringLiteral("sort-as")).isEmpty()) {
                        addr.setSortString((*lineIt).parameter(QStringLiteral("sort-as")));
                    }
                }

                // NAME
                else if (identifier == QLatin1String("name")) {
                    addr.setName((*lineIt).value().toString());
                }

                // NICKNAME
                else if (identifier == QLatin1String("nickname")) {
                    NickName nickName((*lineIt).value().toString());
                    nickName.setParameters((*lineIt).parameterMap());
                    addr.insertExtraNickName(nickName);
                }

                // NOTE
                else if (identifier == QLatin1String("note")) {
                    addr.setNote((*lineIt).value().toString());
                }

                // ORGANIZATION
                else if (identifier == QLatin1String("org")) {
                    const QStringList orgParts = splitString(semicolonSep, (*lineIt).value().toString());
                    const int orgPartsCount(orgParts.count());
                    if (orgPartsCount > 0) {
                        Org organization(orgParts.at(0));
                        organization.setParameters((*lineIt).parameterMap());
                        addr.insertExtraOrganization(organization);
                    }
                    if (orgPartsCount > 1) {
                        addr.setDepartment(orgParts.at(1));
                    }
                    if (!(*lineIt).parameter(QStringLiteral("sort-as")).isEmpty()) {
                        addr.setSortString((*lineIt).parameter(QStringLiteral("sort-as")));
                    }
                }

                // PHOTO
                else if (identifier == QLatin1String("photo")) {
                    Picture picture = parsePicture(*lineIt);
                    if (addr.photo().isEmpty()) {
                        addr.setPhoto(picture);
                    } else {
                        addr.insertExtraPhoto(picture);
                    }
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
                    Role role((*lineIt).value().toString());
                    role.setParameters((*lineIt).parameterMap());
                    addr.insertExtraRole(role);
                }

                // SORT-STRING
                else if (identifier == QLatin1String("sort-string")) {
                    addr.setSortString((*lineIt).value().toString());
                }

                // SOUND
                else if (identifier == QLatin1String("sound")) {
                    Sound sound = parseSound(*lineIt);
                    if (addr.sound().isEmpty()) {
                        addr.setSound(sound);
                    } else {
                        addr.insertExtraSound(sound);
                    }
                }

                // TEL
                else if (identifier == QLatin1String("tel")) {
                    PhoneNumber phone;
                    phone.setNumber((*lineIt).value().toString());

                    PhoneNumber::Type type;
                    bool foundType = false;
                    const QStringList types = (*lineIt).parameters(QStringLiteral("type"));
                    QStringList::ConstIterator typeEnd(types.constEnd());
                    for (QStringList::ConstIterator it = types.constBegin(); it != typeEnd; ++it) {
                        type |= stringToPhoneType((*it).toUpper());
                        foundType = true;
                    }
                    phone.setType(foundType ? type : PhoneNumber::Undefined);
                    phone.setParameters((*lineIt).parameterMap());

                    addr.insertPhoneNumber(phone);
                }

                // TITLE
                else if (identifier == QLatin1String("title")) {
                    Title title((*lineIt).value().toString());
                    title.setParameters((*lineIt).parameterMap());
                    addr.insertExtraTitle(title);
                }

                // TZ
                else if (identifier == QLatin1String("tz")) {
                    //TODO add vcard4 support
                    TimeZone tz;
                    const QString date = (*lineIt).value().toString();

                    if (!date.isEmpty()) {
                        int hours = date.midRef(1, 2).toInt();
                        int minutes = date.midRef(4, 2).toInt();
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
                    const QUrl url = QUrl((*lineIt).value().toString());
                    ResourceLocatorUrl resourceLocatorUrl;
                    resourceLocatorUrl.setUrl(url);
                    resourceLocatorUrl.setParameters((*lineIt).parameterMap());
                    addr.insertExtraUrl(resourceLocatorUrl);
                }
                // SOURCE
                else if (identifier == QLatin1String("source")) {
                    const QUrl url = QUrl((*lineIt).value().toString());
                    addr.insertSourceUrl(url);
                }
                // MEMBER (vcard 4.0)
                else if (identifier == QLatin1String("member")) {
                    addr.insertMember((*lineIt).value().toString());
                }
                // RELATED (vcard 4.0)
                else if (identifier == QLatin1String("related")) {
                    Related related;
                    related.setRelated((*lineIt).value().toString());
                    related.setParameters((*lineIt).parameterMap());
                    addr.insertRelationShip(related);
                }
                // CLIENTPIDMAP (vcard 4.0)
                else if (identifier == QLatin1String("clientpidmap")) {
                    ClientPidMap clientpidmap;
                    clientpidmap.setClientPidMap((*lineIt).value().toString());
                    clientpidmap.setParameters((*lineIt).parameterMap());
                    addr.insertClientPidMap(clientpidmap);
                }
                // X-
                //TODO import X-GENDER
                else if (identifier.startsWith(QLatin1String("x-"))) {
                    QString ident = (*lineIt).identifier();
                    //X-Evolution
                    if (identifier == QLatin1String("x-evolution-spouse") ||
                            identifier == QLatin1String("x-spouse")) {
                        ident = QStringLiteral("X-KADDRESSBOOK-X-SpousesName");
                    } else if (identifier == QLatin1String("x-evolution-blog-url")) {
                        ident = QStringLiteral("X-KADDRESSBOOK-BlogFeed");
                    } else if (identifier == QLatin1String("x-evolution-assistant") ||
                               identifier == QLatin1String("x-assistant")) {
                        ident = QStringLiteral("X-KADDRESSBOOK-X-AssistantsName");
                    } else if (identifier == QLatin1String("x-evolution-anniversary") ||
                               identifier == QLatin1String("x-anniversary")) {
                        ident = QStringLiteral("X-KADDRESSBOOK-X-Anniversary");
                    } else if (identifier == QLatin1String("x-evolution-manager") ||
                               identifier == QLatin1String("x-manager")) {
                        ident = QStringLiteral("X-KADDRESSBOOK-X-ManagersName");
                    } else if (identifier == QLatin1String("x-aim")) {
                        ident = QStringLiteral("X-messaging/aim-All");
                    } else if (identifier == QLatin1String("x-icq")) {
                        ident = QStringLiteral("X-messaging/icq-All");
                    } else if (identifier == QLatin1String("x-jabber")) {
                        ident = QStringLiteral("X-messaging/xmpp-All");
                    } else if (identifier == QLatin1String("x-jabber")) {
                        ident = QStringLiteral("X-messaging/xmpp-All");
                    } else if (identifier == QLatin1String("x-msn")) {
                        ident = QStringLiteral("X-messaging/msn-All");
                    } else if (identifier == QLatin1String("x-yahoo")) {
                        ident = QStringLiteral("X-messaging/yahoo-All");
                    } else if (identifier == QLatin1String("x-gadugadu")) {
                        ident = QStringLiteral("X-messaging/gadu-All");
                    } else if (identifier == QLatin1String("x-skype")) {
                        ident = QStringLiteral("X-messaging/skype-All");
                    } else if (identifier == QLatin1String("x-groupwise")) {
                        ident = QStringLiteral("X-messaging/groupwise-All");
                    } else if (identifier == QLatin1String("x-sms")) {
                        ident = QStringLiteral("X-messaging/sms-All");
                    } else if (identifier == QLatin1String("x-meanwhile")) {
                        ident = QStringLiteral("X-messaging/meanwhile-All");
                    } else if (identifier == QLatin1String("x-irc")) {
                        ident = QStringLiteral("X-messaging/irc-All");
                    } else if (identifier == QLatin1String("x-gtalk")) {
                        ident = QStringLiteral("X-messaging/googletalk-All");
                    } else if (identifier == QLatin1String("x-twitter")) {
                        ident = QStringLiteral("X-messaging/twitter-All");
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

QDateTime VCardTool::parseDateTime(const QString &str, bool *timeValid)
{
    const int posT = str.indexOf(QLatin1Char('T'));
    QString dateString = posT >= 0 ? str.left(posT) : str;
    const bool noYear = dateString.startsWith(QLatin1String("--"));
    dateString = dateString.remove(QLatin1Char('-'));
    QDate date;
    if (noYear) {
        date = QDate::fromString(dateString, QStringLiteral("MMdd"));
        date = date.addYears(-1900);
    } else {
        date = QDate::fromString(dateString, QStringLiteral("yyyyMMdd"));
    }

    QTime time;
    Qt::TimeSpec spec = Qt::LocalTime;
    int offsetSecs = 0;
    if (posT >= 0) {
        QString timeString = str.mid(posT + 1);
        timeString = timeString.remove(QLatin1Char(':'));
        const int zPos = timeString.indexOf(QLatin1Char('Z'));
        const int plusPos = timeString.indexOf(QLatin1Char('+'));
        const int minusPos = timeString.indexOf(QLatin1Char('-'));
        const int tzPos = qMax(qMax(zPos, plusPos), minusPos);
        const QString hhmmssString = tzPos >= 0 ? timeString.left(tzPos) : timeString;
        switch (hhmmssString.size()) {
        case 2:
            time = QTime::fromString(hhmmssString, QStringLiteral("hh"));
            break;
        case 4:
            time = QTime::fromString(hhmmssString, QStringLiteral("hhmm"));
            break;
        case 6:
            time = QTime::fromString(hhmmssString, QStringLiteral("hhmmss"));
            break;
        }
        if (tzPos >= 0) {
            if (zPos >= 0) {
                spec = Qt::UTC;
            } else {
                spec = Qt::OffsetFromUTC;
                QTime offsetTime;
                const QString offsetString = timeString.mid(tzPos + 1);
                switch (offsetString.size()) {
                case 2:
                    offsetTime = QTime::fromString(offsetString, QStringLiteral("hh"));
                    break;
                case 4:
                    offsetTime = QTime::fromString(offsetString, QStringLiteral("hhmm"));
                    break;
                }
                offsetSecs = offsetTime.hour() * 3600 + offsetTime.minute() * 60;
            }
            if (minusPos >= 0) {
                offsetSecs *= -1;
            }
        }
    }
    if (timeValid) {
        *timeValid = time.isValid();
    }

    return QDateTime(date, time, spec, offsetSecs);
}

QString VCardTool::createDateTime(const QDateTime &dateTime, VCard::Version version, bool withTime)
{
    if (!dateTime.date().isValid()) {
        return QString();
    }
    QString str = createDate(dateTime.date(), version);
    if (!withTime) {
        return str;
    }
    str += createTime(dateTime.time(), version);
    if (dateTime.timeSpec() == Qt::UTC) {
        str += QLatin1Char('Z');
    } else if (dateTime.timeSpec() == Qt::OffsetFromUTC) {
        const int offsetSecs = dateTime.offsetFromUtc();
        if (offsetSecs >= 0) {
            str += QLatin1Char('+');
        } else {
            str += QLatin1Char('-');
        }
        QTime offsetTime = QTime(0, 0).addSecs(abs(offsetSecs));
        if (version == VCard::v4_0) {
            str += offsetTime.toString(QStringLiteral("HHmm"));
        } else {
            str += offsetTime.toString(QStringLiteral("HH:mm"));
        }
    }
    return str;
}

QString VCardTool::createDate(const QDate &date, VCard::Version version)
{
    QString format;
    if (date.year() > 0) {
        format = QStringLiteral("yyyyMMdd");
    } else {
        format = QStringLiteral("--MMdd");
    }
    if (version != VCard::v4_0) {
        format = format.replace(QStringLiteral("yyyy"), QStringLiteral("yyyy-"));
        format = format.replace(QStringLiteral("MM"), QStringLiteral("MM-"));
    }
    return date.toString(format);
}

QString VCardTool::createTime(const QTime &time, VCard::Version version)
{
    QString format;
    if (version == VCard::v4_0) {
        format = QStringLiteral("HHmmss");
    } else {
        format = QStringLiteral("HH:mm:ss");
    }
    return QStringLiteral("T") + time.toString(format);
}

Picture VCardTool::parsePicture(const VCardLine &line) const
{
    Picture pic;

    const QStringList params = line.parameterList();
    QString type;
    if (params.contains(QStringLiteral("type"))) {
        type = line.parameter(QStringLiteral("type"));
    }
    if (params.contains(QStringLiteral("encoding"))) {
        pic.setRawData(line.value().toByteArray(), type);
    } else if (params.contains(QStringLiteral("value"))) {
        if (line.parameter(QStringLiteral("value")).toLower() == QLatin1String("uri")) {
            pic.setUrl(line.value().toString());
        }
    }

    return pic;
}

VCardLine VCardTool::createPicture(const QString &identifier, const Picture &pic, VCard::Version version) const
{
    VCardLine line(identifier);

    if (pic.isEmpty()) {
        return line;
    }

    if (pic.isIntern()) {
        line.setValue(pic.rawData());
        if (version == VCard::v2_1) {
            line.addParameter(QStringLiteral("ENCODING"), QStringLiteral("BASE64"));
            line.addParameter(pic.type(), QString());
        } else {
            line.addParameter(QStringLiteral("encoding"), QStringLiteral("b"));
            line.addParameter(QStringLiteral("type"), pic.type());
        }
    } else {
        line.setValue(pic.url());
        line.addParameter(QStringLiteral("value"), QStringLiteral("URI"));
    }

    return line;
}

Sound VCardTool::parseSound(const VCardLine &line) const
{
    Sound snd;

    const QStringList params = line.parameterList();
    if (params.contains(QStringLiteral("encoding"))) {
        snd.setData(line.value().toByteArray());
    } else if (params.contains(QStringLiteral("value"))) {
        if (line.parameter(QStringLiteral("value")).toLower() == QLatin1String("uri")) {
            snd.setUrl(line.value().toString());
        }
    }

    /* TODO: support sound types
      if ( params.contains( "type" ) )
        snd.setType( line.parameter( "type" ) );
    */

    return snd;
}

VCardLine VCardTool::createSound(const Sound &snd, VCard::Version version) const
{
    Q_UNUSED(version);
    VCardLine line(QStringLiteral("SOUND"));

    if (snd.isIntern()) {
        if (!snd.data().isEmpty()) {
            line.setValue(snd.data());
            if (version == VCard::v2_1) {
                line.addParameter(QStringLiteral("ENCODING"), QStringLiteral("BASE64"));
            } else {
                line.addParameter(QStringLiteral("encoding"), QStringLiteral("b"));
            }
            // TODO: need to store sound type!!!
        }
    } else if (!snd.url().isEmpty()) {
        line.setValue(snd.url());
        line.addParameter(QStringLiteral("value"), QStringLiteral("URI"));
    }

    return line;
}

Key VCardTool::parseKey(const VCardLine &line) const
{
    Key key;

    const QStringList params = line.parameterList();
    if (params.contains(QStringLiteral("encoding"))) {
        key.setBinaryData(line.value().toByteArray());
    } else {
        key.setTextData(line.value().toString());
    }

    if (params.contains(QStringLiteral("type"))) {
        if (line.parameter(QStringLiteral("type")).toLower() == QLatin1String("x509")) {
            key.setType(Key::X509);
        } else if (line.parameter(QStringLiteral("type")).toLower() == QLatin1String("pgp")) {
            key.setType(Key::PGP);
        } else {
            key.setType(Key::Custom);
            key.setCustomTypeString(line.parameter(QStringLiteral("type")));
        }
    } else if (params.contains(QStringLiteral("mediatype"))) {
        const QString param = line.parameter(QStringLiteral("mediatype")).toLower();
        if (param == QLatin1String("application/x-x509-ca-cert")) {
            key.setType(Key::X509);
        } else if (param == QLatin1String("application/pgp-keys")) {
            key.setType(Key::PGP);
        } else {
            key.setType(Key::Custom);
            key.setCustomTypeString(line.parameter(QStringLiteral("type")));
        }
    }

    return key;
}

VCardLine VCardTool::createKey(const Key &key, VCard::Version version) const
{
    VCardLine line(QStringLiteral("KEY"));

    if (key.isBinary()) {
        if (!key.binaryData().isEmpty()) {
            line.setValue(key.binaryData());
            if (version == VCard::v2_1) {
                line.addParameter(QStringLiteral("ENCODING"), QStringLiteral("BASE64"));
            } else {
                line.addParameter(QStringLiteral("encoding"), QStringLiteral("b"));
            }
        }
    } else if (!key.textData().isEmpty()) {
        line.setValue(key.textData());
    }

    if (version == VCard::v4_0) {
        if (key.type() == Key::X509) {
            line.addParameter(QStringLiteral("MEDIATYPE"), QStringLiteral("application/x-x509-ca-cert"));
        } else if (key.type() == Key::PGP) {
            line.addParameter(QStringLiteral("MEDIATYPE"), QStringLiteral("application/pgp-keys"));
        } else if (key.type() == Key::Custom) {
            line.addParameter(QStringLiteral("MEDIATYPE"), key.customTypeString());
        }
    } else {
        if (key.type() == Key::X509) {
            line.addParameter(QStringLiteral("type"), QStringLiteral("X509"));
        } else if (key.type() == Key::PGP) {
            line.addParameter(QStringLiteral("type"), QStringLiteral("PGP"));
        } else if (key.type() == Key::Custom) {
            line.addParameter(QStringLiteral("type"), key.customTypeString());
        }
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
    VCardLine line(QStringLiteral("CLASS"));

    int type = secrecy.type();

    if (type == Secrecy::Public) {
        line.setValue(QStringLiteral("PUBLIC"));
    } else if (type == Secrecy::Private) {
        line.setValue(QStringLiteral("PRIVATE"));
    } else if (type == Secrecy::Confidential) {
        line.setValue(QStringLiteral("CONFIDENTIAL"));
    }

    return line;
}

QStringList VCardTool::splitString(QChar sep, const QString &str) const
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
    const QString mid = value.mid(start, l - start + 1);
    if (!mid.isEmpty()) {
        list << mid;
    } else {
        list << QString();
    }

    return list;
}

void VCardTool::imppService(const QString &serviceType, KContacts::Impp &impp) const
{
    if (serviceType == QLatin1String("facebook")) {
        impp.setType(KContacts::Impp::Facebook);
    } else if (serviceType == QLatin1String("jabber")) {
        impp.setType(KContacts::Impp::Jabber);
    } else if (serviceType == QLatin1String("sip")) {
        impp.setType(KContacts::Impp::Sip);
    } else if (serviceType == QLatin1String("aim")) {
        impp.setType(KContacts::Impp::Aim);
    } else if (serviceType == QLatin1String("msn")) {
        impp.setType(KContacts::Impp::Msn);
    } else if (serviceType == QLatin1String("twitter")) {
        impp.setType(KContacts::Impp::Twitter);
    } else if (serviceType == QLatin1String("googletalk")) {
        impp.setType(KContacts::Impp::GoogleTalk);
    } else if (serviceType == QLatin1String("xmpp")) {
        impp.setType(KContacts::Impp::Xmpp);
    } else if (serviceType == QLatin1String("icq")) {
        impp.setType(KContacts::Impp::Icq);
    } else if (serviceType == QLatin1String("yahoo")) {
        impp.setType(KContacts::Impp::Yahoo);
    } else if (serviceType == QLatin1String("qq")) {
        impp.setType(KContacts::Impp::Qq);
    } else if (serviceType == QLatin1String("gadugadu")) {
        impp.setType(KContacts::Impp::GaduGadu);
    } else if (serviceType == QLatin1String("owncloud-handle")) {
        impp.setType(KContacts::Impp::Ownclound);
    } else if (serviceType == QLatin1String("skype")) {
        impp.setType(KContacts::Impp::Skype);
    } else {
        qCDebug(KCONTACTS_LOG) << "unknown service type " << serviceType;
    }
}
