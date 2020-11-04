//
// Created by ondra on 7/26/2020.
//

#ifndef CHESS_ENGINE_CPP_BOARD_H
#define CHESS_ENGINE_CPP_BOARD_H
typedef unsigned long long bitboard;

#include <iostream>
#include <string>
#include "PrecomputedBitboards.h"


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

    // predpocitane utecne pozice pro figurky
    PrecomputedBitboards *precomputed_bitboards;

public:
    Board(PrecomputedBitboards *precomputed_bitboards){
        this->precomputed_bitboards = precomputed_bitboards;
    }
};


#endif  // CHESS_ENGINE_CPP_BOARD_H
