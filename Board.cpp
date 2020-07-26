//
// Created by ondra on 7/26/2020.
//

#include "Board.h"

string Board::BitboardRepresentation(bitboard board) {
    for (int i = 0; i < 64; ++i) {
        cout << (board & 1);
        if (i % 8 == 7) {
            cout << endl;
        }
        board >>= 1;
    }
}