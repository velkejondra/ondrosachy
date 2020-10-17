
//
// Created by ondra on 13.09.2020.
//

#include "../src/Board.h"

#define BOOST_TEST_MODULE test module name
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <iostream>
using namespace std;
BOOST_AUTO_TEST_CASE(my_test) {
    PrecomputedBitboards b = PrecomputedBitboards();
    b.PrecomputeAll();
    cout << b.precomputed_knights[5] << endl;
    BOOST_CHECK_EQUAL(true, true);
}

