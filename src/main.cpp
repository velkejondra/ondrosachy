#include <iostream>
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

    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    cout << BitboardRepresentation(precomp.col_row_mask[CHESS_FILE::ROW_A]);
    Board board = Board();
    Search search = Search();
    Move mov = search.IterativeDeepening(3, board);


    return 0;
}