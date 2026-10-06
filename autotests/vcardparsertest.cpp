/*
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "addressee.h"
#include "parametermap_p.h"
#include "vcardparser_p.h"
#include "vcardtool_p.h"
#include <QTest>

using namespace KContacts;
using namespace Qt::StringLiterals;

class VCardParserTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void rfcAddressExample()
    {
        // RFC 6868, Section 3.2
        const QByteArray input =
            "BEGIN:VCARD\r\nVERSION:4.0\r\n"
            "GEO;X-ADDRESS=\"Pittsburgh Pirates^n115 Federal St^nPitt\r\n"
            " sburgh, PA 15212\":geo:40.446816,-80.00566\r\nEND:VCARD\r\n";
        const auto card = VCardParser::parseVCards(input).first();
        QCOMPARE(card.line(u"GEO"_s).parameter(u"x-address"_s), u"Pittsburgh Pirates\n115 Federal St\nPittsburgh, PA 15212"_s);
        const auto output = VCardParser::createVCards({card});
        QVERIFY(output.contains("Pittsburgh Pirates^n115 Federal St^nPitt"));
        QCOMPARE(VCardParser::parseVCards(output).first().line(u"GEO"_s).parameter(u"x-address"_s), card.line(u"GEO"_s).parameter(u"x-address"_s));
    }

    void parameterRoundTrip_data()
    {
        QTest::addColumn<QString>("identifier");
        QTest::addColumn<QString>("version");
        QTest::newRow("name-v4") << u"N"_s << u"4.0"_s;
        QTest::newRow("note-v4") << u"NOTE"_s << u"4.0"_s;
        QTest::newRow("note-v3") << u"NOTE"_s << u"3.0"_s;
        QTest::newRow("note-v21") << u"NOTE"_s << u"2.1"_s;
    }

    void parameterRoundTrip()
    {
        QFETCH(QString, identifier);
        QFETCH(QString, version);
        VCard card;
        card.addLine(VCardLine(u"VERSION"_s, version));
        VCardLine line(identifier, u"Value"_s);
        const QString authorName = QString(70, u'x') + u"😀 Émilie, Müller; Editor"_s;
        line.addParameter(u"x-uri"_s, u"mailto:author@example.org"_s);
        line.addParameter(u"x-name"_s, authorName);
        line.addParameters(ParameterMap::fromQMap({{u"x-values"_s, {u"first,with comma"_s, u"second;with semicolon"_s}}}));
        card.addLine(line);
        const auto output = VCardParser::createVCards({card});
        auto unfolded = output;
        unfolded.replace("\r\n ", "");
        QVERIFY(unfolded.contains("X-URI=\"mailto:author@example.org\""));
        QVERIFY(unfolded.contains("X-VALUES=\"first,with comma\",\"second;with semicolon\""));
        const auto parsed = VCardParser::parseVCards(output).first().line(identifier);
        QCOMPARE(parsed.parameter(u"x-uri"_s), u"mailto:author@example.org"_s);
        QCOMPARE(parsed.parameter(u"x-name"_s), authorName);
        QCOMPARE(parsed.parameters(u"x-values"_s), QStringList({u"first,with comma"_s, u"second;with semicolon"_s}));
        QCOMPARE(parsed.value(), line.value());
        for (const auto &physicalLine : output.split('\n')) {
            QVERIFY2(physicalLine.isValidUtf8(), physicalLine.constData());
        }
    }

    void structuredNameLists()
    {
        const QByteArray property = "N:Smith,Jones;John,Paul;;;PhD,Jr.";
        const auto card = VCardParser::parseVCards("BEGIN:VCARD\r\nVERSION:4.0\r\n" + property + "\r\nEND:VCARD\r\n").first();
        const auto output = VCardParser::createVCards({card});
        QVERIFY(output.contains(property));
        QCOMPARE(VCardParser::parseVCards(output).first().line(u"N"_s).value(), card.line(u"N"_s).value());
    }

    void legacyLatin1Parameter()
    {
        const QByteArray input = "BEGIN:VCARD\r\nVERSION:3.0\r\nNOTE;X-TEXT=Andr\xe9:Value\r\nEND:VCARD\r\n";
        const auto card = VCardParser::parseVCards(input).first();
        QCOMPARE(card.line(u"NOTE"_s).parameter(u"x-text"_s), u"André"_s);
        const auto output = VCardParser::createVCards({card});
        QCOMPARE(VCardParser::parseVCards(output).first().line(u"NOTE"_s).parameter(u"x-text"_s), u"André"_s);
    }

    void caretEscapes()
    {
        const QByteArray input = "BEGIN:VCARD\r\nVERSION:4.0\r\nNOTE;X-TEXT=^^n^n^'quoted^'^z^N^:Value\r\nEND:VCARD\r\n";
        const auto card = VCardParser::parseVCards(input).first();
        const auto text = card.line(u"NOTE"_s).parameter(u"x-text"_s);
        QCOMPARE(text, u"^n\n\"quoted\"^z^N^"_s);
        const auto output = VCardParser::createVCards({card});
        QCOMPARE(VCardParser::parseVCards(output).first().line(u"NOTE"_s).parameter(u"x-text"_s), text);
        const auto legacy = VCardParser::parseVCards(QByteArray(input).replace("VERSION:4.0", "VERSION:3.0")).first();
        QCOMPARE(legacy.line(u"NOTE"_s).parameter(u"x-text"_s), u"^^n^n^'quoted^'^z^N^"_s);
    }

    void addressParameters()
    {
        Address address;
        address.setStreet(u"Main Street"_s);
        address.setLabel(u"\"Émilie\"\nMain Street, Town"_s);
        Geo geo;
        geo.setLatitude(12.5);
        geo.setLongitude(45.5);
        address.setGeo(geo);
        Addressee contact;
        contact.insertAddress(address);
        VCardTool tool;
        const auto output = tool.createVCards({contact}, VCard::v4_0);
        const auto restored = tool.parseVCards(output).first().addresses().first();
        QCOMPARE(restored.label(), address.label());
        QCOMPARE(restored.geo(), address.geo());
    }
};
QTEST_MAIN(VCardParserTest)
#include "vcardparsertest.moc"
