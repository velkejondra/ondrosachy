//
// Created by ondra on 30.09.2020.
//
#ifndef CHESS_ENGINE_CPP_PGNPARSER_H
#define CHESS_ENGINE_CPP_PGNPARSER_H
#include <iostream>




class PgnParser {
    PgnParser();
    // Vytvoří json soubor
    void parse_pgn_to_file(std::string input_file);

};

class OpeningBook {
    OpeningBook(std::string input_file);

};

#endif //CHESS_ENGINE_CPP_PGNPARSER_H
