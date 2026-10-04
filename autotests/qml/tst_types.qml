/*
    SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/
import QtQuick
import QtTest
import org.kde.contacts

TestCase {
    id: root
    name: "ContactTypes"

    property address postalAddress: ({ street: "Main Street", locality: "Berlin" })
    property addressee contact: ({ givenName: "Carl", addresses: [root.postalAddress] })
    property email mail: "carl@example.com"
    property phoneNumber phone: ({ number: "+49123456789" })
    property impp messaging: ({ address: "xmpp:carl@example.com", isPreferred: true })
    property picture photo: ({ url: "https://example.com/photo.png" })
    property geo location: ({ latitude: 52.5, longitude: 13.4 })

    function test_address(): void {
        compare(root.postalAddress.street, "Main Street");
        compare(root.postalAddress.locality, "Berlin");
        compare(root.postalAddress.isEmpty, false);
    }

    function test_addressee(): void {
        compare(root.contact.givenName, "Carl");
        compare(root.contact.addresses.length, 1);
        compare(root.contact.addresses[0].street, "Main Street");
    }

    function test_email(): void {
        compare(root.mail.email, "carl@example.com");
        compare(root.mail.isValid, true);
    }

    function test_phoneNumber(): void {
        compare(root.phone.number, "+49123456789");
        compare(root.phone.isEmpty, false);
    }

    function test_impp(): void {
        compare(root.messaging.address.toString(), "xmpp:carl@example.com");
        compare(root.messaging.isPreferred, true);
        compare(root.messaging.isValid, true);
    }

    function test_picture(): void {
        compare(root.photo.url, "https://example.com/photo.png");
        compare(root.photo.isEmpty, false);
    }

    function test_geo(): void {
        fuzzyCompare(root.location.latitude, 52.5, 0.001);
        fuzzyCompare(root.location.longitude, 13.4, 0.001);
        compare(root.location.isValid, true);
    }
}
