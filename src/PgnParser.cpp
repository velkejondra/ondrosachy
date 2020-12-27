//
// Created by ondra on 30.09.2020.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Board.h"
#include "PgnParser.h"

using namespace std;

struct MoveCount {
    string move;
    int count = 0;

};

bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

struct Record {
    bitboard zobrist_hash;
    vector<MoveCount> moves;
};

void PgnParser::ParsePgnToFile(const std::string input_file, int depth) {
    ifstream input(input_file);

    unordered_map<bitboard, unordered_map<string, int>> database;
    string line;
    bool info = true;
    bool game = false;
    while (getline(input, line)) {
        //parsovani info tagu
        if (info) {
            if (line.rfind("[BlackElo", 0) == 0) {
                info = false;
            }
        }
        else {
            if (line[0] == '[') {
                continue;
            }
            if (line.size() <= 1) {
                game = !game;
                continue;
            }
            if (!game) {
                info = true;
                continue;
            }
            size_t pos = 0;
            std::string token;
            Board board = Board();
            while ((pos = line.find(' ')) != string::npos) {
                token = line.substr(0, pos);
                if (token[token.size() - 1] == '.') {
                    if (token[0] == '5') {
                        break;
                    }
                    line.erase(0, pos + 1);
                    continue;
                }
                bitboard last_zobrist = board.zobrist_hash;
                board.MakeMoveFromPGN(token);
                database[last_zobrist][board.last_destination]++;
            }
        }
    }


}
