
include_directories( ${CMAKE_SOURCE_DIR}/kabc )


########### next target ###############


########### next target ###############

if(KDE4_BUILD_TESTS)

   set(testread_SRCS testread.cpp )
   
   kde4_automoc(${testread_SRCS})
   
   kde4_add_executable(testread ${testread_SRCS})
   
   target_link_libraries(testread  ${KDE4_KDECORE_LIBS} kabc )
   
   ########### next target ###############
   
   set(testwrite_SRCS testwrite.cpp )
   
   kde4_automoc(${testwrite_SRCS})
   
   kde4_add_executable(testwrite ${testwrite_SRCS})
   
   target_link_libraries(testwrite  ${KDE4_KDECORE_LIBS} kabc )
   
   ########### next target ###############
   
   set(testread2_SRCS testread2.cpp testutils.cpp )
   
   kde4_automoc(${testread2_SRCS})
   
   kde4_add_executable(testread2 ${testread2_SRCS})
   
   target_link_libraries(testread2  ${KDE4_KDECORE_LIBS} kabc )

endif(KDE4_BUILD_TESTS)

########### install files ###############
install( FILES vcard.h vcardline.h vcardparser.h DESTINATION ${INCLUDE_INSTALL_DIR}/kabc )
