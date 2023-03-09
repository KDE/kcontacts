/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "../src/addressformat.h"
#include <KContacts/Address>
#include <KContacts/Addressee>

using namespace KContacts;

int main(int argc, char **argv)
{
    QCoreApplication::setApplicationName(QStringLiteral("kcontacts-qmlintegrationtest"));
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));

    QGuiApplication app(argc, argv);

    // ### this should probably eventually move into a real QML plugin for KContacts
    qmlRegisterUncreatableMetaObject(KContacts::staticMetaObject, "org.kde.contacts", 1, 0, "KContacts", {});
    qmlRegisterSingletonType("org.kde.contacts", 1, 0, "AddressFormatRepository", [](QQmlEngine *, QJSEngine *jsEngine) -> QJSValue {
        return jsEngine->toScriptValue(AddressFormatRepository());
    });
    qRegisterMetaType<AddressFormat>();
    qRegisterMetaType<AddressFormatElement>();
    qRegisterMetaType<AddressFormatRepository>();
    qRegisterMetaType<AddressFormatPreference>();
    qRegisterMetaType<AddressFormatScriptPreference>();

    // test data
    qmlRegisterSingletonType("org.kde.contacts.test", 1, 0, "TestData", [](QQmlEngine *, QJSEngine *jsEngine) -> QJSValue {
        Address address;
        address.setCountry(QStringLiteral("DE"));
        address.setRegion(QStringLiteral("BE"));
        address.setLocality(QStringLiteral("Berlin"));
        address.setPostalCode(QStringLiteral("10969"));
        address.setStreet(QStringLiteral("Prinzenstraße 85 F"));

        Addressee addressee;
        addressee.insertAddress(address);
        addressee.setEmails({QStringLiteral("one@test.local"), QStringLiteral("two@test.local")});

        auto obj = jsEngine->newObject();
        obj.setProperty(QStringLiteral("address"), jsEngine->toScriptValue(address));
        obj.setProperty(QStringLiteral("addressee"), jsEngine->toScriptValue(addressee));
        return obj;
    });

    QQmlApplicationEngine engine;
    engine.load(QStringLiteral("qrc:/qmlintegrationtest.qml"));
    return app.exec();
}
