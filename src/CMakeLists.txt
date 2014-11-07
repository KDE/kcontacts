set(vcardparser_SRCS
  vcardparser/vcard.cpp
  vcardparser/vcardline.cpp
  vcardparser/vcardparser.cpp
)

set(kcontacts_SRCS
  address.cpp
  addressee.cpp
  addresseehelper.cpp
  addresseelist.cpp
  contactgroup.cpp
  contactgrouptool.cpp
  errorhandler.cpp
  field.cpp
  geo.cpp
  key.cpp
  ldapdn.cpp
  ldif.cpp
  ldifconverter.cpp
  phonenumber.cpp
  picture.cpp
  plugin.cpp
  secrecy.cpp
  sortmode.cpp
  sound.cpp
  timezone.cpp
  vcardconverter.cpp
  vcarddrag.cpp
  vcardtool.cpp
  ${vcardparser_SRCS}
)

add_library(KF5Contacts ${kcontacts_SRCS})

generate_export_header(KF5Contacts BASE_NAME kcontacts)

add_library(KF5::Contacts ALIAS KF5Contacts)

target_include_directories(KF5Contacts INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KContacts>")
target_include_directories(KF5Contacts PUBLIC "$<BUILD_INTERFACE:${KContacts_SOURCE_DIR}/src;${KContacts_BINARY_DIR}/src>")
target_include_directories(KF5Contacts PUBLIC "$<BUILD_INTERFACE:${KContacts_SOURCE_DIR}/src/vcardparser>")

target_link_libraries(KF5Contacts
PUBLIC
    KF5::CoreAddons
    Qt5::Gui
PRIVATE
    Qt5::DBus
    KF5::ConfigCore
    KF5::I18n
    KF5::Codecs # for the vcard parser
)

set_target_properties(KF5Contacts PROPERTIES
    VERSION ${KContacts_VERSION_STRING}
    SOVERSION ${KContacts_SOVERSION}
    EXPORT_NAME Contacts
)

install(TARGETS KF5Contacts EXPORT KF5ContactsTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(KContacts_CamelCase_HEADERS
  HEADER_NAMES
  Address
  Addressee
  AddresseeList
  ContactGroup
  ContactGroupTool
  ErrorHandler
  Field
  Geo
  Key
  LDIFConverter
  PhoneNumber
  Picture
  Plugin
  Secrecy
  SortMode
  Sound
  TimeZone
  VCardConverter
  VCardDrag
  PREFIX KContacts
  REQUIRED_HEADERS KContacts_HEADERS
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kcontacts_export.h
    ${KContacts_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KContacts/kcontacts
    COMPONENT Devel
)

install(FILES
    ${KContacts_CamelCase_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KContacts/KContacts
    COMPONENT Devel
)


install(FILES countrytransl.map DESTINATION ${KF5_DATA_INSTALL_DIR}/kcontacts)

ecm_generate_pri_file(BASE_NAME KContacts LIB_NAME KF5Contacts DEPS "CoreAddons" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KF5_INCLUDE_INSTALL_DIR}/KContacts)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
