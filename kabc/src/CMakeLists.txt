set(vcardparser_SRCS
  vcardparser/vcard.cpp
  vcardparser/vcardline.cpp
  vcardparser/vcardparser.cpp
)

set(kabc_SRCS
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
  lock.cpp
  locknull.cpp
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

add_library(KF5Abc ${kabc_SRCS})

generate_export_header(KF5Abc BASE_NAME kabc)

add_library(KF5::Abc ALIAS KF5Abc)

target_include_directories(KF5Abc INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KABC>")
target_include_directories(KF5Abc PUBLIC "$<BUILD_INTERFACE:${KAbc_SOURCE_DIR}/src;${KAbc_BINARY_DIR}/src>")
target_include_directories(KF5Abc PUBLIC "$<BUILD_INTERFACE:${KAbc_SOURCE_DIR}/src/vcardparser>")

target_link_libraries(KF5Abc
PUBLIC
    KF5::CoreAddons
    KF5::KDELibs4Support
PRIVATE
    KF5::I18n
)

set_target_properties(KF5Abc PROPERTIES
    VERSION ${KABC_VERSION_STRING}
    SOVERSION ${KABC_SOVERSION}
    EXPORT_NAME Abc
)

install(TARGETS KF5Abc EXPORT KF5AbcTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(KAbc_CamelCase_HEADERS
  HEADER_NAMES
  Address
  Addressee
  AddresseeList
  ContactGroup
  ContactGroupTool
  ErrorHandler
  Field
  Format
  Geo
  Key
  LDIFConverter
  Lock
  LockNull
  PhoneNumber
  Picture
  Plugin
  ResourceABC
  Secrecy
  SortMode
  Sound
  TimeZone
  VCardConverter
  VCardDrag
  PREFIX KABC
  REQUIRED_HEADERS KAbc_HEADERS
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kabc_export.h
    ${KAbc_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KABC/kabc
    COMPONENT Devel
)

install(FILES
    ${KAbc_CamelCase_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KABC/KABC
    COMPONENT Devel
)


install(FILES countrytransl.map DESTINATION ${KF5_DATA_INSTALL_DIR}/kabc)

ecm_generate_pri_file(BASE_NAME KAbc LIB_NAME KF5Abc DEPS "CoreAddons KDELibs4Support" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KF5_INCLUDE_INSTALL_DIR}/KABC)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
