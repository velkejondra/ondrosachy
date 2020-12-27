#include <iostream>
#include "PgnParser.h"
#include "PrecomputedBitboards.h"
#include "defs.h"
#include "Board.h"
//#include "debug.h"
using namespace std;

int main() {
    Board board = Board();
    cout << BitboardRepresentation(board.AllPieces()) << endl;
    Move m = Move{e2, e4, PAWN};
    Move n = Move{d7, d5, PAWN};
    board.MakeMove(m);
    board.MakeMove(n);
    Move c = Move{e4, d5, PAWN, PAWN, true};
    board.MakeMove(c);
    cout << BitboardRepresentation(board.AllPieces()) << endl;
    return 0;
}