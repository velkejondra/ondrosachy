cmake_minimum_required(VERSION 3.13.4)
project(chess_engine_cpp_exe)

set(CMAKE_CXX_STANDARD 14)

# add_executable(main.cpp Board.h Board.cpp)
set(source_files main.cpp Board.h Board.cpp PgnParser.cpp PgnParser.h PrecomputedBitboards.h PrecomputedBitboards.cpp debug.cpp debug.h)
add_executable(chess_engine_cpp ${source_files})
add_subdirectory(BitboardMaker)