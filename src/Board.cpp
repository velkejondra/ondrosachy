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

void PrecomputedBitboards::PrecomputeKnights() {
    for (int i = 0; i < 64; ++i) {
        //TODO zjednodusit

        // up big right
        if (i % 8 < 6 and i < 56) {
            this->precomputed_knights[i] ^= (1ULL << i) << 10;
        }
        // up big left
        if (i % 8 > 1 and i < 56) {
            this->precomputed_knights[i] ^= (1ULL << i) << 6;
        }
        // down big left
        if (i % 8 > 1 and i > 7) {
            this->precomputed_knights[i] ^= (1ULL << i) >> 10;
        }
        // down big right
        if (i % 8 < 6 and i > 7) {
            this->precomputed_knights[i] ^= (1ULL << i) >> 6;
        }

        // up small right
        if (i % 8 <= 6 and i < 48) {
            this->precomputed_knights[i] ^= (1ULL << i) << 17;
        }
        // up small left
        if (i % 8 >= 1 and i < 48) {
            this->precomputed_knights[i] ^= (1ULL << i) << 15;
        }
        // down small left
        if (i % 8 >= 1 and i > 15) {
            this->precomputed_knights[i] ^= (1ULL << i) >> 17;
        }
        // down small right
        if (i % 8 <= 6 and i > 15) {
            this->precomputed_knights[i] ^= (1ULL << i) >> 15;
        }
    }
}

void PrecomputedBitboards::PrecomputeAll() {
    PrecomputeKnights();
    // PrecomputeKings();
    // FillRowcolMask();
}

void PrecomputedBitboards::PrecomputeKings() {
    for (int i = 0; i < 64; ++i) {
        //TODO pridat pocitani tahu pro krale
    }
}
// void PrecomputedBitboards::FillRowcolMask() {
//     for(int i = 0; i < 16; ++i){
//         if (i < 8){
//             for (int j = 0; j < 8; ++j){
//                 this->col_row_mask[i] |= (1 << j) << (8*i);
//             }
//         }
//     }
// }


