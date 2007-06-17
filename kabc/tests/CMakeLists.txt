include_directories( ${CMAKE_SOURCE_DIR}/kabc )

########### next target ###############

set(addresstest_SRCS addresstest.cpp)

kde4_automoc(${addresstest_SRCS})

kde4_add_unit_test(addresstest TESTNAME kabc-addresstest ${addresstest_SRCS})

target_link_libraries(addresstest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(addresseetest_SRCS addresseetest.cpp)

kde4_automoc(${addresseetest_SRCS})

kde4_add_unit_test(addresseetest TESTNAME kabc-addresseetest ${addresseetest_SRCS})

target_link_libraries(addresseetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(geotest_SRCS geotest.cpp)

kde4_automoc(${geotest_SRCS})

kde4_add_unit_test(geotest TESTNAME kabc-geotest ${geotest_SRCS})

target_link_libraries(geotest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(keytest_SRCS keytest.cpp)

kde4_automoc(${keytest_SRCS})

kde4_add_unit_test(keytest TESTNAME kabc-keytest ${keytest_SRCS})

target_link_libraries(keytest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(phonenumbertest_SRCS phonenumbertest.cpp)

kde4_automoc(${phonenumbertest_SRCS})

kde4_add_unit_test(phonenumbertest TESTNAME kabc-phonenumbertest ${phonenumbertest_SRCS})

target_link_libraries(phonenumbertest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(picturetest_SRCS picturetest.cpp)

kde4_automoc(${picturetest_SRCS})

kde4_add_unit_test(picturetest TESTNAME kabc-picturetest ${picturetest_SRCS})

target_link_libraries(picturetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(secrecytest_SRCS secrecytest.cpp)

kde4_automoc(${secrecytest_SRCS})

kde4_add_unit_test(secrecytest TESTNAME kabc-secrecytest ${secrecytest_SRCS})

target_link_libraries(secrecytest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(soundtest_SRCS soundtest.cpp)

kde4_automoc(${soundtest_SRCS})

kde4_add_unit_test(soundtest TESTNAME kabc-soundtest ${soundtest_SRCS})

target_link_libraries(soundtest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(timezonetest_SRCS timezonetest.cpp)

kde4_automoc(${timezonetest_SRCS})

kde4_add_unit_test(timezonetest TESTNAME kabc-timezonetest ${timezonetest_SRCS})

target_link_libraries(timezonetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testlock_SRCS testlock.cpp)

kde4_automoc(${testlock_SRCS})

kde4_add_test_executable(testlock ${testlock_SRCS})

target_link_libraries(testlock ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testkabc_SRCS testkabc.cpp)

kde4_automoc(${testkabc_SRCS})

kde4_add_test_executable(testkabc ${testkabc_SRCS})

target_link_libraries(testkabc ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testkabcdlg_SRCS testkabcdlg.cpp)

kde4_automoc(${testkabcdlg_SRCS})

kde4_add_test_executable(testkabcdlg ${testkabcdlg_SRCS})

target_link_libraries(testkabcdlg ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

if (UNIX)

   set(bigread_SRCS bigread.cpp)

   kde4_automoc(${bigread_SRCS})


   kde4_add_test_executable(bigread ${bigread_SRCS})

   target_link_libraries(bigread ${KDE4_KDECORE_LIBS} kabc kabc_file_core )

endif (UNIX)


########### next target ###############

if (UNIX)

   set(bigwrite_SRCS bigwrite.cpp)

   kde4_automoc(${bigwrite_SRCS})

   kde4_add_test_executable(bigwrite ${bigwrite_SRCS})

   target_link_libraries(bigwrite ${KDE4_KDECORE_LIBS} kabc kabc_file_core )

endif (UNIX)


########### next target ###############

#set(testaddresseelist_SRCS testaddresseelist.cpp)

#kde4_automoc(${testaddresseelist_SRCS})

#kde4_add_unit_test(testaddresseelist TESTNAME kabc-testaddresseelist ${testaddresseelist_SRCS})

#target_link_libraries(testaddresseelist ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(kabcargl_SRCS kabcargl.cpp)

kde4_automoc(${kabcargl_SRCS})

kde4_add_test_executable(kabcargl ${kabcargl_SRCS})

target_link_libraries(kabcargl ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testaddresslineedit_SRCS testaddresslineedit.cpp)

kde4_automoc(${testaddresslineedit_SRCS})

kde4_add_test_executable(testaddresslineedit ${testaddresslineedit_SRCS})

target_link_libraries(testaddresslineedit ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})
