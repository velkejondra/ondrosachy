//
// Created by ondra on 7/26/2020.
//

#include "Board.h"
#include "PrecomputedBitboards.h"
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
    all_bitboards[BLACK][QUEEN] = 0x1000000000000000ULL;
    all_bitboards[WHITE][KING] = 0x10ULL;
    all_bitboards[BLACK][KING] = 0x800000000000000ULL;

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

bitboard Board::AllBlackPieces() {
    return 0;
}

bitboard Board::AllWhitePieces() {
    return 0;
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
                zobrist_hash ^= board_zobrist[i][j][index - 1];
                cur = cur & ~(1ULL << (index - 1ULL));
                index = __builtin_ffsll(cur);
            }

        }

    }
}
