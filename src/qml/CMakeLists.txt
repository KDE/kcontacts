ecm_add_qml_module(kcontactsqml URI "org.kde.contacts" GENERATE_PLUGIN_SOURCE DEPENDENCIES QtQuick)

target_sources(kcontactsqml PRIVATE types.h)

target_link_libraries(kcontactsqml PRIVATE KF6Contacts Qt6::Qml)

ecm_finalize_qml_module(kcontactsqml)
