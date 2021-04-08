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

TableEntry TranspositionTable::InsertEntry(bitboard zobrist, TableEntry new_entry) {
    TableEntry *entry = GetEntry(zobrist);
    if (entry != nullptr) {
        if (entry->depth_tt < new_entry.depth_tt) {
            table[zobrist] = new_entry;
        }
    }
    else {
        table[zobrist] = new_entry;
    }
}

