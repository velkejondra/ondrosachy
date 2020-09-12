#include <iostream>

#include "Board.h"
int main() {
    PrecomputedBitboards b = PrecomputedBitboards();
    b.PrecomputeAll();
    Board a = Board();
    a.BitboardRepresentation(b.precomputed_knights[20]);
    return 0;
}