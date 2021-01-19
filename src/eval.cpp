//
// Created by velkejondra on 12/28/20.
//

#include "Board.h"
#include "eval.h"

int Eval::evaluate(Board &board) {
    int score = 0;
    // pricteni bodu za hodnotu figurek
    for (int i = 0; i < 5; ++i) {
        score += __builtin_popcountll(board.all_bitboards[WHITE][i]) -
                 __builtin_popcountll(board.all_bitboards[BLACK][i]);
    }
    return score;
}
