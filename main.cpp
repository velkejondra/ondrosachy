#include <iostream>
typedef unsigned long long bitboard;

#include "Board.h"
int main() {
    Board board = Board();
    bitboard test = 511;
    board.BitboardRepresentation(test);
    return 0;
}
