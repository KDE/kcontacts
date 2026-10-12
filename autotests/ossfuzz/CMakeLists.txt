# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-License-Identifier: BSD-2-Clause

if(DEFINED ENV{LIB_FUZZING_ENGINE})
    separate_arguments(fuzzing_engine NATIVE_COMMAND "$ENV{LIB_FUZZING_ENGINE}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(fuzzing_engine -fsanitize=fuzzer)
else()
    message(FATAL_ERROR "Fuzzing requires Clang or LIB_FUZZING_ENGINE")
endif()

add_executable(kcontacts_fuzzer kcontacts_fuzzer.cc)
target_link_libraries(kcontacts_fuzzer PRIVATE KF6Contacts ${fuzzing_engine})

set_target_properties(kcontacts_fuzzer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/fuzzers
)
