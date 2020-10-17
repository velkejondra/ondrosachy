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
public:
    // bitboards
    //bitmapy bilych kusu
    bitboard WhitePawns;
    bitboard WhiteRooks;
    bitboard WhiteKnights;
    bitboard WhiteBishops;
    bitboard WhiteQueens;
    bitboard WhiteKing;

    //bitmapy cernych kusu
    bitboard BlackPawns;
    bitboard BlackRooks;
    bitboard BlackKnights;
    bitboard BlackBishops;
    bitboard BlackQueens;
    bitboard BlackKing;

    // spojene pozice
    bitboard AllWhitePieces;
    bitboard AllBlackPieces;
    bitboard AllPieces;

public:
    void BitboardRepresentation(bitboard board);
};


enum CHESS_FILE {
    COl_1,
    COl_2,
    COl_3,
    COl_4,
    COl_5,
    COl_6,
    COl_7,
    COl_8,
    ROW_A,
    ROW_B,
    ROW_C,
    ROW_D,
    ROW_F,
    ROW_E,
    ROW_G,
    ROW_H,
};

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

    // maska na řádek či sloupec
    bitboard col_row_mask[16];

    // predpočítá všechny potřebné bitmapy na počítání možných pozic
    void PrecomputeAll();

    // vytvoří bitmapy pro mazání určitých řádků či sloupců
    void PrecomputeKnights();

    // vyplni masku
    void FillRowcolMask();

    void PrecomputeKings();

    void PrecomputeRooks();

    void PrecomputePawns();
};

#endif  // CHESS_ENGINE_CPP_BOARD_H
