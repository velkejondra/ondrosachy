//
// Created by ondra on 7/26/2020.
//

#include "Board.h"
#include "PrecomputedBitboards.h"
#include "defs.h"
#include "MoveGenerator.h"
#include <vector>
#include <algorithm>
#include <bitset>
#include <climits>
#include <random>

using namespace std;

void Board::MakeMoveFromPGN(string move) {
    PrecomputedBitboards &precomp_bitboards = PrecomputedBitboards::getInstance();
    //parsovani destinace
    string destination = move.substr(move.length() - 2);
    int destination_int = destination[0] - 'a' + 8 * (destination[1] - '1');


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
        vector<char> pieces = {'N', 'B', 'R', 'Q', 'K'};
        auto it = find(pieces.begin(), pieces.end(), move[0]);

        piece = it - pieces.begin() + 1;
        startpos = 1;
    }
// parsovani pozice odchodu
    string specify_from = string(move.begin() + startpos, move.begin() + endpos);
    bool row_specified = false;
    bool file_specified = false;
    int row_int;
    int file_int;
    MoveGenerator gen = MoveGenerator();
    auto moves = gen.getLegalMoves(*this);
    for (char row_file : specify_from) {
        if (row_file < 'a') {
            row_specified = true;
            row_int = row_file - '1';
        }
        else {
            file_specified = true;
            file_int = row_file - 'a';
        }
    }
    for (Move mov : moves) {
        if (mov.to != destination_int) {
            continue;
        }
        if (mov.piece_type != piece) {
            continue;
        }
        if (row_specified && mov.from / 8 != row_int) {
            continue;
        }
        if (file_specified && mov.from % 8 != file_int) {
            continue;
        }
        this->MakeMove(mov);
        break;
    }

}

void Board::MakeMove(Move move) {
    last_move = move;
    last_en_passant = en_passant;
    en_passant = -1;
    // rosada
//    if (move.castling) {
//        all_bitboards[on_turn][KING] &= ~(1ULL << (move.from));
//        all_bitboards[on_turn][KING] |= (1ULL << (move.to));
//        _all_pieces &= ~(1ULL << (move.from));
//        _all_pieces |= (1ULL << (move.to));
//
//        if (move.to == 2 || move.to == 64) {
//            // velka rosada
//            all_bitboards[on_turn][ROOK] &= ~(move.to - 2);
//            all_bitboards[on_turn][ROOK] |= move.to + 1;
//            _all_pieces &= ~(move.to - 2);
//            _all_pieces |= move.to + 1;
//        }
//        else {
//            // mala rosada
//            all_bitboards[on_turn][ROOK] &= move.to + 1;
//            all_bitboards[on_turn][ROOK] |= 1ULL - 1;
//            all_bitboards[on_turn][ROOK] &= move.to + 1;
//            all_bitboards[on_turn][ROOK] |= 1ULL - 1;
//        }
//    }
    if (move.en_passant) {
        //smazani start pozice a pridani end pozice
        RemovePiece(PAWN, move.from, on_turn);
        AddPiece(PAWN, move.to, on_turn);
//        all_bitboards[on_turn][PAWN] &= ~(1ULL << (move.from));
//        all_bitboards[on_turn][PAWN] |= (1ULL << (move.to));
//        _all_pieces &= ~(1ULL << (move.from));
//        _all_pieces |= (1ULL << (move.to));

        // smazani sebrateho pesaka
        auto plus_minus = on_turn ? &add : &sub;
        RemovePiece(PAWN, plus_minus(move.to, 8), !on_turn);
//        all_bitboards[!on_turn][PAWN] &= ~(1ULL << (plus_minus(move.to, 8)));
//        _all_pieces &= ~(1ULL << (plus_minus(move.to, 8)));
    }
    else if (move.capture) {
        //smazani sebrate figurky
        RemovePiece(move.capture_piece_type, move.to, !on_turn);
//        all_bitboards[!on_turn][move.capture_piece_type] &= ~(1ULL << (move.to));
//        zmeneni pozice utocnika

        RemovePiece(move.piece_type, move.from, on_turn);
//        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
        AddPiece(move.piece_type, move.to, on_turn);
//        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));

//        _all_pieces &= ~(1ULL << (move.to));
//        _all_pieces &= ~(1ULL << (move.from));
//        _all_pieces |= (1ULL << (move.to));
    }
    else {
        // normalni pohyb
        RemovePiece(move.piece_type, move.from, on_turn);
//        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
        AddPiece(move.piece_type, move.to, on_turn);
//        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));
//        _all_pieces &= ~(1ULL << (move.from));
//        _all_pieces |= (1ULL << (move.to));

        if (move.piece_type == PAWN && abs(move.to - move.from) == 16) {
            // pesak poposel o dva dopredu a tedy je mozny en passant
            en_passant = move.from % 8;
        }
    }
    if (move.promotion != 0) {
        RemovePiece(PAWN, move.to, on_turn);
//        all_bitboards[on_turn][PAWN] &= ~(1ULL << move.to);
        AddPiece(QUEEN, move.to, on_turn);
//        all_bitboards[on_turn][QUEEN] |= 1ULL << move.to;
//        _all_pieces &= ~(1ULL << move.to);
//        _all_pieces |= 1ULL << move.to;
    }
    this->AdjustZobrist(move);
    on_turn = !on_turn;
//    CalculateWhitePieces();
//    CalculateBlackPieces();
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
    all_bitboards[BLACK][QUEEN] = 0x0800000000000000ULL;
    all_bitboards[WHITE][KING] = 0x10ull;
    all_bitboards[BLACK][KING] = 0x1000000000000000ULL;

    CalculateAll();
    CalculateBlackPieces();
    CalculateWhitePieces();
    InitZobrist();
    ComputeZobristHash();
}


void Board::CalculateAll() {
    bitboard output = 0ULL;
    for (auto &x : all_bitboards) {
        for (auto &b : x) {
            output |= b;
        }
    }
    _all_pieces = output;
}

bitboard Board::MyPieces() {
    if (on_turn == WHITE) {
        return white_pieces;
    }
    return black_pieces;
}

void Board::CalculateWhitePieces() {
    bitboard output = 0ULL;
    for (auto &b :all_bitboards[WHITE]) {
        output |= b;
    }
    white_pieces = output;
}

bitboard Board::EnemyPieces() {
    if (on_turn == WHITE) {
        return black_pieces;
    }
    return white_pieces;
}

void Board::CalculateBlackPieces() {
    bitboard output = 0ULL;
    for (auto &b :all_bitboards[BLACK]) {
        output |= b;
    }
    black_pieces = output;
}


void Board::InitZobrist() {
    std::mt19937 gen(0xFEE1DEAD);
    static std::uniform_int_distribution<unsigned long long> distrib(0, ULONG_LONG_MAX);
    for (auto &i : board_zobrist) {
        for (auto &j : i) {
            for (unsigned long long &k : j) {
                k = distrib(gen);
//                cout << k << endl;
            }
        }
    }
    for (unsigned long long &num : en_passant_zobrist) {
        num = distrib(gen);
    }
    side_to_move_zobrist = distrib(gen);
}

void Board::AdjustZobrist(Move m) {
    if (this->last_en_passant != -1) {
        zobrist_hash ^= en_passant_zobrist[last_en_passant];
    }
    if (this->en_passant != -1) {
        zobrist_hash ^= en_passant_zobrist[en_passant];
    }
    if (m.en_passant) {
        zobrist_hash ^= board_zobrist[this->on_turn][PAWN][m.from];
        zobrist_hash ^= board_zobrist[this->on_turn][PAWN][m.to];
        zobrist_hash ^= board_zobrist[!this->on_turn][PAWN][m.to + (on_turn * 16 + 8)];
    }
    else {
        zobrist_hash ^= board_zobrist[this->on_turn][m.piece_type][m.from];
        if (m.capture) {
            zobrist_hash ^= board_zobrist[!this->on_turn][m.capture_piece_type][m.to];
        }
        zobrist_hash ^= board_zobrist[this->on_turn][KING][m.to];
    }
    zobrist_hash ^= side_to_move_zobrist;

}

void Board::ComputeZobristHash() {
    zobrist_hash = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 6; ++j) {
            bitboard cur = all_bitboards[i][j];
            int index;
            while (cur) {
                index = __builtin_ffsll(cur);
                index--;
                zobrist_hash ^= board_zobrist[i][j][index];
                cur ^= 1ULL << index;
            }
        }
    }
    zobrist_hash ^= side_to_move_zobrist;
}

//void Board::UndoMove(Move move) {
//
//    last_move = move;
//    en_passant = last_en_passant;
//    // rosada
//    if (move.en_passant) {
//        //smazani start pozice a pridani end pozice
//        all_bitboards[on_turn][PAWN] &= ~(1ULL << (move.from));
//        all_bitboards[on_turn][PAWN] |= (1ULL << (move.to));
//
//        // smazani sebrateho pesaka
//        all_bitboards[!on_turn][PAWN] &= ~(1ULL << (move.to + (on_turn * 16 + 8)));
//        return;
//    }
//    else if (move.capture) {
//        //smazani sebrate figurky
//        all_bitboards[!on_turn][move.capture_piece_type] &= ~(1ULL << (move.to));
//        //zmeneni pozice utocnika
//        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
//        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));
//    }
//    else {
//        // normalni pohyb
//        all_bitboards[on_turn][move.piece_type] &= ~(1ULL << (move.from));
//        all_bitboards[on_turn][move.piece_type] |= (1ULL << (move.to));
//    }
//    on_turn = !on_turn;
//}

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
    bitboard all_pieces = _all_pieces;
    bitboard attacks = 0ULL;

    // pawn attacks
    auto shift = color ? &shift_rightos : &shift_leftos;
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
    return attacks;
}

bool Board::InCheck(int color) {
    bitboard attacks = GetAllBitboardAttacks(color);
    return all_bitboards[!color][KING] & attacks;
}

std::string Board::GetMoveNotation() const {
    int form = last_move.from;
    int to = last_move.to;
    return positions[form] + positions[to];
}

void Board::AddPiece(int piece, int pos, bool color) {
    this->all_bitboards[color][piece] |= (1ULL << (pos));
    this->_all_pieces |= (1ULL << (pos));
    if (color == WHITE) {
        this->white_pieces |= (1ULL << (pos));
    }
    else {
        this->black_pieces |= (1ULL << (pos));
    }
}

void Board::RemovePiece(int piece, int pos, bool color) {
    this->all_bitboards[color][piece] &= ~(1ULL << (pos));
    this->_all_pieces &= ~(1ULL << (pos));
    if (color == WHITE) {
        this->white_pieces &= ~(1ULL << (pos));
    }
    else {
        this->black_pieces &= ~(1ULL << (pos));
    }

}


bool Move::operator==(const Move &rhs) const {
    return from == rhs.from &&
           to == rhs.to &&
           piece_type == rhs.piece_type &&
           capture_piece_type == rhs.capture_piece_type &&
           capture == rhs.capture &&
           promotion == rhs.promotion &&
           castling == rhs.castling &&
           en_passant == rhs.en_passant;
}

bool Move::operator!=(const Move &rhs) const {
    return !(rhs == *this);
}
