
include(ECMMarkAsTest)

########### next target ###############

# FIXME: Build fails
add_executable(testread testread.cpp)
target_link_libraries(testread KF6Contacts KF6::I18n Qt${QT_MAJOR_VERSION}::Gui)

########### next target ###############

# FIXME: Build fails
add_executable(testwrite testwrite.cpp)
target_link_libraries(testwrite KF6Contacts Qt${QT_MAJOR_VERSION}::Gui KF6::I18n)

########### next target ###############

add_executable(testread2 testread2.cpp testutils.cpp)
target_link_libraries(testread2 KF6Contacts Qt${QT_MAJOR_VERSION}::Gui)

if (TARGET Qt${QT_MAJOR_VERSION}::Quick)
    add_executable(qmlintegrationtest
        qmlintegrationtest.cpp
        qmlintegrationtest.qrc
    )
    target_link_libraries(qmlintegrationtest PRIVATE KF6::Contacts Qt${QT_MAJOR_VERSION}::Quick)
endif()
