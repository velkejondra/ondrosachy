#include <iostream>
#include "PgnParser.h"
#include "PrecomputedBitboards.h"
#include "defs.h"
#include "Board.h"
#include "eval.h"
#include "MoveGenerator.h"
//#include "debug.h"
using namespace std;

int main() {

    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    cout << BitboardRepresentation(precomp.col_row_mask[CHESS_FILE::ROW_A]);
    Board board = Board();
    board.MakeMove(Move{e2, e4, PAWN});
    board.MakeMove(Move{d7, d5, PAWN});
    board.MakeMove(Move{d2, d4, PAWN});
    MoveGenerator gen = MoveGenerator();
    vector<Move> moves = gen.getLegalMoves(board);
    for (auto move : moves) {
        if (move.capture) {
            bool test = true;
        }
    }

    return 0;
}