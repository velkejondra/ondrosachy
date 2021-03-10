//
// Created by velkejondra on 1/26/21.
//

#include "Search.h"
#include "eval.h"
#include "MoveGenerator.h"
#include <climits>

int Search::NegaMax(int depth, Board &board, int alpha = INT_MIN, int beta = INT_MAX) {
    MoveGenerator gen = MoveGenerator();
    auto moves = gen.getLegalMoves(board);
    if (moves.empty()) {
        return INT_MIN;
    }
    if (depth == 0) {
        return Eval::evaluate(board);
    }
    int val = INT_MIN;
    for (auto move : moves) {
        Board nextBoard = board;
        nextBoard.MakeMove(move);
        val = std::max(val, -NegaMax(depth - 1, nextBoard, -beta, -alpha));
        alpha = std::max(alpha, val);
        if (alpha >= beta) {
            break;
        }
    }
    return val;
}

Move Search::SearchFromRoot(int depth, Board &board) {
    int max_val = INT_MIN;
    MoveGenerator gen = MoveGenerator();
    int val;
    Move best_move;
    for (auto move : gen.getLegalMoves(board)) {
        Board nextBoard = board;
        nextBoard.MakeMove(move);
        val = -NegaMax(depth, nextBoard);
        if (val > max_val) {
            max_val = val;
            best_move = move;
        }

    }
    return best_move;
}

Move Search::IterativeDeepening(int depth, Board &board) {
    Move bestMove;
    for (int i = 0; i < depth; i++) {
        bestMove = SearchFromRoot(i, board);
    }
    return bestMove;
}
