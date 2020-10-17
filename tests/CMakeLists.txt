cmake_minimum_required(VERSION 3.2)
project(chess_tests)
enable_testing()
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

file(GLOB UNIT_TESTS_INC_FILES ${PROJECT_SOURCE_DIR}/test/*.hpp)
file(GLOB UNIT_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

set(UNIT_TESTS_INCLUDES ${PROJECT_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${UNIT_TESTS_INCLUDES})

set(SOURCE_FILES board_test.cpp)
add_executable(chess_tests board_test.cpp ../src/Board.h ../src/Board.cpp ../src/PgnParser.cpp ../src/PgnParser.h)

target_link_libraries(chess_tests
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )
