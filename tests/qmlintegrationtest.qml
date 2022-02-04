/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import org.kde.i18n.localeData 1.0
import org.kde.contacts 1.0
import org.kde.contacts.test 1.0

ApplicationWindow {
    visible: true
    width: 480
    height: 720

    ColumnLayout {
        ComboBox {
            id: styleCombo
            textRole: "text"
            valueRole: "style"
            model: [
                { text: "Postal address", style: KContacts.AddressFormatStyle.Postal },
                { text: "Multiple lines, domestic", style: KContacts.AddressFormatStyle.MultiLineDomestic },
                { text: "Multiple lines, international", style: KContacts.AddressFormatStyle.MultiLineInternational },
                { text: "Single line, domestic", style: KContacts.AddressFormatStyle.SingleLineDomestic },
                { text: "Single line, international", style: KContacts.AddressFormatStyle.SingleLineInternational }
            ]
        }

        Label {
            text: TestData.address.formatted(styleCombo.currentValue, "Dr. Konqi", "KDE e.V.")
        }

        ComboBox {
            id: countryCombo
            model: Country.allCountries.map(c => c.alpha2)
        }

        Label {
            id: zipFormat
            text: AddressFormatRepository.formatForCountry(countryCombo.currentValue, KContacts.AddressFormatScriptPreference.Local).postalCodeRegularExpression
        }
        TextField {
            id: zipField
            text: "12345"
        }
        Label {
            text: zipField.text.match("^" + zipFormat.text + "$") ? "is a valid zip code" : "is not a valid zip code"
        }

        Label {
            text: "Formatting rule:"
        }
        Repeater {
            model: AddressFormatRepository.formatForCountry(countryCombo.currentValue, KContacts.AddressFormatScriptPreference.Local, KContacts.AddressFormatPreference.Business).elements
            Label {
                text: {
                    const field_map = new Map([
                        [ KContacts.AddressFormatField.Name, "Name" ],
                        [ KContacts.AddressFormatField.Organization, "Organization" ],
                        [ KContacts.AddressFormatField.StreetAddress, "Street" ],
                        [ KContacts.AddressFormatField.Locality, "City" ],
                        [ KContacts.AddressFormatField.DependentLocality, "Dependent Locality" ],
                        [ KContacts.AddressFormatField.PostOfficeBox, "PO Box" ],
                        [ KContacts.AddressFormatField.Country, "Country" ],
                        [ KContacts.AddressFormatField.Region, "State" ],
                        [ KContacts.AddressFormatField.SortingCode, "Sorting Code" ],
                        [ KContacts.AddressFormatField.PostalCode, "Zip Code" ],
                    ]);
                    if (modelData.isField) { return "Field: " + field_map.get(modelData.field); }
                    if (modelData.isLiteral) { return "Literal: \"" + modelData.literal + "\""; }
                    if (modelData.isSeparator) { return "Separator"; }
                }
                font.bold: AddressFormatRepository.formatForCountry(countryCombo.currentValue, KContacts.AddressFormatScriptPreference.LatinScript).requiredFields & modelData.field
            }
        }
    }
}
