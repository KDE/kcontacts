/*
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "vcardparser_p.h"
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
};

QTEST_MAIN(VCardParserTest)
#include "vcardparsertest.moc"
