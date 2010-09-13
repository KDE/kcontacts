
include_directories( ${CMAKE_SOURCE_DIR}/kabc )


########### kabc_file_core library (also used by kaddressbook) ###############

set(kabc_file_core_SRCS resourcefile.cpp resourcefileconfig.cpp )


kde4_add_library(kabc_file_core ${LIBRARY_TYPE} ${kabc_file_core_SRCS})

target_link_libraries(kabc_file_core ${KDE4_KIO_LIBS} kabc kresources)

set_target_properties(kabc_file_core PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install(TARGETS kabc_file_core EXPORT kdepimlibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

########### kabc_file plugin ###############

set(kabc_file_SRCS resourcefileplugin.cpp)


kde4_add_plugin(kabc_file ${kabc_file_SRCS})

target_link_libraries(kabc_file ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} kabc kabc_file_core kresources)


install(TARGETS kabc_file  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES file.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kresources/kabc )
