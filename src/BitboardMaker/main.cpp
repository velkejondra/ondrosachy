//
// Created by velkejondra on 11/3/20.
//


#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

typedef unsigned long long bitboard;

const int RECT_WIDHT = 20;
const int RECT_HEIGHT = 20;
const int RECT_GAP = 0;


bitboard board;

using namespace std;

class BitboardMaker : public olc::PixelGameEngine {
public:
    BitboardMaker() {
        sAppName = "Example";
    }

public:
    bool OnUserCreate() override {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                DrawRect((int32_t) row * RECT_HEIGHT,
                         (int32_t) col * RECT_WIDHT,
                         RECT_HEIGHT, RECT_WIDHT, olc::DARK_CYAN);
            }
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        bitboard change = board;
        if (GetMouse(0).bReleased) {
            auto mouse_pos = GetMousePos();
            if (mouse_pos.x > 0 && mouse_pos.x < 8 * RECT_WIDHT) {
                if (mouse_pos.y > 0 && mouse_pos.y < 8 * RECT_HEIGHT) {
                    board ^= 1ULL << ((mouse_pos.y / RECT_HEIGHT * 8) + mouse_pos.x / RECT_HEIGHT);
                    cout << "0x" << hex << board << "ULL" << endl;
                }
            }

        }

        if (change != board) {
            bitboard loop_bitboard = board;
            int counter = 0;
            while (counter < 64) {
                olc::Pixel color = olc::BLACK;
                if (loop_bitboard & 1ULL) {
                    color = olc::CYAN;
                }
                FillRect(RECT_WIDHT * (counter % 8)+1, RECT_HEIGHT * (counter / 8)+1, RECT_HEIGHT-1, RECT_WIDHT-1, color);
                counter++;
                loop_bitboard >>= 1ULL;
            }
        }
        return true;
    }
};

int main() {
    BitboardMaker maker;
    if (maker.Construct(256, 240, 4, 4))
        maker.Start();
    return 0;
}