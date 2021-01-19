//
// Created by velkejondra on 12/28/20.
//

#ifndef CHESS_ENGINE_CPP_EVAL_H
#define CHESS_ENGINE_CPP_EVAL_H

const int opening_piece_values[] = {
        [PAWN] = 100,
        [KNIGHT] = 325,
        [BISHOP] = 350,
        [ROOK] = 500,
        [QUEEN] = 900,
        [KING] = 0,
};

class Eval {
public:
    static int evaluate(Board &board);
};


#endif //CHESS_ENGINE_CPP_EVAL_H
