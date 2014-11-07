include(ECMAddTests)

set(QT_REQUIRED_VERSION "5.2.0")
find_package(Qt5Test ${QT_REQUIRED_VERSION} CONFIG REQUIRED)

########### next target ###############

set(testroundtrip_srcs testroundtrip.cpp)
qt5_add_resources(testroundtrip_srcs testroundtrip.qrc)
add_executable(testroundtrip ${testroundtrip_srcs})
add_test(NAME kcontacts-testroundtrip COMMAND testroundtrip)
target_link_libraries(testroundtrip KF5Contacts Qt5::Test)

ecm_add_tests(addresstest.cpp geotest.cpp keytest.cpp phonenumbertest.cpp picturetest.cpp secrecytest.cpp soundtest.cpp timezonetest.cpp  contactgrouptest.cpp vcarddragtest.cpp NAME_PREFIX "kcontacts-" LINK_LIBRARIES KF5Contacts Qt5::Test KF5::ConfigCore Qt5::Gui)
