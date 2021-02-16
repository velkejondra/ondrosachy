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
    return score;
}
