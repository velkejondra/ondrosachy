//
// Created by ondra on 10/27/2020.
//

#include <iostream>
#include "debug.h"
using namespace std;

string BitboardRepresentation(bitboard board) {
    string serialized_bitboard;
    for (int i = 7; i >=0; --i) {
        for (int j = 0; j < 8; ++j) {
            serialized_bitboard += to_string(board >> (i * 8 + j) & 1ULL);

        }
        serialized_bitboard += "\n";
    }
    return serialized_bitboard;

}
