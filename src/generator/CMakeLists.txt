set(CLDR_DIR "/usr/share/unicode/cldr/common/main" CACHE STRING "Path to the CLDR data files")
add_executable(generatecountrytoisomap main.cpp translatedcountrylist.cpp)
target_link_libraries(generatecountrytoisomap Qt5::Core)
add_custom_command(
    OUTPUT countrytoisomap_data.cpp
    COMMAND generatecountrytoisomap -o ${CMAKE_CURRENT_SOURCE_DIR}/../countrytoisomap_data.cpp -s ${CLDR_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(ISOCODES_DIR "/usr/share/iso-codes" CACHE STRING "Path to the iso-codes data files")
add_executable(generateisotocountrymap generateisotocountrymap.cpp)
target_link_libraries(generateisotocountrymap Qt5::Core)
add_custom_command(
    OUTPUT isotocountrymap_data.cpp
    COMMAND generateisotocountrymap -o ${CMAKE_CURRENT_SOURCE_DIR}/../isotocountrymap_data.cpp -i ${ISOCODES_DIR}/json/iso_3166-1.json
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(rebuild-countrymaps DEPENDS countrytoisomap_data.cpp isotocountrymap_data.cpp)
