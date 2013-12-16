
project(kabc)

find_package(KF5 NO_MODULE REQUIRED COMPONENTS CoreAddons ItemViews I18n KDE4Support)

include_directories(${KDE4_KIO_INCLUDES})

add_definitions(${QDBUS_DEFINITIONS} -DKDE_DEFAULT_DEBUG_AREA=5700)
add_definitions(-DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DWANT_DEPRECATED_KABC_API)

# This one won't be needed when CMake 2.8.13 is depended on.
add_definitions(
  -DKRESOURCES_DEPRECATED_EXPORT=KRESOURCES_EXPORT
  -DKABC_DEPRECATED_EXPORT=KABC_EXPORT
)

# these apply also for all subdirs
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/vcardparser
  ${CMAKE_BINARY_DIR}/kresources
)

# kabc/vcardparser/Makefile.am: vcards

set(vcards_STAT_SRCS
  vcardparser/vcard.cpp
  vcardparser/vcardline.cpp
  vcardparser/vcardparser.cpp
)

add_subdirectory(vcardparser)
# add_subdirectory(tests)
#add_subdirectory( scripts )

########### next target ###############

set(kabc_LIB_SRCS
  address.cpp
  addressee.cpp
  contactgroup.cpp
  contactgrouptool.cpp
  errorhandler.cpp
  field.cpp
  geo.cpp
  key.cpp
  ldapdn.cpp
  ldif.cpp
  phonenumber.cpp
  picture.cpp
  plugin.cpp
  secrecy.cpp
  sound.cpp
  timezone.cpp
  ldifconverter.cpp
  addresseelist.cpp
  vcardconverter.cpp
  vcardtool.cpp
  addresseehelper.cpp
  lock.cpp
  locknull.cpp
  sortmode.cpp
  vcarddrag.cpp
  ${vcards_STAT_SRCS}
add_library(kabc ${LIBRARY_TYPE} ${kabc_LIB_SRCS})
generate_export_header(kabc)

target_link_libraries(kabc KF5::CoreAddons KF5::ItemViews KF5::I18n KF5::KDE4Support)

set_target_properties(
  kabc PROPERTIES
  VERSION ${GENERIC_LIB_VERSION}
  SOVERSION ${GENERIC_LIB_SOVERSION}
)
install(TARGETS kabc EXPORT kdepimlibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES countrytransl.map DESTINATION ${DATA_INSTALL_DIR}/kabc)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kabc_export.h
  address.h
  addressee.h
  addresseelist.h
  contactgroup.h
  contactgrouptool.h
  emailselectdialog.h
  errorhandler.h
  field.h
  geo.h
  key.h
  ldifconverter.h
  lock.h
  locknull.h
  phonenumber.h
  picture.h
  plugin.h
  secrecy.h
  sortmode.h
  sound.h
  timezone.h
  vcardconverter.h
  vcarddrag.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/kabc COMPONENT Devel
)
