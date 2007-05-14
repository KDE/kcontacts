
include_directories( ${CMAKE_SOURCE_DIR}/kabc )


########### kabc_file_core library (also used by kaddressbook) ###############

set(kabc_file_core_SRCS resourcefile.cpp resourcefileconfig.cpp )

kde4_automoc(${kabc_file_core_SRCS})

kde4_add_library(kabc_file_core SHARED ${kabc_file_core_SRCS})

target_link_libraries(kabc_file_core ${KDE4_KDECORE_LIBS} kabc)

set_target_properties(kabc_file_core PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install(TARGETS kabc_file_core  DESTINATION ${LIB_INSTALL_DIR} )


########### kabc_file plugin ###############

set(kabc_file_SRCS resourcefileplugin.cpp)

kde4_automoc(${kabc_file_SRCS})

kde4_add_plugin(kabc_file ${kabc_file_SRCS})

target_link_libraries(kabc_file ${KDE4_KDECORE_LIBS} kabc_file_core)


install(TARGETS kabc_file  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES file.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kresources/kabc )
