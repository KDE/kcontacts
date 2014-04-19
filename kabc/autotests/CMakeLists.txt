include(ECMMarkAsTest)

set(QT_REQUIRED_VERSION "5.2.0")
find_package(Qt5Test ${QT_REQUIRED_VERSION} CONFIG REQUIRED)

########### next target ###############

set(addresstest_SRCS addresstest.cpp)

add_executable(addresstest ${addresstest_SRCS})
add_test(addresstest addresstest)
ecm_mark_as_test(addresstest)
target_link_libraries(addresstest KF5Abc Qt5::Test)

########### next target ###############

set(geotest_SRCS geotest.cpp)
add_executable(geotest ${geotest_SRCS})
add_test(geotest geotest)
ecm_mark_as_test(geotest)
target_link_libraries(geotest KF5Abc Qt5::Test)

########### next target ###############

set(keytest_SRCS keytest.cpp)
add_executable(keytest ${keytest_SRCS})
add_test(keytest keytest)
ecm_mark_as_test(keytest)
target_link_libraries(keytest KF5Abc Qt5::Test)

########### next target ###############

set(phonenumbertest_SRCS phonenumbertest.cpp)
add_executable(phonenumbertest ${phonenumbertest_SRCS})
add_test(phonenumbertest phonenumbertest)
ecm_mark_as_test(phonenumbertest)
target_link_libraries(phonenumbertest KF5Abc Qt5::Test)

########### next target ###############

set(picturetest_SRCS picturetest.cpp)
add_executable(picturetest ${picturetest_SRCS})
add_test(picturetest picturetest)
ecm_mark_as_test(picturetest)
target_link_libraries(picturetest KF5Abc Qt5::Test)

########### next target ###############

set(secrecytest_SRCS secrecytest.cpp)
add_executable(secrecytest ${secrecytest_SRCS})
add_test(secrecytest secrecytest)
ecm_mark_as_test(secrecytest)
target_link_libraries(secrecytest KF5Abc Qt5::Test)

########### next target ###############

set(soundtest_SRCS soundtest.cpp)
add_executable(soundtest ${soundtest_SRCS})
add_test(soundtest soundtest)
ecm_mark_as_test(soundtest)
target_link_libraries(soundtest KF5Abc Qt5::Test)

########### next target ###############

set(timezonetest_SRCS timezonetest.cpp)
add_executable(timezonetest ${timezonetest_SRCS})
add_test(timezonetest timezonetest)
ecm_mark_as_test(timezonetest)
target_link_libraries(timezonetest KF5Abc Qt5::Test)

########### next target ###############

set(contactgrouptest_SRCS contactgrouptest.cpp)
add_executable(contactgrouptest ${contactgrouptest_SRCS})
add_test(contactgrouptest contactgrouptest)
ecm_mark_as_test(contactgrouptest)
target_link_libraries(contactgrouptest KF5Abc Qt5::Test)

########### next target ###############

set(vcarddragtest_SRCS vcarddragtest.cpp)
add_executable(vcarddragtest ${vcarddragtest_SRCS})
add_test(vcarddragtest vcarddragtest)
ecm_mark_as_test(vcarddragtest)
target_link_libraries(vcarddragtest KF5Abc Qt5::Test)

########### next target ###############

set(testroundtrip_SRCS testroundtrip.cpp)
qt5_add_resources(testroundtrip_SRCS testroundtrip.qrc)
add_executable(testroundtrip ${testroundtrip_SRCS})
add_test(testroundtrip testroundtrip)
ecm_mark_as_test(testroundtrip)
target_link_libraries(testroundtrip KF5Abc Qt5::Test)
