cmake_minimum_required(VERSION 3.10)
project(scopuli CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the githooks directory to auto format and update the readme.
message("${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} -> git config --local core.hooksPath .githooks")
execute_process(
    COMMAND git config --local core.hooksPath .githooks
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

option(SCOPULI_BUILD_TESTS    "Build the scopuli tests, Default=ON." ON)
option(SCOPULI_CODE_COVERAGE  "Enable code coverage, tests must also be enabled, Default=OFF" OFF)
option(SCOPULI_BUILD_EXAMPLES "Build the examples, Defaul=ON" ON)

add_subdirectory(vendor/fmtlib/fmt)
add_subdirectory(vendor/gabime/spdlog)
add_subdirectory(vendor/jbaldwin/libchain)
add_subdirectory(vendor/jbaldwin/libcoro)
add_subdirectory(vendor/jbaldwin/libturbohttp)

set(CHAIN_BUILD_TESTS OFF)
set(CHAIN_BUILD_EXAMPLES OFF)

set(LIBCORO_BUILD_TESTS OFF)
set(LIBCORO_BUILD_EXAMPLES OFF)

set(TURBOHTTP_BUILD_EXAMPLES OFF)
set(TURBOHTTP_BUILD_TESTS OFF)

set(SCOPULI_SOURCE_FILES
    inc/scopuli/scopuli.hpp src/scopuli.cpp
    inc/scopuli/status_code.hpp
    inc/scopuli/version.hpp
)

add_library(${PROJECT_NAME} STATIC ${SCOPULI_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog fmt::fmt chain libcoro turbohttp)
target_include_directories(${PROJECT_NAME} PUBLIC inc)

if(SCOPULI_BUILD_TESTS)
    if(SCOPULI_CODE_COVERAGE)
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
        target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
    endif()

    enable_testing()
    add_subdirectory(test)
endif()

if(SCOPULI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()