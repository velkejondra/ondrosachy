//
// Created by velkejondra on 1/26/21.
//

#include "Search.h"
#include "eval.h"
#include "MoveGenerator.h"
#include <climits>

int Search::NegaMax(int depth, Board &board, int alpha = INT_MIN + 1, int beta = INT_MAX) {
    MoveGenerator gen = MoveGenerator();
    auto moves = gen.getLegalMoves(board);
    if (moves.empty() && board.InCheck(!board.on_turn)) {
        // pricitam jednicku protoze kdyz jsem negoval nejmensi cislo tak jsem dostal integer overflow
        return INT_MIN + 1;
    }
    else if (moves.empty()) {
        return 0;
    }
    if (depth == 0) {
        return Eval::evaluate(board);
    }
    int val = INT_MIN + 1;
    Move best_move;
    for (auto move : moves) {
        Board nextBoard = board;
        nextBoard.MakeMove(move);
        int last_val = val;
        val = std::max(val, -NegaMax(depth - 1, nextBoard, -beta, -alpha));
        if (last_val != val) {
            best_move = move;
        }
        alpha = std::max(alpha, val);
        if (alpha >= beta) {
            break;
        }
    }
    table.InsertEntry(board.zobrist_hash, TableEntry{depth, best_move});
    return val;
}

Move Search::SearchFromRoot(int depth, Board &board) {
    int max_val = INT_MIN;
    MoveGenerator gen = MoveGenerator();
    int val;
    Move best_move;
    TableEntry *entry = table.GetEntry(board.zobrist_hash);
    Move hash_move;
    if (entry != nullptr) {
        hash_move = entry->best_move;
        Board nextBoard = board;
        nextBoard.MakeMove(hash_move);
        val = -NegaMax(depth, nextBoard);
        max_val = val;
        best_move = hash_move;
        if (max_val == INT_MAX) {
            return best_move;
        }
    }
    for (Move move : gen.getLegalMoves(board)) {
        if (move == hash_move) {
            continue;
        }
        Board nextBoard = board;
        nextBoard.MakeMove(move);
        val = -NegaMax(depth, nextBoard);
        if (val > max_val) {
            max_val = val;
            best_move = move;
            if (max_val == INT_MAX) {
                return best_move;
            }
        }
    }
    return best_move;
}

Move Search::IterativeDeepening(int depth, Board &board) {
    Move bestMove{};
    for (int i = 0; i < depth; i++) {
        bestMove = SearchFromRoot(i, board);
        if (table.size() > 1000000) {
            table.clear();
        }
            table.InsertEntry(board.zobrist_hash, TableEntry{i, bestMove});
        }

    return bestMove;
}
