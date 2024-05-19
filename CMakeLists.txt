cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.117.0") # handled by release scripts
set(KF_DEP_VERSION "5.116.0") # handled by release scripts

project(KContacts VERSION ${KF_VERSION})

# ECM setup
include(FeatureSummary)
find_package(ECM 5.116.0 NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_AUTORCC ON)

set(REQUIRED_QT_VERSION 5.15.2)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEGitCommitHooks)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMGenerateExportHeader)
include(ECMDeprecationSettings)

include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(KCONTACTS_LIB_VERSION ${KF_VERSION})

ecm_setup_version(PROJECT VARIABLE_PREFIX KContacts
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kcontacts_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5ContactsConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} NO_MODULE COMPONENTS Gui Quick)
set_package_properties(Qt${QT_MAJOR_VERSION}Gui PROPERTIES
    TYPE REQUIRED
    PURPOSE "Basic library component"
)
set_package_properties(Qt${QT_MAJOR_VERSION}Quick PROPERTIES
    TYPE OPTIONAL
    PURPOSE "QtQuick plugin"
)
find_package(KF5 ${KF_DEP_VERSION} REQUIRED COMPONENTS CoreAddons I18n Config Codecs)
if (QT_MAJOR_VERSION STREQUAL "6")
    find_package(Qt6Core5Compat)
endif()

########### Targets ###########

add_definitions(-DTRANSLATION_DOMAIN=\"kcontacts5\")
ki18n_install(po)

ecm_set_disabled_deprecation_versions(
    QT 5.15.2
    KF 5.95.0
)

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
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KContacts COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
