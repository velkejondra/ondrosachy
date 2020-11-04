//
// Created by ondra on 10/27/2020.
//

#include <climits>
#include "PrecomputedBitboards.h"

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
    FillRowcolMask();
    PrecomputeKnights();
    PrecomputeKings();
    PrecomputeBishipRays();
    PrecomputeRookRays();
    PrecomputeRooks();
}

void PrecomputedBitboards::PrecomputeKings() {
    for (int i = 0; i < 64; ++i) {
        if (i == 7){
            bool test = truehoulcou
        }
        this->precomputed_kings[i] ^= (1ULL << i) << 7 & ~this->col_row_mask[CHESS_FILE::COl_8];
        this->precomputed_kings[i] ^= (1ULL << i) << 8;
        this->precomputed_kings[i] ^= (1ULL << i) << 9 & ~this->col_row_mask[CHESS_FILE::COl_1];
        this->precomputed_kings[i] ^= (1ULL << i) << 1 & ~this->col_row_mask[CHESS_FILE::COl_1];
        this->precomputed_kings[i] ^= (1ULL << i) >> 7 & ~this->col_row_mask[CHESS_FILE::COl_1];
        this->precomputed_kings[i] ^= (1ULL << i) >> 8;
        this->precomputed_kings[i] ^= (1ULL << i) >> 9 & ~this->col_row_mask[CHESS_FILE::COl_8];
        this->precomputed_kings[i] ^= (1ULL << i) >> 1 & ~this->col_row_mask[CHESS_FILE::COl_8];
    }
}

void PrecomputedBitboards::FillRowcolMask() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            this->col_row_mask[i] |= (1ULL << i) << (8 * j);
        }
    }
    for (int i = 8; i < 16; ++i) {
        this->col_row_mask[i] = 255ULL << ((i - 8) * 8);
    }
}

inline int _popLsb(bitboard &board) {
    //TODO compiler independent
    int lsbIndex = __builtin_ffsll(board) - 1;
    board &= board - 1;
    return lsbIndex;
}

bitboard GetBlockersFromIndex(int index, bitboard mask) {
    bitboard blockers = 0ULL;
    int bits = __builtin_popcountll(mask); //TODO compiler independent
    for (int i = 0; i < bits; i++) {
        int bitPos = _popLsb(mask);
        if (index & (1 << i)) {
            blockers |= (1ULL << bitPos);
        }
    }
    return blockers;
}


void PrecomputedBitboards::PrecomputeRooks() {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 1ULL << rook_index_bits[i]; ++j) {
            if (j==1036){
                bool test = true;
            }
            bitboard blockers = GetBlockersFromIndex(j, this->rook_rays[i]);
            this->precomputed_rooks[i][(rooks_magic[i] * blockers) >> (64 - rook_index_bits[i])] = ComputeRook(this->rook_rays[i], blockers, i);
        }
    }
}

void PrecomputedBitboards::PrecomputeRookRays() {
    for (int i = 0; i < 64; ++i) {
        this->rook_rays[i] |= 0x0101010101010101ULL << (i % 8);
        this->rook_rays[i] |= 0xFFULL << (i / 8 * 8);
    }
}

void PrecomputedBitboards::PrecomputePawns() {

}

void PrecomputedBitboards::PrecomputeBishipRays() {
    for (int i = 0; i < 64; ++i) {
        int row = i / 8;
        int col = i % 8;
        if (row > col) {
            bitboard top_mask = ULLONG_MAX;
            for (int j = 0; j < row - col; ++j) {
                top_mask ^= this->col_row_mask[8 + j];
            }
            this->biship_rays[i] |= (0x8040201008040201ULL >> (row - col)) & top_mask;
        } else {
            bitboard mask = ULLONG_MAX;
            for (int j = 0; j < col - row; ++j) {
                mask ^= this->col_row_mask[15 - j];
            }
            this->biship_rays[i] |= (0x8040201008040201ULL << (col - row)) & mask;
        }
        if (row > 7 - col) {
            bitboard side_mask = ULLONG_MAX;
            for (int i = 0; i < (row - (7 - col)); ++i) {
                side_mask ^= this->col_row_mask[i];
            }
            this->biship_rays[i] |= (0x102040810204080ULL >> (row - (7 - col))) & side_mask;
        } else {
            bitboard side_mask = ULLONG_MAX;
            for (int j = 0; j < ((7 - col) - row); ++j) {
                side_mask ^= this->col_row_mask[7 - j];
            }
            this->biship_rays[i] |= (0x102040810204080ULL >> ((7 - col) - row)) & side_mask;
        }
    }
}

void CheckRookRay(bitboard &attacks, bitboard &blockers, int pos, int nahoru, int max, int min) {
    bool blocked = false;
    while (pos <= max and pos >= min) {
        pos += nahoru;
        if (blocked) {
            attacks ^= 1ULL << (pos);
        }
        if ((blockers >> (pos)) & 1ULL) {
            blocked = true;
        }
    }
}

bitboard ComputeRook(bitboard attacks, bitboard blockers, int pos) {
    //nahoru
    CheckRookRay(attacks, blockers, pos, 8, 63, 0);
    //dolu
    CheckRookRay(attacks, blockers, pos, -8, 63, 0);
    //doleva
    CheckRookRay(attacks, blockers, pos, -1, pos/8 * 8 + 6, pos / 8 * 8);
    //doprava
    CheckRookRay(attacks, blockers, pos, 1, pos/8 * 8 + 6, pos / 8 * 8);

    return attacks;
}

