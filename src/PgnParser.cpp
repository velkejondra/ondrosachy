//
// Created by ondra on 30.09.2020.
//
#include <fstream>
#include <iostream>
#include "Board.h"
#include "PgnParser.h"
using namespace std;
void PgnParser::parse_pgn_to_file(const std::string input_file) {
    ifstream input(input_file);

    string line;
    while(getline(input, line)){
    }
}
