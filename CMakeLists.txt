cmake_minimum_required(VERSION 2.8.12)

project(KContacts)

# ECM setup
find_package(ECM 1.3.0 CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${KContacts_SOURCE_DIR}/cmake)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)

set(KF5_VERSION "5.4.0")
set(KDEPIMLIBS_VERSION "4.73.0")

ecm_setup_version(${KDEPIMLIBS_VERSION} VARIABLE_PREFIX KContacts
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcontacts_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5ContactsConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
find_package(Qt5 5.3 REQUIRED COMPONENTS DBus Gui)
find_package(KF5 ${KF5_VERSION} REQUIRED COMPONENTS CoreAddons I18n Config Codecs)

########### Targets ###########

add_definitions(-DTRANSLATION_DOMAIN=\"kcontacts5\")
add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5Contacts")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5ContactsConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5ContactsConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5ContactsConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5ContactsConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5ContactsTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5ContactsTargets.cmake NAMESPACE KF5::)

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/kcontacts_version.h
  DESTINATION ${KF5_INCLUDE_INSTALL_DIR} COMPONENT Devel
)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${KContacts_BINARY_DIR}")
  feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
endif()
