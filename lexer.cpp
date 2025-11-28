#include <bits/stdc++.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "internals.h"

bool isDigitString(std::string str)
{
    for (char x : str)
    {
        if (!(std::isdigit(x)))
        {
            return false;
        }
    }
    return true;
}
int getTermVariableIndex(std::string strTerm, int index = 0)
{
    while (isdigit(strTerm[index]))
    {
        index++;
    }
    return index;
}

// Term getTerm(std::tuple<std::string, std::string, bool> eqtuple)
// {
//     token term;
//     std::string eqstr = std::get<0>(eqtuple);
// 		term.operation = std::get<1>(eqtuple);
//     term.lhs = std::get<2>(eqtuple);
//     int indexOfVariable = getTermVariableIndex(eqstr);
//     int coff = std::stoi(
//         eqstr.substr(0, indexOfVariable));  // turn the substr of the coefficient to an int
//     term.coefficient = coff;
//     term.variable = eqstr[indexOfVariable];
//     if (indexOfVariable + 1 < static_cast<int>(eqstr.length()) && eqstr[indexOfVariable + 1] ==
//     '^')
//     {
//         if (indexOfVariable + 2 < static_cast<int>(eqstr.length()) &&
//             std::isdigit(eqstr[indexOfVariable + 2]))
//         {
//             int last = eqstr.length() - 1;
//             std::string powerSubStr = eqstr.substr(
//                 indexOfVariable + 2,
//                 last);  // +2 cause i want to skip the variable itself & the ^ charachter
//             term.power = std::stoi(powerSubStr);
//         }
//     }
//
//     return term;
// }

// std::vector<Token> Lex(std::string equation)
// {
//     // euation example 3y^2 + 8y + 3x + 100
//     termMap strterms;
//     std::string currentTerm{};
//     int termN = 0;
// 		char lastOperation;
//     bool lhs = true;
//     for (char x : equation)
//     {
//         if (!isOperator(x) && x != ' ')
//         {
//             currentTerm += x;
//         }
//         else if (isOperator(x) && x != ' ')
//         {
//             if (!lhs)
//             {
//                 x = '\0';  // empty char, because if its lhs then its just the same op as the
//                 last
//             }
//
//             strterms[termN] = std::make_tuple(
//
//                 currentTerm, x,
//                 lhs);    // term:(OPERATION,LHS?)to know which operation was in the equation
//                 later
//             lhs = !lhs;  // flip lhs
//             currentTerm = "";
//             termN += 1;
//             lastOperation = x;
//         }
//     }
//     if (currentTerm != "")
//     {
//         strterms[termN] = std::make_tuple(
//
//             currentTerm, '\0',
//             lhs);  // term:(OPERATION,LHS?)to know which operation was in the equation later
//     }
//
//     printContainer(strterms);
//     std::vector<Term> terms;
//     for (const auto& [_, value] : strterms)
//     {
//         terms.emplace_back(getTerm(value));
//     }
//    return terms;
// }

std::vector<std::string> split(std::string base, char delim)
{
    std::stringstream ss(base);
    std::string temp;
    std::vector<std::string> v;
    while (std::getline(ss, temp, delim))
    {
        v.emplace_back(temp);
    }

    printContainer(v);
    return v;
}

TokenTypes getType(char x)
{
    if (std::isdigit(x))
    {
        return NUM;
    }
    else
    {
        return VAR;
    }
}
std::tuple<Token, std::string> _lex(std::string tokenBuilder, std::string equation,
                                    TokenTypes last_type = NUL)
{
    char x = equation[0];
    if (x == ' ')
    {  // skip spaces

        return _lex(tokenBuilder, equation.substr(1, equation.size()), last_type);
    }

    if (last_type != getType(x) && last_type != NUL)
    {
        return make_tuple(Token{tokenBuilder, last_type}, equation);
    }
    switch (x)
    {
        case '+':
            return std::make_tuple(Token{(std::string() + x), ADDOP},
                                   equation.substr(1, equation.size()));

        case '-':
            return std::make_tuple(Token{(std::string() + x), SUBOP},
                                   equation.substr(1, equation.size()));
        case '*':
            return std::make_tuple(Token{(std::string() + x), MULOP},
                                   equation.substr(1, equation.size()));
        case '/':
            return std::make_tuple(Token{(std::string() + x), DIVOP},
                                   equation.substr(1, equation.size()));
        case '^':
            return std::make_tuple(Token{(std::string() + x), POWOP},
                                   equation.substr(1, equation.size()));
    }

    if (std::isdigit(x) && (last_type == NUM || last_type == NUL))
    {
        tokenBuilder += x;
        return _lex(tokenBuilder, equation.substr(1, equation.size()),
                    NUM);  // remove the first charachter
    }

    return std::make_tuple(Token{(std::string() + x), VAR}, equation.substr(1, equation.size()));
    std::string error = "ERROR\n x->" + (std::string() + x) + "\nEquation ->" + equation;

    throw error;
}

std::vector<Token> Lex(std::string equation)
{
    std::vector<Token> Tokens;
    std::vector<Token> x;
    while (equation.size() != 0)
    {
        std::tuple<Token, std::string> y;

        y = _lex("", equation);
        x.emplace_back(std::get<0>(y));
        equation = std::get<1>(y);
    }

		printContainer(x);
    std::exit(0);
}
