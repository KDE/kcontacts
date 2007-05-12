# Test apps should be in the bin dir so that add_test can locate them
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( ${CMAKE_SOURCE_DIR}/kabc )

if(KDE4_BUILD_TESTS)

########### next target ###############

set(addresstest_SRCS addresstest.cpp)

kde4_automoc(${addresstest_SRCS})

kde4_add_executable(addresstest NOGUI RUN_UNINSTALLED ${addresstest_SRCS})

target_link_libraries(addresstest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-addresstest ${EXECUTABLE_OUTPUT_PATH}/addresstest)

########### next target ###############

set(addresseetest_SRCS addresseetest.cpp)

kde4_automoc(${addresseetest_SRCS})

kde4_add_executable(addresseetest NOGUI RUN_UNINSTALLED ${addresseetest_SRCS})

target_link_libraries(addresseetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-addresseetest ${EXECUTABLE_OUTPUT_PATH}/addresseetest)

########### next target ###############

set(geotest_SRCS geotest.cpp)

kde4_automoc(${geotest_SRCS})

kde4_add_executable(geotest NOGUI RUN_UNINSTALLED ${geotest_SRCS})

target_link_libraries(geotest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-geotest ${EXECUTABLE_OUTPUT_PATH}/geotest)

########### next target ###############

set(keytest_SRCS keytest.cpp)

kde4_automoc(${keytest_SRCS})

kde4_add_executable(keytest NOGUI RUN_UNINSTALLED ${keytest_SRCS})

target_link_libraries(keytest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-keytest ${EXECUTABLE_OUTPUT_PATH}/keytest)

########### next target ###############

set(phonenumbertest_SRCS phonenumbertest.cpp)

kde4_automoc(${phonenumbertest_SRCS})

kde4_add_executable(phonenumbertest NOGUI RUN_UNINSTALLED ${phonenumbertest_SRCS})

target_link_libraries(phonenumbertest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-phonenumbertest ${EXECUTABLE_OUTPUT_PATH}/phonenumbertest)

########### next target ###############

set(picturetest_SRCS picturetest.cpp)

kde4_automoc(${picturetest_SRCS})

kde4_add_executable(picturetest NOGUI RUN_UNINSTALLED ${picturetest_SRCS})

target_link_libraries(picturetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-picturetest ${EXECUTABLE_OUTPUT_PATH}/picturetest)

########### next target ###############

set(secrecytest_SRCS secrecytest.cpp)

kde4_automoc(${secrecytest_SRCS})

kde4_add_executable(secrecytest NOGUI RUN_UNINSTALLED ${secrecytest_SRCS})

target_link_libraries(secrecytest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-secrecytest ${EXECUTABLE_OUTPUT_PATH}/secrecytest)

########### next target ###############

set(soundtest_SRCS soundtest.cpp)

kde4_automoc(${soundtest_SRCS})

kde4_add_executable(soundtest NOGUI RUN_UNINSTALLED ${soundtest_SRCS})

target_link_libraries(soundtest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-soundtest ${EXECUTABLE_OUTPUT_PATH}/soundtest)

########### next target ###############

set(timezonetest_SRCS timezonetest.cpp)

kde4_automoc(${timezonetest_SRCS})

kde4_add_executable(timezonetest NOGUI RUN_UNINSTALLED ${timezonetest_SRCS})

target_link_libraries(timezonetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-timezonetest ${EXECUTABLE_OUTPUT_PATH}/timezonetest)

########### next target ###############

set(testlock_SRCS testlock.cpp)

kde4_automoc(${testlock_SRCS})

kde4_add_executable(testlock RUN_UNINSTALLED ${testlock_SRCS})

target_link_libraries(testlock ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testkabc_SRCS testkabc.cpp)

kde4_automoc(${testkabc_SRCS})

kde4_add_executable(testkabc NOGUI RUN_UNINSTALLED ${testkabc_SRCS})

target_link_libraries(testkabc ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testkabcdlg_SRCS testkabcdlg.cpp)

kde4_automoc(${testkabcdlg_SRCS})

kde4_add_executable(testkabcdlg RUN_UNINSTALLED ${testkabcdlg_SRCS})

target_link_libraries(testkabcdlg ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

if (UNIX)

   set(bigread_SRCS bigread.cpp)

   kde4_automoc(${bigread_SRCS})


   kde4_add_executable(bigread NOGUI RUN_UNINSTALLED ${bigread_SRCS})

   target_link_libraries(bigread ${KDE4_KDECORE_LIBS} kabc kabc_file_core )

endif (UNIX)


########### next target ###############

if (UNIX)

   set(bigwrite_SRCS bigwrite.cpp)

   kde4_automoc(${bigwrite_SRCS})

   kde4_add_executable(bigwrite NOGUI RUN_UNINSTALLED ${bigwrite_SRCS})

   target_link_libraries(bigwrite ${KDE4_KDECORE_LIBS} kabc kabc_file_core )

endif (UNIX)


########### next target ###############

#set(testaddresseelist_SRCS testaddresseelist.cpp)

#kde4_automoc(${testaddresseelist_SRCS})

#kde4_add_executable(testaddresseelist NOGUI RUN_UNINSTALLED ${testaddresseelist_SRCS})

#target_link_libraries(testaddresseelist ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

#add_test(kabc-testaddresselist ${EXECUTABLE_OUTPUT_PATH}/testaddresseelist)

########### next target ###############

set(kabcargl_SRCS kabcargl.cpp)

kde4_automoc(${kabcargl_SRCS})

kde4_add_executable(kabcargl NOGUI RUN_UNINSTALLED ${kabcargl_SRCS})

target_link_libraries(kabcargl ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testaddresslineedit_SRCS testaddresslineedit.cpp)

kde4_automoc(${testaddresslineedit_SRCS})

kde4_add_executable(testaddresslineedit NOGUI RUN_UNINSTALLED ${testaddresslineedit_SRCS})

target_link_libraries(testaddresslineedit ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

endif(KDE4_BUILD_TESTS)
