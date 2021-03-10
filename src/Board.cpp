//
// Created by ondra on 7/26/2020.
//

#include "Board.h"
#include "PrecomputedBitboards.h"
#include "defs.h"
#include "MoveGenerator.h"
#include "RandomGenerator.h"
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

void Board::MakeMoveFromPGN(string move) {
    Move m{};
    PrecomputedBitboards &precomp_bitboards = PrecomputedBitboards::getInstance();
    if (move == "O-O") {
        this->MakeMove(true);
    }
    if (move == "O-O-O") {
        this->MakeMove(false);
    }

    //parsovani destinace
    string destination = move.substr(move.length() - 2);


    // parsovani brani
    bool capture = false;
    int endpos = move.length() - 2;
    if (move.length() - 3 >= 0 && move[move.length() - 3] == 'x') {
        capture = true;
        endpos--;
    }

//  parsovani figurky
    int piece = PAWN;
    int startpos = 0;
    if (!islower(move[startpos])) {
        vector<char> pieces = {'K', 'B', 'R', 'Q', 'K'};
        auto it = find(pieces.begin(), pieces.end(), move[0]);

        piece = it - pieces.begin() + 1;
        startpos = 1;
    }
// parsovani pozice odchodu
    string specify_from = move.substr(startpos, endpos);
    cout << specify_from << endl;


}

void Board::MakeMove(bool small_castle) {

}

void Board::MakeMove(Move move) {
    last_move = move;
    last_moves.push_back(move);
    last_en_passant = en_passant;
    en_passant = -1;
    // rosada
    if (move.castling) {
        all_bitboards[on_turn][KING] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][KING] |= (1ULL << (move.to));

        if (move.to == 2 || move.to == 64) {
            // velka rosada
            all_bitboards[on_turn][ROOK] &= ~(move.to - 2);
            all_bitboards[on_turn][ROOK] |= move.to + 1;
        }
        else {
            // mala rosada
            all_bitboards[on_turn][ROOK] &= move.to + 1;
            all_bitboards[on_turn][ROOK] |= 1ULL - 1;
        }
    }
    else if (move.en_passant) {
        //smazani start pozice a pridani end pozice
        all_bitboards[on_turn][PAWN] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][PAWN] |= (1ULL << (move.to));

        // smazani sebrateho pesaka
        auto plus_minus = on_turn ? &add : &sub;
        all_bitboards[!on_turn][PAWN] &= ~(1ULL << (plus_minus(move.to, 8)));
    }
    else if (move.capture) {
        //smazani sebrate figurky
        all_bitboards[!on_turn][move.capture_piece_type] &= ~(1ULL << (move.to));
        //zmeneni pozice utocnika
        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));
    }
    else {
        // normalni pohyb
        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));

        if (move.piece_type == PAWN && abs(move.to - move.from) == 16) {
            // pesak poposel o dva dopredu a tedy je mozny en passant
            en_passant = move.from % 8;
        }
    }
    if (move.promotion != 0) {
        all_bitboards[on_turn][PAWN] &= ~(1ULL << move.to);
        all_bitboards[on_turn][QUEEN] |= 1ULL << move.to;
    }
    on_turn = !on_turn;
}

Board::Board() {
    all_bitboards[WHITE][PAWN] = 0xff00ULL;
    all_bitboards[BLACK][PAWN] = 0xff000000000000ULL;
    all_bitboards[WHITE][KNIGHT] = 0x42ULL;
    all_bitboards[BLACK][KNIGHT] = 0x4200000000000000ULL;
    all_bitboards[WHITE][BISHOP] = 0x24ULL;
    all_bitboards[BLACK][BISHOP] = 0x2400000000000000ULL;
    all_bitboards[WHITE][ROOK] = 0x81ULL;
    all_bitboards[BLACK][ROOK] = 0x8100000000000000ULL;
    all_bitboards[WHITE][QUEEN] = 0x8ULL;
    all_bitboards[BLACK][QUEEN] = 0x800000000000000ULL;
    all_bitboards[WHITE][KING] = 0x10ULL;
    all_bitboards[BLACK][KING] = 0x1000000000000000ULL;

    InitZobrist();
    ComputeZobristHash();
}

bitboard Board::AllPieces() {
    bitboard output = 0;
    for (auto &x : all_bitboards) {
        for (auto &b : x) {
            output |= b;
        }
    }
    return output;
}

bitboard Board::MyPieces() {
    bitboard output = 0;
    for (auto &b :all_bitboards[on_turn]) {
        output |= b;
    }
    return output;
}

bitboard Board::EnemyPieces() {
    bitboard output = 0;
    for (auto &b :all_bitboards[!on_turn]) {
        output |= b;
    }
    return output;
}


void Board::InitZobrist() {
    for (auto &i : board_zobrist) {
        for (auto &j : i) {
            for (unsigned long long &k : j) {
                k = distrib(gen);
            }
        }
    }
    for (bitboard &num : castling_zobrist) {
        num = distrib(gen);
    }
    for (unsigned long long &num : en_passant_zobrist) {
        num = distrib(gen);
    }
    side_to_move_zobrist = distrib(gen);
}

void Board::AdjustZobrist(Move m) {

}

void Board::ComputeZobristHash() {
    zobrist_hash = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 6; ++j) {
            bitboard cur = all_bitboards[i][j];
            int index = __builtin_ffsll(cur);
            while (index) {
                zobrist_hash ^= board_zobrist[i][j][index - 1ULL];
                cur = cur & ~(1ULL << (index - 1ULL));
                index = __builtin_ffsll(cur);
            }
        }
    }
}

void Board::UndoMove(Move move) {

    last_move = move;
    en_passant = -1;
    // rosada
    if (move.castling) {
        all_bitboards[on_turn][KING] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][KING] |= (1ULL << (move.to));

        if (move.to == 2 || move.to == 64) {
            // velka rosada
            all_bitboards[on_turn][ROOK] &= ~(move.to - 2);
            all_bitboards[on_turn][ROOK] |= move.to + 1;
        }
        else {
            // mala rosada
            all_bitboards[on_turn][ROOK] &= move.to + 1;
            all_bitboards[on_turn][ROOK] |= 1ULL - 1;
        }
    }
    else if (move.en_passant) {
        //smazani start pozice a pridani end pozice
        all_bitboards[on_turn][PAWN] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][PAWN] |= (1ULL << (move.to));

        // smazani sebrateho pesaka
        all_bitboards[!on_turn][PAWN] &= ~(1ULL << (move.to + (on_turn * 16 + 8)));
        return;
    }
    else if (move.capture) {
        //smazani sebrate figurky
        all_bitboards[!on_turn][move.capture_piece_type] &= ~(1ULL << (move.to));
        //zmeneni pozice utocnika
        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));
    }
    else {
        // normalni pohyb
        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));
    }
    on_turn = !on_turn;

}

int Board::getPieceAt(int position) {
    for (int i = 0; i < 6; ++i) {
        if (all_bitboards[!on_turn][i] >> position & 1ULL) {
            return i;
        }
        if (all_bitboards[on_turn][i] >> position & 1ULL) {
            return i;
        }
    }
    return -1;
}

bitboard Board::GetAllBitboardAttacks(bool color) {
    PrecomputedBitboards &precomp = PrecomputedBitboards::getInstance();
    bitboard enemy_pieces = this->MyPieces();
    bitboard my_pieces = this->EnemyPieces();
    bitboard all_pieces = this->AllPieces();
    bitboard attacks = 0ULL;

    // pawn attacks
    auto shift = color ? &shift_right : &shift_left;
    if (color == WHITE) {
        attacks |= shift(all_bitboards[color][PAWN], 7) & ~precomp.col_row_mask[CHESS_FILE::COl_8];
        attacks |= shift(all_bitboards[color][PAWN], 9) & ~precomp.col_row_mask[CHESS_FILE::COl_1];

    }
    else {
        attacks |= shift(all_bitboards[color][PAWN], 7) & ~precomp.col_row_mask[CHESS_FILE::COl_1];
        attacks |= shift(all_bitboards[color][PAWN], 9) & ~precomp.col_row_mask[CHESS_FILE::COl_8];
    }

    // knights attacks
    bitboard knights = all_bitboards[color][KNIGHT];
    int knight_index;
    while (knights) {
        knight_index = __builtin_ffsll(knights);
        knight_index--;
        knights ^= 1ULL << knight_index;
        attacks |= precomp.precomputed_knights[knight_index];
    }

    // bishop attacks
    bitboard bishops = all_bitboards[color][BISHOP];
    while (bishops) {
        int bishop = __builtin_ffsll(bishops);
        bishop--;
        bishops ^= 1ULL << bishop;
        bitboard bishop_blockers = precomp.bishop_rays[bishop] & all_pieces;
        bitboard bishop_index = (bishop_magics[bishop] * bishop_blockers) >> (64 - bishop_index_bits[bishop]);
        attacks |= precomp.precomputed_bishops[bishop][bishop_index];
    }
    // rook attacks
    bitboard rooks = all_bitboards[color][ROOK];
    while (rooks) {
        int rook = __builtin_ffsll(rooks);
        rook--;
        rooks ^= 1ULL << rook;
        bitboard rook_blockers = precomp.rook_rays[rook] & all_pieces;
        bitboard rook_index = (rooks_magic[rook] * rook_blockers) >> (64 - rook_index_bits[rook]);
        attacks |= precomp.precomputed_rooks[rook][rook_index];
    }

    // queen attacks
    bitboard queens = all_bitboards[color][QUEEN];
    while (queens) {
        int queen = __builtin_ffsll(queens);
        queen--;
        // bishop part
        queens ^= 1ULL << queen;
        bitboard bishop_blockers = precomp.bishop_rays[queen] & all_pieces;
        bitboard bishop_index = (bishop_magics[queen] * bishop_blockers) >> (64 - bishop_index_bits[queen]);
        attacks |= precomp.precomputed_bishops[queen][bishop_index];

        // rook part
        bitboard rook_blockers = precomp.rook_rays[queen] & all_pieces;
        bitboard rook_index = (rooks_magic[queen] * rook_blockers) >> (64 - rook_index_bits[queen]);
        attacks |= precomp.precomputed_rooks[queen][rook_index];
    }
    // king attacks
    bitboard king = all_bitboards[color][KING];
    int king_index = __builtin_ffsll(king);
    --king_index;
    attacks |= precomp.precomputed_kings[king_index];
    attacks &= ~enemy_pieces;
    return attacks;
}

bool Board::InCheck(int color) {
    bitboard attacks = GetAllBitboardAttacks(on_turn);
    return all_bitboards[!on_turn][KING] & attacks;
}

std::string Board::GetMoveNotation() {
    int form = last_move.from;
    int to = last_move.to;
    return positions[form] + positions[to];
}

bitboard Board::PiecesOfColor(bool color) {
    bitboard output = 0;
    for (auto &b :all_bitboards[color]) {
        output |= b;
    }
    return output;
}


