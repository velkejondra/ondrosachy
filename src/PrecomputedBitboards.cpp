//
// Created by ondra on 10/27/2020.
//

#include "PrecomputedBitboards.h"
#include "debug.h"

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
    PrecomputeBishopRays();
    PrecomputeBiships();
    PrecomputeRookRays();
    PrecomputeRooks();
}

void PrecomputedBitboards::PrecomputeKings() {
    for (int i = 0; i < 64; ++i) {
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
    int lsbIndex = __builtin_ffsll(board) - 1;
    board &= board - 1;
    return lsbIndex;
}

inline int _popCount(bitboard board) {
    return __builtin_popcountll(board);
}

bitboard GetBlockersFromIndex(int index, bitboard mask) {
    bitboard blockers = 0ULL;
    int bits = _popCount(mask);
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
            bitboard blockers = GetBlockersFromIndex(j, this->rook_rays[i]);
            this->precomputed_rooks[i][(rooks_magic[i] * blockers) >> (64 - rook_index_bits[i])] = ComputeRook(
                    this->rook_rays[i], blockers, i);
        }
    }
}

void PrecomputedBitboards::PrecomputeRookRays() {
    for (int i = 0; i < 64; ++i) {
        this->rook_rays[i] |= 0x1010101010100ULL << (i % 8);
        this->rook_rays[i] |= 0x7eULL << (i / 8 * 8);
        this->rook_rays[i] &= ~(1ULL << i);
    }
}


void PrecomputedBitboards::PrecomputeBishopRays() {
    for (int i = 0; i < 64; ++i) {


        int directions[] = {9, 7, -7, -9};
        for (int dir : directions) {
            this->bishop_rays[i] |= slow_bishop_ray(i, dir);

        }

    }
}

void PrecomputedBitboards::PrecomputeBiships() {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 1ULL << bishop_index_bits[i]; ++j) {
            bitboard blockers = GetBlockersFromIndex(j, this->bishop_rays[i]);
            this->precomputed_bishops[i][(bishop_magics[i] * blockers) >> (64 - bishop_index_bits[i])] = ComputeBishop(
                    this->bishop_rays[i], blockers, i);
        }
    }

}

bitboard PrecomputedBitboards::slow_bishop_ray(int pos, int direction) {
    bitboard ray = 0ULL;
    int last = pos % 8;
    pos += direction;
    while (pos >= 0 && pos < 64 && abs(last - pos % 8) < 2) {
        ray |= 1ULL << pos;
        last = pos % 8;
        pos += direction;
    }
    // odstraneni okraju
    return ray & ~0xff818181818181ffULL;


}

void CheckRookRay(bitboard &attacks, bitboard &blockers, int pos, int nahoru, int max, int min) {
    bool blocked = false;
    pos += nahoru;
    while (pos <= max and pos >= min) {
        if (blocked) {
            attacks &= ~(1ULL << (pos));
        }
        else {
            attacks |= 1ULL << (pos);
        }
        if ((blockers >> (pos)) & 1ULL) {
            blocked = true;
        }
        pos += nahoru;
    }
}

void CheckBishopRay(bitboard &attacks, bitboard &blockers, int pos, int nahoru, int max, int min) {
    bool blocked = false;
    int last = pos % 8;
    pos += nahoru;
    while (pos >= 0 && pos < 64 && abs(last - pos % 8) < 2) {
        if (blocked) {
            attacks &= ~(1ULL << (pos));
        }
        else {
            attacks |= 1ULL << (pos);
        }
        if ((blockers >> (pos)) & 1ULL) {
            blocked = true;
        }
        last = pos % 8;
        pos += nahoru;
    }
}

bitboard ComputeRook(bitboard attacks, bitboard blockers, int pos) {
    //nahoru
    CheckRookRay(attacks, blockers, pos, 8, 63, 0);
    //dolu
    CheckRookRay(attacks, blockers, pos, -8, 63, 0);
    //doleva
    CheckRookRay(attacks, blockers, pos, -1, pos / 8 * 8 + 7, pos / 8 * 8);
    //doprava
    CheckRookRay(attacks, blockers, pos, 1, pos / 8 * 8 + 7, pos / 8 * 8);

    return attacks;
}

bitboard ComputeBishop(bitboard attacks, bitboard blockers, int pos) {
    // pravo nahoru
    CheckBishopRay(attacks, blockers, pos, 9, 63, 0);
    // vlevo dolu
    CheckBishopRay(attacks, blockers, pos, 7, 63, 0);
    // vlevo nahoru
    CheckBishopRay(attacks, blockers, pos, -9, pos / 8 * 8 + 7, pos / 8 * 8);
    // vpravo dolu
    CheckBishopRay(attacks, blockers, pos, -7, pos / 8 * 8 + 7, pos / 8 * 8);

    return attacks;

}
