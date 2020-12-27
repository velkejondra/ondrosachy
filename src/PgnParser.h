//
// Created by ondra on 30.09.2020.
//
#ifndef CHESS_ENGINE_CPP_PGNPARSER_H
#define CHESS_ENGINE_CPP_PGNPARSER_H

#include <iostream>


class PgnParser {

public:
    PgnParser() = default;

// Vytvoří soubor

    void ParsePgnToFile(const std::string input_file, int depth);
};

class OpeningBook {
    OpeningBook(std::string input_file);

};

#endif //CHESS_ENGINE_CPP_PGNPARSER_H
