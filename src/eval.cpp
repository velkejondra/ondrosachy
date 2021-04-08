//
// Created by velkejondra on 12/28/20.
//

#include "Board.h"
#include "eval.h"

int Eval::evaluate(Board &board) {
    int score = 0;
    // pricteni bodu za hodnotu figurek
    int side = board.on_turn ? -1 : 1;
    for (int i = 0; i < 5; ++i) {
        int white = __builtin_popcountll(board.all_bitboards[WHITE][i]);
        int black = __builtin_popcountll(board.all_bitboards[BLACK][i]);
        score += (white - black) * opening_piece_values[i] * side;
    }
    auto &piece_table = board.on_turn ? white_piece_place_table : black_piece_place_table;
    for (int i = 0; i < 6; ++i) {
        bitboard piece_bitboard = board.all_bitboards[board.on_turn][i];

        while (piece_bitboard) {
            int piece = __builtin_ffsll(piece_bitboard);
            piece--;
            piece_bitboard ^= 1ULL << piece;
            score += piece_table[i][piece];
        }
    }
    return score;
}
