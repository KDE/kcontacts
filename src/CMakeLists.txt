add_library(KF6Contacts)
add_library(KF6::Contacts ALIAS KF6Contacts)

qt_extract_metatypes(KF6Contacts)

set_target_properties(KF6Contacts PROPERTIES
    VERSION     ${KCONTACTS_VERSION}
    SOVERSION   ${KCONTACTS_SOVERSION}
    EXPORT_NAME Contacts
)

target_sources(KF6Contacts PRIVATE
    address.cpp
    addressee.cpp
    addressee.h
    addresseehelper.cpp
    addresseehelper_p.h
    addresseelist.h
    addressformat.cpp
    addressformatparser.cpp
    addressformatscript.cpp
    addressformatter.cpp
    address.h
    calendarurl.cpp
    calendarurl.h
    clientpidmap.cpp
    clientpidmap.h
    contactgroup.cpp
    contactgroup.h
    contactgrouptool.cpp
    contactgrouptool.h
    converter/ldifconverter.cpp
    converter/ldifconverter.h
    converter/vcardconverter.cpp
    converter/vcardconverter.h
    email.cpp
    email.h
    fieldgroup.cpp
    fieldgroup.h
    gender.cpp
    gender.h
    geo.cpp
    geo.h
    impp.cpp
    impp.h
    key.cpp
    key.h
    lang.cpp
    lang.h
    ldif.cpp
    ldif_p.h
    namespace.cpp
    nickname.cpp
    nickname.h
    note.cpp
    note.h
    org.cpp
    org.h
    parametermap_p.h
    phonenumber.cpp
    phonenumber.h
    picture.cpp
    picture.h
    related.cpp
    related.h
    resourcelocatorurl.cpp
    resourcelocatorurl.h
    role.cpp
    role.h
    secrecy.cpp
    secrecy.h
    sound.cpp
    sound.h
    timezone.cpp
    timezone.h
    title.cpp
    title.h
    vcarddrag.cpp
    vcarddrag.h
    vcardparser/vcard.cpp
    vcardparser/vcardline.cpp
    vcardparser/vcardline_p.h
    vcardparser/vcardparser.cpp
    vcardparser/vcardparser_p.h
    vcardparser/vcard_p.h
    vcardtool.cpp
    vcardtool_p.h

    kcontacts.qrc
)

qt6_add_resources(KF6Contacts "improtocols"
    PREFIX "/org.kde.kcontacts/improtocols/"
    BASE "improtocols"
    OUTPUT_TARGETS _out_targets
    FILES
        improtocols/aim.desktop
        improtocols/facebook.desktop
        improtocols/gg.desktop
        improtocols/googletalk.desktop
        improtocols/groupwise.desktop
        improtocols/icq.desktop
        improtocols/irc.desktop
        improtocols/meanwhile.desktop
        improtocols/msn.desktop
        improtocols/qq.desktop
        improtocols/sip.desktop
        improtocols/skype.desktop
        improtocols/sms.desktop
        improtocols/twitter.desktop
        improtocols/xmpp.desktop
        improtocols/ymsgr.desktop
        improtocols/matrix.desktop
        improtocols/telegram.desktop
        improtocols/rocketchat.desktop
        improtocols/signal.desktop
        improtocols/threema.desktop
)

ecm_qt_declare_logging_category(KF6Contacts
    HEADER kcontacts_debug.h
    IDENTIFIER KCONTACTS_LOG
    CATEGORY_NAME kf.contacts
    OLD_CATEGORY_NAMES log_kcontacts org.kde.pim.kcontacts
    DESCRIPTION "KContacts"
    EXPORT KCONTACTS
)

ecm_generate_export_header(KF6Contacts
    EXPORT_FILE_NAME kcontacts_export.h
    BASE_NAME kcontacts
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    USE_VERSION_HEADER
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

target_include_directories(KF6Contacts INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KContacts>")
set(KContacts_BUILD_INCLUDE_DIRS
    ${KContacts_SOURCE_DIR}/src
    ${KContacts_BINARY_DIR}/src
    ${KContacts_SOURCE_DIR}/src/vcardparser
)
target_include_directories(KF6Contacts PUBLIC "$<BUILD_INTERFACE:${KContacts_BUILD_INCLUDE_DIRS}>")

target_link_libraries(KF6Contacts
PUBLIC
    Qt6::Core
    Qt6::Gui
PRIVATE
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::I18n
    KF6::I18nLocaleData
    KF6::Codecs # for the vcard parser
)

install(TARGETS KF6Contacts ${_out_targets} EXPORT KF6ContactsTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(KContacts_CamelCase_HEADERS
  HEADER_NAMES
  Address
  AddressFormat
  Addressee
  AddresseeList
  CalendarUrl
  ContactGroup
  ContactGroupTool
  Email
  Geo
  Gender
  Key
  Lang
  Namespace
  Impp
  PhoneNumber
  Picture
  Related
  ResourceLocatorUrl
  Secrecy
  Sound
  TimeZone
  Title
  Role
  Note
  Org
  NickName
  VCardDrag
  FieldGroup
  ClientPidMap
  PREFIX KContacts
  REQUIRED_HEADERS KContacts_HEADERS
)

ecm_generate_qdoc(KF6Contacts kcontacts.qdocconf)

if (USE_QML)
  add_subdirectory(qml)
endif ()
add_subdirectory(converter)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kcontacts_export.h
    ${KContacts_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KContacts/kcontacts
    COMPONENT Devel
)

install(FILES
    ${KContacts_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KContacts/KContacts
    COMPONENT Devel
)

ecm_qt_install_logging_categories(
    EXPORT KCONTACTS
    FILE kcontacts.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)
