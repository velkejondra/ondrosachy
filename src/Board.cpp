//
// Created by ondra on 7/26/2020.
//

#include "Board.h"

void Board::BitboardRepresentation(bitboard board) {
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
        // up big right
        if (i % 8 < 6 and i < 56) {
            this->PrecomputedKnights[i] ^= (1ULL << i) << 10;
        }
        // up big lef
        if (i % 8 > 1 and i < 56) {
            this->PrecomputedKnights[i] ^= (1ULL << i) << 6;
        }
        // down big left
        if (i % 8 > 1 and i > 7) {
            this->PrecomputedKnights[i] ^= (1ULL << i) >> 10;
        }
        // down big right
        if (i % 8 < 6 and i > 7) {
            this->PrecomputedKnights[i] ^= (1ULL << i) >> 6;
        }

        // up small right
        if (i % 8 <= 6 and i < 48) {
            this->PrecomputedKnights[i] ^= (1ULL << i) << 17;
        }
        // up small left
        if (i % 8 >= 1 and i < 48) {
            this->PrecomputedKnights[i] ^= (1ULL << i) << 15;
        }
        // down small left
        if (i % 8 >= 1 and i > 15) {
            this->PrecomputedKnights[i] ^= (1ULL << i) >> 17;
        }
        // down small right
        if (i % 8 <= 6 and i > 15) {
            this->PrecomputedKnights[i] ^= (1ULL << i) >> 15;
        }
    }
}

void Board::PrecomputeAll() { PrecomputeKnights(); }
