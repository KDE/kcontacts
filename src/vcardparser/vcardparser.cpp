/*
    This file is part of the KContacts framework..
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
#include <QDebug>
#include <QtCore/QTextCodec>

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

#define FOLD_WIDTH 75

using namespace KContacts;

static void addEscapes(QByteArray &str, bool excludeEscapteComma)
{
    str.replace('\\', (char *)"\\\\");
    if (!excludeEscapteComma) {
        str.replace(',', (char *)"\\,");
    }
    str.replace('\r', (char *)"\\r");
    str.replace('\n', (char *)"\\n");
}

static void removeEscapes(QByteArray &str)
{
    str.replace((char *)"\\n", "\n");
    str.replace((char *)"\\N", "\n");
    str.replace((char *)"\\r", "\r");
    str.replace((char *)"\\,", ",");
    str.replace((char *)"\\\\", "\\");
}

VCardParser::VCardParser()
    : d(0)
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

    QList<QByteArray> lines = text.split('\n');

    bool inVCard = false;
    QList<QByteArray>::Iterator it(lines.begin());
    QList<QByteArray>::Iterator linesEnd(lines.end());

    StringCache cache;

    for (; it != linesEnd; ++it) {
        // remove the trailing \r, left from \r\n
        if ((*it).endsWith('\r')) {
            (*it).chop(1);
        }

        if ((*it).startsWith(' ') ||
                (*it).startsWith('\t')) {      //folded line => append to previous
            currentLine.append((*it).mid(1));
            continue;
        } else {
            if ((*it).trimmed().isEmpty()) {     // empty line
                continue;
            }
            if (inVCard && !currentLine.isEmpty()) {   // now parse the line
                int colon = currentLine.indexOf(':');
                if (colon == -1) {   // invalid line
                    currentLine = (*it);
                    continue;
                }

                VCardLine vCardLine;
                const QByteArray key = currentLine.left(colon).trimmed();
                QByteArray value = currentLine.mid(colon + 1);

                QList<QByteArray> params = key.split(';');

                // check for group
                int groupPos = params[ 0 ].indexOf('.');
                if (groupPos != -1) {
                    vCardLine.setGroup(cache.fromLatin1(params[ 0 ].left(groupPos)));
                    vCardLine.setIdentifier(cache.fromLatin1(params[ 0 ].mid(groupPos + 1)));
                } else {
                    vCardLine.setIdentifier(cache.fromLatin1(params[ 0 ]));
                }

                if (params.count() > 1) {   // find all parameters
                    QList<QByteArray>::ConstIterator paramIt(params.constBegin());
                    for (++paramIt; paramIt != params.constEnd(); ++paramIt) {
                        QList<QByteArray> pair = (*paramIt).split('=');
                        if (pair.count() == 1) {
                            // correct the fucking 2.1 'standard'
                            if (pair[ 0 ].toLower() == "quoted-printable") {
                                pair[ 0 ] = "encoding";
                                pair.append("quoted-printable");
                            } else if (pair[ 0 ].toLower() == "base64") {
                                pair[ 0 ] = "encoding";
                                pair.append("base64");
                            } else {
                                pair.prepend("type");
                            }
                        }
                        if (pair[ 1 ].indexOf(',') != -1) {     // parameter in type=x,y,z format
                            const QList<QByteArray> args = pair[ 1 ].split(',');
                            QList<QByteArray>::ConstIterator argIt;
                            QList<QByteArray>::ConstIterator argEnd(args.constEnd());
                            for (argIt = args.constBegin(); argIt != argEnd; ++argIt) {
                                vCardLine.addParameter(cache.fromLatin1(pair[ 0 ].toLower()),
                                                       cache.fromLatin1(*argIt));
                            }
                        } else {
                            vCardLine.addParameter(cache.fromLatin1(pair[ 0 ].toLower()),
                                                   cache.fromLatin1(pair[ 1 ]));
                        }
                    }
                }

                removeEscapes(value);

                QByteArray output;
                bool wasBase64Encoded = false;

                if (vCardLine.parameterList().contains(QLatin1String("encoding"))) {
                    const QString encoding = vCardLine.parameter(QLatin1String("encoding")).toLower();

                    // have to decode the data
                    if (encoding == QLatin1String("b") || encoding == QLatin1String("base64")) {
                        output = QByteArray::fromBase64(value);
                        wasBase64Encoded = true;
                    } else if (encoding == QLatin1String("quoted-printable")) {
                        // join any qp-folded lines
                        while (value.endsWith('=') && it != linesEnd) {
                            value.chop(1);   // remove the '='
                            value.append(*it);
                            ++it;
                            // remove the trailing \r, left from \r\n
                            if ((*it).endsWith('\r')) {
                                (*it).chop(1);
                            }
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

                if (vCardLine.parameterList().contains(QLatin1String("charset"))) {
                    // have to convert the data
                    QTextCodec *codec = QTextCodec::codecForName(
                                            vCardLine.parameter(QLatin1String("charset")).toLatin1());
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

                currentVCard.addLine(vCardLine);
            }

            // we do not save the start and end tag as vcardline
            if ((*it).toLower().startsWith("begin:vcard")) {       //krazy:exclude=strings
                inVCard = true;
                currentLine.clear();
                currentVCard.clear(); // flush vcard
                continue;
            }

            if ((*it).toLower().startsWith("end:vcard")) {       //krazy:exclude=strings
                inVCard = false;
                vCardList.append(currentVCard);
                currentLine.clear();
                currentVCard.clear(); // flush vcard
                continue;
            }

            currentLine = (*it);
        }
    }

    return vCardList;
}

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
    VCard::List::ConstIterator listEnd(list.end());
    for (cardIt = list.begin(); cardIt != listEnd; ++cardIt) {
        text.append("BEGIN:VCARD\r\n");

        idents = (*cardIt).identifiers();
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
                                encodingType = (*lineIt).parameter(QLatin1String("encoding")).toLower();
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
                    if ((*lineIt).parameterList().contains(QLatin1String("charset"))) {
                        // have to convert the data
                        const QString value = (*lineIt).value().toString();
                        QTextCodec *codec = QTextCodec::codecForName(
                                                (*lineIt).parameter(QLatin1String("charset")).toLatin1());
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
                    addEscapes(output, (*lineIt).identifier() == QLatin1String("CATEGORIES"));

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
