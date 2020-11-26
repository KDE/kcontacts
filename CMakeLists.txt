cmake_minimum_required(VERSION 3.5)

set(KF5_VERSION "5.77.0") # handled by release scripts
set(KF5_DEP_VERSION "5.76.0") # handled by release scripts

project(KContacts VERSION ${KF5_VERSION})

# ECM setup
include(FeatureSummary)
find_package(ECM 5.76.0 NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${KContacts_SOURCE_DIR}/cmake)
set(CMAKE_AUTORCC ON)

set(REQUIRED_QT_VERSION 5.13.0)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(KCONTACTS_LIB_VERSION ${KF5_VERSION})

ecm_setup_version(PROJECT VARIABLE_PREFIX KContacts
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcontacts_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5ContactsConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Gui)
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS CoreAddons I18n Config Codecs)
find_package(IsoCodes MODULE)
set_package_properties("IsoCodes" PROPERTIES PURPOSE "Needed for country name translations." TYPE RUNTIME)

########### Targets ###########

add_definitions(-DTRANSLATION_DOMAIN=\"kcontacts5\")
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install(po)
endif()

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050d00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054B00)

add_definitions(-DQT_NO_FOREACH)
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Contacts")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5Contacts_QCH
        FILE KF5ContactsQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5ContactsQchTargets.cmake\")")
endif()

configure_package_config_file(
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
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
