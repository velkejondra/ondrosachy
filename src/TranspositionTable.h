//
// Created by velkejondra on 1/23/21.
//

#ifndef CHESS_ENGINE_CPP_EXE_TRANSPOSITIONTABLE_H
#define CHESS_ENGINE_CPP_EXE_TRANSPOSITIONTABLE_H


#include "Board.h"
#include <unordered_map>

struct TableEntry {
    int depth;
    Move best_move;
};

class TranspositionTable {
public:
    TranspositionTable() = default;

    // vrati bud null pokud zaznam neexistuje, nebo nejlepsí pohyb ktery byl pro pozici minule nalezen
    TableEntry GetEntry(bitboard hash);

    // vlozi nejlepsi nalezenou pozici pro tento stav
    TableEntry InserEntry(TableEntry);

    // vycisti tabulku
    void clear();


private:
    std::unordered_map<bitboard, TableEntry> table;
};


#endif //CHESS_ENGINE_CPP_EXE_TRANSPOSITIONTABLE_H
