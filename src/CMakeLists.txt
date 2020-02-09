set(vcardparser_SRCS
  vcardparser/vcard.cpp
  vcardparser/vcardline.cpp
  vcardparser/vcardparser.cpp
)

set(kcontacts_SRCS
  address.cpp
  addressee.cpp
  addresseehelper.cpp
  calendarurl.cpp
  contactgroup.cpp
  contactgrouptool.cpp
  countrytoisomap_p.cpp
  email.cpp
  field.cpp
  geo.cpp
  gender.cpp
  impp.cpp
  key.cpp
  lang.cpp
  ldif.cpp
  phonenumber.cpp
  picture.cpp
  related.cpp
  resourcelocatorurl.cpp
  secrecy.cpp
  sound.cpp
  timezone.cpp
  vcarddrag.cpp
  vcardtool.cpp
  fieldgroup.cpp
  title.cpp
  nickname.cpp
  role.cpp
  note.cpp
  org.cpp
  clientpidmap.cpp
  ${vcardparser_SRCS}
  kcontacts.qrc
  improtocols/improtocols.qrc
)

set(kcontacts_converter_SRCS
  converter/vcardconverter.cpp
  converter/ldifconverter.cpp
)

ecm_qt_declare_logging_category(kcontacts_converter_SRCS
    HEADER kcontacts_debug.h
    IDENTIFIER KCONTACTS_LOG
    CATEGORY_NAME org.kde.pim.kcontacts
    OLD_CATEGORY_NAMES log_kcontacts
    DESCRIPTION "kcontacts (pim lib)"
    EXPORT KCONTACTS
)

add_library(KF5Contacts ${kcontacts_SRCS} ${kcontacts_converter_SRCS})

generate_export_header(KF5Contacts BASE_NAME kcontacts)

add_library(KF5::Contacts ALIAS KF5Contacts)

target_include_directories(KF5Contacts INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts>")
set(KContacts_BUILD_INCLUDE_DIRS
    ${KContacts_SOURCE_DIR}/src
    ${KContacts_BINARY_DIR}/src
    ${KContacts_SOURCE_DIR}/src/vcardparser
)
target_include_directories(KF5Contacts PUBLIC "$<BUILD_INTERFACE:${KContacts_BUILD_INCLUDE_DIRS}>")

target_link_libraries(KF5Contacts
PUBLIC
    Qt5::Core
PRIVATE
    KF5::CoreAddons
    Qt5::Gui
    KF5::ConfigCore
    KF5::I18n
    KF5::Codecs # for the vcard parser
)

set_target_properties(KF5Contacts PROPERTIES
    VERSION ${KContacts_VERSION}
    SOVERSION ${KContacts_SOVERSION}
    EXPORT_NAME Contacts
)

install(TARGETS KF5Contacts EXPORT KF5ContactsTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(KContacts_CamelCase_HEADERS
  HEADER_NAMES
  Address
  Addressee
  AddresseeList
  CalendarUrl
  ContactGroup
  ContactGroupTool
  Email
  Field
  Geo
  Gender
  Key
  Lang
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

add_subdirectory(converter)
add_subdirectory(generator)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kcontacts_export.h
    ${KContacts_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts/kcontacts
    COMPONENT Devel
)

install(FILES
    ${KContacts_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts/KContacts
    COMPONENT Devel
)

ecm_qt_install_logging_categories(
    EXPORT KCONTACTS
    FILE kcontacts.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)

ecm_generate_pri_file(BASE_NAME KContacts LIB_NAME KF5Contacts DEPS "CoreAddons" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if(BUILD_QCH)
    ecm_add_qch(
        KF5Contacts_QCH
        NAME KContacts
        BASE_NAME KF5Contacts
        VERSION ${KContacts_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KContacts_HEADERS}
            "${CMAKE_SOURCE_DIR}/docs/AddressFormat.md"
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
        INCLUDE_DIRS
            ${KContacts_BUILD_INCLUDE_DIRS}
        BLANK_MACROS
            KCONTACTS_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
