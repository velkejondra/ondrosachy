#include <iostream>

#include "Board.h"
int main() {
    bitboard a = 255;
    Board b = Board();
    b.PrecomputeAll();
    b.BitboardRepresentation(b.PrecomputedKnights[20]);
    return 0;
}
