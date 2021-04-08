//
// Created by velkejondra on 1/26/21.
//

#ifndef CHESS_ENGINE_CPP_EXE_SEARCH_H
#define CHESS_ENGINE_CPP_EXE_SEARCH_H


#include "Board.h"
#include "TranspositionTable.h"

class Search {
public:
    Move IterativeDeepening(int depth, Board &board);

    Move SearchFromRoot(int depth, Board &board);

    TranspositionTable table{};

private:
    int NegaMax(int depth, Board &board, int alpha, int beta);
};


#endif //CHESS_ENGINE_CPP_EXE_SEARCH_H
