//
// Created by velkejondra on 12/15/20.
//

#ifndef CHESS_ENGINE_CPP_RANDOMGENERATOR_H
#define CHESS_ENGINE_CPP_RANDOMGENERATOR_H

#include <climits>
#include <random>


static std::mt19937 gen;
static std::uniform_int_distribution<unsigned long long> distrib(0, ULONG_LONG_MAX);


#endif //CHESS_ENGINE_CPP_RANDOMGENERATOR_H
