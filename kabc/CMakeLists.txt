
project(kabc)

include_directories( ${KDE4_KIO_INCLUDES} )

add_definitions(${QDBUS_DEFINITIONS} -DKDE_DEFAULT_DEBUG_AREA=5700 )

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
)


kde4_add_library(kabc SHARED ${kabc_LIB_SRCS})

target_link_libraries(kabc kresources kldap ${KDE4_KDE3SUPPORT_LIBRARY})

set_target_properties(kabc PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kabc ${INSTALL_TARGETS_DEFAULT_ARGS})

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
   distributionlist.h
   distributionlistdialog.h
   emailselectdialog.h
   errorhandler.h
   field.h
   format.h
   formatfactory.h
   format.h
   geo.h
   key.h
   phonenumber.h
   picture.h
   plugin.h
   resource.h
   resourceabc.h
   resourcecached.h
   secrecy.h
   sound.h
   stdaddressbook.h
   timezone.h
   vcardformat.h
   vcardconverter.h
   ldifconverter.h
   addresslineedit.h
   addresseelist.h
   lock.h
   locknull.h
   sortmode.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/kabc COMPONENT Devel)

