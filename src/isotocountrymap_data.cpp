
/*
    Generated file based on iso-codes data. Do not edit!
    See src/generator/ for how to re-generate this.

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "isotocountrymap_p.h"

using namespace KContacts;

static const char en_country_name_stringtable[] = {
    "Andorra\0"
    "United Arab Emirates\0"
    "Afghanistan\0"
    "Antigua and Barbuda\0"
    "Anguilla\0"
    "Albania\0"
    "Armenia\0"
    "Angola\0"
    "Antarctica\0"
    "Argentina\0"
    "American Samoa\0"
    "Austria\0"
    "Australia\0"
    "Aruba\0"
    "Åland Islands\0"
    "Azerbaijan\0"
    "Bosnia and Herzegovina\0"
    "Barbados\0"
    "Bangladesh\0"
    "Belgium\0"
    "Burkina Faso\0"
    "Bulgaria\0"
    "Bahrain\0"
    "Burundi\0"
    "Benin\0"
    "Saint Barthélemy\0"
    "Bermuda\0"
    "Brunei Darussalam\0"
    "Bolivia, Plurinational State of\0"
    "Bonaire, Sint Eustatius and Saba\0"
    "Brazil\0"
    "Bahamas\0"
    "Bhutan\0"
    "Bouvet Island\0"
    "Botswana\0"
    "Belarus\0"
    "Belize\0"
    "Canada\0"
    "Cocos (Keeling) Islands\0"
    "Congo, The Democratic Republic of the\0"
    "Central African Republic\0"
    "Congo\0"
    "Switzerland\0"
    "Côte d'Ivoire\0"
    "Cook Islands\0"
    "Chile\0"
    "Cameroon\0"
    "China\0"
    "Colombia\0"
    "Costa Rica\0"
    "Cuba\0"
    "Cabo Verde\0"
    "Curaçao\0"
    "Christmas Island\0"
    "Cyprus\0"
    "Czechia\0"
    "Germany\0"
    "Djibouti\0"
    "Denmark\0"
    "Dominica\0"
    "Dominican Republic\0"
    "Algeria\0"
    "Ecuador\0"
    "Estonia\0"
    "Egypt\0"
    "Western Sahara\0"
    "Eritrea\0"
    "Spain\0"
    "Ethiopia\0"
    "Finland\0"
    "Fiji\0"
    "Falkland Islands (Malvinas)\0"
    "Micronesia, Federated States of\0"
    "Faroe Islands\0"
    "France\0"
    "Gabon\0"
    "United Kingdom\0"
    "Grenada\0"
    "Georgia\0"
    "French Guiana\0"
    "Guernsey\0"
    "Ghana\0"
    "Gibraltar\0"
    "Greenland\0"
    "Gambia\0"
    "Guinea\0"
    "Guadeloupe\0"
    "Equatorial Guinea\0"
    "Greece\0"
    "South Georgia and the South Sandwich Islands\0"
    "Guatemala\0"
    "Guam\0"
    "Guinea-Bissau\0"
    "Guyana\0"
    "Hong Kong\0"
    "Heard Island and McDonald Islands\0"
    "Honduras\0"
    "Croatia\0"
    "Haiti\0"
    "Hungary\0"
    "Indonesia\0"
    "Ireland\0"
    "Israel\0"
    "Isle of Man\0"
    "India\0"
    "British Indian Ocean Territory\0"
    "Iraq\0"
    "Iran, Islamic Republic of\0"
    "Iceland\0"
    "Italy\0"
    "Jersey\0"
    "Jamaica\0"
    "Jordan\0"
    "Japan\0"
    "Kenya\0"
    "Kyrgyzstan\0"
    "Cambodia\0"
    "Kiribati\0"
    "Comoros\0"
    "Saint Kitts and Nevis\0"
    "Korea, Democratic People's Republic of\0"
    "Korea, Republic of\0"
    "Kuwait\0"
    "Cayman Islands\0"
    "Kazakhstan\0"
    "Lao People's Democratic Republic\0"
    "Lebanon\0"
    "Saint Lucia\0"
    "Liechtenstein\0"
    "Sri Lanka\0"
    "Liberia\0"
    "Lesotho\0"
    "Lithuania\0"
    "Luxembourg\0"
    "Latvia\0"
    "Libya\0"
    "Morocco\0"
    "Monaco\0"
    "Moldova, Republic of\0"
    "Montenegro\0"
    "Saint Martin (French part)\0"
    "Madagascar\0"
    "Marshall Islands\0"
    "Macedonia, Republic of\0"
    "Mali\0"
    "Myanmar\0"
    "Mongolia\0"
    "Macao\0"
    "Northern Mariana Islands\0"
    "Martinique\0"
    "Mauritania\0"
    "Montserrat\0"
    "Malta\0"
    "Mauritius\0"
    "Maldives\0"
    "Malawi\0"
    "Mexico\0"
    "Malaysia\0"
    "Mozambique\0"
    "Namibia\0"
    "New Caledonia\0"
    "Niger\0"
    "Norfolk Island\0"
    "Nigeria\0"
    "Nicaragua\0"
    "Netherlands\0"
    "Norway\0"
    "Nepal\0"
    "Nauru\0"
    "Niue\0"
    "New Zealand\0"
    "Oman\0"
    "Panama\0"
    "Peru\0"
    "French Polynesia\0"
    "Papua New Guinea\0"
    "Philippines\0"
    "Pakistan\0"
    "Poland\0"
    "Saint Pierre and Miquelon\0"
    "Pitcairn\0"
    "Puerto Rico\0"
    "Palestine, State of\0"
    "Portugal\0"
    "Palau\0"
    "Paraguay\0"
    "Qatar\0"
    "Réunion\0"
    "Romania\0"
    "Serbia\0"
    "Russian Federation\0"
    "Rwanda\0"
    "Saudi Arabia\0"
    "Solomon Islands\0"
    "Seychelles\0"
    "Sudan\0"
    "Sweden\0"
    "Singapore\0"
    "Saint Helena, Ascension and Tristan da Cunha\0"
    "Slovenia\0"
    "Svalbard and Jan Mayen\0"
    "Slovakia\0"
    "Sierra Leone\0"
    "San Marino\0"
    "Senegal\0"
    "Somalia\0"
    "Suriname\0"
    "South Sudan\0"
    "Sao Tome and Principe\0"
    "El Salvador\0"
    "Sint Maarten (Dutch part)\0"
    "Syrian Arab Republic\0"
    "Swaziland\0"
    "Turks and Caicos Islands\0"
    "Chad\0"
    "French Southern Territories\0"
    "Togo\0"
    "Thailand\0"
    "Tajikistan\0"
    "Tokelau\0"
    "Timor-Leste\0"
    "Turkmenistan\0"
    "Tunisia\0"
    "Tonga\0"
    "Turkey\0"
    "Trinidad and Tobago\0"
    "Tuvalu\0"
    "Taiwan, Province of China\0"
    "Tanzania, United Republic of\0"
    "Ukraine\0"
    "Uganda\0"
    "United States Minor Outlying Islands\0"
    "United States\0"
    "Uruguay\0"
    "Uzbekistan\0"
    "Holy See (Vatican City State)\0"
    "Saint Vincent and the Grenadines\0"
    "Venezuela, Bolivarian Republic of\0"
    "Virgin Islands, British\0"
    "Virgin Islands, U.S.\0"
    "Viet Nam\0"
    "Vanuatu\0"
    "Wallis and Futuna\0"
    "Samoa\0"
    "Yemen\0"
    "Mayotte\0"
    "South Africa\0"
    "Zambia\0"
    "Zimbabwe\0"

};

static const IsoToCountryIndex iso_to_country_index[] = {
    IsoToCountryIndex{"ad", 0},
    IsoToCountryIndex{"ae", 8},
    IsoToCountryIndex{"af", 29},
    IsoToCountryIndex{"ag", 41},
    IsoToCountryIndex{"ai", 61},
    IsoToCountryIndex{"al", 70},
    IsoToCountryIndex{"am", 78},
    IsoToCountryIndex{"ao", 86},
    IsoToCountryIndex{"aq", 93},
    IsoToCountryIndex{"ar", 104},
    IsoToCountryIndex{"as", 114},
    IsoToCountryIndex{"at", 129},
    IsoToCountryIndex{"au", 137},
    IsoToCountryIndex{"aw", 147},
    IsoToCountryIndex{"ax", 153},
    IsoToCountryIndex{"az", 168},
    IsoToCountryIndex{"ba", 179},
    IsoToCountryIndex{"bb", 202},
    IsoToCountryIndex{"bd", 211},
    IsoToCountryIndex{"be", 222},
    IsoToCountryIndex{"bf", 230},
    IsoToCountryIndex{"bg", 243},
    IsoToCountryIndex{"bh", 252},
    IsoToCountryIndex{"bi", 260},
    IsoToCountryIndex{"bj", 268},
    IsoToCountryIndex{"bl", 274},
    IsoToCountryIndex{"bm", 292},
    IsoToCountryIndex{"bn", 300},
    IsoToCountryIndex{"bo", 318},
    IsoToCountryIndex{"bq", 350},
    IsoToCountryIndex{"br", 383},
    IsoToCountryIndex{"bs", 390},
    IsoToCountryIndex{"bt", 398},
    IsoToCountryIndex{"bv", 405},
    IsoToCountryIndex{"bw", 419},
    IsoToCountryIndex{"by", 428},
    IsoToCountryIndex{"bz", 436},
    IsoToCountryIndex{"ca", 443},
    IsoToCountryIndex{"cc", 450},
    IsoToCountryIndex{"cd", 474},
    IsoToCountryIndex{"cf", 512},
    IsoToCountryIndex{"cg", 537},
    IsoToCountryIndex{"ch", 543},
    IsoToCountryIndex{"ci", 555},
    IsoToCountryIndex{"ck", 570},
    IsoToCountryIndex{"cl", 583},
    IsoToCountryIndex{"cm", 589},
    IsoToCountryIndex{"cn", 598},
    IsoToCountryIndex{"co", 604},
    IsoToCountryIndex{"cr", 613},
    IsoToCountryIndex{"cu", 624},
    IsoToCountryIndex{"cv", 629},
    IsoToCountryIndex{"cw", 640},
    IsoToCountryIndex{"cx", 649},
    IsoToCountryIndex{"cy", 666},
    IsoToCountryIndex{"cz", 673},
    IsoToCountryIndex{"de", 681},
    IsoToCountryIndex{"dj", 689},
    IsoToCountryIndex{"dk", 698},
    IsoToCountryIndex{"dm", 706},
    IsoToCountryIndex{"do", 715},
    IsoToCountryIndex{"dz", 734},
    IsoToCountryIndex{"ec", 742},
    IsoToCountryIndex{"ee", 750},
    IsoToCountryIndex{"eg", 758},
    IsoToCountryIndex{"eh", 764},
    IsoToCountryIndex{"er", 779},
    IsoToCountryIndex{"es", 787},
    IsoToCountryIndex{"et", 793},
    IsoToCountryIndex{"fi", 802},
    IsoToCountryIndex{"fj", 810},
    IsoToCountryIndex{"fk", 815},
    IsoToCountryIndex{"fm", 843},
    IsoToCountryIndex{"fo", 875},
    IsoToCountryIndex{"fr", 889},
    IsoToCountryIndex{"ga", 896},
    IsoToCountryIndex{"gb", 902},
    IsoToCountryIndex{"gd", 917},
    IsoToCountryIndex{"ge", 925},
    IsoToCountryIndex{"gf", 933},
    IsoToCountryIndex{"gg", 947},
    IsoToCountryIndex{"gh", 956},
    IsoToCountryIndex{"gi", 962},
    IsoToCountryIndex{"gl", 972},
    IsoToCountryIndex{"gm", 982},
    IsoToCountryIndex{"gn", 989},
    IsoToCountryIndex{"gp", 996},
    IsoToCountryIndex{"gq", 1007},
    IsoToCountryIndex{"gr", 1025},
    IsoToCountryIndex{"gs", 1032},
    IsoToCountryIndex{"gt", 1077},
    IsoToCountryIndex{"gu", 1087},
    IsoToCountryIndex{"gw", 1092},
    IsoToCountryIndex{"gy", 1106},
    IsoToCountryIndex{"hk", 1113},
    IsoToCountryIndex{"hm", 1123},
    IsoToCountryIndex{"hn", 1157},
    IsoToCountryIndex{"hr", 1166},
    IsoToCountryIndex{"ht", 1174},
    IsoToCountryIndex{"hu", 1180},
    IsoToCountryIndex{"id", 1188},
    IsoToCountryIndex{"ie", 1198},
    IsoToCountryIndex{"il", 1206},
    IsoToCountryIndex{"im", 1213},
    IsoToCountryIndex{"in", 1225},
    IsoToCountryIndex{"io", 1231},
    IsoToCountryIndex{"iq", 1262},
    IsoToCountryIndex{"ir", 1267},
    IsoToCountryIndex{"is", 1293},
    IsoToCountryIndex{"it", 1301},
    IsoToCountryIndex{"je", 1307},
    IsoToCountryIndex{"jm", 1314},
    IsoToCountryIndex{"jo", 1322},
    IsoToCountryIndex{"jp", 1329},
    IsoToCountryIndex{"ke", 1335},
    IsoToCountryIndex{"kg", 1341},
    IsoToCountryIndex{"kh", 1352},
    IsoToCountryIndex{"ki", 1361},
    IsoToCountryIndex{"km", 1370},
    IsoToCountryIndex{"kn", 1378},
    IsoToCountryIndex{"kp", 1400},
    IsoToCountryIndex{"kr", 1439},
    IsoToCountryIndex{"kw", 1458},
    IsoToCountryIndex{"ky", 1465},
    IsoToCountryIndex{"kz", 1480},
    IsoToCountryIndex{"la", 1491},
    IsoToCountryIndex{"lb", 1524},
    IsoToCountryIndex{"lc", 1532},
    IsoToCountryIndex{"li", 1544},
    IsoToCountryIndex{"lk", 1558},
    IsoToCountryIndex{"lr", 1568},
    IsoToCountryIndex{"ls", 1576},
    IsoToCountryIndex{"lt", 1584},
    IsoToCountryIndex{"lu", 1594},
    IsoToCountryIndex{"lv", 1605},
    IsoToCountryIndex{"ly", 1612},
    IsoToCountryIndex{"ma", 1618},
    IsoToCountryIndex{"mc", 1626},
    IsoToCountryIndex{"md", 1633},
    IsoToCountryIndex{"me", 1654},
    IsoToCountryIndex{"mf", 1665},
    IsoToCountryIndex{"mg", 1692},
    IsoToCountryIndex{"mh", 1703},
    IsoToCountryIndex{"mk", 1720},
    IsoToCountryIndex{"ml", 1743},
    IsoToCountryIndex{"mm", 1748},
    IsoToCountryIndex{"mn", 1756},
    IsoToCountryIndex{"mo", 1765},
    IsoToCountryIndex{"mp", 1771},
    IsoToCountryIndex{"mq", 1796},
    IsoToCountryIndex{"mr", 1807},
    IsoToCountryIndex{"ms", 1818},
    IsoToCountryIndex{"mt", 1829},
    IsoToCountryIndex{"mu", 1835},
    IsoToCountryIndex{"mv", 1845},
    IsoToCountryIndex{"mw", 1854},
    IsoToCountryIndex{"mx", 1861},
    IsoToCountryIndex{"my", 1868},
    IsoToCountryIndex{"mz", 1877},
    IsoToCountryIndex{"na", 1888},
    IsoToCountryIndex{"nc", 1896},
    IsoToCountryIndex{"ne", 1910},
    IsoToCountryIndex{"nf", 1916},
    IsoToCountryIndex{"ng", 1931},
    IsoToCountryIndex{"ni", 1939},
    IsoToCountryIndex{"nl", 1949},
    IsoToCountryIndex{"no", 1961},
    IsoToCountryIndex{"np", 1968},
    IsoToCountryIndex{"nr", 1974},
    IsoToCountryIndex{"nu", 1980},
    IsoToCountryIndex{"nz", 1985},
    IsoToCountryIndex{"om", 1997},
    IsoToCountryIndex{"pa", 2002},
    IsoToCountryIndex{"pe", 2009},
    IsoToCountryIndex{"pf", 2014},
    IsoToCountryIndex{"pg", 2031},
    IsoToCountryIndex{"ph", 2048},
    IsoToCountryIndex{"pk", 2060},
    IsoToCountryIndex{"pl", 2069},
    IsoToCountryIndex{"pm", 2076},
    IsoToCountryIndex{"pn", 2102},
    IsoToCountryIndex{"pr", 2111},
    IsoToCountryIndex{"ps", 2123},
    IsoToCountryIndex{"pt", 2143},
    IsoToCountryIndex{"pw", 2152},
    IsoToCountryIndex{"py", 2158},
    IsoToCountryIndex{"qa", 2167},
    IsoToCountryIndex{"re", 2173},
    IsoToCountryIndex{"ro", 2182},
    IsoToCountryIndex{"rs", 2190},
    IsoToCountryIndex{"ru", 2197},
    IsoToCountryIndex{"rw", 2216},
    IsoToCountryIndex{"sa", 2223},
    IsoToCountryIndex{"sb", 2236},
    IsoToCountryIndex{"sc", 2252},
    IsoToCountryIndex{"sd", 2263},
    IsoToCountryIndex{"se", 2269},
    IsoToCountryIndex{"sg", 2276},
    IsoToCountryIndex{"sh", 2286},
    IsoToCountryIndex{"si", 2331},
    IsoToCountryIndex{"sj", 2340},
    IsoToCountryIndex{"sk", 2363},
    IsoToCountryIndex{"sl", 2372},
    IsoToCountryIndex{"sm", 2385},
    IsoToCountryIndex{"sn", 2396},
    IsoToCountryIndex{"so", 2404},
    IsoToCountryIndex{"sr", 2412},
    IsoToCountryIndex{"ss", 2421},
    IsoToCountryIndex{"st", 2433},
    IsoToCountryIndex{"sv", 2455},
    IsoToCountryIndex{"sx", 2467},
    IsoToCountryIndex{"sy", 2493},
    IsoToCountryIndex{"sz", 2514},
    IsoToCountryIndex{"tc", 2524},
    IsoToCountryIndex{"td", 2549},
    IsoToCountryIndex{"tf", 2554},
    IsoToCountryIndex{"tg", 2582},
    IsoToCountryIndex{"th", 2587},
    IsoToCountryIndex{"tj", 2596},
    IsoToCountryIndex{"tk", 2607},
    IsoToCountryIndex{"tl", 2615},
    IsoToCountryIndex{"tm", 2627},
    IsoToCountryIndex{"tn", 2640},
    IsoToCountryIndex{"to", 2648},
    IsoToCountryIndex{"tr", 2654},
    IsoToCountryIndex{"tt", 2661},
    IsoToCountryIndex{"tv", 2681},
    IsoToCountryIndex{"tw", 2688},
    IsoToCountryIndex{"tz", 2714},
    IsoToCountryIndex{"ua", 2743},
    IsoToCountryIndex{"ug", 2751},
    IsoToCountryIndex{"um", 2758},
    IsoToCountryIndex{"us", 2795},
    IsoToCountryIndex{"uy", 2809},
    IsoToCountryIndex{"uz", 2817},
    IsoToCountryIndex{"va", 2828},
    IsoToCountryIndex{"vc", 2858},
    IsoToCountryIndex{"ve", 2891},
    IsoToCountryIndex{"vg", 2925},
    IsoToCountryIndex{"vi", 2949},
    IsoToCountryIndex{"vn", 2970},
    IsoToCountryIndex{"vu", 2979},
    IsoToCountryIndex{"wf", 2987},
    IsoToCountryIndex{"ws", 3005},
    IsoToCountryIndex{"ye", 3011},
    IsoToCountryIndex{"yt", 3017},
    IsoToCountryIndex{"za", 3025},
    IsoToCountryIndex{"zm", 3038},
    IsoToCountryIndex{"zw", 3045},
};
