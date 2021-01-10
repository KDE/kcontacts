include(ECMAddTests)

find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG REQUIRED)

########### next target ###############

set(testroundtrip_srcs testroundtrip.cpp)
qt5_add_resources(testroundtrip_srcs testroundtrip.qrc)
add_executable(testroundtrip ${testroundtrip_srcs})
add_test(NAME kcontacts-testroundtrip COMMAND testroundtrip)
target_link_libraries(testroundtrip KF5Contacts Qt5::Test)

ecm_add_tests(birthdaytest.cpp
    addresseetest.cpp
    impptest.cpp
    emailtest.cpp
    addresstest.cpp
    geotest.cpp
    keytest.cpp
    phonenumbertest.cpp
    secrecytest.cpp
    soundtest.cpp
    timezonetest.cpp
    contactgrouptest.cpp
    vcarddragtest.cpp
    langtest.cpp gendertest.cpp
    calendarurltest.cpp
    ldifconvertertest.cpp
    relatedtest.cpp
    resourcelocatorurltest.cpp
    customidentifiertest.cpp
    vcardlinetest.cpp
    fieldgrouptest.cpp
    importexportvcardtest.cpp
    titletest.cpp
    nicknametest.cpp
    roletest.cpp
    orgtest.cpp
    datetimetest.cpp
    clientpidmaptest.cpp
    NAME_PREFIX "kcontacts-" LINK_LIBRARIES KF5Contacts Qt5::Test KF5::ConfigCore)

ecm_add_tests(picturetest.cpp
    NAME_PREFIX "kcontacts-" LINK_LIBRARIES KF5Contacts Qt5::Test KF5::ConfigCore Qt5::Gui)

# Benchmark, compiled, but not run automatically with ctest
add_executable(vcardtool_benchmark vcardtool_benchmark.cpp)
target_link_libraries(vcardtool_benchmark KF5Contacts Qt5::Test KF5::ConfigCore)
