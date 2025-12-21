#include "internals.h"

#include <bits/stdc++.h>
#include <cctype>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

bool isDigitString(std::string str) {
    for (char x : str) {
        if (!(std::isdigit(x))) {
            return false;
        }
    }
    return true;
}

int getTermVariableIndex(std::string strTerm, int index = 0) {
    while (isdigit(strTerm[index])) {
        index++;
    }
    return index;
}

std::vector<std::string> split(std::string base, char delim) {
    std::stringstream ss(base);
    std::string temp;
    std::vector<std::string> v;
    while (std::getline(ss, temp, delim)) {
        v.emplace_back(temp);
    }

    printContainer(v);
    return v;
}

TokenTypes getType(char x) {
    if (std::isdigit(x)) {
        return NUM;
    } else {
        return VAR;
    }
}

std::tuple<Token, std::string> genToken(char x, TokenTypes TYPE, std::string equation) {
    if (std::isdigit(x))
        return std::make_tuple(Token {std::stod(std::string {x}), NUM},
                               equation.substr(1, equation.size()));
    return std::make_tuple(Token {(std::string() + x), TYPE}, equation.substr(1, equation.size()));
}

std::tuple<Token, std::string>
_lex(std::string tokenBuilder, std::string equation, TokenTypes last_type = NUL) {
    char x = equation[0];
    if (x == ' ') {  // skip spaces

        return _lex(tokenBuilder, equation.substr(1, equation.size()), last_type);
    }

    if (last_type != getType(x) && last_type != NUL) {
        if (isDigitString(tokenBuilder))
            return make_tuple(Token {std::stod(std::string {tokenBuilder}), last_type}, equation);
    }
    switch (x) {
    case '+':
        return genToken(x, PLUSOP, equation);
    case '-':
        return genToken(x, SUBOP, equation);
    case '*':
        return genToken(x, MULOP, equation);
    case '/':
        return genToken(x, DIVOP, equation);
    case '^':
        return genToken(x, POWOP, equation);
    case '(':
        return genToken(x, OPENPAR, equation);
    case ')':
        return genToken(x, CLOSEDPAR, equation);
    };

    if (std::isdigit(x) && (last_type == NUM || last_type == NUL)) {
        tokenBuilder += x;
        return _lex(tokenBuilder,
                    equation.substr(1, equation.size()),
                    NUM);  // remove the first charachter
    }

    return std::make_tuple(Token {(std::string() + x), VAR}, equation.substr(1, equation.size()));
}

std::vector<Token> Lex(std::string equation, bool dump = false, bool verbose = false) {
    std::vector<Token> Tokens;
    std::vector<Token> x;
    while (equation.size() != 0) {
        std::tuple<Token, std::string> y;

        y = _lex("", equation);
        if (verbose) {
            std::cout << "\n" << std::get<0>(y) << "\n";
        }
        x.emplace_back(std::get<0>(y));

        equation = std::get<1>(y);
    }

    x.emplace_back(Token {"", NUL});  // add nul
    printContainer(x);
    if (dump) {
        writeToFile("dump.lex", x);
        std::cout << "\n[TOKENS DUMPED] dump.lex\n";
    }

    return x;
}
