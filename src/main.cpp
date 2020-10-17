#include <iostream>
#include "Board.h"
int main() {
    PrecomputedBitboards b = PrecomputedBitboards();
    b.PrecomputeAll();
    Board a = Board();
    a.BitboardRepresentation(b.col_row_mask[CHESS_FILE::COl_2]);
    return 0;
}