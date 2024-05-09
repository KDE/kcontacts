ecm_add_qml_module(kcontactsqml URI "org.kde.contacts" CLASS_NAME KContactsQmlPlugin DEPENDENCIES QtQuick)

target_sources(kcontactsqml PRIVATE
    kcontactsqmlplugin.cpp
    types.h
)

target_link_libraries(kcontactsqml PRIVATE KF6Contacts Qt6::Qml)

ecm_finalize_qml_module(kcontactsqml)
