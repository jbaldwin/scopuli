cmake_minimum_required(VERSION 3.10)
project(scopuli_test CXX)

set(SCOPULI_TEST_SOURCE_FILES
    http_client_test.cpp
    http_server_test.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${SCOPULI_TEST_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE scopuli)

if(SCOPULI_CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
    target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
endif()

add_test(NAME scopuli_tests COMMAND ${PROJECT_NAME})
