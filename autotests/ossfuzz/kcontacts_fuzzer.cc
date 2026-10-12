// SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
// SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: LGPL-2.0-or-later

#include "converter/vcardconverter.h"

#include <QCoreApplication>
#include <QtEnvironmentVariables>

#include <cstddef>
#include <cstdint>
#include <initializer_list>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    qputenv("QT_ENABLE_REGEXP_JIT", "1");

    int argc = 0;
    QCoreApplication application(argc, nullptr);

    const QByteArray input(reinterpret_cast<const char *>(data), static_cast<qsizetype>(size));
    KContacts::VCardConverter converter;
    const auto contacts = converter.parseVCards(input);

    for (const auto version : {KContacts::VCardConverter::v2_1, KContacts::VCardConverter::v3_0, KContacts::VCardConverter::v4_0}) {
        const auto serialized = converter.createVCards(contacts, version);
        const auto exported = converter.exportVCards(contacts, version);
        (void)converter.parseVCards(serialized);
        (void)converter.parseVCards(exported);
    }
    return 0;
}
