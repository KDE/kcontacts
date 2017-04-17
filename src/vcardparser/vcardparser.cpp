/*
    This file is part of the KContacts framework.
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

#include "vcardparser.h"
#include <kcodecs.h>
#include "kcontacts_debug.h"
#include <QtCore/QTextCodec>

// This cache for QString::fromLatin1() isn't about improving speed, but about reducing memory usage by sharing common strings
class StringCache
{
public:
    QString fromLatin1(const QByteArray &value)
    {
        if (value.isEmpty()) {
            return QString();
        }

        QHash<QByteArray, QString>::const_iterator it = m_values.constFind(value);
        if (it != m_values.constEnd()) {
            return it.value();
        }

        QString string = QString::fromLatin1(value);
        m_values.insert(value, string);
        return string;
    }

private:
    QHash<QByteArray, QString> m_values;
};

using namespace KContacts;

static void addEscapes(QByteArray &str, bool excludeEscapedComma)
{
    str.replace('\\', "\\\\");
    if (!excludeEscapedComma) {
        str.replace(',', "\\,");
    }
    str.replace('\r', "\\r");
    str.replace('\n', "\\n");
}

static void removeEscapes(QByteArray &str)
{
    // It's more likely that no escape is present, so add fast path
    if (!str.contains('\\')) {
        return;
    }
    str.replace("\\n", "\n");
    str.replace("\\N", "\n");
    str.replace("\\r", "\r");
    str.replace("\\,", ",");
    str.replace("\\\\", "\\");
}

class VCardLineParser
{
public:
    VCardLineParser(StringCache &cache, std::function<QByteArray()> fetchAnotherLine)
      : m_cache(cache), m_fetchAnotherLine(fetchAnotherLine)
    {
    }

    void parseLine(const QByteArray &currentLine, VCardLine &vCardLine);

private:
    StringCache &m_cache;
    std::function<QByteArray()> m_fetchAnotherLine;
};

void VCardLineParser::parseLine(const QByteArray& currentLine, KContacts::VCardLine& vCardLine)
{
    // ### The syntax is key:value, but the key can contain semicolon-separated parameters, which can contain a ':', so indexOf(':') is wrong.
    // EXAMPLE: "ADR;GEO=\"geo:22.500000,45.099998\";LABEL=\"My Label\";TYPE=home:P.O. Box 101;;;Any Town;CA;91921-1234;
    const int colon = currentLine.indexOf(':');
    if (colon == -1) {   // invalid line
        return;
    }
    const QByteArray key = currentLine.left(colon).trimmed();
    QByteArray value = currentLine.mid(colon + 1);
    const QList<QByteArray> params = key.split(';');
    //qDebug() << "key=" << QString::fromLatin1(key) << "params=" << params;
    // check for group
    const QByteArray firstParam = params.at(0);
    const int groupPos = firstParam.indexOf('.');
    if (groupPos != -1) {
        vCardLine.setGroup(m_cache.fromLatin1(firstParam.left(groupPos)));
        vCardLine.setIdentifier(m_cache.fromLatin1(firstParam.mid(groupPos + 1)));
        //qDebug() << "group" << vCardLine.group() << "identifier" << vCardLine.identifier();
    } else {
        vCardLine.setIdentifier(m_cache.fromLatin1(firstParam));
        //qDebug() << "identifier" << vCardLine.identifier();
    }

    if (params.count() > 1) {   // find all parameters
        QList<QByteArray>::ConstIterator paramIt(params.constBegin());
        for (++paramIt; paramIt != params.constEnd(); ++paramIt) {
            //qDebug() << "param" << QString::fromLatin1(*paramIt);
            QList<QByteArray> pair = (*paramIt).split('=');
            QByteArray first = pair.at(0).toLower();
            if (pair.count() == 1) {
                // correct the fucking 2.1 'standard'
                if (first == "quoted-printable") {
                    pair[ 0 ] = "encoding";
                    pair.append("quoted-printable");
                } else if (first == "base64") {
                    pair[ 0 ] = "encoding";
                    pair.append("base64");
                } else {
                    pair.prepend("type");
                }
                first = pair.at(0);
            }
            const QByteArray second = pair.at(1);
            if (second.contains(',')) {     // parameter in type=x,y,z format
                const QList<QByteArray> args = second.split(',');
                for (QByteArray tmpArg : args) {
                    if (tmpArg.startsWith('"')) {
                        tmpArg = tmpArg.mid(1);
                    }
                    if (tmpArg.endsWith('"')) {
                        tmpArg.chop(1);
                    }
                    vCardLine.addParameter(m_cache.fromLatin1(first),
                                           m_cache.fromLatin1(tmpArg));
                }
            } else {
                vCardLine.addParameter(m_cache.fromLatin1(first),
                                       m_cache.fromLatin1(second));
            }
        }
    }

    removeEscapes(value);

    QByteArray output;
    bool wasBase64Encoded = false;

    const QString encoding = vCardLine.parameter(QStringLiteral("encoding")).toLower();
    if (!encoding.isEmpty()) {

        // have to decode the data
        if (encoding == QLatin1String("b") || encoding == QLatin1String("base64")) {
            output = QByteArray::fromBase64(value);
            wasBase64Encoded = true;
        } else if (encoding == QLatin1String("quoted-printable")) {
            // join any qp-folded lines
            while (value.endsWith('=')) {
                value.chop(1);   // remove the '='
                value.append(m_fetchAnotherLine());
            }
            KCodecs::quotedPrintableDecode(value, output);
        } else if (encoding == QLatin1String("8bit")) {
            output = value;
        } else {
            qDebug("Unknown vcard encoding type!");
        }
    } else {
        output = value;
    }

    const QString charset = vCardLine.parameter(QStringLiteral("charset"));
    if (!charset.isEmpty()) {
        // have to convert the data
        QTextCodec *codec = QTextCodec::codecForName(charset.toLatin1());
        if (codec) {
            vCardLine.setValue(codec->toUnicode(output));
        } else {
            vCardLine.setValue(QString::fromUtf8(output));
        }
    } else if (wasBase64Encoded) {
        vCardLine.setValue(output);
    } else {
        vCardLine.setValue(QString::fromUtf8(output));
    }
}

VCardParser::VCardParser()
    : d(nullptr)
{
}

VCardParser::~VCardParser()
{
}

VCard::List VCardParser::parseVCards(const QByteArray &text)
{
    VCard currentVCard;
    VCard::List vCardList;
    QByteArray currentLine;

    const QList<QByteArray> lines = text.split('\n');

    bool inVCard = false;
    const QList<QByteArray>::const_iterator linesEnd(lines.end());

    StringCache cache;
    for (auto it = lines.begin(); it != linesEnd; ++it) {
        QByteArray cur = *it;
        // remove the trailing \r, left from \r\n
        if (cur.endsWith('\r')) {
            cur.chop(1);
        }

        if (cur.startsWith(' ') ||
            cur.startsWith('\t')) {      //folded line => append to previous
            currentLine.append(cur.mid(1));
            continue;
        } else {
            if (cur.trimmed().isEmpty()) {     // empty line
                continue;
            }
            if (inVCard && !currentLine.isEmpty()) {   // now parse the line
                VCardLine vCardLine;

                // Provide a way for the parseVCardLine function to read more lines (for quoted-printable support)
                auto fetchAnotherLine = [&it, &linesEnd, &cur]() -> QByteArray {
                    const QByteArray ret = cur;
                    if (it != linesEnd) {
                        cur = *(++it);
                        // remove the trailing \r, left from \r\n
                        if (cur.endsWith('\r')) {
                            cur.chop(1);
                        }
                    }
                    return ret;
                };

                VCardLineParser lineParser(cache, fetchAnotherLine);

                lineParser.parseLine(currentLine, vCardLine);

                currentVCard.addLine(vCardLine);
            }

            const QByteArray curLower = cur.toLower();

            // we do not save the start and end tag as vcardline
            if (curLower.startsWith("begin:vcard")) {       //krazy:exclude=strings
                inVCard = true;
                currentLine.clear();
                currentVCard.clear(); // flush vcard
                continue;
            }

            if (curLower.startsWith("end:vcard")) {       //krazy:exclude=strings
                inVCard = false;
                vCardList.append(currentVCard);
                currentLine.clear();
                currentVCard.clear(); // flush vcard
                continue;
            }

            currentLine = cur;
        }
    }

    return vCardList;
}

static const int FOLD_WIDTH = 75;

QByteArray VCardParser::createVCards(const VCard::List &list)
{
    QByteArray text;
    QByteArray textLine;
    QString encodingType;
    QStringList idents;
    QStringList params;
    QStringList values;
    QStringList::ConstIterator identIt;
    QStringList::Iterator paramIt;
    QStringList::ConstIterator valueIt;

    VCardLine::List lines;
    VCardLine::List::ConstIterator lineIt;
    VCard::List::ConstIterator cardIt;

    bool hasEncoding;

    text.reserve(list.size() * 300);   // reserve memory to be more efficient

    // iterate over the cards
    const VCard::List::ConstIterator listEnd(list.end());
    for (cardIt = list.begin(); cardIt != listEnd; ++cardIt) {
        text.append("BEGIN:VCARD\r\n");

        idents = (*cardIt).identifiers();
        //VERSION must be first
        if (idents.contains(QStringLiteral("VERSION"))) {
            const QString str = idents.takeAt(idents.indexOf(QStringLiteral("VERSION")));
            idents.prepend(str);
        }

        for (identIt = idents.constBegin(); identIt != idents.constEnd(); ++identIt) {
            lines = (*cardIt).lines((*identIt));

            // iterate over the lines
            for (lineIt = lines.constBegin(); lineIt != lines.constEnd(); ++lineIt) {
                QVariant val = (*lineIt).value();
                if (val.isValid()) {
                    if ((*lineIt).hasGroup()) {
                        textLine = (*lineIt).group().toLatin1() + '.' + (*lineIt).identifier().toLatin1();
                    } else {
                        textLine = (*lineIt).identifier().toLatin1();
                    }

                    params = (*lineIt).parameterList();
                    hasEncoding = false;
                    if (!params.isEmpty()) {   // we have parameters
                        for (paramIt = params.begin(); paramIt != params.end(); ++paramIt) {
                            if ((*paramIt) == QLatin1String("encoding")) {
                                hasEncoding = true;
                                encodingType = (*lineIt).parameter(QStringLiteral("encoding")).toLower();
                            }

                            values = (*lineIt).parameters(*paramIt);
                            for (valueIt = values.constBegin(); valueIt != values.constEnd(); ++valueIt) {
                                textLine.append(';' + (*paramIt).toLatin1().toUpper());
                                if (!(*valueIt).isEmpty()) {
                                    textLine.append('=' + (*valueIt).toLatin1());
                                }
                            }
                        }
                    }

                    QByteArray input, output;
                    bool checkMultibyte = false;  // avoid splitting a multibyte character

                    // handle charset
                    const QString charset = (*lineIt).parameter(QStringLiteral("charset"));
                    if (!charset.isEmpty()) {
                        // have to convert the data
                        const QString value = (*lineIt).value().toString();
                        QTextCodec *codec = QTextCodec::codecForName(charset.toLatin1());
                        if (codec) {
                            input = codec->fromUnicode(value);
                        } else {
                            checkMultibyte = true;
                            input = value.toUtf8();
                        }
                    } else if ((*lineIt).value().type() == QVariant::ByteArray) {
                        input = (*lineIt).value().toByteArray();
                    } else {
                        checkMultibyte = true;
                        input = (*lineIt).value().toString().toUtf8();
                    }

                    // handle encoding
                    if (hasEncoding) {   // have to encode the data
                        if (encodingType == QLatin1String("b")) {
                            checkMultibyte = false;
                            output = input.toBase64();
                        } else if (encodingType == QLatin1String("quoted-printable")) {
                            checkMultibyte = false;
                            KCodecs::quotedPrintableEncode(input, output, false);
                        }
                    } else {
                        output = input;
                    }
                    addEscapes(output, ((*lineIt).identifier() == QLatin1String("CATEGORIES") || (*lineIt).identifier() == QLatin1String("GEO")));

                    if (!output.isEmpty()) {
                        textLine.append(':' + output);

                        if (textLine.length() > FOLD_WIDTH) {   // we have to fold the line
                            if (checkMultibyte) {
                                // RFC 6350: Multi-octet characters MUST remain contiguous.
                                // we know that textLine contains UTF-8 encoded characters
                                int lineLength = 0;
                                for (int i = 0; i < textLine.length(); ++i) {
                                    if ((textLine[i] & 0xC0) == 0xC0) {    // a multibyte sequence follows
                                        int sequenceLength = 2;
                                        if ((textLine[i] & 0xE0) == 0xE0) {
                                            sequenceLength = 3;
                                        } else if ((textLine[i] & 0xF0) == 0xF0) {
                                            sequenceLength = 4;
                                        }
                                        if ((lineLength + sequenceLength) > FOLD_WIDTH) {
                                            // the current line would be too long. fold it
                                            text += "\r\n " + textLine.mid(i, sequenceLength);
                                            lineLength = 1 + sequenceLength;  // incl. leading space
                                        } else {
                                            text += textLine.mid(i, sequenceLength);
                                            lineLength += sequenceLength;
                                        }
                                        i += sequenceLength - 1;
                                    } else {
                                        text += textLine[i];
                                        ++lineLength;
                                    }
                                    if ((lineLength == FOLD_WIDTH) && (i < (textLine.length() - 1))) {
                                        text += "\r\n ";
                                        lineLength = 1;  // leading space
                                    }
                                }
                                text += "\r\n";
                            } else {
                                for (int i = 0; i <= (textLine.length() / FOLD_WIDTH); ++i) {
                                    text.append(
                                        (i == 0 ? "" : " ") + textLine.mid(i * FOLD_WIDTH, FOLD_WIDTH) + "\r\n");
                                }
                            }
                        } else {
                            text.append(textLine + "\r\n");
                        }
                    }
                }
            }
        }

        text.append("END:VCARD\r\n");
        text.append("\r\n");
    }

    return text;
}
