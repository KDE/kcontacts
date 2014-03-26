
include_directories(${CMAKE_SOURCE_DIR}/kabc ${CMAKE_BINARY_DIR}/kabc)

########### next target ###############

set(kabcformat_binary_PART_SRCS binaryformat.cpp)

kde4_add_plugin(kabcformat_binary ${kabcformat_binary_PART_SRCS})

target_link_libraries(kabcformat_binary ${KDE4_KDECORE_LIBS} kabc)

install(TARGETS kabcformat_binary DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install(FILES binary.desktop DESTINATION ${DATA_INSTALL_DIR}/kabc/formats)
