//
// Created by velkejondra on 2/5/21.
//

#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include "../Board.h"
#include "../MoveGenerator.h"
#include "../Search.h"
#include <filesystem>
#include "defs.h"
#include "../PgnParser.h"

#include <thread>
#include <unordered_set>

typedef unsigned long long bitboard;

const int RECT_SIZE = 42;

static const olc::Pixel light_magenta = olc::Pixel(220, 167, 250);


// za jakou barvu hrajeme
bool side = WHITE;


// generator pohybu
MoveGenerator gen = MoveGenerator();

// urcuje stav hry
//jestli hráč vybírá pohyb nebo hraje počítač
enum Mode {
    COMP, CHOOSING_FROM, CHOOSING_TO
};

std::atomic<int> mod = {CHOOSING_FROM};

Move human_move;

// hlavni objekt na hrani
Board board = Board();

using namespace std;

unordered_set<int> legal_moves_to;


class GUI_Chess : public olc::PixelGameEngine {
public:
    GUI_Chess() {
        sAppName = "OndraSachy";
    }

public:
    bool OnUserCreate() override {
        SetPixelMode(olc::Pixel::MASK); // Dont draw pixels which have any transparency
        sprTile[BLACK][PAWN] = std::make_unique<olc::Sprite>("gui/imgs/pawn1.png");
        sprTile[WHITE][PAWN] = std::make_unique<olc::Sprite>("gui/imgs/pawn.png");
        sprTile[BLACK][KNIGHT] = std::make_unique<olc::Sprite>("gui/imgs/knight1.png");
        sprTile[WHITE][KNIGHT] = std::make_unique<olc::Sprite>("gui/imgs/knight.png");
        sprTile[BLACK][BISHOP] = std::make_unique<olc::Sprite>("gui/imgs/bishop1.png");
        sprTile[WHITE][BISHOP] = std::make_unique<olc::Sprite>("gui/imgs/bishop.png");
        sprTile[BLACK][ROOK] = std::make_unique<olc::Sprite>("gui/imgs/rook1.png");
        sprTile[WHITE][ROOK] = std::make_unique<olc::Sprite>("gui/imgs/rook.png");
        sprTile[BLACK][QUEEN] = std::make_unique<olc::Sprite>("gui/imgs/queen1.png");
        sprTile[WHITE][QUEEN] = std::make_unique<olc::Sprite>("gui/imgs/queen.png");
        sprTile[BLACK][KING] = std::make_unique<olc::Sprite>("gui/imgs/king1.png");
        sprTile[WHITE][KING] = std::make_unique<olc::Sprite>("gui/imgs/king.png");
        DrawChessboard(side);
        DrawPieces();
        return true;
    }

    void DrawChessboard(bool side) {
        olc::Pixel color;
        side = !side;
        for (int row = 0; row < 8; ++row) {
            side = !side;
            for (int col = 0; col < 8; ++col) {
                side = !side;
                color = side ? olc::WHITE : olc::VERY_DARK_GREY;

                FillRect((int32_t) row * RECT_SIZE,
                         (int32_t) col * RECT_SIZE,
                         RECT_SIZE, RECT_SIZE, color);
            }
        }
    }

    void DrawPieces() {
        for (int piece = PAWN; piece <= KING; piece++) {
            for (int color = WHITE; color <= BLACK; color++) {
                bitboard pieces = board.all_bitboards[color][piece];
                int position;
                while (pieces) {
                    position = __builtin_ffsll(pieces);
                    position--;
                    pieces ^= 1ULL << position;
                    DrawSprite(olc::vi2d(position % 8, 7 - position / 8) * 42, sprTile[color][piece].get());
                }
            }
        }
    }

    void DrawDestinations() {
        DrawChessboard(side);
        vector<Move> moves = gen.getLegalMoves(board);
        legal_moves_to.clear();
        for (auto move : moves) {
            if (move.from == human_move.from) {
                legal_moves_to.insert(move.to);
                FillRect(move.to % 8 * RECT_SIZE,
                         (7 - move.to / 8) * RECT_SIZE,
                         RECT_SIZE, RECT_SIZE, light_magenta);
            }
        }

        DrawPieces();

    }

    bool OnUserUpdate(float fElapsedTime) override {
        // zkontoluje vstup uzivatele
        if (GetMouse(0).bReleased && mod.load() != COMP) {
            auto mouse_pos = GetMousePos();
            if (mouse_pos.x > 0 && mouse_pos.x < 8 * RECT_SIZE) {
                if (mouse_pos.y > 0 && mouse_pos.y < 8 * RECT_SIZE) {
                    // vybrani figurky na pohyb
                    if (mod.load() == CHOOSING_FROM) {
                        if (1ULL << (((7 - mouse_pos.y / RECT_SIZE) * 8) + mouse_pos.x / RECT_SIZE) &
                            board.MyPieces()) {
                            human_move.from = ((7 - mouse_pos.y / RECT_SIZE) * 8) + mouse_pos.x / RECT_SIZE;
                            human_move.piece_type = board.getPieceAt(human_move.from);
                            mod.store(CHOOSING_TO);
                            DrawDestinations();
                        }
                    }
                        // vybrani ciloveho mista na pohyb figurkou
                    else if (mod.load() == CHOOSING_TO) {
                        if (legal_moves_to.count((((7 - mouse_pos.y / RECT_SIZE) * 8) + mouse_pos.x / RECT_SIZE))) {
                            human_move.to = ((7 - mouse_pos.y / RECT_SIZE) * 8) + mouse_pos.x / RECT_SIZE;
                            if (board.getPieceAt(human_move.to) != -1) {
                                human_move.capture = true;
                                human_move.capture_piece_type = board.getPieceAt(human_move.to);
                            }
                            if (human_move.piece_type == PAWN && board.getPieceAt(human_move.to) == -1 &&
                                abs(human_move.to - human_move.from) % 8 != 0) {
                                human_move.capture = true;
                                human_move.en_passant = true;
                                human_move.capture_piece_type = PAWN;
                            }
                            if (human_move.to / 8 == 7 - (side * 7) && human_move.piece_type == PAWN) {
                                human_move.promotion = QUEEN;
                            }
                            board.MakeMove(human_move);
                            human_move = Move{};
                            DrawChessboard(side);
                            DrawPieces();
                            mod.store(COMP);
                        }
                        else {
                            mod.store(CHOOSING_FROM);
                            DrawChessboard(side);
                            DrawPieces();
                        }

                    }
                }
            }
        }

        return true;
    }
};

[[noreturn]] void chess_bot(GUI_Chess &gui) {
    Search search;
    OpeningBook book = OpeningBook("openingbook.index", "openingbook.data");
    while (true) {
        if (mod.load() == COMP) {
            if (gen.getLegalMoves(board).empty()) {
                cout << "sach mat vyhral hrac\n";
            }
            else {
                string pgn_move = book.GetPGNMove(board.zobrist_hash);
                if (!pgn_move.empty()) {
                    board.MakeMoveFromPGN(pgn_move);
                }
                else {
                    Move bot_move = search.IterativeDeepening(4, board);
                    board.MakeMove(bot_move);
                }
                gui.DrawChessboard(side);
                gui.DrawPieces();
                mod.store(CHOOSING_FROM);
//                cout << board.zobrist_hash << endl;
            }
        }
        this_thread::yield();
    }
}

int main(int argc, char *argv[]) {
    GUI_Chess maker;
    std::thread bot_thread(chess_bot, std::ref(maker));
    if (maker.Construct(336, 336, 2, 2, false, true)) {
        maker.Start();
    }
    return 0;
}

