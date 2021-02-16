//
// Created by velkejondra on 12/29/20.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "../src/Board.h"
#include "../src/MoveGenerator.h"
#include "../src/debug.h"
#include "../src/defs.h"

using namespace std;

unsigned long long perft(int depth, Board &board) {
    MoveGenerator gen = MoveGenerator();
    if (depth == 0) {
        return 1;
    }
    if (depth == 1) {
        return gen.getLegalMoves(board).size();
    }
    unsigned long long nodes = 0;
    for (auto move : gen.getLegalMoves(board)) {
        Board next_board = board;
        next_board.MakeMove(move);
        nodes += perft(depth - 1, next_board);
    }

    return nodes;
};

void perft_divide(int depht) {
    Board board = Board();
    MoveGenerator gen = MoveGenerator();

    for (auto move : gen.getLegalMoves(board)) {
        Board next_board = board;
        next_board.MakeMove(move);
        if (next_board.GetMoveNotation() == "a5b6") {
            bool test = 0;
        }
        int result = perft(depht - 1, next_board);
        cout << positions[move.from] << positions[move.to] << ": " << result << "\n" << flush;
    }
}

static int INDEX = 0;


TEST_CASE("Perft test", "[perft]") {
    Board board = Board();
    REQUIRE(perft(1, board) == 20);
    REQUIRE(perft(2, board) == 400);
    REQUIRE(perft(3, board) == 8902);
    REQUIRE(perft(4, board) == 197281);
//    REQUIRE(perft(5, board) == 4865609);
//    REQUIRE(perft(6, board) == 119060324);
//    perft_divide(2);
}