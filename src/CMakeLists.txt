cmake_minimum_required(VERSION 3.13.4)
project(chess_engine_cpp_exe)

set(CMAKE_CXX_STANDARD 14)

# add_executable(main.cpp Board.h Board.cpp)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(source_files main.cpp Board.h Board.cpp PgnParser.cpp PgnParser.h)
add_executable(chess_engine_cpp ${source_files})