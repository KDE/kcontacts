
include_directories( ${CMAKE_SOURCE_DIR}/kabc )


########### next target ###############

set(kabc_ldapkio_SRCS resourceldapkioplugin.cpp resourceldapkio.cpp resourceldapkioconfig.cpp )


kde4_add_plugin(kabc_ldapkio ${kabc_ldapkio_SRCS})

target_link_libraries(kabc_ldapkio  ${KDE4_KIO_LIBS} kabc kldap kresources)


install(TARGETS kabc_ldapkio  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES ldapkio.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kresources/kabc )
