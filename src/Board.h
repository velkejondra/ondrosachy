//
// Created by ondra on 7/26/2020.
//

#ifndef CHESS_ENGINE_CPP_BOARD_H
#define CHESS_ENGINE_CPP_BOARD_H
typedef unsigned long long bitboard;

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
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
    bool operator==(const Move &rhs) const;

    bool operator!=(const Move &rhs) const;

    int from = 0;
    int to = 0;
    int piece_type = 0;
    int capture_piece_type = 0;
    bool capture = false;
    int promotion = 0;
    bool castling = false;
    bool en_passant = false;

};

class Board {
public:
    bitboard all_bitboards[2][6]{};

    // spojene pozice

    bitboard board_zobrist[2][6][64]{};
    bitboard side_to_move_zobrist{};
    bitboard en_passant_zobrist[8]{};

public:

    Board();

    bool on_turn = WHITE;

    void MakeMove(Move move);

    bool InCheck(int color);

    void MakeMoveFromPGN(std::string move);

    void InitZobrist();

    void ComputeZobristHash();

    void AdjustZobrist(Move m);

    int getPieceAt(int position);

    Move last_move{};
    bitboard zobrist_hash{};
    int en_passant = -1;
    int last_en_passant = -1;

    bitboard MyPieces();

    bitboard EnemyPieces();

    bitboard GetAllBitboardAttacks(bool color);

    std::string GetMoveNotation() const;

    bitboard _all_pieces = 0ULL;
    bitboard _my_pieces = 0ULL;
    bitboard _enemy_pieces = 0ULL;
private:

    void CalculateAll();

    void CalculateWhitePieces();

    void CalculateBlackPieces();


    void AddPiece(int piece, int pos, bool color);

    bitboard white_pieces;
    bitboard black_pieces;

    void RemovePiece(int piece, int pos, bool color);
};


#endif  // CHESS_ENGINE_CPP_BOARD_H
