#include <iostream>
#include <experimental/filesystem>
#include "PgnParser.h"
#include "PrecomputedBitboards.h"
#include "defs.h"
#include "Board.h"
#include "eval.h"
#include "Search.h"
#include "MoveGenerator.h"

//#include "debug.h"
using namespace std;

int main() {
    PgnParser parser = PgnParser();
    parser.ParsePgnToFile("/home/velkejondra/repos/chess_gitlab/src/millionbase-2.5.pgn", 14, 1000000);
    OpeningBook book = OpeningBook("/home/velkejondra/repos/chess_gitlab/src/openingbook.index",
                                   "/home/velkejondra/repos/chess_gitlab/src/openingbook.data");

    Board board = Board();
    auto lol = book.GetPGNMove(board.zobrist_hash);
}