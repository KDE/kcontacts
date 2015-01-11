ecm_generate_headers(KContacts_CamelCase_Converter_HEADERS
  HEADER_NAMES
  LDIFConverter
  VCardConverter
  PREFIX KContacts
  REQUIRED_HEADERS KContacts_Converter_HEADERS
)

install(FILES
    ${KContacts_CamelCase_Converter_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts/KContacts
    COMPONENT Devel
)


install(FILES
    ${KContacts_Converter_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KContacts/kcontacts
    COMPONENT Devel
)


