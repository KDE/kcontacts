include(ECMAddTests)

find_package(Qt6Test ${REQUIRED_QT_VERSION} CONFIG REQUIRED)

set(ECM_TEST_NAME_PREFIX "kcontacts-")

########### next target ###############

add_executable(testroundtrip)
target_sources(testroundtrip PRIVATE testroundtrip.cpp testroundtrip.qrc)
add_test(NAME kcontacts-testroundtrip COMMAND testroundtrip)
target_link_libraries(testroundtrip KF6Contacts Qt6::Test)

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
    LINK_LIBRARIES KF6Contacts Qt6::Test KF6::ConfigCore)

ecm_add_test(vcardparsertest.cpp
    ../src/vcardparser/vcard.cpp
    ../src/vcardparser/vcardparser.cpp
    TEST_NAME vcardparsertest
    LINK_LIBRARIES KF6Contacts Qt6::Test KF6::Codecs)

ecm_add_test(addressformattest.cpp ../src/kcontacts.qrc
    TEST_NAME addressformattest
    LINK_LIBRARIES KF6Contacts KF6::I18nLocaleData KF6::ConfigCore Qt6::Test)

ecm_add_tests(picturetest.cpp
    LINK_LIBRARIES KF6Contacts Qt6::Test KF6::ConfigCore Qt6::Gui)

# Benchmark, compiled, but not run automatically with ctest
add_executable(vcardtool_benchmark vcardtool_benchmark.cpp)
target_link_libraries(vcardtool_benchmark KF6Contacts Qt6::Test KF6::ConfigCore)

if (USE_QML AND BUILD_SHARED_LIBS)
    add_test(NAME kcontacts-qmltypestest
        COMMAND Qt6::qmltestrunner
            -input ${CMAKE_CURRENT_SOURCE_DIR}/qml/tst_types.qml
            -import ${CMAKE_BINARY_DIR}/bin
    )
    set_tests_properties(kcontacts-qmltypestest PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
