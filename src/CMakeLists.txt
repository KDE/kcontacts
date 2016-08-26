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
  calendarurl.cpp
  contactgroup.cpp
  contactgrouptool.cpp
  email.cpp
  field.cpp
  geo.cpp
  gender.cpp
  impp.cpp
  key.cpp
  lang.cpp
  ldapdn.cpp
  ldif.cpp
  phonenumber.cpp
  picture.cpp
  related.cpp
  resourcelocatorurl.cpp
  secrecy.cpp
  sortmode.cpp
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
)

set(kcontacts_converter_SRCS
  converter/vcardconverter.cpp
  converter/ldifconverter.cpp
)

ecm_qt_declare_logging_category(kcontacts_converter_SRCS HEADER kcontacts_debug.h IDENTIFIER KCONTACTS_LOG CATEGORY_NAME log_kcontacts)

add_library(KF5Contacts ${kcontacts_SRCS} ${kcontacts_converter_SRCS})

generate_export_header(KF5Contacts BASE_NAME kcontacts)

add_library(KF5::Contacts ALIAS KF5Contacts)

target_include_directories(KF5Contacts INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts>")
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
  SortMode
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


install(FILES countrytransl.map DESTINATION ${KDE_INSTALL_DATADIR_KF5}/kcontacts)

ecm_generate_pri_file(BASE_NAME KContacts LIB_NAME KF5Contacts DEPS "CoreAddons" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
