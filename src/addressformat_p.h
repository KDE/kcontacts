/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_ADDRESSFORMAT_P_H
#define KCONTACTS_ADDRESSFORMAT_P_H

#include "addressformat.h"

#include <QSharedData>

namespace KContacts
{

/**
 * Classify the script used in an address.
 * This is used for two things:
 * - determining the line style separator
 * - decide whether to use local or latin script format alternatives when available
 *
 * @see address_formatter.cc in libaddressinput
 * @internal
 */
namespace AddressFormatScript
{
enum ScriptType {
    LatinLikeScript,
    ArabicLikeScript,
    HanLikeScript,
    HangulLikeScript,
};

ScriptType detect(const QString &s);
ScriptType detect(const Address &addr);
}

class AddressFormatElementPrivate : public QSharedData
{
public:
    static inline AddressFormatElementPrivate *get(const AddressFormatElement &elem)
    {
        return elem.d.data();
    }

    AddressFormatField field = AddressFormatField::NoField;
    QString literal;
};

class AddressFormatPrivate : public QSharedData
{
public:
    static inline AddressFormatPrivate *get(const AddressFormat &format)
    {
        return format.d.data();
    }

    AddressFormatFields upper = AddressFormatField::NoField;
    AddressFormatFields required = AddressFormatField::NoField;
    std::vector<AddressFormatElement> elements;
    QString postalCodeFormat;
    QString country;
};

}

#endif // KCONTACTS_ADDRESSFORMAT_P_H
