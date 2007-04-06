# Test apps should be in the bin dir so that add_test can locate them
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( ${CMAKE_SOURCE_DIR}/kabc )
add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

if(KDE4_BUILD_TESTS)

########### next target ###############

set(testlock_SRCS testlock.cpp)

kde4_automoc(${testlock_SRCS})

kde4_add_executable(testlock RUN_UNINSTALLED ${testlock_SRCS})

target_link_libraries(testlock ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-testlock ${EXECUTABLE_OUTPUT_PATH}/testlock)

########### next target ###############

set(geotest_SRCS geotest.cpp)

kde4_automoc(${geotest_SRCS})

kde4_add_executable(geotest NOGUI RUN_UNINSTALLED ${geotest_SRCS})

target_link_libraries(geotest ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-geotest ${EXECUTABLE_OUTPUT_PATH}/geotest)

########### next target ###############

set(testkabc_SRCS testkabc.cpp)

kde4_automoc(${testkabc_SRCS})

kde4_add_executable(testkabc NOGUI RUN_UNINSTALLED ${testkabc_SRCS})

target_link_libraries(testkabc ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-testkabc ${EXECUTABLE_OUTPUT_PATH}/testkabc)

########### next target ###############

set(testkabcdlg_SRCS testkabcdlg.cpp)

kde4_automoc(${testkabcdlg_SRCS})

kde4_add_executable(testkabcdlg RUN_UNINSTALLED ${testkabcdlg_SRCS})

target_link_libraries(testkabcdlg ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-testkabcdlg ${EXECUTABLE_OUTPUT_PATH}/testkabcdlg)

########### next target ###############

if (UNIX)

   set(bigread_SRCS bigread.cpp)

   kde4_automoc(${bigread_SRCS})


   kde4_add_executable(bigread NOGUI RUN_UNINSTALLED ${bigread_SRCS})

   target_link_libraries(bigread ${KDE4_KDECORE_LIBS} kabc kabc_file_core )

   add_test(kabc-bigread ${EXECUTABLE_OUTPUT_PATH}/bigread)

endif (UNIX)


########### next target ###############

if (UNIX)

   set(bigwrite_SRCS bigwrite.cpp)

   kde4_automoc(${bigwrite_SRCS})

   kde4_add_executable(bigwrite NOGUI RUN_UNINSTALLED ${bigwrite_SRCS})

   target_link_libraries(bigwrite ${KDE4_KDECORE_LIBS} kabc kabc_file_core )

   add_test(kabc-bigwrite ${EXECUTABLE_OUTPUT_PATH}/bigwrite)

endif (UNIX)


########### next target ###############

set(testaddressee_SRCS testaddressee.cpp)

kde4_automoc(${testaddressee_SRCS})

kde4_add_executable(testaddressee NOGUI RUN_UNINSTALLED ${testaddressee_SRCS})

target_link_libraries(testaddressee ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-testaddressee ${EXECUTABLE_OUTPUT_PATH}/testaddressee)

########### next target ###############

#set(testaddresseelist_SRCS testaddresseelist.cpp)

#kde4_automoc(${testaddresseelist_SRCS})

#kde4_add_executable(testaddresseelist NOGUI RUN_UNINSTALLED ${testaddresseelist_SRCS})

#target_link_libraries(testaddresseelist ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

#add_test(kabc-testaddresselist ${EXECUTABLE_OUTPUT_PATH}/testaddresseelist)

########### next target ###############

set(testaddressfmt_SRCS testaddressfmt.cpp)

kde4_automoc(${testaddressfmt_SRCS})

kde4_add_executable(testaddressfmt NOGUI RUN_UNINSTALLED ${testaddressfmt_SRCS})

target_link_libraries(testaddressfmt ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-testaddressfmt ${EXECUTABLE_OUTPUT_PATH}/testaddressfmt)

########### next target ###############

set(kabcargl_SRCS kabcargl.cpp)

kde4_automoc(${kabcargl_SRCS})

kde4_add_executable(kabcargl NOGUI RUN_UNINSTALLED ${kabcargl_SRCS})

target_link_libraries(kabcargl ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-kabcargl ${EXECUTABLE_OUTPUT_PATH}/kabcargl)

########### next target ###############

set(testaddresslineedit_SRCS testaddresslineedit.cpp)

kde4_automoc(${testaddresslineedit_SRCS})

kde4_add_executable(testaddresslineedit NOGUI RUN_UNINSTALLED ${testaddresslineedit_SRCS})

target_link_libraries(testaddresslineedit ${KDE4_KDECORE_LIBS} kabc ${QT_QTTEST_LIBRARY})

add_test(kabc-testaddresslineedit ${EXECUTABLE_OUTPUT_PATH}/testaddresslineedit)

endif(KDE4_BUILD_TESTS)
