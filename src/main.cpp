#include <iostream>
#include "Board.h"
#include "PrecomputedBitboards.h"
//#include "debug.h"
using namespace std;

int main() {
    auto *b = new PrecomputedBitboards();
    cout << BitboardRepresentation(b->precomputed_kings[63]) << endl;
    return 0;
}