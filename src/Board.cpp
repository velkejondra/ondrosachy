//
// Created by ondra on 7/26/2020.
//

#include "Board.h"

void Board::BitboardRepresentation(bitboard board) {
    cout << board << endl;
    for (int i = 0; i < 64; ++i) {
        cout << (board & 1);
        if (i % 8 == 7) {
            cout << endl;
        }
        board >>= 1;
    }
}

void Board::PrecomputeKnights() {
    for (int i = 0; i < 64; ++i) {
        if (i % 8 < 7 and i < 55) {
            cout << ((1 << i) << 10) << endl;
            this->PrecomputedKnights[i] ^= (1 << i) << 10;
        }
    }
}

void Board::PrecomputeAll() { PrecomputeKnights(); }
