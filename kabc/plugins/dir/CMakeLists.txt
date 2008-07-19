
include_directories( ${CMAKE_SOURCE_DIR}/kabc )


########### next target ###############

set(kabc_directory_SRCS resourcedirplugin.cpp resourcedir.cpp resourcedirconfig.cpp )


kde4_add_plugin(kabc_directory ${kabc_directory_SRCS})

target_link_libraries(kabc_directory ${KDE4_KIO_LIBS} kabc kresources )

install(TARGETS kabc_directory  DESTINATION ${PLUGIN_INSTALL_DIR} )

########### install files ###############

install( FILES dir.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kresources/kabc )
