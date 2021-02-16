//
// Created by ondra on 7/26/2020.
//

#ifndef CHESS_ENGINE_CPP_BOARD_H
#define CHESS_ENGINE_CPP_BOARD_H
typedef unsigned long long bitboard;

#include <iostream>
#include <string>
#include <vector>
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
    // bitboards
    //bitmapy bilych kusu

    bitboard all_bitboards[2][6]{};


    // spojene pozice
    inline bitboard AllWhitePieces();

    inline bitboard AllBlackPieces();

    bitboard PiecesOfColor(bool color);

    bitboard AllPieces();

    bitboard board_zobrist[2][6][64]{};
    bitboard castling_zobrist[4]{};
    bitboard side_to_move_zobrist{};
    bitboard en_passant_zobrist[8]{};

public:

    Board();

    bool on_turn = WHITE;

    void MakeMove(Move move);

    bool InCheck(int color);

    void UndoMove(Move move);

    void MakeMove(bool small_castle);


    void MakeMoveFromPGN(std::string move);

    void InitZobrist();

    void ComputeZobristHash();

    void AdjustZobrist(Move m);

    int getPieceAt(int position);

    std::string last_destination;
    Move last_move{};
    std::vector<Move> last_moves;
    bitboard zobrist_hash{};
    int en_passant = -1;
    int last_en_passant = -1;

    std::vector<Move> getLegalMoves();

    std::vector<Move> getPseudoLegalMoves();

    bitboard MyPieces();

    bitboard EnemyPieces();

    bitboard GetAllBitboardAttacks(bool color);

    std::string GetMoveNotation();
};


#endif  // CHESS_ENGINE_CPP_BOARD_H
