//
// Created by ondra on 7/26/2020.
//

#ifndef CHESS_ENGINE_CPP_BOARD_H
#define CHESS_ENGINE_CPP_BOARD_H
typedef unsigned long long bitboard;

#include <iostream>
#include <string>

using namespace std;

class Board {
    /* The white piece positions */
public:
    bitboard PrecomputedKnights[64];


    // bitboards
    //white piece bitboards
    bitboard WhitePawns;
    bitboard WhiteRooks;
    bitboard WhiteKnights;
    bitboard WhiteBishops;
    bitboard WhiteQueens;
    bitboard WhiteKing;

    //white piece bitboards
    bitboard BlackPawns;
    bitboard BlackRooks;
    bitboard BlackKnights;
    bitboard BlackBishops;
    bitboard BlackQueens;
    bitboard BlackKing;

    // compound positions
    bitboard AllWhitePieces;
    bitboard AllBlackPieces;
    bitboard AllPieces;

public:
    void BitboardRepresentation(bitboard board);
};


// enum FILE {
//     ROW_A,
//     ROW_B,
//     ROW_C,
//     ROW_D,
//     ROW_F,
//     ROW_E,
//     ROW_G,
//     ROW_H,
//     COl_1,
//     COl_2,
//     COl_3,
//     COl_4,
//     COl_5,
//     COl_6,
//     COl_7,
//     COl_8,
// };
//Předpočíta pozice kam může daná figurka útočit
//pozice se spočítají při inicializaci objektu
//Příklad použití:
//    PrecomputedBitboards precomp_bitboards = PrecomputedBitboards();
//
//    // vrátí bitmapu s možnýmy pozicemi na táhnutí,
//    tahy nemusí být validní podle šachových pravidel. Pozice se musí ještě ověřit.
//    precomp_bitboards.precomputed_knights[knight_position];
//
class PrecomputedBitboards {
public:
    bitboard precomputed_knights[64];
    bitboard precomputed_kings[64];

    // maska na řádek
    bitboard col_row_mask[16];

    // precomputes all bitboards and masks
    void PrecomputeAll();
    // vytvoří bitmapy pro mazání určitých řádků či sloupců
    void PrecomputeKnights();
    void FillRowcolMask();
    void PrecomputeKings();
    void PrecomputeRooks();
    void PrecomputePawns();
};

#endif  // CHESS_ENGINE_CPP_BOARD_H
