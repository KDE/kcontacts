cmake_minimum_required(VERSION 2.8.12)

project(KAbc)

# ECM setup
find_package(ECM 0.0.11 CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${KAbc_SOURCE_DIR}/cmake)

include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)

set(KF5_VERSION "4.97.0")

# FIXME ? KF5AbcConfig ? KF5AddressBookConfig ? anything else ?
ecm_setup_version(${KF5_VERSION} VARIABLE_PREFIX KABC
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kabc_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5AbcConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
find_package(KF5 ${KF5_VERSION} CONFIG REQUIRED COMPONENTS CoreAddons ItemViews I18n KDE4Support)

########### Targets ###########

add_subdirectory(src)
#add_subdirectory(autotests)
#add_subdirectory(cmake)

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
  DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel
)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${KAbc_BINARY_DIR}")
  feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
endif()
