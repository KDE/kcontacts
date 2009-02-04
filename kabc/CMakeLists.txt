
project(kabc)

include_directories( ${KDE4_KIO_INCLUDES} )

add_definitions(${QDBUS_DEFINITIONS} -DKDE_DEFAULT_DEBUG_AREA=5700)
add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )

# these apply also for all subdirs
include_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}/kab
 ${CMAKE_CURRENT_BINARY_DIR}/kab
 ${CMAKE_CURRENT_SOURCE_DIR}/vcardparser
)

# kabc/vcardparser/Makefile.am: vcards

set(vcards_STAT_SRCS
  vcardparser/vcard.cpp
  vcardparser/vcardline.cpp
  vcardparser/vcardparser.cpp
)

add_subdirectory( vcardparser )
add_subdirectory( formats )
add_subdirectory( plugins )
add_subdirectory( tests )
#add_subdirectory( scripts )
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/scripts)



add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/field.cpp ${CMAKE_CURRENT_BINARY_DIR}/addressee.h ${CMAKE_CURRENT_BINARY_DIR}/addressee.cpp
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/scripts
   COMMAND ${PERL_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/makeaddressee
   DEPENDS
   scripts/makeaddressee
   scripts/addressee.src.cpp
   scripts/addressee.src.h
   scripts/entrylist
   scripts/field.src.cpp
   )


########### next target ###############

set(kabc_LIB_SRCS
   address.cpp
   addressbook.cpp
   ${CMAKE_CURRENT_BINARY_DIR}/addressee.cpp
   ${CMAKE_CURRENT_BINARY_DIR}/addressee.h
   ${CMAKE_CURRENT_BINARY_DIR}/field.cpp
   addresseedialog.cpp
   distributionlist.cpp
   distributionlistdialog.cpp
   emailselectdialog.cpp
   errorhandler.cpp
   formatfactory.cpp
   geo.cpp
   key.cpp
   phonenumber.cpp
   picture.cpp
   plugin.cpp
   resource.cpp
   resourceabc.cpp
   resourcecached.cpp
   secrecy.cpp
   sound.cpp
   stdaddressbook.cpp
   vcardconverter.cpp
   timezone.cpp
   vcardformat.cpp
   ldifconverter.cpp
   addresslineedit.cpp
   addresseelist.cpp
   vcardtool.cpp
   addresseehelper.cpp
   lock.cpp
   locknull.cpp
   sortmode.cpp
   ${vcards_STAT_SRCS}
   contactgroup.cpp
   contactgrouptool.cpp
)


kde4_add_library(kabc SHARED ${kabc_LIB_SRCS})

target_link_libraries(kabc kresources kldap ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS})
target_link_libraries(kabc LINK_INTERFACE_LIBRARIES kresources ${KDE4_KDEUI_LIBS})

set_target_properties(kabc PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kabc EXPORT kdepimlibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install( FILES countrytransl.map  DESTINATION  ${DATA_INSTALL_DIR}/kabc )
install( FILES kabc_manager.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kresources )
install( FILES
   kabc_export.h
   address.h
   addressbook.h
   ${CMAKE_CURRENT_BINARY_DIR}/addressee.h
   addresseelist.h
   addresseedialog.h
   addresslineedit.h
   contactgroup.h
   contactgrouptool.h
   distributionlist.h
   distributionlistdialog.h
   emailselectdialog.h
   errorhandler.h
   field.h
   format.h
   formatfactory.h
   geo.h
   key.h
   ldifconverter.h
   lock.h
   locknull.h
   phonenumber.h
   picture.h
   plugin.h
   resource.h
   resourceabc.h
   resourcecached.h
   secrecy.h
   sortmode.h
   sound.h
   stdaddressbook.h
   timezone.h
   vcardformat.h
   vcardconverter.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/kabc COMPONENT Devel)

