
include_directories( ${CMAKE_SOURCE_DIR}/kabc )


########### next target ###############

set(kabc_net_SRCS resourcenetplugin.cpp resourcenet.cpp resourcenetconfig.cpp )

kde4_automoc(${kabc_net_SRCS})

kde4_add_plugin(kabc_net ${kabc_net_SRCS})

target_link_libraries(kabc_net  ${KDE4_KIO_LIBS} kabc)


install(TARGETS kabc_net  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES net.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kresources/kabc )
