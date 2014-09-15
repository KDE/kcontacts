cmake_minimum_required(VERSION 2.8.12)

project(KAbc)

# ECM setup
find_package(ECM 1.3.0 CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${KAbc_SOURCE_DIR}/cmake)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)

set(KF5_VERSION "5.2.0")
set(KDEPIMLIBS_VERSION "4.71.0")

# FIXME ? KF5AbcConfig ? KF5AddressBookConfig ? anything else ?
ecm_setup_version(${KDEPIMLIBS_VERSION} VARIABLE_PREFIX KABC
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kabc_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5AbcConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
find_package(KF5CoreAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5KDELibs4Support ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_VERSION} CONFIG REQUIRED)

########### Targets ###########

add_definitions(-DTRANSLATION_DOMAIN=\"kabc5\")
add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5Abc")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5AbcConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5AbcConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5AbcConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5AbcConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5AbcTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5AbcTargets.cmake NAMESPACE KF5::)

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/kabc_version.h
  DESTINATION ${KF5_INCLUDE_INSTALL_DIR} COMPONENT Devel
)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${KAbc_BINARY_DIR}")
  feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
endif()
