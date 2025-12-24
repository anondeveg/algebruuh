#pragma once
#include "lexer.hpp"
#include "parser.hpp"

#include <cmath>
#include <random>

double FACTORIAL(double num) {
    if (num == 1) {
        return num;
    } else {
        return FACTORIAL(num - 1) * num;
    }
}

double RAND(double MAX, double MIN=0) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(MIN,MAX);
		return dist(rng);
}


