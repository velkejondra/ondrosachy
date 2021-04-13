//
// Created by velkejondra on 1/23/21.
//

#include "TranspositionTable.h"

TableEntry *TranspositionTable::GetEntry(bitboard hash) {
    if (this->table.count(hash)) {
        return &table[hash];
    }
    else { return nullptr; }
}

void TranspositionTable::InsertEntry(bitboard zobrist, TableEntry new_entry) {
    auto entry = table.insert(std::make_pair(zobrist, new_entry));
    if (!entry.second) {
        if (new_entry.depth_tt >= entry.first->second.depth_tt) {
            entry.first->second = new_entry;
        }
    }
}

void TranspositionTable::clear() {
    table.clear();

}

int TranspositionTable::size() {
    return table.size();
}

