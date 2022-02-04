include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION}Test ${REQUIRED_QT_VERSION} CONFIG REQUIRED)

########### next target ###############

add_executable(testroundtrip)
target_sources(testroundtrip PRIVATE testroundtrip.cpp testroundtrip.qrc)
add_test(NAME kcontacts-testroundtrip COMMAND testroundtrip)
target_link_libraries(testroundtrip KF5Contacts Qt${QT_MAJOR_VERSION}::Test)

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
    NAME_PREFIX "kcontacts-" LINK_LIBRARIES KF5Contacts Qt${QT_MAJOR_VERSION}::Test KF5::ConfigCore)

ecm_add_test(addressformattest.cpp ../src/kcontacts.qrc
    TEST_NAME addressformattest
    NAME_PREFIX "kcontacts-" LINK_LIBRARIES KF5Contacts KF5::I18nLocaleData KF5::ConfigCore Qt${QT_MAJOR_VERSION}::Test)

ecm_add_tests(picturetest.cpp
    NAME_PREFIX "kcontacts-" LINK_LIBRARIES KF5Contacts Qt${QT_MAJOR_VERSION}::Test KF5::ConfigCore Qt${QT_MAJOR_VERSION}::Gui)

# Benchmark, compiled, but not run automatically with ctest
add_executable(vcardtool_benchmark vcardtool_benchmark.cpp)
target_link_libraries(vcardtool_benchmark KF5Contacts Qt${QT_MAJOR_VERSION}::Test KF5::ConfigCore)
