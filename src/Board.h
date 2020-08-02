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
    void PrecomputeKnights();
    void PrecomputeAll();

    bitboard WhitePawns;
    bitboard WhiteRooks;
    bitboard WhiteKnights;
    bitboard WhiteBishops;
    bitboard WhiteQueens;
    bitboard WhiteKing;

    /* The black piece positions */
    bitboard BlackPawns;
    bitboard BlackRooks;
    bitboard BlackKnights;
    bitboard BlackBishops;
    bitboard BlackQueens;
    bitboard BlackKing;

    /* Commonly derived positions */
    bitboard AllWhitePieces;
    bitboard AllBlackPieces;
    bitboard AllPieces;

   public:
    void BitboardRepresentation(bitboard board);
};

#endif  // CHESS_ENGINE_CPP_BOARD_H
