/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2003 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_VCARDTOOL_H
#define KCONTACTS_VCARDTOOL_H

#include "addressee.h"
#include "kcontacts_export.h"
#include "vcardparser/vcardparser.h"

class QDateTime;

namespace KContacts
{
class Key;
class Picture;
class Secrecy;
class Sound;

class KCONTACTS_EXPORT VCardTool
{
public:
    VCardTool();
    ~VCardTool();

    /**
      Creates a string that contains the addressees from the list in
      the vCard format.
     */
    Q_REQUIRED_RESULT QByteArray createVCards(const Addressee::List &list, VCard::Version version = VCard::v3_0) const;

    /**
     * @since 4.9.1
     */
    Q_REQUIRED_RESULT QByteArray exportVCards(const Addressee::List &list, VCard::Version version = VCard::v3_0) const;
    /**
      Parses the string and returns a list of addressee objects.
     */
    Q_REQUIRED_RESULT Addressee::List parseVCards(const QByteArray &vcard) const;

    static QDateTime parseDateTime(const QString &str, bool *timeValid = nullptr);
    static QString createDateTime(const QDateTime &dateTime, VCard::Version version, bool withTime = true);
    static QString createDate(const QDate &date, VCard::Version version);
    static QString createTime(const QTime &time, VCard::Version version);

private:
    QByteArray createVCards(const Addressee::List &list, VCard::Version version, bool exportVcard) const;

    /**
      Split a string and replaces escaped separators on the fly with
      unescaped ones.
     */
    QStringList splitString(QChar sep, const QString &value) const;

    Picture parsePicture(const VCardLine &line) const;
    VCardLine createPicture(const QString &identifier, const Picture &pic, VCard::Version version) const;

    Sound parseSound(const VCardLine &line) const;
    VCardLine createSound(const Sound &snd, VCard::Version version) const;

    Key parseKey(const VCardLine &line) const;
    VCardLine createKey(const Key &key, VCard::Version version) const;

    Secrecy parseSecrecy(const VCardLine &line) const;
    VCardLine createSecrecy(const Secrecy &secrecy) const;

    void addParameter(VCardLine &line, VCard::Version version, const QString &key, const QStringList &valueStringList) const;
    void addParameters(VCardLine &line, const QMap<QString, QStringList> &params) const;

    /** Translate alternative or legacy IMPP service types. */
    QString normalizeImppServiceType(const QString &serviceType) const;

    void processAddresses(const Address::List &addresses, VCard::Version version, VCard *card) const;
    void processEmailList(const Email::List &emailList, VCard::Version version, VCard *card) const;
    void processOrganizations(const Addressee &addressee, VCard::Version version, VCard *card) const;

    Q_DISABLE_COPY(VCardTool)
};
}

#endif
