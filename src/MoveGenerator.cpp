//
// Created by velkejondra on 12/31/20.
//

#include "MoveGenerator.h"
#include "PrecomputedBitboards.h"
#include <algorithm>

using namespace std;


void MoveGenerator::getKnightAttacks(vector<Move> &moves, Board &board) {
    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    bitboard enemy_pieces = board.EnemyPieces();
    bitboard my_pieces = board.MyPieces();
    bitboard my_knights = board.all_bitboards[board.on_turn][KNIGHT];
    int from;
    int to;
    while (my_knights) {
        // nacteni pozice dalsiho kone
        from = __builtin_ffsll(my_knights);
        from--;
        // smazani nacteneho kone
        my_knights ^= 1ULL << (from);

        // utoky ze soucasneho kone
        bitboard attacks = precomp.precomputed_knights[from];
        attacks &= ~my_pieces;
        while (attacks) {
            //nacteni pozice uteceni
            to = __builtin_ffsll(attacks);
            to--;

            // smazani nactene pozice
            attacks ^= 1ULL << (to);
            if (enemy_pieces >> to & 1ULL) {
                // pohyb sebere figurku
                moves.push_back(Move{from, to, KNIGHT, board.getPieceAt(to), true});

            }
            else {
                // pohyb nesebere figurku
                moves.push_back(Move{from, to, KNIGHT});
            }
        }
    }
}

void MoveGenerator::getPseudoLegalMoves(std::vector<Move> &moves, Board &board) {
    moves.reserve(200);
    getPawnAttacks(moves, board);
    getKnightAttacks(moves, board);
    getBishopAttacks(moves, board);
    getRookAttacks(moves, board);
    getQueenAttacks(moves, board);
    getKingAttacks(moves, board);
}

bitboard shift_leftos(bitboard num, int shift_size) {
    return num << shift_size;
}

bitboard shift_rightos(bitboard num, int shift_size) {
    return num >> shift_size;
}

inline int add(int a, int b) {
    return a + b;
}

inline int sub(int a, int b) {
    return a - b;
}

void MoveGenerator::getPawnAttacks(vector<Move> &moves, Board &board) {
    // podle toho jestli jsem cerny nebo bily posouvam figurky bud nahoru nebo dolu
    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    auto shift = board.on_turn ? &shift_rightos : &shift_leftos;
    auto plus_minus = board.on_turn ? &add : &sub;
    int maxrow = board.on_turn ? 0 : 7;

    bitboard left_pawn_attacks;
    if (board.on_turn == WHITE) {
        left_pawn_attacks =
                shift(board.all_bitboards[board.on_turn][PAWN], 9) & ~precomp.col_row_mask[CHESS_FILE::COl_1];
    }
    else {
        left_pawn_attacks =
                shift(board.all_bitboards[board.on_turn][PAWN], 9) & ~precomp.col_row_mask[CHESS_FILE::COl_8];

    }
    bitboard my_pieces = board.MyPieces();
    bitboard enemy_pieces = board.EnemyPieces();
    left_pawn_attacks &= enemy_pieces;
    int to;
    while (left_pawn_attacks) {
        to = __builtin_ffsll(left_pawn_attacks);
        to--;

        left_pawn_attacks ^= 1ULL << (to);
        moves.push_back(Move{plus_minus(to, 9), to, PAWN, board.getPieceAt(to), true});
        if (to / 8 == maxrow) {
            moves[moves.size() - 1].promotion = QUEEN;
        }
    }
    bitboard right_pawn_attacks;
    if (board.on_turn == WHITE) {

        right_pawn_attacks =
                shift(board.all_bitboards[board.on_turn][PAWN], 7) & ~precomp.col_row_mask[CHESS_FILE::COl_8];
    }
    else {
        right_pawn_attacks =
                shift(board.all_bitboards[board.on_turn][PAWN], 7) & ~precomp.col_row_mask[CHESS_FILE::COl_1];

    }
    right_pawn_attacks &= enemy_pieces;
    while (right_pawn_attacks) {
        to = __builtin_ffsll(right_pawn_attacks);
        to--;

        right_pawn_attacks ^= 1ULL << (to);
        moves.push_back(Move{plus_minus(to, 7), to, PAWN, board.getPieceAt(to), true});
        if (to / 8 == maxrow) {
            moves[moves.size() - 1].promotion = QUEEN;
        }
    }
    bitboard mask_rank = board.on_turn ? precomp.col_row_mask[CHESS_FILE::ROW_G]
                                       : precomp.col_row_mask[CHESS_FILE::ROW_B];

    bitboard front_pawn_moves = shift(board.all_bitboards[board.on_turn][PAWN], 8);
    front_pawn_moves &= ~(enemy_pieces | my_pieces);
    while (front_pawn_moves) {
        to = __builtin_ffsll(front_pawn_moves);
        to--;
        front_pawn_moves ^= 1ULL << to;
        moves.push_back(Move{plus_minus(to, 8), to, PAWN});
        if (to / 8 == maxrow) {
            moves[moves.size() - 1].promotion = QUEEN;
        }
    }
    bitboard double_pawn_moves = shift(board.all_bitboards[board.on_turn][PAWN] & mask_rank, 8);
    double_pawn_moves &= ~(enemy_pieces | my_pieces);
    double_pawn_moves = shift(double_pawn_moves, 8);
    double_pawn_moves &= ~(enemy_pieces | my_pieces);
    while (double_pawn_moves) {
        to = __builtin_ffsll(double_pawn_moves);
        to--;
        double_pawn_moves ^= 1ULL << to;
        moves.push_back(Move{plus_minus(to, 16), to, PAWN});
    }


    if (board.en_passant != -1) {
        // rank pesaku ktery mohou provest en passant
        bitboard rank = board.on_turn ? CHESS_FILE::ROW_D : CHESS_FILE::ROW_E;
        bitboard pawns = board.all_bitboards[board.on_turn][PAWN] & precomp.col_row_mask[rank];
        while (pawns) {
            int from = __builtin_ffsll(pawns);
            from--;
            pawns ^= 1ULL << from;
            if (abs(from % 8 - board.en_passant) == 1) {
                int add_to_position = board.on_turn ? 24 : 40;
                moves.push_back(
                        Move{from, add_to_position + board.en_passant, PAWN, PAWN, true, 0, false,
                             true});
            }
        }
    }

}

void MoveGenerator::getBishopAttacks(vector<Move> &vector, Board &board) {
    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    bitboard bishops = board.all_bitboards[board.on_turn][BISHOP];
    bitboard all_pieces = board._all_pieces;
    bitboard enemy_pieces = board.EnemyPieces();
    bitboard my_pieces = board.MyPieces();

    while (bishops) {
        int bishop = __builtin_ffsll(bishops);
        bishop--;
        bishops ^= 1ULL << bishop;

        bitboard bishop_blockers = precomp.bishop_rays[bishop] & all_pieces;

        bitboard bishop_index = (bishop_magics[bishop] * bishop_blockers) >> (64 - bishop_index_bits[bishop]);

        bitboard attacks = precomp.precomputed_bishops[bishop][bishop_index] & ~my_pieces;

        int to;
        while (attacks) {
            to = __builtin_ffsll(attacks);
            to--;
            attacks ^= 1ULL << to;

            if (enemy_pieces >> to & 1ULL) {
                vector.push_back(Move{bishop, to, BISHOP, board.getPieceAt(to), true});
            }
            else {
                vector.push_back(Move{bishop, to, BISHOP});
            }
        }

    }

}

void MoveGenerator::getRookAttacks(vector<Move> &vector, Board &board) {
    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    bitboard rooks = board.all_bitboards[board.on_turn][ROOK];
    bitboard all_pieces = board._all_pieces;
    bitboard enemy_pieces = board.EnemyPieces();
    bitboard my_pieces = board.MyPieces();

    while (rooks) {
        int rook = __builtin_ffsll(rooks);
        rook--;
        rooks ^= 1ULL << rook;

        bitboard rook_blockers = precomp.rook_rays[rook] & all_pieces;

        bitboard rook_index = (rooks_magic[rook] * rook_blockers) >> (64 - rook_index_bits[rook]);

        bitboard attacks = precomp.precomputed_rooks[rook][rook_index] & ~my_pieces;

        int to;
        while (attacks) {
            to = __builtin_ffsll(attacks);
            to--;
            attacks ^= 1ULL << to;

            if (enemy_pieces >> to & 1ULL) {
                vector.push_back(Move{rook, to, ROOK, board.getPieceAt(to), true});
            }
            else {
                vector.push_back(Move{rook, to, ROOK});
            }
        }

    }

}

void MoveGenerator::getQueenAttacks(vector<Move> &vector, Board &board) {
    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    bitboard all_pieces = board._all_pieces;
    bitboard my_pieces = board.MyPieces();
    bitboard enemy_pieces = board.EnemyPieces();
    bitboard queens = board.all_bitboards[board.on_turn][QUEEN];
    while (queens) {
        int queen = __builtin_ffsll(queens);
        queen--;
        queens ^= 1ULL << queen;

        // věžové útoky dámy
        bitboard rook_blockers = precomp.rook_rays[queen] & all_pieces;

        bitboard rook_index = (rooks_magic[queen] * rook_blockers) >> (64 - rook_index_bits[queen]);

        bitboard attacks = precomp.precomputed_rooks[queen][rook_index] & ~my_pieces;

        int to;
        while (attacks) {
            to = __builtin_ffsll(attacks);
            to--;
            attacks ^= 1ULL << to;

            if (enemy_pieces >> to & 1ULL) {
                vector.push_back(Move{queen, to, QUEEN, board.getPieceAt(to), true});
            }
            else {
                vector.push_back(Move{queen, to, QUEEN});
            }
        }
    }



    //střelcové útoky dámy

    queens = board.all_bitboards[board.on_turn][QUEEN];

    while (queens) {
        int queen = __builtin_ffsll(queens);
        queen--;
        queens ^= 1ULL << queen;

        bitboard bishop_blockers = precomp.bishop_rays[queen] & all_pieces;

        bitboard bishop_index = (bishop_magics[queen] * bishop_blockers) >> (64 - bishop_index_bits[queen]);

        bitboard bishop_attacks = precomp.precomputed_bishops[queen][bishop_index] & ~my_pieces;

        int to;
        while (bishop_attacks) {
            to = __builtin_ffsll(bishop_attacks);
            to--;
            bishop_attacks ^= 1ULL << to;

            if (enemy_pieces >> to & 1ULL) {
                vector.push_back(Move{queen, to, QUEEN, board.getPieceAt(to), true});
            }
            else {
                vector.push_back(Move{queen, to, QUEEN});
            }
        }

    }


}

void MoveGenerator::getKingAttacks(vector<Move> &vector, Board &board) {
    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    bitboard enemy_pieces = board.EnemyPieces();
    bitboard my_pieces = board.MyPieces();
    bitboard myKings = board.all_bitboards[board.on_turn][KING];
    int from;
    int to;
    while (myKings) {
        // nacteni pozice dalsiho kone
        from = __builtin_ffsll(myKings);
        from--;
        // smazani nacteneho kone
        myKings ^= 1ULL << (from);

        // utoky ze soucasneho kone
        bitboard attacks = precomp.precomputed_kings[from];
        attacks &= ~my_pieces;
        while (attacks) {
            //nacteni pozice uteceni
            to = __builtin_ffsll(attacks);
            to--;

            // smazani nactene pozice
            attacks ^= 1ULL << (to);
            if (enemy_pieces >> to & 1ULL) {
                // pohyb sebere figurku
                vector.push_back(Move{from, to, KING, board.getPieceAt(to), true});

            }
            else {
                // pohyb nesebere figurku
                vector.push_back(Move{from, to, KING});
            }
        }
    }


}

std::vector<Move> MoveGenerator::getLegalMoves(Board &board) {
    vector<Move> moves;
    MoveGenerator gen = MoveGenerator();
    gen.getPseudoLegalMoves(moves, board);
    vector<int> indexes;
    auto move_iter = moves.begin();
    while (move_iter != moves.end()) {
        auto move = *move_iter;
        Board tempboard = board;
        tempboard.MakeMove(move);
        if (tempboard.InCheck(tempboard.on_turn)) {
            move_iter = moves.erase(move_iter);
        }
        else {
            move_iter++;
        }
    }
    return moves;
}

