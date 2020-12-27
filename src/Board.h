//
// Created by ondra on 7/26/2020.
//

#ifndef CHESS_ENGINE_CPP_BOARD_H
#define CHESS_ENGINE_CPP_BOARD_H
typedef unsigned long long bitboard;

#include <iostream>
#include <string>
#include "PrecomputedBitboards.h"

enum PIECE {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
};
enum COLOR {
    WHITE,
    BLACK,
};

struct Move {
    int from;
    int to;
    int piece_type;
    int capture_piece_type;
    bool capture;
    int promotion;
    bool castling;
    bool en_passant;
};

class Board {
public:
    // bitboards
    //bitmapy bilych kusu

    bitboard all_bitboards[2][6]{};


    // spojene pozice
    inline bitboard AllWhitePieces();

    inline bitboard AllBlackPieces();

    bitboard AllPieces();

    bitboard board_zobrist[2][6][64]{};
    bitboard castling_zobrist[4]{};
    bitboard side_to_move_zobrist{};
    bitboard en_passant_zobrist[8]{};

public:

    Board();

    int on_turn = WHITE;

    void MakeMove(Move move);

    void MakeMove(bool small_castle);


    void MakeMoveFromPGN(std::string move);

    void InitZobrist();

    void ComputeZobristHash();

    void AdjustZobrist(Move m);

    std::string last_destination;
    Move last_move{};
    bitboard zobrist_hash{};
    int en_passant{};
};


#endif  // CHESS_ENGINE_CPP_BOARD_H
