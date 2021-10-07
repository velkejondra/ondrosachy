//
// Created by ondra on 30.09.2020.
//
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <iomanip>
#include <random>
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
    } else {
        return false;
    }
}

struct Record {
    bitboard zobrist_hash;
    vector<MoveCount> moves;
};

void PgnParser::ParsePgnToFile(const std::string input_file, int max_depth, int num_of_games) {
    ifstream input(input_file);
    int counter = 0;

    map<bitboard, unordered_map<string, int>> database;
    string line;
    string linecopy;
    while (getline(input, line)) {
        //parsovani info tagu
        if (line.substr(0, 2) != "1.") {
            continue;
        }
//        if (line == "1. e4 c5 2. Nc3 Nc6 3. Nge2 e5 4. Nd5 Be7 5. Nec3 Nf6 6. Bc4 O-O 7. d3 Nxd5\r\r") {
//            bool testik = true;
//        }
        linecopy = line;

        //printování progresu
        if (counter % 100000) {
            int barWidth = 70;

            std::cout << "[";
            int pos = barWidth * counter / num_of_games;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int((double) counter / num_of_games * 100.0) << " %\r";
            std::cout.flush();
        }
        size_t pos = 0;
        std::string token;
        Board board = Board();
        int depth = 0;
        counter++;
        while ((pos = line.find(' ')) != string::npos && max_depth != depth) {
            token = line.substr(0, pos);
            if (token[token.size() - 1] == '.') {
                line.erase(0, pos + 1);
                continue;
            }
            if (token == "O-O" || token == "O-O-O") {
                break;
            }
            if (token[token.size() - 1] == '+') {
                token.erase(token.end() - 1);
            }
//            if (board.zobrist_hash == 15611873093638524977ULL) {
//                bool lmao = true;
//            }
            depth++;
            database[board.zobrist_hash][token]++;
            board.MakeMoveFromPGN(token);
            line.erase(0, pos + 1);
        }
        if (counter == num_of_games) {
            break;
        }
    }

//    zapsani pozic do dvou soboru
//    v jednom je hash pozice a index pro druhy soubor
//    a v druhem jsou jsou vhodne pohyby pro danou pozici

    ofstream index_book;
    ofstream data_book;
    index_book.open("openingbook.index");
    data_book.open("openingbook.data");
    for (const auto &kv: database) {
        // zapise hash
        index_book << std::setfill('0') << std::setw(20) << kv.first << " " << std::setfill('0') << std::setw(20)
                   << data_book.tellp();
        data_book << kv.second.size();
        for (const auto &second_kv: kv.second) {
            data_book << " " << second_kv.first << " " << second_kv.second;
        }
        index_book << '\n';
        data_book << '\n';
    }
    index_book.close();
    data_book.close();
}

OpeningBook::OpeningBook(const std::string &index_book, const std::string &data_book, int line_size) {
    this->index_book.open(index_book);
    this->data_book.open(data_book);
    this->line_size = line_size;
}

int64_t OpeningBook::BinarySearchFile(uint64_t l, uint64_t r, bitboard hash) {
    uint64_t second_r = r / line_size;
    uint64_t second_l = l / line_size;
    if (second_r >= second_l) {
        uint64_t mid = second_l + (second_r - second_l) / 2;
        index_book.seekg(mid * this->line_size - 1);
        bitboard cur_hash;
        index_book >> cur_hash;
        if (cur_hash == hash) {
            uint64_t index_for_data_file;
            index_book >> index_for_data_file;
            return index_for_data_file;
        }

        if (cur_hash > hash)
            return BinarySearchFile(l, (mid - 1) * line_size, hash);

        return BinarySearchFile((mid + 1) * line_size, r, hash);
    }
    return -1;
}

std::string OpeningBook::GetPGNMove(bitboard zobrist_hash) {
    // vyhledani pozice v indexovem souboru
    this->index_book.seekg(0, ios::end);
    int64_t file_end = this->index_book.tellg();
    int64_t index_for_data_file = BinarySearchFile(0, file_end, zobrist_hash);
    if (index_for_data_file == -1) {
        return "";
    }
    // nacteni pozice z datoveho souboru podle indexu a generovani nahodne s ohledem na jejich cetnost
    data_book.seekg(index_for_data_file);
    int arr_size;
    data_book >> arr_size;
    vector<MoveEntry> moves;
    moves.reserve(arr_size);
    int sum = 0;
    for (int i = 0; i < arr_size; ++i) {
        string move_pgn;
        int freq;
        data_book >> move_pgn >> freq;
        moves.push_back(MoveEntry{move_pgn, freq});
        sum += freq;
    }
    std::mt19937 gen(time(NULL));
    std::uniform_int_distribution<int> distribution(1, sum);

    int random_sum = distribution(gen);

    for (const MoveEntry &move_entry: moves) {
        random_sum -= move_entry.frequency;
        if (random_sum <= 0) {
            return move_entry.move_pgn_string;
        }
    }
    throw std::invalid_argument("");
}

