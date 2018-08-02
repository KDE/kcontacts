set(CLDR_DIR "/usr/share/unicode/cldr/common/main" CACHE STRING "Path to the CLDR data files")

add_executable(generatecountrytoisomap main.cpp translatedcountrylist.cpp)
target_link_libraries(generatecountrytoisomap Qt5::Core)

add_custom_command(
    OUTPUT countrytoisomap_data.cpp
    COMMAND generatecountrytoisomap -o ${CMAKE_CURRENT_SOURCE_DIR}/../countrytoisomap_data.cpp -s ${CLDR_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(rebuild-countrymaps DEPENDS countrytoisomap_data.cpp)
