//
// Created by ondra on 30.09.2020.
//
#ifndef CHESS_ENGINE_CPP_PGNPARSER_H
#define CHESS_ENGINE_CPP_PGNPARSER_H

#include <iostream>
#include <fstream>
#include "TranspositionTable.h"


class PgnParser {

public:
    PgnParser() = default;

    void ParsePgnToFile(std::string input_file, int max_depth, int num_of_games);
};

class OpeningBook {
public:
    OpeningBook(const std::string &index_book, const std::string &data_book, int line_size = 42);

    std::string GetPGNMove(bitboard zobrist_hash);

private:
    int64_t BinarySearchFile(uint64_t l, uint64_t r, bitboard hash);

    std::ifstream index_book;
    std::ifstream data_book;
    int line_size;
};

struct MoveEntry {
    std::string move_pgn_string;
    int frequency;
};

#endif //CHESS_ENGINE_CPP_PGNPARSER_H
