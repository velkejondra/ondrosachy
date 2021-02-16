cmake_minimum_required(VERSION 3.13.4)
project(chess_engine_cpp_exe)

set(CMAKE_CXX_STANDARD 14)

# add_executable(main.cpp Board.h Board.cpp)
set(source_files Board.h Board.cpp PgnParser.cpp PgnParser.h PrecomputedBitboards.h PrecomputedBitboards.cpp debug.cpp debug.h eval.h eval.cpp MoveGenerator.cpp MoveGenerator.h TranspositionTable.cpp TranspositionTable.h Search.cpp Search.h)

add_executable(chess_engine_cpp main.cpp ${source_files})

add_executable(print_dot_graph ../tests/generate_dot_graph.cpp ${source_files})

add_executable(chess_tests ../tests/peft.cpp ../tests/catch.cpp ../tests/catch.hpp ${source_files})

add_executable(gui_chess gui/main.cpp gui/olcPixelGameEngine.h ${source_files} gui/defs.h)
target_link_libraries(gui_chess ${CMAKE_THREAD_LIBS_INIT} X11 Xext png z m glut GL pthread GLU stdc++fs)


add_subdirectory(BitboardMaker)
