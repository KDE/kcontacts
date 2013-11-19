set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/kabc ${CMAKE_BINARY_DIR}/kabc/plugins/file)

########### next target ###############

set(addresstest_SRCS addresstest.cpp)


kde4_add_unit_test(addresstest TESTNAME kabc-addresstest ${addresstest_SRCS})

target_link_libraries(addresstest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(addresseetest_SRCS addresseetest.cpp)


kde4_add_unit_test(addresseetest TESTNAME kabc-addresseetest ${addresseetest_SRCS})

target_link_libraries(addresseetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(geotest_SRCS geotest.cpp)


kde4_add_unit_test(geotest TESTNAME kabc-geotest ${geotest_SRCS})

target_link_libraries(geotest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(keytest_SRCS keytest.cpp)


kde4_add_unit_test(keytest TESTNAME kabc-keytest ${keytest_SRCS})

target_link_libraries(keytest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(phonenumbertest_SRCS phonenumbertest.cpp)


kde4_add_unit_test(phonenumbertest TESTNAME kabc-phonenumbertest ${phonenumbertest_SRCS})

target_link_libraries(phonenumbertest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(picturetest_SRCS picturetest.cpp)


kde4_add_unit_test(picturetest TESTNAME kabc-picturetest ${picturetest_SRCS})

target_link_libraries(picturetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(secrecytest_SRCS secrecytest.cpp)


kde4_add_unit_test(secrecytest TESTNAME kabc-secrecytest ${secrecytest_SRCS})

target_link_libraries(secrecytest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(soundtest_SRCS soundtest.cpp)


kde4_add_unit_test(soundtest TESTNAME kabc-soundtest ${soundtest_SRCS})

target_link_libraries(soundtest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(timezonetest_SRCS timezonetest.cpp)


kde4_add_unit_test(timezonetest TESTNAME kabc-timezonetest ${timezonetest_SRCS})

target_link_libraries(timezonetest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

if (NOT KDEPIM_NO_KRESOURCES)
set(testlock_SRCS testlock.cpp)


kde4_add_executable(testlock TEST ${testlock_SRCS})

target_link_libraries(testlock ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} kabc ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY})

########### next target ###############

set(testkabc_SRCS testkabc.cpp)


kde4_add_executable(testkabc TEST ${testkabc_SRCS})

target_link_libraries(testkabc ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${KDE4_KDEUI_LIBS})

########### next target ###############

set(testkabcdlg_SRCS testkabcdlg.cpp)


kde4_add_executable(testkabcdlg TEST ${testkabcdlg_SRCS})

target_link_libraries(testkabcdlg ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${KDE4_KDEUI_LIBS})

########### next target ###############

if (UNIX)

   set(bigread_SRCS bigread.cpp)



   kde4_add_executable(bigread TEST ${bigread_SRCS})

   target_link_libraries(bigread ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} kabc kabc_file_core )

endif()


########### next target ###############

if (UNIX)

   set(bigwrite_SRCS bigwrite.cpp)


   kde4_add_executable(bigwrite TEST ${bigwrite_SRCS})

   target_link_libraries(bigwrite ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} kabc kabc_file_core )

endif()
endif()


########### next target ###############

#set(testaddresseelist_SRCS testaddresseelist.cpp)


#kde4_add_unit_test(testaddresseelist TESTNAME kabc-testaddresseelist ${testaddresseelist_SRCS})

#target_link_libraries(testaddresseelist ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

if (NOT KDEPIM_NO_KRESOURCES)
set(kabcargl_SRCS kabcargl.cpp)


kde4_add_executable(kabcargl TEST ${kabcargl_SRCS})

target_link_libraries(kabcargl ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${KDE4_KDEUI_LIBS} )

########### next target ###############

set(testaddresslineedit_SRCS testaddresslineedit.cpp)


kde4_add_executable(testaddresslineedit TEST ${testaddresslineedit_SRCS})

target_link_libraries(testaddresslineedit ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY} ${KDE4_KDEUI_LIBS})
endif()

########### next target ###############

set(contactgrouptest_SRCS contactgrouptest.cpp)


kde4_add_unit_test(contactgrouptest TESTNAME kabc-contactgrouptest ${contactgrouptest_SRCS})

target_link_libraries(contactgrouptest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

########### next target ###############

set(vcarddragtest_SRCS vcarddragtest.cpp)

kde4_add_unit_test(vcarddragtest TESTNAME kabc-vcarddragtest ${vcarddragtest_SRCS})

target_link_libraries(vcarddragtest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})
