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

TokenTypes getTokenType(std::string x)
{
    if (x.size() == 1)
    {
        char y = x[0];
        switch (y)
        {
            case '+':
                return ADDOP;
            case '-':
                return SUBOP;
            case '*':
                return MULOP;
            case '/':
                return DIVOP;
            case '^':
                return POWOP;
            case ' ':
                return NUL;
        }
    }
    if (isDigitString(x))
    {
        return NUM;
    }
		if(x[0] < 91 || x[0] > 96){
			if(x[0] >= 65 && x[0] <= 122){
				return VAR;
		}}
    throw "NOT IMPLEMENTED CHAR -> " + x;
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
std::vector<Token> Lex(std::string equation)
{
    std::vector<Token> Tokens;
    std::vector<std::string> splitedEquation = split(equation, ' ');
    for (std::string x : splitedEquation)
    {
        if (getTokenType(x) != NUL)
        {
            Token t;
            t.token = x;
            t.type = getTokenType(x);
            Tokens.emplace_back(t);
        }
    }
		return Tokens;
}
