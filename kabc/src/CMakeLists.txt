# FIXME: Make it build with this flag
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)

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

# FIXME ? KF5::Abc? KF5::AddressBook? KF5::ABC?
add_library(KF5::Abc ALIAS KF5Abc)

target_include_directories(KF5Abc INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/kabc>")
target_include_directories(KF5Abc PUBLIC "$<BUILD_INTERFACE:${KAbc_SOURCE_DIR}/src;${KAbc_BINARY_DIR}/src>")
target_include_directories(KF5Abc PUBLIC "$<BUILD_INTERFACE:${KAbc_SOURCE_DIR}/src/vcardparser>")

target_link_libraries(KF5Abc
    KF5::CoreAddons
    KF5::ItemViews
    KF5::I18n
    KF5::KDE4Support
)

set_target_properties(KF5Abc PROPERTIES
    VERSION ${KABC_VERSION_STRING}
    SOVERSION ${KABC_SOVERSION}
    EXPORT_NAME KAbc
)

install(TARGETS KF5Abc EXPORT KF5AbcTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(KAbc_CamelCase_HEADERS
  HEADER_NAMES
  Address
  #AddressBook FIXME
  #AddressLineEdit
  Addressee
  # AddresseeDialog
  AddresseeList
  ContactGroup
  ContactGroupTool
  #DistributionList
  #DistributionListDialog
  EmailSelectDialog
  ErrorHandler
  Field
  #Format
  #FormatFactory
  Geo
  Key
  LDIFConverter
  Lock
  LockNull
  PhoneNumber
  Picture
  Plugin
  #Resource
  #ResourceABC
  #ResourceCached
  Secrecy
  SortMode
  Sound
  #StdAddressBook
  TimeZone
  VCardConverter
  VCardDrag
  #VCardFormat

  REQUIRED_HEADERS KAbc_HEADERS
  PREFIX KAbc
)

install(FILES
    ${KAbc_CamelCase_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/KABC
    COMPONENT Devel
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kabc_export.h
    ${KAbc_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/kabc
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KAbc LIB_NAME KF5KAbc DEPS "CoreAddons ItemViews I18n KDE4Support" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
if(FALSE)

# FIXME: vcardparser contains manual tests it seems
#add_subdirectory(vcardparser)
# add_subdirectory(tests)
#add_subdirectory( scripts )


add_definitions(-DWANT_DEPRECATED_KABC_API)

# This one won't be needed when CMake 2.8.13 is depended on.
add_definitions(
  -DKRESOURCES_DEPRECATED_EXPORT=KRESOURCES_EXPORT
  -DKABC_DEPRECATED_EXPORT=KABC_EXPORT
)

########### install files ###############

install(FILES countrytransl.map DESTINATION ${DATA_INSTALL_DIR}/kabc)
endif()
