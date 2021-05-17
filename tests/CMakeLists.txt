
include(ECMMarkAsTest)

########### next target ###############

# FIXME: Build fails
add_executable(testread testread.cpp)
target_link_libraries(testread KF5Contacts KF5::I18n Qt5::Gui)

########### next target ###############

# FIXME: Build fails
add_executable(testwrite testwrite.cpp)
target_link_libraries(testwrite KF5Contacts Qt5::Gui KF5::I18n)

########### next target ###############

add_executable(testread2 testread2.cpp testutils.cpp)
target_link_libraries(testread2 KF5Contacts Qt5::Gui)
