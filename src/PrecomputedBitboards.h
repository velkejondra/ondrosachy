//
// Created by ondra on 10/27/2020.
//

#ifndef CHESS_ENGINE_CPP_PRECOMPUTEDBITBOARDS_H
#define CHESS_ENGINE_CPP_PRECOMPUTEDBITBOARDS_H

#include "debug.h"

typedef unsigned long long bitboard;

enum CHESS_FILE {
    COl_1,
    COl_2,
    COl_3,
    COl_4,
    COl_5,
    COl_6,
    COl_7,
    COl_8,
    ROW_A,
    ROW_B,
    ROW_C,
    ROW_D,
    ROW_F,
    ROW_E,
    ROW_G,
    ROW_H,
};
/*
Magicke cisla ukradnuta z https://github.com/GunshipPenguin/shallow-blue/blob/c6d7e9615514a86533a9e0ffddfc96e058fc9cfd/src/attacks.h#L120
*/

const bitboard rooks_magic[64] = {
        0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
        0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
        0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
        0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
        0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
        0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
        0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
        0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
        0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
        0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
        0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
        0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
        0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};
const bitboard bishop_magics[64] = {
        0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
        0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
        0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
        0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
        0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
        0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
        0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
        0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
        0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
        0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
        0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
        0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
        0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,

};
// Počet bitů použiváných na indexování pro tahy věží a střelcem
const int rook_index_bits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};
const int bishop_index_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};

bitboard ComputeRook(bitboard attacks, bitboard blockers, int pos);

bitboard ComputeBishop(bitboard attacks, bitboard blockers, int pos);

bitboard GetBlockersFromIndex(int index, bitboard mask);

void CheckRookRay(bitboard &attacks, bitboard &blockers, int pos, int nahoru, int max, int min);


/*
Předpočíta pozice kam může daná figurka útočit
pozice se spočítají při inicializaci objektu

Tato třída je singleton takže může existovat jenom jedna instace této třídy

Příklad použití:
    PrecomputedBitboards &precomp_bitboards = PrecomputedBitboards::getInstance();

    bitboard pozice = precomp_bitboards.precomputed_knights[knight_position];
    // vrátí bitmapu s možnýmy pozicemi na táhnutí,
    tahy nemusí být validní podle šachových pravidel. Pozice se musí ještě ověřit.

*/


class PrecomputedBitboards {
public:
    static PrecomputedBitboards &getInstance() {
        static PrecomputedBitboards instance;
        return instance;
    }

    PrecomputedBitboards(PrecomputedBitboards const &) = delete;

    void operator=(PrecomputedBitboards const &) = delete;

    bitboard precomputed_knights[64];
    bitboard precomputed_kings[64];


    bitboard rook_rays[64];
    bitboard bishop_rays[64];

    bitboard precomputed_rooks[64][4096];
    bitboard precomputed_bishops[64][4096];


    // maska na řádek či sloupec
    bitboard col_row_mask[16];
private:

    PrecomputedBitboards() {
        PrecomputeAll();
    }


    // predpočítá všechny potřebné bitmapy na počítání možných pozic
    void PrecomputeAll();

    // vytvoří bitmapy pro mazání určitých řádků či sloupců
    void PrecomputeKnights();

    // vyplni masku
    void FillRowcolMask();

    void PrecomputeKings();

    void PrecomputeRooks();

    void PrecomputeRookRays();

    void PrecomputeBishopRays();

    void PrecomputeBiships();


    bitboard slow_bishop_ray(int pos, int direction);
};

#endif //CHESS_ENGINE_CPP_PRECOMPUTEDBITBOARDS_H