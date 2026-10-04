/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gendertest.h"
#include "gender.h"
#include "grammaticalgender.h"
#include "vcardtool_p.h"
#include <QMetaProperty>
#include <QTest>

GenderTest::GenderTest(QObject *parent)
    : QObject(parent)
{
}

GenderTest::~GenderTest()
{
}

void GenderTest::shouldHaveDefaultValue()
{
    KContacts::Gender gender;
    QVERIFY(!gender.isValid());
    QVERIFY(gender.gender().isEmpty());
    QVERIFY(gender.comment().isEmpty());
}

void GenderTest::shouldAssignValue()
{
    const QString genderStr(QStringLiteral("F"));
    KContacts::Gender gender(genderStr);
    const QString commentStr(QStringLiteral("foo"));
    gender.setComment(commentStr);
    QVERIFY(gender.isValid());
    QVERIFY(!gender.gender().isEmpty());
    QCOMPARE(gender.gender(), genderStr);
    QVERIFY(!gender.comment().isEmpty());
    QCOMPARE(gender.comment(), commentStr);
}

void GenderTest::shouldAssignExternal()
{
    KContacts::Gender gender;
    const QString genderStr(QStringLiteral("H"));
    gender.setGender(genderStr);
    QVERIFY(gender.isValid());
    QVERIFY(!gender.gender().isEmpty());
    QCOMPARE(gender.gender(), genderStr);
}

void GenderTest::shouldSerialized()
{
    KContacts::Gender gender;
    KContacts::Gender result;
    const QString genderStr(QStringLiteral("H"));
    gender.setGender(genderStr);
    gender.setComment(QStringLiteral("foo"));

    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << gender;

    QDataStream t(&data, QIODevice::ReadOnly);
    t >> result;

    QVERIFY(gender == result);
}

void GenderTest::shouldEqualGender()
{
    KContacts::Gender gender;
    KContacts::Gender result;
    const QString genderStr(QStringLiteral("H"));
    gender.setGender(genderStr);
    gender.setComment(QStringLiteral("foo"));

    result = gender;
    QVERIFY(gender == result);
}

void GenderTest::shouldParseGender_data()
{
    QTest::addColumn<QByteArray>("vcarddata");
    QTest::addColumn<QString>("genre");
    QTest::addColumn<QString>("comment");
    QTest::addColumn<bool>("hasGender");

    QByteArray str(
        "BEGIN:VCARD\n"
        "VERSION:3.0\n"
        "N:LastName;FirstName;;;\n"
        "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
        "REV:2015-03-14T09:24:45+00:00\n"
        "FN:FirstName LastName\n"
        "END:VCARD\n");
    QTest::newRow("nogender") << str << QString() << QString() << false;

    str = QByteArray(
        "BEGIN:VCARD\n"
        "VERSION:3.0\n"
        "N:LastName;FirstName;;;\n"
        "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
        "REV:2015-03-14T09:24:45+00:00\n"
        "FN:FirstName LastName\n"
        "GENDER:H\n"
        "END:VCARD\n");
    QTest::newRow("hasgenderbutnocomment") << str << QStringLiteral("H") << QString() << true;

    str = QByteArray(
        "BEGIN:VCARD\n"
        "VERSION:3.0\n"
        "N:LastName;FirstName;;;\n"
        "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
        "REV:2015-03-14T09:24:45+00:00\n"
        "FN:FirstName LastName\n"
        "GENDER:;foo\n"
        "END:VCARD\n");
    QTest::newRow("hasgenderbutnotypebutcomment") << str << QString() << QStringLiteral("foo") << true;

    str = QByteArray(
        "BEGIN:VCARD\n"
        "VERSION:3.0\n"
        "N:LastName;FirstName;;;\n"
        "UID:c80cf296-0825-4eb0-ab16-1fac1d522a33@xxxxxx.xx\n"
        "REV:2015-03-14T09:24:45+00:00\n"
        "FN:FirstName LastName\n"
        "GENDER:H;foo\n"
        "END:VCARD\n");
    QTest::newRow("hasgendertypeandcomment") << str << QStringLiteral("H") << QStringLiteral("foo") << true;
}

void GenderTest::shouldParseGender()
{
    QFETCH(QByteArray, vcarddata);
    QFETCH(QString, genre);
    QFETCH(QString, comment);
    QFETCH(bool, hasGender);

    KContacts::VCardTool vcard;
    const KContacts::AddresseeList lst = vcard.parseVCards(vcarddata);
    QCOMPARE(lst.count(), 1);
    QCOMPARE(lst.at(0).gender().isValid(), hasGender);
    QCOMPARE(lst.at(0).gender().comment(), comment);
    QCOMPARE(lst.at(0).gender().gender(), genre);
}

QByteArray GenderTest::createCard(const QByteArray &gender)
{
    QByteArray expected(
        "BEGIN:VCARD\r\n"
        "VERSION:4.0\r\n"
        "EMAIL:foo@kde.org\r\n");
    if (!gender.isEmpty()) {
        expected += gender + "\r\n";
    }
    expected += QByteArray(
        "N:;;;;\r\n"
        "UID:testuid\r\n"
        "END:VCARD\r\n\r\n");
    return expected;
}

void GenderTest::shouldExportEmptyGender()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected = createCard(QByteArray());
    QCOMPARE(ba, expected);
}

void GenderTest::shouldExportOnlyGenderWithoutCommentGender()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Gender gender;
    gender.setGender(QStringLiteral("H"));
    addr.setGender(gender);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected = createCard(QByteArray("GENDER:H"));
    QCOMPARE(ba, expected);
}

void GenderTest::shouldExportOnlyGenderWithCommentGender()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Gender gender;
    gender.setGender(QStringLiteral("H"));
    gender.setComment(QStringLiteral("comment"));
    addr.setGender(gender);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected = createCard(QByteArray("GENDER:H;comment"));
    QCOMPARE(ba, expected);
}

void GenderTest::shouldExportOnlyGenderWithoutTypeCommentGender()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Gender gender;
    gender.setComment(QStringLiteral("comment"));
    addr.setGender(gender);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v4_0);
    QByteArray expected = createCard(QByteArray("GENDER:;comment"));
    QCOMPARE(ba, expected);
}

void GenderTest::shouldNotExportInVcard3()
{
    KContacts::AddresseeList lst;
    KContacts::Addressee addr;
    addr.setEmails(QStringList() << QStringLiteral("foo@kde.org"));
    addr.setUid(QStringLiteral("testuid"));
    KContacts::Gender gender;
    gender.setComment(QStringLiteral("comment"));
    addr.setGender(gender);
    lst << addr;
    KContacts::VCardTool vcard;
    const QByteArray ba = vcard.exportVCards(lst, KContacts::VCard::v3_0);
    QByteArray expected(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "EMAIL:foo@kde.org\r\n"
        "N:;;;;\r\n"
        "UID:testuid\r\n"
        "END:VCARD\r\n\r\n");
    QCOMPARE(ba, expected);
}

QTEST_MAIN(GenderTest)

#include "moc_gendertest.cpp"

void GenderTest::grammaticalGenderLanguage()
{
    using namespace Qt::StringLiterals;
    KContacts::GrammaticalGender gender(u"feminine"_s);
    QVERIFY(gender.language().isEmpty());
    gender.setParameters({{u"LANGUAGE"_s, {u"de"_s}}, {u"PROP-ID"_s, {u"g1"_s}}});
    QCOMPARE(gender.language(), u"de"_s);
    auto copy = gender;
    const auto property = KContacts::GrammaticalGender::staticMetaObject.property(KContacts::GrammaticalGender::staticMetaObject.indexOfProperty("language"));
    QVERIFY(property.writeOnGadget(&copy, u"en"_s));
    QCOMPARE(copy.language(), u"en"_s);
    QCOMPARE(gender.language(), u"de"_s);
    QCOMPARE(copy.gender(), u"feminine"_s);
    QCOMPARE(copy.parameters().value(u"prop-id"_s), QStringList{u"g1"_s});
    KContacts::Addressee contact;
    contact.setGrammaticalGenders({gender, copy});
    KContacts::VCardTool tool;
    const auto restored = tool.parseVCards(tool.createVCards({contact}, KContacts::VCard::v4_0)).first();
    QCOMPARE(restored.grammaticalGenders(), contact.grammaticalGenders());
    QByteArray data;
    QDataStream writer(&data, QIODevice::WriteOnly);
    writer << copy;
    QDataStream reader(data);
    KContacts::GrammaticalGender streamed;
    reader >> streamed;
    QCOMPARE(streamed, copy);
    copy.setLanguage({});
    QVERIFY(copy.language().isEmpty());
    QVERIFY(!copy.parameters().contains(u"language"_s));
    QCOMPARE(copy.parameters().value(u"prop-id"_s), QStringList{u"g1"_s});
    const auto grouped = tool.parseVCards(
                                 "BEGIN:VCARD\r\nVERSION:4.0\r\nFN:Example\r\n"
                                 "item1.GRAMGENDER;LANGUAGE=de:feminine\r\n"
                                 "item1.PRONOUNS;LANGUAGE=en:they/them\r\n"
                                 "item1.SOCIALPROFILE:https://example.com/profile\r\nEND:VCARD\r\n")
                             .first();
    QVERIFY(grouped.grammaticalGenders().isEmpty());
    QVERIFY(grouped.pronouns().isEmpty());
    QVERIFY(grouped.socialProfiles().isEmpty());
    QCOMPARE(grouped.fieldGroupList().size(), 3);
    const auto groupedRestored = tool.parseVCards(tool.createVCards({grouped}, KContacts::VCard::v4_0)).first();
    QCOMPARE(groupedRestored.fieldGroupList(), grouped.fieldGroupList());
    QCOMPARE(KContacts::GrammaticalGender::staticMetaObject.indexOfProperty("group"), -1);
    QCOMPARE(KContacts::Pronouns::staticMetaObject.indexOfProperty("group"), -1);
    QCOMPARE(KContacts::SocialProfile::staticMetaObject.indexOfProperty("group"), -1);
    copy.setLanguage(u"fr"_s);
    QCOMPARE(copy.language(), u"fr"_s);
}
