set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/kabc)

########### next target ###############

set(testroundtrip_SRCS testroundtrip.cpp)

qt4_add_resources(testroundtrip_SRCS testroundtrip.qrc)

kde4_add_unit_test(testroundtrip TESTNAME kabc-vcardroundtriptest ${testroundtrip_SRCS})

target_link_libraries(testroundtrip kabc ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(testread_SRCS testread.cpp)

kde4_add_executable(testread TEST ${testread_SRCS})

target_link_libraries(testread ${KDE4_KDECORE_LIBS} kabc)

########### next target ###############

set(testwrite_SRCS testwrite.cpp)

kde4_add_executable(testwrite TEST ${testwrite_SRCS})

target_link_libraries(testwrite ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} kabc ${KDE4_KDE4SUPPORT_LIBS})

########### next target ###############

set(testread2_SRCS testread2.cpp testutils.cpp)

kde4_add_executable(testread2 TEST ${testread2_SRCS})

target_link_libraries(testread2 ${KDE4_KDECORE_LIBS} kabc)

########### install files ###############
install(
  FILES
  vcard.h
  vcardline.h
  vcardparser.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/kabc COMPONENT Devel
)
