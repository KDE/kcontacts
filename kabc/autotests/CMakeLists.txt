include(ECMAddTests)

set(QT_REQUIRED_VERSION "5.2.0")
find_package(Qt5Test ${QT_REQUIRED_VERSION} CONFIG REQUIRED)

########### next target ###############

ecm_add_tests(addresstest.cpp geotest.cpp keytest.cpp phonenumbertest.cpp picturetest.cpp secrecytest.cpp soundtest.cpp timezonetest.cpp  contactgrouptest.cpp vcarddragtest.cpp testroundtrip.cpp NAME_PREFIX "ktnetf-" LINK_LIBRARIES KF5Abc Qt5::Test KF5::ConfigCore Qt5::Widgets)
