
include_directories( ${CMAKE_SOURCE_DIR}/kabc )


########### next target ###############

set(kabc_directory_SRCS resourcedirplugin.cpp resourcedir.cpp resourcedirconfig.cpp )

kde4_automoc(${kabc_directory_SRCS})

kde4_add_plugin(kabc_directory ${kabc_directory_SRCS})

target_link_libraries(kabc_directory ${KDE4_KDECORE_LIBS} kabc )


kde4_install_libtool_file(${PLUGIN_INSTALL_DIR} kabc_directory )


install(TARGETS kabc_directory  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES dir.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kresources/kabc )
install( FILES resourcedir.h DESTINATION ${INCLUDE_INSTALL_DIR}/kabc )
